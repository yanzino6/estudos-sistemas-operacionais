# Onde estão os modelos de resposta?

**Em `estudo/<parte>/respostas/<id>.c`.** São 49 arquivos C iniciais, um para cada questão. Abra o arquivo, implemente os TODOs e rode o comando indicado no cabeçalho. Não há mais resposta em JSON ou dissertação para entregar.

Exemplo, a partir da raiz `provaPratica`:

```bash
# Edite estudo/03_pipes/respostas/p01.c no seu editor.
python3 estudo/03_pipes/testes/p01/corrigir.py
```

O script sem argumentos encontra automaticamente seu arquivo. Você também pode passar o caminho de outro `.c` ou executável. Os modelos são **esqueletos, não soluções prontas**: inicialmente retornam 1 e reprovam; isso é esperado.

## Semáforos no Linux e macOS

Os modelos incluem `_infra/semaforo_portatil.h`, que fornece `so_sem_t`, `so_sem_init(&s, valor)`, `so_sem_wait(&s)`, `so_sem_post(&s)` e `so_sem_destroy(&s)`. Use esses nomes no lugar das funções POSIX diretas se quiser executar os exercícios de semáforos em ambos os sistemas. No Linux usa semáforos sem nome; no macOS usa semáforos nomeados com remoção imediata do nome. O adaptador é para threads **do mesmo processo**; não é memória compartilhada entre processos. Os exercícios de pipes usam processos e não dependem desse adaptador.

As funções encerram o processo em erro; a atividade é implementar o protocolo concorrente. Também é permitido usar diretamente a API POSIX quando seu ambiente a suporta. Não use sleep como sincronização.

## Índice de arquivos

