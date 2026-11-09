#!/usr/bin/env python3
"""Correção local, sem dependências externas além de Python 3 e compilador C."""
import json, os, pathlib, signal, subprocess, sys, tempfile

def executar(cmd, entrada, cwd, timeout, env=None):
    p = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                         stderr=subprocess.PIPE, text=True, cwd=cwd, env=env,
                         start_new_session=True)
    try:
        out, err = p.communicate(entrada, timeout=timeout)
        return p.returncode, out, err
    except subprocess.TimeoutExpired:
        os.killpg(p.pid, signal.SIGKILL)
        p.communicate()
        return -1, '', 'limite de tempo excedido (possível deadlock)'
    finally:
        # Elimina descendentes deixados pelo programa, inclusive após a saída do pai.
        try: os.killpg(p.pid, signal.SIGKILL)
        except ProcessLookupError: pass

def normalizar(s, modo):
    if modo == 'linhas_ordenadas': return sorted(' '.join(x.split()) for x in s.splitlines() if x.strip())
    return s.split()

def main(pasta, solucao):
    pasta, solucao = pathlib.Path(pasta).resolve(), pathlib.Path(solucao).resolve()
    try:
        spec = json.loads((pasta/'casos.json').read_text())
        if spec['tipo'] == 'analise':
            respostas = json.loads(solucao.read_text())
            falhas = 0
            for caso in spec['casos']:
                valor = respostas.get(caso['id']) if isinstance(respostas,dict) else None
                ok = type(valor) is type(caso['esperado']) and valor == caso['esperado']
                print(('OK' if ok else 'FALHOU') + ': ' + caso['id'] + ' - ' + caso['pergunta'])
                if not ok: print('  Reveja o cenário e consulte gabarito.md depois de tentar.')
                falhas += not ok
            print('REVISÃO MANUAL OBRIGATÓRIA: confira resposta.md/pseudocódigo com rubrica.md. Este script corrige apenas os casos objetivos.')
            return int(falhas > 0)
        with tempfile.TemporaryDirectory(prefix='estudo-so-') as tmp:
            tmp = pathlib.Path(tmp)
            if solucao.suffix == '.c':
                cmd = [os.environ.get('CC','cc'), '-std=c11','-D_POSIX_C_SOURCE=200809L','-Wall','-Wextra','-pthread',str(solucao),'-o',str(tmp/'programa')]
                r = subprocess.run(cmd,capture_output=True,text=True,timeout=30)
                if r.stderr: print(r.stderr)
                if r.returncode: print('FALHOU: compilação'); return 1
                programa = tmp/'programa'
            else: programa = solucao
            falhas = 0
            for caso in spec['casos']:
                env = os.environ.copy()
                if 'ping' in caso:
                    mocks = tmp/'bin'; mocks.mkdir(exist_ok=True)
                    # Comando controlado para testar o pipeline sem rede; grep continua real.
                    (tmp/'ping.txt').write_text(caso['ping'])
                    (mocks/'ping').write_text('#!/bin/sh\n[ "$*" = "-c 5 google.com" ] || exit 65\ncat "$PING_FIXTURE"\n')
                    (mocks/'ping').chmod(0o755)
                    env['PATH'] = str(mocks)+os.pathsep+env.get('PATH','')
                    env['PING_FIXTURE'] = str(tmp/'ping.txt')
                esperado = caso.get('saida')
                entrada = caso.get('entrada','')
                if 'entrada_arquivo' in caso: entrada = (pasta/caso['entrada_arquivo']).read_text()
                if 'saida_arquivo' in caso: esperado = (pasta/caso['saida_arquivo']).read_text()
                for repeticao in range(caso.get('repeticoes',spec.get('repeticoes',1))):
                    rc, out, err = executar([str(programa)]+caso.get('args',[]), entrada, tmp, spec.get('timeout',5), env)
                    ok = rc == caso.get('retorno',0) and (esperado is None or normalizar(out,spec.get('comparacao','tokens')) == normalizar(esperado,spec.get('comparacao','tokens')))
                    print(('OK' if ok else 'FALHOU')+f": {caso['nome']} [{repeticao+1}]")
                    if not ok:
                        print(f'  retorno={rc}; stderr={err[:1000]!r}; saída={out[:1000]!r}; esperado={str(esperado)[:1000]!r}')
                        falhas += 1
                        break
            print('Revise também os requisitos estruturais em rubrica.md; saída correta não prova uso de threads/pipes nem ausência de corridas.')
            return int(falhas > 0)
    except (OSError, ValueError, KeyError, subprocess.TimeoutExpired) as e:
        print('ERRO de configuração/arquivo:', e)
        return 2

if __name__ == '__main__':
    if len(sys.argv)!=3:
        print('Uso: python3 corretor.py PASTA_DOS_CASOS SOLUCAO.c|EXECUTAVEL|RESPOSTAS.json'); sys.exit(2)
    sys.exit(main(*sys.argv[1:]))
