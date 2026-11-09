#!/usr/bin/env python3
"""Valida contratos e scripts, não a correção concorrente de soluções de alunos."""
import json, pathlib, subprocess, sys, tempfile
ROOT=pathlib.Path(__file__).resolve().parents[1]
def run(cmd): return subprocess.run(cmd,capture_output=True,text=True)
def main():
    catalogo=json.loads((ROOT/'catalogo.json').read_text())
    modelo=(ROOT/'_infra/fixtures/contratos.c').read_text()
    total_casos=0
    with tempfile.TemporaryDirectory() as td:
        tmp=pathlib.Path(td)
        for e in catalogo:
            pasta=ROOT/e['parte']/'testes'/e['id']
            spec=json.loads((pasta/'casos.json').read_text())
            total_casos+=len(spec['casos'])
            if spec['tipo']=='analise':
                sol=tmp/(e['id']+'.json')
                respostas={c['id']:c['esperado'] for c in spec['casos']}
                sol.write_text(json.dumps(respostas))
            else:
                sol=tmp/(e['id']+'.c')
                sol.write_text('#define EX '+json.dumps(e['id'])+'\n'+modelo)
            comando=[sys.executable,str(pasta/'corrigir.py'),str(sol)]
            bom=run(comando)
            if bom.returncode!=0:
                print(e['id'],'REJEITOU referência de contrato:',bom.stdout,bom.stderr);return 1
            if spec['tipo']=='analise':
                respostas[spec['casos'][0]['id']]=None
                sol.write_text(json.dumps(respostas))
            else: sol.write_text('int main(void){return 0;}\n')
            ruim=run(comando)
            if ruim.returncode!=1:
                print(e['id'],'NÃO REJEITOU solução incompleta:',ruim.stdout,ruim.stderr);return 1
            print(e['id'],'OK: referência aceita e solução incompleta rejeitada')
    print(f'{len(catalogo)} exercícios; {total_casos} cenários; todos os contratos validados.')
    return 0
if __name__=='__main__':sys.exit(main())
