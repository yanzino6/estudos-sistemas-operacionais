# 03 · Arquitetura e contratos

Este arquivo substitui a reunião de kickoff. **Os headers da seção 2 são o contrato:**
no marco M0, A copia todos para o repositório exatamente como estão aqui. A partir daí,
eles só mudam por PR com o rótulo `contrato`, aprovado pelos dois (arquivo 00, seção 3).
O pseudocódigo das seções 5 a 9 é o "como fazer" e não precisa ser seguido linha a
linha, mas **as regras marcadas com ⚠️ são obrigatórias**: cada uma corrige um erro
real encontrado na revisão.

## 1. Módulos e donos

Todos os fontes ficam **na raiz do repositório** (o professor roda `tar -xzvf` e `make` na mesma pasta).

| Arquivo | Dono | Responsabilidade |
|---|---|---|
| `main.c` | A | ordem de inicialização, `join`, gravação do arquivo e liberação de memória; define `g_cfg` e `g_pol` |
| `tempo.c/.h` | A | T0, `agora_ms`, `arred100`, `dormir_ate` |
| `config.h` | A | `Config`, enums de política e de modo de fila |
| `entrada.c/.h` | A | leitura do arquivo e ordenação por (chegada, PID) |
| `barreira.c/.h` | A | barreira reutilizável com líder (mutex + cond) |
| `pcb.c/.h` | A | PCB, TCB, `pcb_iniciar/destruir` e a thread trabalhadora (protocolo da fatia) |
| `geradora.c/.h` | A | thread geradora (chegadas em lote), `esperar_chegadas_ate` |
| `politica.c/.h` | A (bônus: B) | FCFS, RR e PRIORIDADE atrás de `Politica` |
| `escalonador.c/.h` | A | **o único laço de escalonador** (mono, GLOBAL e POR_CPU), `despachar` |
| `fila.c/.h` | B | lista duplamente encadeada intrusiva, sem travas internas |
| `cpus.c/.h` | B | estado das CPUs e **toda** a diferença entre GLOBAL e POR_CPU: travas, colocação, migração, ociosidade, término |
| `log.c/.h` | B | buffer de eventos e ordenação final |
| `metricas.c/.h` | B | bloco `=== METRICAS ===` |
| `Makefile` | B (A escreve a 1ª versão no M0) | alvos `monoprocessador`, `multiprocessador`, `clean`, `teste_unidade` |
| `tests/` | B (A: estresse) | casos, `compare.py`, `run_all.sh`, testes unitários |
| `ferramentas/` | A e B | oráculo, gerador de estresse, análise, empacotamento |

Dependências (quem chama quem): `main → entrada, tempo, politica, log, cpus, geradora, escalonador, metricas` · `escalonador → cpus, pcb, geradora, log, tempo` · `geradora → cpus, pcb, tempo` · `cpus → fila, politica, log` · `pcb → barreira, log, tempo`.

## 2. Headers (contrato)

```c
/* ===================== tempo.h ===================== */
#ifndef TEMPO_H
#define TEMPO_H
#define FATIA      100          /* ms */
#define T_INFINITO 0x3fffffff
void tempo_iniciar(void);       /* marca T0 com gettimeofday; chamar uma vez, logo após ler a entrada */
int  agora_ms(void);            /* ms reais desde T0 */
int  arred100(int ms);          /* múltiplo de 100 mais próximo; só para ferramentas e depuração */
void dormir_ate(int t_ms);      /* usleep em pedaços de no máximo 500 ms até agora_ms() >= t_ms */
#endif
```

```c
/* ===================== config.h ===================== */
#ifndef CONFIG_H
#define CONFIG_H
typedef enum { POL_FCFS, POL_RR, POL_PRIORIDADE } TipoPolitica;
typedef enum { FILA_GLOBAL, FILA_POR_CPU } ModoFila;
typedef struct {
    int num_cpus;               /* 1..8 (com -DMONO, sempre 1) */
    int quantum;                /* ms */
    TipoPolitica politica;
    ModoFila modo_fila;         /* com -DMONO, sempre FILA_GLOBAL */
    int n_processos;
} Config;
extern Config g_cfg;            /* definido em main.c; só leitura depois de entrada_ler */
#endif
```

