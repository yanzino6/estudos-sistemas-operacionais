#!/usr/bin/env python3
"""Extrai de um log (log_execucao_minikernel.txt ou saida do oraculo) as medidas do bonus de afinidade.

NAO faz parte da entrega.

Uso: python3 contar_migracoes.py log_execucao_minikernel.txt

Imprime uma linha "chave=valor" por medida:
  migracoes_log     linhas "Migracao" (so existem no POR_CPU)
  despachos_outra   despachos de um PID numa CPU diferente da do seu despacho anterior
                    (a "migracao implicita" do GLOBAL; no POR_CPU inclui as migracoes)
  retornos          despachos de um PID na mesma CPU do seu despacho anterior
  despachos         total de linhas "Executando"
  trocas, espera_media, turnaround_medio, resposta_media, makespan, utilizacao_media
"""
import re
import sys

RE_EXEC = re.compile(r"\[CPU(\d+)\] Executando PID (\d+)")


def medir(caminho):
    migracoes = despachos = outra = retornos = 0
    ultima = {}                                   # pid -> CPU do despacho anterior
    m = {}
    utils = []
    with open(caminho, encoding="utf-8") as f:
        for linha in f:
            linha = linha.strip()
            if "] Migracao PID " in linha:
                migracoes += 1
            ex = RE_EXEC.search(linha)
            if ex:
                cpu, pid = int(ex.group(1)), int(ex.group(2))
                despachos += 1
                if pid in ultima:
                    if ultima[pid] == cpu:
                        retornos += 1
                    else:
                        outra += 1
                ultima[pid] = cpu
            elif linha.startswith("Espera media:"):
                m["espera_media"] = linha.split()[2]
            elif linha.startswith("Turnaround medio:"):
                m["turnaround_medio"] = linha.split()[2]
            elif linha.startswith("Resposta media:"):
                m["resposta_media"] = linha.split()[2]
            elif linha.startswith("Trocas de contexto:"):
                m["trocas"] = linha.split()[3]
            elif linha.startswith("Makespan:"):
                m["makespan"] = linha.split()[1]
            elif linha.startswith("Utilizacao CPU"):
                utils.append(float(linha.split()[2].rstrip("%")))
    res = [("migracoes_log", migracoes), ("despachos_outra", outra), ("retornos", retornos),
           ("despachos", despachos)]
    for chave in ("trocas", "espera_media", "turnaround_medio", "resposta_media", "makespan"):
        res.append((chave, m.get(chave, "?")))
    res.append(("utilizacao_media", "%.2f" % (sum(utils) / len(utils)) if utils else "?"))
    return res


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.stderr.write(__doc__)
        sys.exit(1)
    for chave, valor in medir(sys.argv[1]):
        print("%s=%s" % (chave, valor))