| Arquivo | Exercício | Comando da raiz do projeto |
|---|---|---|
| [s01](01_semaforos/respostas/s01.c) | Cinco blocos ordenados por semáforos | `python3 estudo/01_semaforos/testes/s01/corrigir.py` |
| [s02](01_semaforos/respostas/s02.c) | Grafo de nove funções | `python3 estudo/01_semaforos/testes/s02/corrigir.py` |
| [s03](01_semaforos/respostas/s03.c) | Junção e bifurcação com semáforos | `python3 estudo/01_semaforos/testes/s03/corrigir.py` |
| [s04](01_semaforos/respostas/s04.c) | Pombo correio em lotes de 20 | `python3 estudo/01_semaforos/testes/s04/corrigir.py` |
| [s05](01_semaforos/respostas/s05.c) | Montanha-russa com desembarque | `python3 estudo/01_semaforos/testes/s05/corrigir.py` |
| [s06](01_semaforos/respostas/s06.c) | Canibais e cozinheiro com encerramento | `python3 estudo/01_semaforos/testes/s06/corrigir.py` |
| [s07](01_semaforos/respostas/s07.c) | Sessões de filme com término sincronizado | `python3 estudo/01_semaforos/testes/s07/corrigir.py` |
| [s08](01_semaforos/respostas/s08.c) | Base compartilhada com leitores e escritores | `python3 estudo/01_semaforos/testes/s08/corrigir.py` |
| [s09](01_semaforos/respostas/s09.c) | Barbeiro com semáforos | `python3 estudo/01_semaforos/testes/s09/corrigir.py` |
| [s10](01_semaforos/respostas/s10.c) | Cinco filósofos usando semáforos | `python3 estudo/01_semaforos/testes/s10/corrigir.py` |
| [s11](01_semaforos/respostas/s11.c) | Grafo da Lista 6 | `python3 estudo/01_semaforos/testes/s11/corrigir.py` |
| [s12](01_semaforos/respostas/s12.c) | Aquisição múltipla tudo-ou-nada | `python3 estudo/01_semaforos/testes/s12/corrigir.py` |
| [s13](01_semaforos/respostas/s13.c) | Agente e fumantes por rodadas | `python3 estudo/01_semaforos/testes/s13/corrigir.py` |
| [s14](01_semaforos/respostas/s14.c) | Buffer circular SPSC sem mutex | `python3 estudo/01_semaforos/testes/s14/corrigir.py` |
| [s15](01_semaforos/respostas/s15.c) | Reserva de recursos limitados | `python3 estudo/01_semaforos/testes/s15/corrigir.py` |
| [s16](01_semaforos/respostas/s16.c) | Contador protegido por semáforo | `python3 estudo/01_semaforos/testes/s16/corrigir.py` |
| [m01](02_monitores/respostas/m01.c) | Corrigir buffer para vários produtores e consumidores | `python3 estudo/02_monitores/testes/m01/corrigir.py` |
| [m02](02_monitores/respostas/m02.c) | Porto com prioridade de saída em C | `python3 estudo/02_monitores/testes/m02/corrigir.py` |
| [m03](02_monitores/respostas/m03.c) | Buffer com monitor Hoare implementado por semáforos | `python3 estudo/02_monitores/testes/m03/corrigir.py` |
| [m04](02_monitores/respostas/m04.c) | Barbearia encapsulada em monitor | `python3 estudo/02_monitores/testes/m04/corrigir.py` |
| [m05](02_monitores/respostas/m05.c) | Cinco filósofos usando monitor | `python3 estudo/02_monitores/testes/m05/corrigir.py` |
| [m06](02_monitores/respostas/m06.c) | Barreira reutilizável com condição | `python3 estudo/02_monitores/testes/m06/corrigir.py` |
| [m07](02_monitores/respostas/m07.c) | Buffer limitado e soma | `python3 estudo/02_monitores/testes/m07/corrigir.py` |
| [p01](03_pipes/respostas/p01.c) | Anel pai e dois filhos | `python3 estudo/03_pipes/testes/p01/corrigir.py` |
| [p02](03_pipes/respostas/p02.c) | Pipeline ping | grep | `python3 estudo/03_pipes/testes/p02/corrigir.py` |
| [p03](03_pipes/respostas/p03.c) | Avô, pai e neto | `python3 estudo/03_pipes/testes/p03/corrigir.py` |
| [p04](03_pipes/respostas/p04.c) | Ler um pipe até EOF | `python3 estudo/03_pipes/testes/p04/corrigir.py` |
| [p05](03_pipes/respostas/p05.c) | Dois filhos enviando registros | `python3 estudo/03_pipes/testes/p05/corrigir.py` |
| [p06](03_pipes/respostas/p06.c) | Pipeline de filtro e contagem | `python3 estudo/03_pipes/testes/p06/corrigir.py` |
| [p07](03_pipes/respostas/p07.c) | Pipe com escrita não bloqueante | `python3 estudo/03_pipes/testes/p07/corrigir.py` |
| [p08](03_pipes/respostas/p08.c) | FIFO com dois escritores e limpeza | `python3 estudo/03_pipes/testes/p08/corrigir.py` |
| [p09](03_pipes/respostas/p09.c) | Soma pelo processo filho | `python3 estudo/03_pipes/testes/p09/corrigir.py` |
| [t01](04_threads/respostas/t01.c) | Duas threads com a mesma rotina | `python3 estudo/04_threads/testes/t01/corrigir.py` |
| [t02](04_threads/respostas/t02.c) | Garantir término antes de sair da main | `python3 estudo/04_threads/testes/t02/corrigir.py` |
| [t03](04_threads/respostas/t03.c) | Argumentos por estrutura | `python3 estudo/04_threads/testes/t03/corrigir.py` |
| [t04](04_threads/respostas/t04.c) | Corrigir passagem de &i | `python3 estudo/04_threads/testes/t04/corrigir.py` |
| [t05](04_threads/respostas/t05.c) | Retorno de dados alocados pela thread | `python3 estudo/04_threads/testes/t05/corrigir.py` |
| [t06](04_threads/respostas/t06.c) | Soma dos dez primos em duas threads | `python3 estudo/04_threads/testes/t06/corrigir.py` |
| [t07](04_threads/respostas/t07.c) | Dez threads identificadas | `python3 estudo/04_threads/testes/t07/corrigir.py` |
| [t08](04_threads/respostas/t08.c) | Dois modos de criação e join | `python3 estudo/04_threads/testes/t08/corrigir.py` |
| [t09](04_threads/respostas/t09.c) | Soma paralela de dez inteiros | `python3 estudo/04_threads/testes/t09/corrigir.py` |
| [t10](04_threads/respostas/t10.c) | Impressão concorrente de primos | `python3 estudo/04_threads/testes/t10/corrigir.py` |
| [t11](04_threads/respostas/t11.c) | Partilhar heap entre trabalhadores | `python3 estudo/04_threads/testes/t11/corrigir.py` |
| [t12](04_threads/respostas/t12.c) | Quadrados retornados por threads | `python3 estudo/04_threads/testes/t12/corrigir.py` |
| [x01](05_mutex_e_paralelismo/respostas/x01.c) | Distribuição dinâmica com índice global seguro | `python3 estudo/05_mutex_e_paralelismo/testes/x01/corrigir.py` |
| [x02](05_mutex_e_paralelismo/respostas/x02.c) | Soma com reserva de índice protegida | `python3 estudo/05_mutex_e_paralelismo/testes/x02/corrigir.py` |
| [x03](05_mutex_e_paralelismo/respostas/x03.c) | Soma de mil elementos com N threads | `python3 estudo/05_mutex_e_paralelismo/testes/x03/corrigir.py` |
| [x04](05_mutex_e_paralelismo/respostas/x04.c) | Partição com vetor de tamanho variável | `python3 estudo/05_mutex_e_paralelismo/testes/x04/corrigir.py` |
| [x05](05_mutex_e_paralelismo/respostas/x05.c) | Trylock e sincronização sem sleep | `python3 estudo/05_mutex_e_paralelismo/testes/x05/corrigir.py` |