```c
/* ===================== barreira.h ===================== */
#ifndef BARREIRA_H
#define BARREIRA_H
#include <pthread.h>
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int total;                  /* participantes */
    int chegaram;               /* quantos já chegaram nesta geração */
    unsigned geracao;           /* muda a cada abertura; evita wakeup espúrio e permite reuso */
} Barreira;
void barreira_iniciar(Barreira *b, int total);
int  barreira_esperar(Barreira *b);   /* devolve 1 para a ÚLTIMA thread a chegar (a líder) e 0 para as demais */
void barreira_destruir(Barreira *b);
#endif
```

```c
/* ===================== pcb.h ===================== */
#ifndef PCB_H
#define PCB_H
#include <pthread.h>
#include "barreira.h"
typedef enum { READY, RUNNING, FINISHED } ProcessState;
struct TCB;
typedef struct PCB {
    /* ---- campos do enunciado (§2.9), mesmos nomes ---- */
    int pid;
    int process_len;            /* duração total (ms) */
    int remaining_time;         /* tempo restante (ms) */
    int priority;               /* 1 (mais alta) a 5 */
    int num_threads;
    int start_time;             /* chegada (ms) */
    int cpu_id;                 /* CPU atual; -1 se nenhuma */
    int first_dispatch;         /* instante LÓGICO do 1º despacho; -1 se ainda não */
    int finish_time;            /* instante LÓGICO da finalização; -1 se ainda não */
    ProcessState state;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
    pthread_t *thread_ids;
    /* ---- extras (justificar no README) ---- */
    Barreira barreira;              /* alinha as threads no fim de cada fatia */
    pthread_cond_t cv_escalonador;  /* líder da fatia -> escalonador; par: mutex */
    int orcamento;                  /* fatias que ainda pode usar neste despacho */
    int fatias_no_despacho;         /* fatias executadas neste despacho */
    int t_despacho;                 /* instante lógico do despacho atual */
    int ultima_cpu;                 /* CPU do último despacho (-1 = nunca); usado no bônus */
    struct TCB *tcbs;               /* vetor de num_threads TCBs (liberado no fim) */
    /* ---- nó intrusivo da fila: só mexe quem segura o mutex da fila onde o PCB está ---- */
    struct PCB *ant, *prox;
    long seq;                       /* ordem de entrada na fila (desempate FIFO da prioridade) */
} PCB;
typedef struct TCB { PCB *pcb; int thread_index; } TCB;
#define ORCAMENTO_ILIMITADO 0x3fffffff
void  pcb_iniciar(PCB *p);          /* chamar depois de preencher pid..start_time */
void  pcb_destruir(PCB *p);         /* chamar depois do join das threads do processo */
void *thread_trabalhadora(void *arg_tcb);
#endif
```

```c
/* ===================== fila.h ===================== */
#ifndef FILA_H
#define FILA_H
#include <pthread.h>
#include "pcb.h"
typedef struct {
    PCB *inicio, *fim;
    int tamanho;
    long proximo_seq;
    pthread_mutex_t mutex;
} Fila;
void fila_iniciar(Fila *f);
void fila_destruir(Fila *f);
/* NENHUMA função abaixo trava: o chamador já segura f->mutex. */
void fila_inserir_fim(Fila *f, PCB *p);   /* p->seq = f->proximo_seq++ */
void fila_remover(Fila *f, PCB *p);       /* início, meio ou fim, em O(1) */
PCB *fila_remover_inicio(Fila *f);        /* NULL se vazia */
PCB *fila_remover_fim(Fila *f);           /* NULL se vazia (usada na migração) */
#endif
```

```c
/* ===================== politica.h ===================== */
#ifndef POLITICA_H
#define POLITICA_H
#include "config.h"
#include "fila.h"
typedef struct {
    const char *nome;                     /* "FCFS", "RR" ou "PRIORIDADE": vai no log */
    int usa_quantum;                      /* 0 no FCFS */
    PCB *(*escolher)(Fila *f, int cpu);   /* chamador segura f->mutex; remove e devolve o escolhido; NULL se vazia */
} Politica;
const Politica *politica_obter(TipoPolitica t);
extern const Politica *g_pol;             /* definido em main.c */
#endif
```

```c
/* ===================== log.h ===================== */
#ifndef LOG_H
#define LOG_H
#include <stdio.h>
enum { EV_CHEGADA = 0, EV_FINALIZADO = 1, EV_OUTRO = 2, EV_TERMINO = 3 };
void log_iniciar(const char *nome_politica);
/* cpu = -1: linha sem "[CPUk]". fmt é só a mensagem, ex.: "Executando PID %d".
 * t_logico = instante teórico do evento (múltiplo de 100), usado para ORDENAR;
 * a linha impressa usa o tempo medido agora_ms(). */
void log_registrar(int t_logico, int cpu, int categoria, const char *fmt, ...);
void log_gravar(FILE *saida);   /* ordena por (t_logico, categoria, ordem de registro) e escreve */
void log_destruir(void);
#endif
```

