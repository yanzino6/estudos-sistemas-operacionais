#!/usr/bin/env python3
"""Oraculo do TP de SO (mini-kernel): simulador deterministico das regras.

NAO faz parte da entrega. Serve para gerar a saida esperada de qualquer entrada
(mono, GLOBAL ou POR_CPU) e para dizer se uma entrada e "ambigua" (o resultado
depende da ordem em que CPUs tratam eventos do mesmo instante).

Uso:
  python3 oraculo.py entrada.txt                  saida esperada (usa CPUS e FILA do arquivo)
  python3 oraculo.py entrada.txt --mono           como "make monoprocessador" (CPUS=1, FILA ignorado)
  python3 oraculo.py entrada.txt --ambiguo        idem, mas sai com codigo 2 se a entrada for ambigua
  python3 oraculo.py entrada.txt --afinidade      bonus ligado (sem efeito com 1 CPU)
  python3 oraculo.py entrada.txt --retencao-nao-conta   ponto em aberto P1: CPU retida nao conta na carga

Modelo (o mesmo do programa em C). Em cada instante t (multiplo de 100):
  1. chegadas de t, em ordem de PID, entram na fila (POR_CPU: CPU de menor carga);
     em seguida as CPUs que JA estavam ociosas recebem trabalho, em ordem de indice;
  2. finalizacoes de t;
  3. cada CPU liberada em t (fim de quantum, fim de retencao, ou finalizacao no FCFS)
     reinsere o processo preemptado no fim da sua fila e escolhe o proximo na mesma
     secao critica; depois disso as ociosas tentam de novo;
  4. termino, quando nao sobra nada.
Linhas de um mesmo instante saem na ordem: chegadas, finalizacoes, resto, termino.
"""
import sys

FATIA = 100
CHEGADA, FINALIZADO, OUTRO, TERMINO = 0, 1, 2, 3


def ler_entrada(caminho):
    with open(caminho, encoding="utf-8") as f:
        tok = f.read().split()
    cab = {tok[i]: tok[i + 1] for i in range(0, 10, 2)}
    n = int(cab["PROCESSOS"])
    procs = []
    for i in range(n):
        pid, cheg, dur, prio, nth = (int(x) for x in tok[10 + 5 * i: 15 + 5 * i])
        procs.append({"pid": pid, "chegada": cheg, "duracao": dur, "prio": prio, "threads": nth})
    return {"cpus": int(cab["CPUS"]), "quantum": int(cab["QUANTUM"]),
            "politica": cab["POLITICA"], "fila": cab["FILA"], "procs": procs}


