"""Contratos: reprovar saída errada, travamento e resposta conceitual incorreta."""
import json, pathlib, shutil, subprocess, sys, tempfile, unittest
RUNNER = pathlib.Path(__file__).with_name('corretor.py')
class CorretorTest(unittest.TestCase):
 def invoke(self, spec, content, suffix, arquivos=None):
  with tempfile.TemporaryDirectory() as d:
   p=pathlib.Path(d); (p/'casos.json').write_text(json.dumps(spec)); (p/('sol'+suffix)).write_text(content)
   for nome, texto in (arquivos or {}).items():
    arquivo=p/nome; arquivo.parent.mkdir(parents=True,exist_ok=True); arquivo.write_text(texto)
   r=subprocess.run([sys.executable,str(RUNNER),str(p),str(p/('sol'+suffix))],capture_output=True,text=True)
   r.arquivos={str(f.relative_to(p)):f.read_text() for f in p.rglob('*') if f.is_file()}
   return r
 def test_salva_saida_e_erro_mesmo_reprovando(self):
  r=self.invoke({'casos':[{'nome':'errado','saida_arquivo':'esperados/01.out'}]}, '#include <stdio.h>\nint main(void){puts("6");fputs("diagnostico",stderr);}', '.c', {'esperados/01.out':'5\n'})
  self.assertEqual(r.returncode,1,r.stdout+r.stderr)
  self.assertEqual(r.arquivos['saidas/01.out'],'6\n')
  self.assertEqual(r.arquivos['saidas/01.err'],'diagnostico')
  self.assertEqual(r.arquivos['esperados/01.out'],'5\n')
 def test_salva_todas_as_repeticoes(self):
  r=self.invoke({'repeticoes':2,'casos':[{'nome':'repetido','saida':'ok'}]}, '#include <stdio.h>\nint main(void){puts("ok");}', '.c')
  self.assertEqual(r.returncode,0,r.stdout+r.stderr)
  for nome in ('saidas/01_01.out','saidas/01_02.out'):
   self.assertEqual(r.arquivos[nome],'ok\n')
 def test_timeout_preserva_saida_parcial(self):
  r=self.invoke({'timeout':0.2,'casos':[{'nome':'bloqueio'}]}, '#include <stdio.h>\nint main(void){puts("inicio");fflush(stdout);for(;;){}}', '.c')
  self.assertEqual(r.returncode,1,r.stdout+r.stderr)
  self.assertEqual(r.arquivos['saidas/01.out'],'inicio\n')
  self.assertIn('tempo',r.arquivos['saidas/01.err'])
 def test_cria_gabarito_da_referencia_e_rejeita_solucao_errada(self):
  with tempfile.TemporaryDirectory() as d:
   raiz=pathlib.Path(d); infra=raiz/'_infra'; infra.mkdir()
   for nome in ('corretor.py','eventos.py'):
    shutil.copy(RUNNER.with_name(nome),infra/nome)
   shutil.copytree(RUNNER.parent/'fixtures',infra/'fixtures')
   (raiz/'catalogo.json').write_text(json.dumps([{'parte':'parte','id':'s01'}]))
   pasta=raiz/'parte/testes/s01'; pasta.mkdir(parents=True)
   (pasta/'casos.json').write_text(json.dumps({'casos':[{'nome':'blocos','entrada':'1','saida_arquivo':'esperados/01.out'}]}))
   sol=raiz/'sol.c'; sol.write_text('#include <stdio.h>\nint main(void){puts("errado");}')
   cmd=[sys.executable,str(infra/'corretor.py'),str(pasta),str(sol)]
   r=subprocess.run(cmd,capture_output=True,text=True)
   self.assertEqual(r.returncode,1,r.stdout+r.stderr)
   self.assertEqual((pasta/'esperados/01.out').read_text(),'1\n2\n3\n4\n5\n')
   self.assertEqual((pasta/'saidas/01.out').read_text(),'errado\n')
   sol.write_text('#include <stdio.h>\nint main(void){puts("1 2 3 4 5");}')
   r=subprocess.run(cmd,capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stdout+r.stderr)
   self.assertEqual((pasta/'saidas/01.out').read_text(),'1 2 3 4 5\n')
 def test_aceita_programa_real(self):
  r=self.invoke({'tipo':'programa','casos':[{'nome':'soma','entrada':'2 3','saida':'5'}]},'#include <stdio.h>\nint main(void){int a,b;scanf("%d%d",&a,&b);printf("%d",a+b);}', '.c')
  self.assertEqual(r.returncode,0,r.stdout+r.stderr)
 def test_rejeita_saida_errada(self):
  r=self.invoke({'tipo':'programa','casos':[{'nome':'soma','entrada':'2 3','saida':'5'}]},'#include <stdio.h>\nint main(void){puts("6");}', '.c')
  self.assertEqual(r.returncode,1); self.assertIn('FALHOU',r.stdout)
 def test_timeout(self):
  r=self.invoke({'tipo':'programa','timeout':0.2,'casos':[{'nome':'bloqueio','entrada':'','saida':''}]},'int main(void){for(;;){}}', '.c')
  self.assertEqual(r.returncode,1); self.assertIn('tempo',r.stdout)
 def test_eventos_rejeita_ordem_invalida(self):
  r=self.invoke({'tipo':'programa','validador':'grafo','casos':[{'nome':'grafo','entrada':'','parametros':{'nos':['A','B'],'arestas':[['A','B']]}}]},'#include <stdio.h>\nint main(void){puts("B\\nA");}', '.c')
  self.assertEqual(r.returncode,1,r.stdout); self.assertIn('FALHOU',r.stdout)
 def test_eventos_aceita_ordem_valida(self):
  r=self.invoke({'tipo':'programa','validador':'grafo','casos':[{'nome':'grafo','entrada':'','parametros':{'nos':['A','B'],'arestas':[['A','B']]}}]},'#include <stdio.h>\nint main(void){puts("A\\nB");}', '.c')
  self.assertEqual(r.returncode,0,r.stdout+r.stderr)
 def test_validador_por_caso(self):
  r=self.invoke({'tipo':'programa','casos':[{'nome':'grafo','entrada':'','validador':'grafo','parametros':{'nos':['A','B'],'arestas':[['A','B']]}}]},'#include <stdio.h>\nint main(void){puts("B\\nA");}', '.c')
  self.assertEqual(r.returncode,1,r.stdout)
 def test_rejeita_fifo_nao_removida(self):
  r=self.invoke({'tipo':'programa','casos':[{'nome':'limpeza','entrada':'','saida':'','ausentes':['canal.fifo']}]},'#include <sys/stat.h>\nint main(void){return mkfifo("canal.fifo",0600);}', '.c')
  self.assertEqual(r.returncode,1,r.stdout)
 def test_linhas_fora_de_ordem(self):
  r=self.invoke({'tipo':'programa','comparacao':'linhas_ordenadas','casos':[{'nome':'ordem','entrada':'','saida':'a\nb'}]},'#include <stdio.h>\nint main(void){puts("b");puts("a");}', '.c')
  self.assertEqual(r.returncode,0,r.stdout+r.stderr)
if __name__=='__main__': unittest.main()