```c
/* ===================== cpus.h ===================== */
#ifndef CPUS_H
#define CPUS_H
#include <pthread.h>
#include "pcb.h"
#ifndef CARGA_CONTA_RETENCAO
#define CARGA_CONTA_RETENCAO 1      /* ponto em aberto P1 (arquivo 02, seção 9) */
#endif
typedef struct {
    int indice;
    pthread_t tid;
    /* --- protegidos pela trava de filas (cpus_travar) --- */
    pthread_cond_t cond;        /* par: mutex da fila desta CPU (GLOBAL: o da fila global) */
    int ociosa;                 /* dormindo em cond, à espera de trabalho */
    PCB *entregue;              /* processo escolhido PARA ela por quem a acordou (entrega direta) */
    int t_entrega;              /* instante lógico dessa entrega = instante do despacho */
    PCB *executando;            /* processo despachado ou entregue, inclusive durante a retenção */
    int retida;                 /* 1 durante a retenção ociosa (RR/PRIORIDADE) */
    /* --- só a própria thread escalonadora escreve; main lê depois do join --- */
    int ultimo_pid;             /* -1 no início */
    int ocupado_ms;             /* soma de fatias x 100 */
    int trocas;                 /* trocas de contexto nesta CPU */
} CPU;
extern CPU *g_cpus;             /* vetor de g_cfg.num_cpus posições, criado em cpus_iniciar */

void cpus_iniciar(void);        /* usa g_cfg e g_pol */
void cpus_destruir(void);
/* GLOBAL: trava o mutex da fila global. POR_CPU: trava TODAS as filas, em ordem crescente. */
void cpus_travar(void);
void cpus_destravar(void);
/* ---- as funções abaixo exigem cpus_travar() já feito ---- */
PCB *cpus_tirar_proximo(int k, int t);  /* política na fila da CPU k; no POR_CPU, com fila vazia, migra
                                           (registra "Migracao" em t). Se devolver p: executando=p e
                                           cpus_executando++ */
void cpus_inicio_retencao(int k);       /* retida = 1 */
void cpus_devolver(int k, PCB *p);      /* fim do despacho: executando=NULL, retida=0, cpus_executando--;
                                           se p != NULL, reinsere p no fim da fila da CPU k */
void cpus_entregar_ociosas(int t);      /* para cada CPU ociosa, em ordem crescente de índice: escolhe por ela
                                           (cpus_tirar_proximo), guarda em entregue/t_entrega e dá signal */
PCB *cpus_esperar_trabalho(int k, int *t); /* dorme até receber uma entrega (devolve o PCB e põe o instante
                                           em *t) ou até encerrar (devolve NULL); volta com cpus_travar() feito */
int  cpus_verificar_termino(int t);     /* se terminou: encerrar=1, registra o término (uma vez) em t,
                                           acorda todas; devolve encerrar */
int  cpus_encerrado(void);
/* ---- chamada pela geradora SEM nenhuma trava ---- */
void cpus_inserir_lote(PCB *lote, int n, int t, int ultimo);
     /* trava; insere os n processos (já em ordem de PID; no POR_CPU, cada um na CPU de menor
        carga); registra cada "Chegada" em t; se ultimo, generator_done=1; entrega às ociosas; destrava */
#endif
```

```c
/* ===================== geradora.h ===================== */
#ifndef GERADORA_H
#define GERADORA_H
#include "pcb.h"
void  geradora_iniciar(PCB *pcbs, int n);  /* ANTES de criar qualquer thread: proxima_chegada = pcbs[0].start_time */
void *thread_geradora(void *arg);           /* arg ignorado */
void  esperar_chegadas_ate(int t);          /* bloqueia até toda chegada com start_time <= t estar na fila */
void  geradora_destruir(void);
#endif
```

```c
/* ===================== entrada.h / escalonador.h / metricas.h ===================== */
int   entrada_ler(const char *caminho, Config *cfg, PCB **pcbs);
      /* devolve n (ou -1 se não abrir). Vetor alocado, ordenado por (start_time, pid), com
         pcb_iniciar já chamado. Com -DMONO: num_cpus = 1 e modo_fila = FILA_GLOBAL. */
void *thread_escalonador(void *arg_cpu);    /* arg = CPU* */
void  metricas_gravar(FILE *saida, const PCB *pcbs, int n, const CPU *cpus, int num_cpus);
```

