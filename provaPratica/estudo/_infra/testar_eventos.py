import unittest
try:
 from eventos import validar
except ImportError:
 def validar(*args): return False
class EventosTest(unittest.TestCase):
 def test_grafo_aceita_ordens_independentes(self):
  self.assertTrue(validar('grafo',{'nos':['A','B','C'],'arestas':[['A','C'],['B','C']]},'B\nA\nC'))
 def test_grafo_rejeita_precedencia_e_duplicacao(self):
  p={'nos':['A','B'],'arestas':[['A','B']]}
  self.assertFalse(validar('grafo',p,'B\nA'));self.assertFalse(validar('grafo',p,'A\nA\nB'))
 def test_lote_nao_parte_incompleto(self):
  p={'capacidade':2,'viagens':1,'pessoas':2,'saida':True}
  self.assertTrue(validar('lote',p,'ENTRA 0\nENTRA 1\nINICIO\nFIM\nSAI 1\nSAI 0'))
  self.assertFalse(validar('lote',p,'ENTRA 0\nINICIO\nFIM\nSAI 0'))
 def test_pombo_sem_desembarque(self):
  p={'capacidade':2,'viagens':1,'pessoas':2,'saida':False,'unicos':True}
  self.assertTrue(validar('lote',p,'ENTRA 0\nENTRA 1\nINICIO\nFIM'))
  self.assertFalse(validar('lote',p,'ENTRA 0\nENTRA 0\nINICIO\nFIM'))
 def test_comida_so_repoe_vazio(self):
  p={'pessoas':2,'refeicoes':1,'capacidade':1}
  self.assertTrue(validar('comida',p,'COME 1\nREPOE\nCOME 0'))
  self.assertFalse(validar('comida',p,'REPOE\nCOME 1\nCOME 0'))
 def test_recurso_exclusao_e_finalizacao(self):
  p={'pessoas':3,'vezes':1,'capacidade':2}
  self.assertTrue(validar('recurso',p,'IN 0\nIN 1\nOUT 0\nIN 2\nOUT 1\nOUT 2'))
  self.assertFalse(validar('recurso',p,'IN 0\nIN 1\nIN 2\nOUT 0\nOUT 1\nOUT 2'))
 def test_filosofos_vizinhos(self):
  p={'pessoas':3,'vezes':1,'capacidade':3,'vizinhos':True}
  self.assertTrue(validar('recurso',p,'IN 0\nOUT 0\nIN 1\nOUT 1\nIN 2\nOUT 2'))
  self.assertFalse(validar('recurso',p,'IN 0\nIN 2\nOUT 0\nOUT 2\nIN 1\nOUT 1'))
 def test_barreira_geracoes(self):
  p={'pessoas':2,'rodadas':2}
  self.assertTrue(validar('barreira',p,'CHEGA 0 0\nCHEGA 0 1\nPASSA 0 0\nCHEGA 1 0\nPASSA 0 1\nCHEGA 1 1\nPASSA 1 1\nPASSA 1 0'))
  self.assertFalse(validar('barreira',p,'CHEGA 0 0\nPASSA 0 0'))
 def test_leitores_escritores(self):
  p={'leitores':1,'escritores':1,'vezes':1}
  self.assertTrue(validar('rw',p,'RI 0 0\nRF 0\nWI 0\nWF 0 1'))
  self.assertFalse(validar('rw',p,'RI 0 0\nWI 0\nWF 0 1\nRF 0'))
 def test_porto_capacidade_e_prioridade(self):
  p={'pessoas':2,'capacidade':1}
  ok='PE 0\nEN 0\nAT 0\nPS 0\nSA 0\nFI 0\nPE 1\nEN 1\nAT 1\nPS 1\nSA 1\nFI 1'
  self.assertTrue(validar('porto',p,ok))
  self.assertFalse(validar('porto',p,'PE 0\nEN 0\nAT 0\nPE 1\nEN 1'))
  self.assertFalse(validar('porto',{'pessoas':2,'capacidade':2},'PE 0\nEN 0\nAT 0\nPS 0\nPE 1\nEN 1'))
 def test_barbearia_fila_e_rejeicao(self):
  p={'operacoes':['A 0','A 1','A 2','T','T'],'cadeiras':1}
  self.assertTrue(validar('barbearia',p,'ATENDE 0\nESPERA 1\nRECUSA 2\nFIM 0\nATENDE 1\nFIM 1\nLIVRE'))
  self.assertFalse(validar('barbearia',p,'ATENDE 0\nESPERA 1\nESPERA 2'))
 def test_saida_malformada_e_extra(self):
  self.assertFalse(validar('recurso',{'pessoas':1,'vezes':1,'capacidade':1},'IN xyz'))
  self.assertFalse(validar('grafo',{'nos':['A'],'arestas':[]},'A\nB'))
if __name__=='__main__':unittest.main()