class Sim:
    def __init__(self, ent, mono=False, afinidade=False, retencao_conta=True,
                 ordem="crescente", ociosa_recebe_na_chegada=True):
        self.pol = ent["politica"]
        self.q = ent["quantum"]
        self.ncpu = 1 if mono else ent["cpus"]
        self.por_cpu = (not mono) and ent["fila"] == "POR_CPU" and self.ncpu >= 1
        self.afinidade = afinidade and self.ncpu > 1
        self.retencao_conta = retencao_conta
        self.ordem = ordem
        self.ociosa_recebe_na_chegada = ociosa_recebe_na_chegada
        self.procs = sorted((dict(p) for p in ent["procs"]), key=lambda p: (p["chegada"], p["pid"]))
        for p in self.procs:
            p.update(resta=p["duracao"], primeiro=-1, fim=-1, ultima_cpu=-1, seq=0)
        nf = self.ncpu if self.por_cpu else 1
        self.filas = [[] for _ in range(nf)]
        self.seq = [0] * nf
        # CPU: exec = processo despachado (fica durante a retencao); livre_em = instante de liberacao
        self.cpus = [{"exec": None, "retida": False, "t_desp": 0, "livre_em": None,
                      "ultimo_pid": -1, "ocupado": 0, "trocas": 0} for _ in range(self.ncpu)]
        self.eventos = []   # (t, categoria, ordem, texto)

    # ---------- utilitarios ----------
    def log(self, t, cat, cpu, msg):
        pre = "[t=%dms] [%s] " % (t, self.pol)
        if cpu >= 0:
            pre += "[CPU%d] " % cpu
        self.eventos.append((t, cat, len(self.eventos), pre + msg))

    def fila_de(self, k):
        return self.filas[k if self.por_cpu else 0]

    def inserir_fim(self, qi, p):
        p["seq"] = self.seq[qi]
        self.seq[qi] += 1
        self.filas[qi].append(p)

    def carga(self, k):
        c = self.cpus[k]
        ocupa = c["exec"] is not None and (self.retencao_conta or not c["retida"])
        return len(self.filas[k]) + (1 if ocupa else 0)

    # ---------- politica ----------
    def escolher(self, fila, cpu):
        if not fila:
            return None
        if self.pol != "PRIORIDADE":
            return fila.pop(0)
        melhor = None
        for p in fila:                                   # do inicio (mais antigo) para o fim
            if melhor is None or p["prio"] < melhor["prio"]:
                melhor = p
            elif (self.afinidade and p["prio"] == melhor["prio"]
                  and melhor["ultima_cpu"] != cpu and p["ultima_cpu"] == cpu):
                melhor = p
        fila.remove(melhor)
        return melhor

    def tirar_proximo(self, k, t):
        p = self.escolher(self.fila_de(k), k)
        if p is None and self.por_cpu:
            maior = max((len(f) for j, f in enumerate(self.filas) if j != k), default=0)
            if maior > 0:
                cands = [j for j, f in enumerate(self.filas) if j != k and len(f) == maior]
                origem = cands[0]
                if self.afinidade:
                    for j in cands:
                        if self.filas[j][-1]["ultima_cpu"] == k:
                            origem = j
                            break
                p = self.filas[origem].pop()             # rouba o ULTIMO
                self.log(t, OUTRO, k, "Migracao PID %d CPU%d -> CPU%d" % (p["pid"], origem, k))
        return p

    def despachar(self, k, p, t):
        c = self.cpus[k]
        if p["pid"] != c["ultimo_pid"]:
            c["trocas"] += 1
        c["ultimo_pid"] = p["pid"]
        c["exec"], c["retida"], c["t_desp"] = p, False, t
        p["ultima_cpu"] = k
        if p["primeiro"] < 0:
            p["primeiro"] = t
        if self.pol == "FCFS":
            c["livre_em"] = t + p["resta"]
            p["fim_previsto"] = t + p["resta"]
        else:
            c["livre_em"] = t + self.q
            p["fim_previsto"] = t + p["resta"] if p["resta"] <= self.q else None
        self.log(t, OUTRO, k, "Executando PID %d" % p["pid"])

    def entregar_ociosas(self, t):
        mudou = True
        while mudou:
            mudou = False
            for k in range(self.ncpu):
                if self.cpus[k]["exec"] is None:
                    p = self.tirar_proximo(k, t)
                    if p is not None:
                        self.despachar(k, p, t)
                        mudou = True

    # ---------- laco principal ----------
    def rodar(self):
        i, n = 0, len(self.procs)
        t = 0
        terminou = n == 0
        if terminou:
            self.log(0, TERMINO, -1, "Escalonador terminou execução de todos processos")
        while not terminou:
            # 1. chegadas de t (ja ordenadas por PID dentro do instante)
            chegou = False
            while i < n and self.procs[i]["chegada"] == t:
                p = self.procs[i]
                i += 1
                chegou = True
                if self.por_cpu:
                    destino = min(range(self.ncpu), key=lambda k: (self.carga(k), k))
                else:
                    destino = 0
                self.inserir_fim(destino, p)
                self.log(t, CHEGADA, -1, "Chegada PID %d" % p["pid"])
            if chegou and self.ociosa_recebe_na_chegada:
                self.entregar_ociosas(t)
            # 2. finalizacoes de t
            for k, c in enumerate(self.cpus):
                p = c["exec"]
                if p is not None and not c["retida"] and p.get("fim_previsto") == t:
                    c["ocupado"] += p["resta"]
                    p["resta"] = 0
                    p["fim"] = t
                    p["fim_previsto"] = None
                    self.log(t, FINALIZADO, k, "PID %d finalizado" % p["pid"])
                    if c["livre_em"] > t:
                        c["retida"] = True
            # 3. liberacoes de t
            liberadas = [k for k, c in enumerate(self.cpus) if c["exec"] is not None and c["livre_em"] == t]
            if self.ordem == "decrescente":
                liberadas.reverse()
            for k in liberadas:
                c = self.cpus[k]
                p = c["exec"]
                c["exec"], c["retida"] = None, False
                if p["resta"] > 0:                       # fim de quantum sem terminar
                    c["ocupado"] += self.q
                    p["resta"] -= self.q
                    self.log(t, OUTRO, k, "Quantum esgotado PID %d" % p["pid"])
                    self.inserir_fim(k if self.por_cpu else 0, p)
                prox = self.tirar_proximo(k, t)          # mesma secao critica da reinsercao
                if prox is not None:
                    self.despachar(k, prox, t)
                if self.ociosa_recebe_na_chegada:
                    self.entregar_ociosas(t)
            if not self.ociosa_recebe_na_chegada:
                self.entregar_ociosas(t)
            # 4. termino
            if (i == n and all(not f for f in self.filas)
                    and all(c["exec"] is None for c in self.cpus)):
                self.log(t, TERMINO, -1, "Escalonador terminou execução de todos processos")
                terminou = True
            t += FATIA
        return self

    def texto(self):
        linhas = [e[3] for e in sorted(self.eventos, key=lambda e: e[:3])]
        linhas.append("=== METRICAS ===")
        n = len(self.procs)
        makespan = max((p["fim"] for p in self.procs), default=0)
        se = st = sr = 0
        for p in sorted(self.procs, key=lambda p: p["pid"]):
            ta = p["fim"] - p["chegada"]
            es = ta - p["duracao"]
            re = p["primeiro"] - p["chegada"]
            se, st, sr = se + es, st + ta, sr + re
            linhas.append("PID %d | espera=%dms | turnaround=%dms | resposta=%dms" % (p["pid"], es, ta, re))
        d = float(n) if n else 1.0
        linhas.append("Espera media: %.2f ms" % (se / d))
        linhas.append("Turnaround medio: %.2f ms" % (st / d))
        linhas.append("Resposta media: %.2f ms" % (sr / d))
        linhas.append("Trocas de contexto: %d" % sum(c["trocas"] for c in self.cpus))
        for k, c in enumerate(self.cpus):
            linhas.append("Utilizacao CPU%d: %.2f%%" % (k, 100.0 * c["ocupado"] / makespan if makespan else 0.0))
        linhas.append("Throughput: %.2f processos/s" % (n / (makespan / 1000.0) if makespan else 0.0))
        linhas.append("Makespan: %d ms" % makespan)
        return "\n".join(linhas) + "\n"


def main(argv):
    if len(argv) < 2:
        sys.stderr.write(__doc__)
        return 1
    flags = set(argv[2:])
    ent = ler_entrada(argv[1])
    base = dict(mono="--mono" in flags, afinidade="--afinidade" in flags,
                retencao_conta="--retencao-nao-conta" not in flags)
    saida = Sim(ent, **base).rodar().texto()
    sys.stdout.write(saida)
    if "--ambiguo" in flags:
        variantes = [dict(ordem="decrescente"),
                     dict(ociosa_recebe_na_chegada=False),
                     dict(ordem="decrescente", ociosa_recebe_na_chegada=False)]
        for v in variantes:
            if Sim(ent, **base, **v).rodar().texto() != saida:
                return 2
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