```make
# ===================== Makefile (1ª versão, escrita por A no M0) =====================
CC     = gcc
CFLAGS = -Wall -pthread -O2 $(CFLAGS_EXTRA)
ifeq ($(AFINIDADE),1)
CFLAGS += -DAFINIDADE
endif
SRC = main.c tempo.c barreira.c entrada.c pcb.c geradora.c politica.c escalonador.c \
      fila.c cpus.c log.c metricas.c

.PHONY: monoprocessador multiprocessador clean teste_unidade

monoprocessador:
	$(CC) $(CFLAGS) -DMONO -o trabSO $(SRC)

multiprocessador:
	$(CC) $(CFLAGS) -o trabSO $(SRC)

clean:
	rm -f trabSO log_execucao_minikernel.txt tests/bin/*
```
- **Nunca** `-Werror` no Makefile entregue. Para treinar localmente: `make multiprocessador CFLAGS_EXTRA=-Werror`.
- Compilar tudo de uma vez evita misturar objetos de um alvo com o outro.
- Não usar `-std=c11` sozinho: ele esconde `usleep`. Se quiser fixar o padrão, use `-std=gnu11`.

## 3. Modelo de tempo ⚠️

Todo evento acontece num múltiplo de 100 ms. Por isso, cada evento tem um **instante
lógico** exato, calculado e não medido:

| Evento | Instante lógico |
|---|---|
| chegada | `start_time` |
| despacho logo após um fim de despacho | o instante em que a CPU foi liberada (`t_lib`) |
| despacho de uma CPU que estava ociosa | `t_entrega`: o instante de quem fez a entrega (a chegada ou a liberação da outra CPU) |
| fim da k-ésima fatia de um despacho | `t_despacho + 100·k` |
| finalização | fim da fatia em que `remaining_time` chegou a 0 |
| fim de quantum | `t_despacho + Q` |
| fim da retenção | `t_despacho + Q` |
| término | `t_lib` da última CPU liberada |

Regras:
1. ⚠️ **Prazos absolutos.** Toda espera é `dormir_ate(instante lógico)`, e nunca "dormir X ms a partir de agora". Assim, o atraso de uma fatia não se soma ao da próxima (erro R5).
2. ⚠️ **Quem registra o evento passa o instante lógico** para `log_registrar`. O log ordena por ele e imprime o tempo medido (`agora_ms()` no momento do registro), que é o que o enunciado pede.
3. ⚠️ `first_dispatch` e `finish_time` guardam **instantes lógicos**. As métricas saem exatas e iguais ao esperado (documentar no README).
4. ⚠️ `dormir_ate` nunca chama `usleep` com 1 s ou mais (erro R4).

```
[tempo.c]
dormir_ate(t):
    repita:
        resto ← t − agora_ms()
        se resto ≤ 0: sair
        usleep(min(resto, 500) × 1000)
```

## 4. Travas, condições e quem protege o quê ⚠️

**Ordem global (a frase vai para o README):**
`filas (em ordem crescente de índice) → log` e `processo → log`.
**Fila e processo nunca são travados ao mesmo tempo.** `mutex_chegadas` e o mutex da
barreira são folhas: sempre travados sozinhos. Com uma ordem fixa, não existe ciclo de
espera, e portanto não existe deadlock.

| Quem | Travas, nesta ordem | Observação |
|---|---|---|
| thread trabalhadora | processo; só a líder, ao finalizar: processo → log | dorme a fatia **sem trava**; chama `barreira_esperar` sem trava |
| geradora | `cpus_travar` (filas) → log; depois, sozinho, `mutex_chegadas` | cria as threads **antes** de travar |
| escalonador, escolhendo e reinserindo | `cpus_travar` (filas) → log (migração) | solta as filas **antes** de despachar |
| escalonador, despachando | processo → log | nunca segura fila aqui |
| escalonador, `esperar_chegadas_ate` | só `mutex_chegadas` | sem nenhuma outra trava |

**Pares condição/mutex (cada condição tem exatamente um mutex parceiro):**
`p->cv` e `p->cv_escalonador` → `p->mutex` · `cpu.cond` → mutex da fila daquela CPU
(no GLOBAL, todas as CPUs usam o mutex da fila global) · `cv_chegadas` →
`mutex_chegadas` · `barreira.cond` → `barreira.mutex`.

