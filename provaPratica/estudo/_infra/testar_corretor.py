"""Contratos: reprovar saída errada, travamento e resposta conceitual incorreta."""
import json, pathlib, subprocess, sys, tempfile, unittest
RUNNER = pathlib.Path(__file__).with_name('corretor.py')
class CorretorTest(unittest.TestCase):
 def invoke(self, spec, content, suffix):
  with tempfile.TemporaryDirectory() as d:
   p=pathlib.Path(d); (p/'casos.json').write_text(json.dumps(spec)); (p/('sol'+suffix)).write_text(content)
   return subprocess.run([sys.executable,str(RUNNER),str(p),str(p/('sol'+suffix))],capture_output=True,text=True)
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
