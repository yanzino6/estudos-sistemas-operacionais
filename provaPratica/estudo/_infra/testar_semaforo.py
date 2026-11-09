import pathlib,subprocess,tempfile,unittest
ROOT=pathlib.Path(__file__).resolve().parent
class SemaforoTest(unittest.TestCase):
 def test_bloqueio_e_exclusao_em_threads_reais(self):
  c=r'''#include "semaforo_portatil.h"
#include <pthread.h>
#include <stdio.h>
so_sem_t inicio, m; int total=0;
void *rotina(void *p){(void)p;so_sem_wait(&inicio);for(int i=0;i<10000;i++){so_sem_wait(&m);total++;so_sem_post(&m);}return NULL;}
int main(void){pthread_t t[4];so_sem_init(&inicio,0);so_sem_init(&m,1);for(int i=0;i<4;i++)if(pthread_create(&t[i],NULL,rotina,NULL))return 2;for(int i=0;i<4;i++)so_sem_post(&inicio);for(int i=0;i<4;i++)pthread_join(t[i],NULL);so_sem_destroy(&inicio);so_sem_destroy(&m);printf("%d\n",total);return 0;}
'''
  with tempfile.TemporaryDirectory() as d:
   p=pathlib.Path(d);(p/'test.c').write_text(c)
   r=subprocess.run(['cc','-std=c11','-D_POSIX_C_SOURCE=200809L','-pthread','-I',str(ROOT),str(p/'test.c'),'-o',str(p/'test')],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
   r=subprocess.run([str(p/'test')],capture_output=True,text=True,timeout=15)
   self.assertEqual((r.returncode,r.stdout),(0,'40000\n'))
if __name__=='__main__':unittest.main()