**Três regras de todo o código:** todo `pthread_cond_wait` fica dentro de um `while
(condição)` · nenhum `usleep` com trava de fila ou de processo segurada · quem muda uma
condição muda sob o mutex parceiro e acorda em seguida (sem lost wakeup).

**Proteção dos campos:**
| Campo | Protegido por |
|---|---|
| `pid, process_len, priority, num_threads, start_time` | nada: imutáveis depois da leitura |
| `remaining_time, state, cpu_id, first_dispatch, finish_time, orcamento, fatias_no_despacho, t_despacho, ultima_cpu` | `p->mutex` (`ultima_cpu` é lido pela política sob a trava da fila, mas só é escrito enquanto o processo **não** está em fila nenhuma) |
| `ant, prox, seq` | mutex da fila em que o PCB está |
| `CPU.ociosa, entregue, t_entrega, executando, retida`, `cpus_executando`, `generator_done`, `encerrar` | `cpus_travar` (GLOBAL: o mutex global; POR_CPU: todos) |
| `CPU.ultimo_pid, ocupado_ms, trocas` | só a própria thread escalonadora escreve; `main` lê depois do `join` |
| `proxima_chegada` | `mutex_chegadas` |

## 5. Protocolo da fatia (thread trabalhadora) [A, `pcb.c`]

```
thread_trabalhadora(tcb):
    p ← tcb.pcb;  i ← tcb.thread_index;  n ← p.num_threads
    parte ← 100 / n + (i < 100 mod n ? 1 : 0)
    repita:
        travar p.mutex
        enquanto p.state = READY: esperar(p.cv, p.mutex)
        se p.state = FINISHED: destravar; sair do laço
        prazo ← p.t_despacho + (p.fatias_no_despacho + 1) × FATIA      ⚠️ prazo absoluto
        destravar p.mutex

        dormir_ate(prazo)                          (a fatia; usleep sem trava nenhuma)

        travar p.mutex;  p.remaining_time −= parte;  destravar

        se barreira_esperar(p.barreira) = 1:       (1ª barreira; só a líder entra aqui)
            travar p.mutex
            p.fatias_no_despacho += 1;  p.orcamento −= 1
            t_fim ← p.t_despacho + p.fatias_no_despacho × FATIA
            se p.remaining_time = 0:
                p.state ← FINISHED;  p.finish_time ← t_fim
                log_registrar(t_fim, p.cpu_id, EV_FINALIZADO, "PID %d finalizado", p.pid)
                broadcast(p.cv);  signal(p.cv_escalonador)
            senão se p.orcamento = 0:
                p.state ← READY                      (fim do quantum)
                signal(p.cv_escalonador)
            destravar p.mutex
        barreira_esperar(p.barreira)                (2ª barreira: ninguém volta ao topo antes da decisão)
```
- **Por que duas barreiras.** Sem a 2ª, uma thread não líder voltaria ao topo, veria RUNNING e começaria uma fatia a mais antes de a líder gravar READY.
- **Por que só a líder decide.** Assim ninguém registra "finalizado" duas vezes nem desconta o orçamento duas vezes.
- **Por que `remaining_time` nunca fica negativo.** As durações são múltiplas de 100, e cada fatia desconta exatamente 100 no total.
- **A líder testa `remaining_time = 0` antes do orçamento.** Por isso, quem termina exatamente no fim do quantum só gera "finalizado" [L].
- Nunca sair do laço sem passar pelas duas barreiras: as outras threads ficariam presas.

Barreira [A, `barreira.c`]:
```
barreira_esperar(b):
    travar b.mutex
    g ← b.geracao;  b.chegaram += 1
    se b.chegaram = b.total:
        b.chegaram ← 0;  b.geracao += 1;  broadcast(b.cond);  destravar;  devolver 1
    enquanto g = b.geracao: esperar(b.cond, b.mutex)
    destravar;  devolver 0
```

## 6. Escalonador: um único laço para mono, GLOBAL e POR_CPU [A, `escalonador.c`]

