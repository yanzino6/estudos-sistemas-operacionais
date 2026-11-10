#!/usr/bin/env python3
"""Correção local, sem dependências externas além de Python 3 e compilador C."""
import json, os, pathlib, signal, subprocess, sys, tempfile
from eventos import validar

RAIZ = pathlib.Path(__file__).resolve().parents[1]

def compilar(fonte, destino, extras=()):
    cmd = [os.environ.get('CC', 'cc'), '-std=c11', '-D_POSIX_C_SOURCE=200809L',
           '-Wall', '-Wextra', '-pthread', *extras, str(fonte), '-o', str(destino)]
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
    if r.stderr: print(r.stderr)
    return r.returncode == 0

def referencia(pasta, destino):
    # Só o oráculo do exercício pode criar um gabarito; nunca a solução do aluno.
    catalogo = json.loads((RAIZ/'catalogo.json').read_text())
    exercicio = next((e for e in catalogo
                     if (RAIZ/e['parte']/'testes'/e['id']).resolve() == pasta), None)
    if exercicio is None:
        raise ValueError('gabarito ausente e exercício sem referência no catálogo')
    if not compilar(RAIZ/'_infra/fixtures/contratos_praticos.c', destino,
                    ['-DEX='+json.dumps(exercicio['id'])]):
        raise ValueError('não foi possível compilar a referência do exercício')
    return destino

def salvar(caminho, conteudo):
    caminho.parent.mkdir(parents=True, exist_ok=True)
    caminho.write_text(conteudo)

def executar(cmd, entrada, cwd, timeout, env=None):
    p = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                         stderr=subprocess.PIPE, text=True, cwd=cwd, env=env,
                         start_new_session=True)
    try:
        out, err = p.communicate(entrada, timeout=timeout)
        return p.returncode, out, err
    except subprocess.TimeoutExpired:
        os.killpg(p.pid, signal.SIGKILL)
        out, err = p.communicate()
        return -1, out, err + '\nlimite de tempo excedido (possível deadlock)'
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
        with tempfile.TemporaryDirectory(prefix='estudo-so-') as tmp:
            tmp = pathlib.Path(tmp)
            oraculo = None
            if solucao.suffix == '.c':
                if not compilar(solucao, tmp/'programa'):
                    print('FALHOU: compilação'); return 1
                programa = tmp/'programa'
            else: programa = solucao
            falhas = 0
            for indice, caso in enumerate(spec['casos'], 1):
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
                validador = caso.get('validador',spec.get('validador'))
                if 'saida_arquivo' in caso:
                    gabarito = pasta/caso['saida_arquivo']
                    if not gabarito.exists():
                        if oraculo is None: oraculo = referencia(pasta, tmp/'referencia')
                        rc_ref, out_ref, err_ref = executar(
                            [str(oraculo)]+caso.get('args',[]), entrada, tmp,
                            spec.get('timeout',5), env)
                        if rc_ref != caso.get('retorno',0) or (validador and not validar(validador,caso['parametros'],out_ref)):
                            raise ValueError(f"referência falhou em {caso['nome']}: {err_ref}")
                        salvar(gabarito, out_ref)
                        print(f'Gabarito criado: {gabarito}')
                    esperado = gabarito.read_text()
                for repeticao in range(caso.get('repeticoes',spec.get('repeticoes',1))):
                    rc, out, err = executar([str(programa)]+caso.get('args',[]), entrada, tmp, spec.get('timeout',5), env)
                    # Uma saída por caso e repetição, inclusive quando o teste reprova.
                    nome = f'{indice:02d}'
                    if caso.get('repeticoes',spec.get('repeticoes',1)) > 1:
                        nome += f'_{repeticao+1:02d}'
                    salvar(pasta/'saidas'/(nome+'.out'), out)
                    salvar(pasta/'saidas'/(nome+'.err'), err)
                    ok = rc == caso.get('retorno',0) and (esperado is None or normalizar(out,spec.get('comparacao','tokens')) == normalizar(esperado,spec.get('comparacao','tokens')))
                    if validador:
                        ok = rc == caso.get('retorno',0) and validar(validador,caso['parametros'],out)
                    restantes = [nome for nome in caso.get('ausentes',[]) if (tmp/nome).exists()]
                    if restantes:
                        ok = False
                        err += ' Arquivos não removidos: ' + ', '.join(restantes)
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
        print('Uso: python3 corretor.py PASTA_DOS_CASOS SOLUCAO.c|EXECUTAVEL'); sys.exit(2)
    sys.exit(main(*sys.argv[1:]))
