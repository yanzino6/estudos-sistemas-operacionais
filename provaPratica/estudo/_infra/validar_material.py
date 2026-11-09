#!/usr/bin/env python3
"""Verifica 49 corretores e compila os modelos. Não é gabarito dos exercícios."""
import json,pathlib,subprocess,sys,tempfile
ROOT=pathlib.Path(__file__).resolve().parents[1]
def run(cmd):return subprocess.run(cmd,capture_output=True,text=True)
def main():
    catalogo=json.loads((ROOT/'catalogo.json').read_text());total=0
    fixture=ROOT/'_infra/fixtures/contratos_praticos.c'
    with tempfile.TemporaryDirectory() as td:
        tmp=pathlib.Path(td)
        for e in catalogo:
            pasta=ROOT/e['parte']/'testes'/e['id'];spec=json.loads((pasta/'casos.json').read_text())
            assert spec['tipo']=='programa';total+=len(spec['casos'])
            sol=tmp/(e['id']+'.c')
            sol.write_text('#define EX '+json.dumps(e['id'])+'\n#include '+json.dumps(str(fixture))+'\n')
            cmd=[sys.executable,str(pasta/'corrigir.py'),str(sol)]
            bom=run(cmd)
            if bom.returncode:print(e['id'],'REJEITOU contrato:',bom.stdout,bom.stderr);return 1
            sol.write_text('int main(void){return 0;}\n')
            ruim=run(cmd)
            if ruim.returncode!=1:print(e['id'],'NÃO REJEITOU incompleto:',ruim.stdout,ruim.stderr);return 1
            # Modelos são do usuário: apenas compilar, nunca exigir que continuem vazios.
            modelo=ROOT/e['modelo']
            compilacao=run(['cc','-std=c11','-D_POSIX_C_SOURCE=200809L','-Wall','-Wextra','-pthread',str(modelo),'-o',str(tmp/'modelo')])
            if compilacao.returncode:print(e['id'],'modelo não compila:',compilacao.stderr);return 1
            print(e['id'],'OK: contrato aceito, incompleto rejeitado, modelo compila')
    print(f'{len(catalogo)} exercícios C e {total} casos verificados.')
    return 0
if __name__=='__main__':sys.exit(main())