```
despachar(cpu, p, t) → (resultado, t_fim):
    se p.pid ≠ cpu.ultimo_pid: cpu.trocas += 1          (mesmo PID reescolhido não conta)
    cpu.ultimo_pid ← p.pid
    travar p.mutex
    p.state ← RUNNING;  p.cpu_id ← cpu.indice;  p.ultima_cpu ← cpu.indice
    p.orcamento ← (g_pol.usa_quantum ? g_cfg.quantum / FATIA : ORCAMENTO_ILIMITADO)
    p.fatias_no_despacho ← 0;  p.t_despacho ← t
    se p.first_dispatch < 0: p.first_dispatch ← t
    log_registrar(t, cpu.indice, EV_OUTRO, "Executando PID %d", p.pid)
    broadcast(p.cv)
    enquanto p.state = RUNNING: esperar(p.cv_escalonador, p.mutex)
    resultado ← p.state;  fatias ← p.fatias_no_despacho
    destravar p.mutex
    cpu.ocupado_ms += fatias × FATIA                   (a retenção fica de fora automaticamente)
    devolver (resultado, t + fatias × FATIA)

thread_escalonador(cpu):
    k ← cpu.indice;  t ← 0
    cpus_travar()
    repita:
        p ← cpus_tirar_proximo(k, t)                   (no POR_CPU pode registrar "Migracao")
        se p = NULO:
            se cpus_verificar_termino(t): sair do laço
            p ← cpus_esperar_trabalho(k, &t)           (volta com a trava; t ← instante da entrega)
            se p = NULO: sair do laço                  (encerrou)
        cpus_entregar_ociosas(t)                       (só DEPOIS de escolher: regra do GLOBAL)
        cpus_destravar()

        (res, t_fim) ← despachar(cpu, p, t)
        t_lib ← t_fim
        se res = FINISHED e g_pol.usa_quantum e t_fim < t + g_cfg.quantum:
            t_lib ← t + g_cfg.quantum                  (CPU retida e ociosa até o fim do quantum)
            cpus_travar();  cpus_inicio_retencao(k);  cpus_destravar()
            dormir_ate(t_lib)
        esperar_chegadas_ate(t_lib)                    ⚠️ R1: SEMPRE, qualquer que seja res
        se res = READY:
            log_registrar(t_lib, k, EV_OUTRO, "Quantum esgotado PID %d", p.pid)
        cpus_travar()
        cpus_devolver(k, res = READY ? p : NULO)       (reinsere no fim da fila da CPU)
        t ← t_lib
        (volta ao topo SEM soltar a trava: reinserir e escolher na mesma seção crítica)
    cpus_destravar()
```

Por que esse laço cumpre cada regra:
- **Regra 2 e R1:** `esperar_chegadas_ate(t_lib)` garante que todas as chegadas até `t_lib` já estão na fila antes da reinserção e da escolha.
- **Reinserir e escolher na mesma seção crítica:** entre `cpus_devolver` e `cpus_tirar_proximo`, a trava não é solta. `cpus_entregar_ociosas` só roda depois da escolha, e por isso nenhuma ociosa "rouba" o processo preemptado.
- **Regra 3:** as ociosas recebem trabalho em ordem crescente de índice, escolhido por quem as acorda. Elas não disputam a trava (erro R9).
- **Prioridade:** o processo atual volta ao fim da fila **antes** da escolha e concorre com os outros. Se for o único com a melhor prioridade, é reescolhido. Nesse caso saem `Quantum esgotado` e `Executando` com o mesmo PID, sem troca de contexto.
- **Retenção:** a CPU continua com `executando ≠ NULO` e ninguém despacha nela até `t_lib`. A utilização não conta a retenção, porque só soma fatias.
- **Término:** quem solta o último processo verifica o término em `t_lib`, que é o fim da retenção quando ela existe [C 05: 2800].
- **FCFS:** o orçamento é ilimitado, então o processo só sai do RUNNING ao terminar.

## 7. `cpus.c`: GLOBAL e POR_CPU [B]

Estado interno (`static`): `Fila *filas; int nfilas` (1 no GLOBAL, `num_cpus` no
POR_CPU), `cpus_executando`, `generator_done`, `encerrar`. `fila_da_cpu(k) =
&filas[nfilas == 1 ? 0 : k]`. Mono é GLOBAL com 1 CPU.

```
cpus_travar():   para q de 0 até nfilas−1: travar filas[q].mutex
cpus_destravar(): para q de nfilas−1 até 0: destravar filas[q].mutex

carga(k):   filas[k].tamanho + (g_cpus[k].executando ≠ NULO E (CARGA_CONTA_RETENCAO OU NÃO g_cpus[k].retida) ? 1 : 0)

cpus_tirar_proximo(k, t):
    p ← g_pol.escolher(fila_da_cpu(k), k)
    se p = NULO e nfilas > 1:                                  (POR_CPU: migração)
        origem ← índice j ≠ k de MAIOR filas[j].tamanho > 0; empate: menor j
        se existe origem:
            p ← fila_remover_fim(filas[origem])                (rouba o ÚLTIMO)
            log_registrar(t, k, EV_OUTRO, "Migracao PID %d CPU%d -> CPU%d", p.pid, origem, k)
    se p ≠ NULO: g_cpus[k].executando ← p;  cpus_executando += 1
    devolver p

cpus_inicio_retencao(k):  g_cpus[k].retida ← 1
cpus_devolver(k, p):
    g_cpus[k].executando ← NULO;  g_cpus[k].retida ← 0;  cpus_executando −= 1
    se p ≠ NULO: fila_inserir_fim(fila_da_cpu(k), p)          (não acorda ninguém aqui)

cpus_entregar_ociosas(t):                                      ⚠️ R9: entrega direta (regra 3)
    repita:
        mudou ← 0
        para k de 0 até num_cpus−1:                            (menor índice primeiro)
            c ← g_cpus[k]
            se c.ociosa E c.entregue = NULO:
                p ← cpus_tirar_proximo(k, t)                   (GLOBAL: pela política; POR_CPU: própria fila
                                                                ou migração, que já registra "Migracao" em t)
                se p ≠ NULO: c.entregue ← p;  c.t_entrega ← t;  signal(c.cond);  mudou ← 1
    até mudou = 0                                              (no POR_CPU, uma entrega pode liberar outra)

cpus_esperar_trabalho(k, *t):                                  ⚠️ R8
    c ← g_cpus[k];  m ← fila_da_cpu(k).mutex
    c.ociosa ← 1
    se nfilas > 1: destravar todas as filas EXCETO a de k, em ordem decrescente
    enquanto c.entregue = NULO E NÃO encerrar: esperar(c.cond, m)
    c.ociosa ← 0
    p ← c.entregue;  c.entregue ← NULO;  *t ← c.t_entrega
    se nfilas > 1: destravar m;  cpus_travar()                 (solta m ANTES de retravar tudo: respeita a ordem)
    devolver p                                                 (NULO = encerrou)

cpus_verificar_termino(t):
    se NÃO encerrar E generator_done E cpus_executando = 0 E todas as filas vazias:
        encerrar ← 1
        log_registrar(t, −1, EV_TERMINO, "Escalonador terminou execução de todos processos")
        para cada k: broadcast(g_cpus[k].cond)
    devolver encerrar

cpus_inserir_lote(lote, n, t, ultimo):                         ⚠️ R2
    cpus_travar()
    para i de 0 até n−1:                                       (lote já em ordem crescente de PID)
        destino ← (nfilas = 1 ? 0 : k de menor (carga(k), k))
        fila_inserir_fim(filas[destino], lote[i])
        log_registrar(t, −1, EV_CHEGADA, "Chegada PID %d", lote[i].pid)
    se ultimo: generator_done ← 1
    cpus_entregar_ociosas(t)
    se ultimo: cpus_verificar_termino(t)                       (só importa se n_processos = 0)
    cpus_destravar()
```
- **Lost wakeup.** A CPU ociosa dorme segurando só o mutex da sua fila. Quem entrega segura esse mutex também (no POR_CPU, segura todos). Então, ou ela já está em `wait` e recebe o sinal, ou ainda vai testar `entregue` e vai encontrá-lo preenchido.
- **Por que a entrega já marca `executando`.** `cpus_tirar_proximo` marca `executando` e soma `cpus_executando` na hora da entrega. Assim, entre o `signal` e o despacho, ninguém conclui que acabou nem coloca outro processo nessa CPU.
- **Por que travar todas no POR_CPU.** O contador `cpus_executando`, o campo `executando` (carga), a colocação, a migração, a entrega e o término dependem de várias filas ao mesmo tempo. Com no máximo 8 mutexes, o custo é de microssegundos, e o helgrind fica limpo. Cada CPU continua com sua fila e seu mutex, como manda o enunciado (justificar no README).
- **Sem migração falsa.** Uma ociosa tem carga 0, então uma chegada vai para a ociosa de menor índice. Na entrega, cada CPU tenta primeiro a própria fila (`cpus_tirar_proximo`), em ordem de índice. Isso reproduz exatamente o passo "CPUs ociosas tentam de novo a cada inserção" do oráculo.

## 8. Geradora [A, `geradora.c`]

```
geradora_iniciar(pcbs, n): proxima_chegada ← (n > 0 ? pcbs[0].start_time : T_INFINITO); iniciar mutex e cond

thread_geradora():
    se n = 0: cpus_inserir_lote(NULO, 0, 0, 1); terminar
    i ← 0
    enquanto i < n:
        t ← pcbs[i].start_time
        dormir_ate(t)
        j ← i
        enquanto j < n E pcbs[j].start_time = t:              ⚠️ R2: lote do mesmo instante
            para x de 0 até pcbs[j].num_threads − 1:
                pcbs[j].tcbs[x] ← (pcb = &pcbs[j], thread_index = x)
                pthread_create(&pcbs[j].thread_ids[x], thread_trabalhadora, &pcbs[j].tcbs[x])
            j += 1
        cpus_inserir_lote(&pcbs[i], j − i, t, j = n)
        travar mutex_chegadas
        proxima_chegada ← (j < n ? pcbs[j].start_time : T_INFINITO)
        broadcast(cv_chegadas)
        destravar mutex_chegadas
        i ← j

esperar_chegadas_ate(t):
    travar mutex_chegadas
    enquanto proxima_chegada ≤ t: esperar(cv_chegadas, mutex_chegadas)
    destravar
```
- As threads são criadas antes da inserção. Elas testam o **estado** (e não o sinal), então nunca perdem o despacho.
- A espera em `esperar_chegadas_ate` dura no máximo o atraso da geradora (poucos ms).

## 9. Log e métricas [B]

```
[log.c]
Evento { int t_logico, t_real, categoria; long ordem; char texto[128]; }
log_registrar(t_logico, cpu, categoria, fmt, ...):
    msg ← vsnprintf(fmt, ...)                       (fora da trava)
    travar mutex_log
    t_real ← agora_ms()
    se qtd = cap: cap ← 2·cap; realloc
    e ← eventos[qtd];  e.(t_logico, t_real, categoria, ordem) ← (t_logico, t_real, categoria, qtd)
    e.texto ← cpu ≥ 0 ? "[t=%dms] [%s] [CPU%d] %s" : "[t=%dms] [%s] %s"  com (t_real, politica, [cpu,] msg)
    qtd += 1
    destravar
log_gravar(f): qsort por (t_logico, categoria, ordem); fputs(texto + "\n") de cada um
```
A categoria cria as faixas chegadas → finalizações → resto → término. Dentro de "resto",
a ordem de registro mantém `Quantum esgotado`/`Migracao` antes do `Executando` da mesma
CPU (é a mesma thread que registra os dois, nessa ordem).

```
[metricas.c]
metricas_gravar(f, pcbs, n, cpus, ncpu):
    ord ← cópia dos ponteiros, ordenada por pid
    makespan ← maior finish_time
    para cada p em ord:
        ta ← finish_time − start_time;  es ← ta − process_len;  re ← first_dispatch − start_time
        "PID %d | espera=%dms | turnaround=%dms | resposta=%dms"
    "Espera media: %.2f ms" (soma/(double)n) · "Turnaround medio: %.2f ms" · "Resposta media: %.2f ms"
    "Trocas de contexto: %d" (soma das CPUs)
    para cada k: "Utilizacao CPU%d: %.2f%%" (100.0 · ocupado_ms / makespan)
    "Throughput: %.2f processos/s" (n / (makespan / 1000.0))
    "Makespan: %d ms"
    (proteger contra makespan = 0)
```

## 10. `main` [A]

```
main(argc, argv):
    se argc ≠ 2: devolver 1                              (sem imprimir nada)
    n ← entrada_ler(argv[1], &g_cfg, &pcbs);  se n < 0: devolver 1
    tempo_iniciar()                                      (T0: logo após a leitura)
    g_pol ← politica_obter(g_cfg.politica)
    log_iniciar(g_pol.nome);  cpus_iniciar();  geradora_iniciar(pcbs, n)
    criar a geradora;  para cada k: criar thread_escalonador(&g_cpus[k])
    join da geradora;  join de cada escalonador;  join de cada thread de cada processo
    f ← fopen("log_execucao_minikernel.txt", "w");  log_gravar(f);  metricas_gravar(f, ...);  fclose(f)
    log_destruir();  cpus_destruir();  geradora_destruir()
    para cada p: pcb_destruir(p);  free(pcbs)
    devolver 0
```
