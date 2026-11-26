# Trilha B (dupla) · infraestrutura, multiprocessador, bônus e entrega

Este é o **único arquivo que você precisa abrir** para implementar a sua metade e para
entender o trabalho inteiro, inclusive a metade de A. Ele supõe que você já leu o
enunciado e o `LEIA-ME` dos casos; não repete o escopo, mas relembra a regra exata no
ponto em que ela vira código. Fontes dos lembretes: **[E §x]** = seção do enunciado,
**[L]** = LEIA-ME dos casos, **[C nn]** = caso público que prova a regra.

Como a trilha está organizada:
- **Etapas B0 a B12**, na ordem em que você implementa. Cada uma tem: *Lembretes do enunciado*, *Contrato* (o `.h`), *Pseudocódigo*, *Por que é assim*, *Armadilhas*, *Teste* e *Pronto quando*. O pseudocódigo foi validado: uma implementação feita linha a linha a partir dele passa nos 6 casos públicos, nos 14 casos extras, no valgrind e no helgrind. As linhas com ⚠️ são as que, se mudarem, quebram algum caso.
- **Paradas 1 e 2**: o código de A explicado (contrato, pseudocódigo e porquês). Você não implementa essas partes, mas o seu `cpus.c` existe para servi-las, você revisa os PRs e pode ter de explicar tudo numa entrevista.
- **Apêndices**: pontos em aberto, tabela "sintoma → causa" e perguntas de entrevista.

**Índice:** 0. Calendário e combinados · 1. O desenho em uma página · 2. Ambiente e ferramentas · B0 comparador · B1 `fila.c` · B2 `log.c` · B3 `teste_unidade` · *Parada 1: o lado de A (PCB e fatia, tempo, política, geradora, escalonador)* · B4 `cpus.c` · B5 `metricas.c` · B6 `run_all.sh` (M1-B) · B7 revisão do M2 · B8 multi GLOBAL (M3) · B9 POR_CPU (M4) · B10 bônus · B11 README (M5) · B12 congelamento (M6) e entrega · *Parada 2: barreira, entrada, `main`, estresse, análise* · Apêndices A (pontos em aberto), B (sintoma → causa), C (entrevista)

Seus arquivos: `Makefile` (a partir do M0), `fila.c`, `log.c`, `cpus.c`, `metricas.c`, `tests/compare.py`, `tests/run_all.sh`, `tests/test_fila.c`, `tests/test_log.c`, `tests/test_cpus.c`, `tests/test_metricas.c`, `ferramentas/medir_afinidade.sh`, `ferramentas/empacotar.sh`, o trecho `/* BONUS AFINIDADE */` de `politica.c`, `ANALISE_AFINIDADE.md` e, no README, as seções "Decisões de projeto" e "Bônus: afinidade" e o parágrafo GLOBAL × POR_CPU.

---

## 0. Calendário e combinados

| Marco | Prazo | A (Yan) entrega | B (dupla) entrega |
|---|---|---|---|
| **M0** | seg 23/11, 14h | repositório, contrato (`.h`), stubs, Makefile, casos e ferramentas | (adianta `compare.py`, que não depende do repositório) |
| | seg 23/11, noite | `tempo.c`, `barreira.c`, `entrada.c`, `pcb.c` + testes | `fila.c`, `log.c` + testes, **mesclados ainda na segunda** |
| **M1** | ter 24/11, 23h | `politica.c`, `geradora.c`, `escalonador.c`, `main.c` (PR `M1-A`) | `cpus.c`, `metricas.c`, `compare.py`, `run_all.sh` (PR `M1-B`) |
| **M2** | qua 25/11, 23h | **integra**: casos 01, 03 e 05 passando no mono | revisa o PR; põe os casos extras no `run_all.sh` |
| **M3** | sex 27/11, 23h | gerador de estresse, verificador de invariantes, helgrind | **multi GLOBAL**: casos 13, 15, 17, ex45 e g01 a g06 |
| **M4** | dom 29/11, 23h | helgrind e estresse com POR_CPU e 8 CPUs | **POR_CPU**: casos p01 a p07 |
| **M5** | seg 30/11, 23h | análise comparativa no README | bônus de afinidade, `ANALISE_AFINIDADE.md`, "Decisões de projeto" no README |
| **M6** | ter 01/12, 23h | revisa; teste "de professor" na quarta 02/12 | código congelado, tag `v1.0`, `empacotar.sh` |
| Entrega | qui 03/12 | envia o `.tar.gz` no Classroom | baixa o arquivo enviado e confere |

**Prazo oficial: sexta 04/12/2026, 07h59, sem aceitação com atraso.** A meta interna é quinta.
Caminho crítico: `M0 → M1 → M2 (mono) → M3 (GLOBAL) → M4 (POR_CPU) → M6`. Se um marco atrasar mais de 24 h, o outro assume a próxima tarefa do atrasado e registra no `STATUS.md`. **O bônus é o primeiro a ser cortado; o helgrind nunca é cortado.**

Combinados (substituem reunião):
- **Repositório privado no GitHub**, fora da pasta `estudos-sistemas-operacionais` (ela já é um repositório git; não crie um dentro do outro). A `main` **sempre compila nos dois alvos**. Quem quebra a `main` conserta antes de qualquer outra coisa: erro de compilação vale nota zero.
- **Branches** `a/<tema>` e `b/<tema>`, PR para a `main`. Todo PR de marco tem o título `M<n>: ...` e, no corpo, "Como verificar": os comandos e a saída obtida. Revisão em até 24 h; sem resposta em 24 h e com a verificação passando, o autor faz o merge e anota no `STATUS.md`.
- **`STATUS.md`** na raiz (fica fora do `.tar.gz`). Cada um atualiza a própria seção ao fim de toda sessão: `Feito`, `Próximo`, `Bloqueado por`, `Perguntas para o outro`.
- **Contrato:** os `.h` só mudam por PR com o rótulo `contrato`, aprovado pelo outro. Depois do M0, vale o `.h` do repositório, não o texto desta trilha.
- **Dúvida:** issue com o rótulo `duvida`, resposta em até 12 h. Enquanto a resposta não vem, siga o padrão escrito nesta trilha (todo ponto em aberto tem um: apêndice A).
- **Nada de `goto`** (nota zero), nada impresso na tela (nem em `stderr`), nunca `-Werror` no Makefile entregue.

---

## 1. O desenho em uma página

Leia esta seção inteira antes de escrever a primeira linha. Tudo o que vem depois é detalhe dela.

### 1.1 As threads do programa

| Thread | Quantas | O que faz | Código |
|---|---|---|---|
| `main` | 1 | lê a entrada, cria as outras, faz `join`, grava o arquivo, libera a memória | `main.c` (A) |
| geradora | 1 | dorme até cada instante de chegada e insere na fila o **lote** de processos daquele instante | `geradora.c` (A) |
| escalonadora | 1 por CPU | recebe ou escolhe um processo, despacha, espera o fim do processo ou do quantum | laço em `escalonador.c` (A), que só conversa com `cpus.c` (B) |
| trabalhadora | `num_threads` por processo | executa as fatias de 100 ms do seu processo | `pcb.c` (A) |

### 1.2 Módulos e donos

Todos os fontes ficam **na raiz** do repositório (o professor roda `tar -xzvf` e `make` na mesma pasta).

| Arquivo | Dono | Responsabilidade |
|---|---|---|
| `main.c` | A | ordem de inicialização, `join`, gravação do arquivo, liberação; define `g_cfg` e `g_pol` |
| `tempo.c/.h` | A | T0, `agora_ms`, `dormir_ate` |
| `config.h` | A | `Config` e os enums de política e de modo de fila |
| `entrada.c/.h` | A | leitura do arquivo e ordenação por (chegada, PID) |
| `barreira.c/.h` | A | barreira reutilizável com líder (sobre `pthread_barrier_t`) |
| `pcb.c/.h` | A | PCB, TCB e a thread trabalhadora (protocolo da fatia) |
| `politica.c/.h` | A (bônus: B) | FCFS, RR e PRIORIDADE atrás da interface `Politica` |
| `geradora.c/.h` | A | thread geradora e `esperar_chegadas_ate` |
| `escalonador.c/.h` | A | **o único laço de escalonador** (mono, GLOBAL e POR_CPU) e `despachar` |
| `fila.c/.h` | B | lista duplamente encadeada intrusiva, sem travas internas |
| `log.c/.h` | B | buffer de eventos em memória, ordenado na gravação |
| `cpus.c/.h` | B | estado das CPUs e **toda** a diferença entre GLOBAL e POR_CPU: travas, colocação, migração, ociosidade, término |
| `metricas.c/.h` | B | bloco `=== METRICAS ===` |
| `Makefile` | B (A escreve a 1ª versão) | alvos `monoprocessador`, `multiprocessador`, `clean`, `teste_unidade` |
| `tests/` | B (A: estresse) | casos, `compare.py`, `run_all.sh`, testes unitários |
| `ferramentas/` | A e B | oráculo, estresse, análise, empacotamento (**não vai na entrega**) |

Quem chama quem: `main → entrada, tempo, politica, log, cpus, geradora, escalonador, metricas` · `escalonador → cpus, pcb, geradora, log, tempo` · `geradora → cpus, pcb, tempo` · `cpus → fila, politica, log` · `pcb → barreira, log, tempo`.

### 1.3 O caminho de um processo, do arquivo ao log

1. **Leitura** (`entrada_ler`): o arquivo vira um vetor de PCBs ordenado por (chegada, PID). Essa ordenação já resolve o desempate 1 (mesmo instante → PID crescente).
2. **Chegada** (geradora): dorme até `start_time`, cria as threads de **todos** os processos daquele instante (elas nascem bloqueadas em `p->cv`, porque o estado é READY) e chama `cpus_inserir_lote`. Lá dentro, com as filas travadas: cada processo entra na fila (no POR_CPU, na da CPU de menor carga), a `Chegada` vai para o log e as CPUs ociosas **recebem** trabalho.
3. **Despacho** (escalonadora da CPU k): ela tem um processo `p` na mão, ou porque alguém entregou, ou porque ela mesma tirou da fila pela política. `despachar` põe `p` em RUNNING, dá o orçamento de fatias (`quantum/100`, ou ilimitado no FCFS), registra `Executando` e faz broadcast em `p->cv`.
4. **Execução** (trabalhadoras de `p`): a cada fatia, todas dormem até o mesmo prazo, cada uma desconta a sua parte de `remaining_time` e todas se encontram numa barreira. Uma delas, a líder (a única que a barreira marca), decide: zerou → FINISHED e `finalizado` no log; orçamento acabou → READY; senão, mais uma fatia.
5. **Fim do despacho** (escalonadora): acorda quando o estado deixa de ser RUNNING. Se o processo terminou antes do fim do quantum, a CPU fica **retida** até lá. Depois espera as chegadas daquele instante, registra `Quantum esgotado` se for o caso, e, numa única seção crítica, devolve a CPU, reinsere o preemptado e escolhe o próximo.
6. **Término**: quando a geradora acabou, as filas estão vazias e nenhuma CPU está executando nem retida, quem percebe registra `Escalonador terminou...` e acorda as outras. A `main` faz os `join`, grava log e métricas e libera tudo.

### 1.4 As quatro ideias que sustentam o desenho

**(1) Instante lógico × tempo medido.** Chegada, duração e quantum são múltiplos de 100, então todo evento tem um instante exato que pode ser **calculado**, sem olhar o relógio:

| Evento | Instante lógico |
|---|---|
| chegada | `start_time` |
| despacho logo após um fim de despacho | o instante em que a CPU foi liberada (`t_lib`) |
| despacho de uma CPU que estava ociosa | `t_entrega`: o instante de quem fez a entrega (a chegada, ou a liberação de outra CPU) |
| fim da k-ésima fatia de um despacho | `t_despacho + 100·k` |
| finalização | fim da fatia em que `remaining_time` chegou a 0 |
| fim de quantum e fim de retenção | `t_despacho + Q` |
| término | `t_lib` da última CPU liberada |

Disso saem quatro regras: toda espera é `dormir_ate(instante lógico)`, nunca "dormir X ms a partir de agora" (o atraso de uma fatia não se soma ao da seguinte); quem registra um evento **passa o instante lógico** a `log_registrar`, que ordena por ele e imprime o tempo medido; `first_dispatch` e `finish_time` guardam instantes lógicos, então as métricas saem exatas; e `dormir_ate` nunca chama `usleep` com 1 s ou mais.

**(2) Orçamento de fatias e duas barreiras.** O escalonador nunca interrompe um processo: ele entrega um orçamento de fatias e quem o gasta são as próprias threads. A decisão de parar é tomada por **uma** thread (a líder), **entre** duas barreiras, quando todas as outras estão paradas. É assim que "o estado só muda em fronteira de fatia" vira código.

**(3) Entrega direta.** CPU ociosa não disputa fila. Quem insere ou libera trabalho (a geradora ou uma CPU que acabou de escolher) percorre as ociosas **em ordem crescente de índice**, escolhe por elas e deixa o processo em `cpu.entregue`. Toda CPU **nasce ociosa**, então até o primeiro despacho passa por aí. É isso que garante "a CPU de menor índice atende primeiro" com threads reais.

**(4) Um laço só.** Mono é GLOBAL com 1 CPU (`-DMONO` força `num_cpus = 1`). O laço do escalonador é o mesmo nos três modos e só chama `cpus_*`. Quais mutexes travar, onde inserir, de quem roubar e quem acordar fica tudo dentro de `cpus.c`.

### 1.5 Travas: a ordem global e quem protege o quê

**Ordem global (a frase vai para o README):** `filas (em ordem crescente de índice) → log` e `processo → log`. **Fila e processo nunca são travados ao mesmo tempo.** `mutex_chegadas` é folha: sempre travado sozinho. A barreira (`pthread_barrier_t`) não expõe mutex: `barreira_esperar` é chamada sem nenhuma trava. Com uma ordem fixa não existe ciclo de espera e, portanto, não existe deadlock.

| Quem | Travas, nesta ordem | Observação |
|---|---|---|
| trabalhadora | processo; só a líder, ao finalizar: processo → log | dorme a fatia e espera na barreira **sem trava** |
| geradora | `cpus_travar` (filas) → log; depois, sozinho, `mutex_chegadas` | cria as threads **antes** de travar |
| escalonadora, escolhendo e reinserindo | `cpus_travar` (filas) → log (migração) | solta as filas **antes** de despachar |
| escalonadora, despachando | processo → log | nunca segura fila aqui |
| escalonadora, em `esperar_chegadas_ate` | só `mutex_chegadas` | sem nenhuma outra trava |

| Campo | Protegido por |
|---|---|
| `pid, process_len, priority, num_threads, start_time` | nada: imutáveis depois da leitura |
| `remaining_time, state, cpu_id, first_dispatch, finish_time, orcamento, fatias_no_despacho, t_despacho, ultima_cpu` | `p->mutex` (`ultima_cpu` é lido pela política sob a trava da fila, mas só é escrito enquanto o processo não está em fila nenhuma) |
| `ant, prox, seq` | mutex da fila em que o PCB está |
| `CPU.ociosa, entregue, t_entrega, executando`, `cpus_executando`, `generator_done`, `encerrar` | `cpus_travar` (GLOBAL: o mutex da fila global; POR_CPU: todos) |
| `CPU.ultimo_pid, ocupado_ms, trocas` | só a própria escalonadora escreve; a `main` lê depois do `join` |
| `proxima_chegada` | `mutex_chegadas` |

Pares condição/mutex (cada condição tem um único mutex parceiro): `p->cv` e `p->cv_escalonador` → `p->mutex` · `cpu.cond` → mutex da fila daquela CPU (no GLOBAL, o da fila global) · `cv_chegadas` → `mutex_chegadas`. A barreira não entra nesta lista: a sincronização dela fica dentro da `pthread_barrier_t`.

**Três regras que valem para todo o código:** todo `pthread_cond_wait` fica dentro de um `while (condição)` · nenhum `usleep` com trava de fila ou de processo segurada · quem muda uma condição muda sob o mutex parceiro e acorda em seguida.

---

## 2. Ambiente e ferramentas

O programa é avaliado no Ubuntu do LabGrad, e todo teste tem de rodar em Linux. Qualquer uma das opções serve:
- **Linux nativo:** `sudo apt install build-essential valgrind python3 git`.
- **Docker** (Mac ou Windows), na pasta do repositório:
  ```bash
  docker run --rm -it -v "$PWD":/tp -w /tp ubuntu:24.04 bash
  apt update && apt install -y build-essential valgrind python3 git
  ```
  Para não reinstalar a cada vez: `docker commit <id> tp-so` e depois `docker run ... tp-so bash`.
- **WSL2** (Windows) ou uma **VM Ubuntu**.
- **LabGrad** por SSH (é onde vale o teste final).

No macOS o valgrind não roda e não existe `pthread_barrier_t` (que a `barreira.c` usa): não teste lá. O ambiente está pronto quando `gcc --version`, `valgrind --version` e `python3 --version` respondem.

As duas ferramentas abaixo já estão prontas na pasta `ferramentas/` e **não vão na entrega**.

**`oraculo.py`**: simulador determinístico das regras, em passos de 100 ms, sem threads. Reproduz **byte a byte** as saídas dos casos 01, 03, 05, 13, 15, 17 e do exemplo 4.5 do enunciado.
```bash
python3 ferramentas/oraculo.py entrada.txt            > esperado.txt   # multi (usa CPUS e FILA do arquivo)
python3 ferramentas/oraculo.py entrada.txt --mono     > esperado.txt   # como make monoprocessador
python3 ferramentas/oraculo.py entrada.txt --ambiguo  > esperado.txt   # sai com código 2 se a entrada for ambígua
python3 ferramentas/oraculo.py entrada.txt --afinidade                 # bônus ligado (sem efeito com 1 CPU)
```
- **Ambígua** é a entrada cujo resultado muda conforme a ordem em que duas CPUs tratam eventos do mesmo instante (pontos P4 e P7 do apêndice A). O programa real pode, legitimamente, dar outra saída. **Só compare com o oráculo entradas não ambíguas** (`--ambiguo` saindo com 0). Os casos oficiais não são ambíguos.
- O **mono é sempre determinístico**: qualquer entrada serve de teste contra `oraculo.py --mono`.
- Com 8 CPUs, quase toda entrada aleatória é ambígua. Para essas vale o verificador de invariantes.

**`contar_migracoes.py log.txt`**: imprime `migracoes_log` (linhas `Migracao`), `despachos_outra` (despachos de um PID numa CPU diferente da do seu despacho anterior), `retornos`, `despachos`, `trocas`, as três médias, `makespan` e `utilizacao_media`. É a régua do bônus.

**`tests/casos/`** recebe os 6 casos públicos e os 14 casos de `casos_extras/` (gerados pelo oráculo, todos não ambíguos; não são oficiais):

| Casos | Modo | O que exercitam |
|---|---|---|
| `ex45` | FCFS, GLOBAL, 2 CPUs | o exemplo 4.5 do enunciado |
| `g01`, `g02`, `g03` | GLOBAL, 2 CPUs: FCFS, RR, PRIORIDADE | várias chegadas em t=0 e no mesmo instante, retenção, reescolha do mesmo PID |
| `g04`, `g05`, `g06` | GLOBAL, 4 CPUs: FCFS, RR, PRIORIDADE | idem, com mais CPUs que processos em parte do tempo |
| `p01` | FCFS, POR_CPU, 2 CPUs | o exemplo pequeno desta trilha (1 migração) |
| `p02`, `p03`, `p04` | POR_CPU, 2 CPUs: FCFS, RR, PRIORIDADE | colocação por carga e 2 migrações cada |
| `p05`, `p06`, `p07` | POR_CPU, 4 CPUs: FCFS, RR, PRIORIDADE | idem (`p03`, `p06` e `p07` têm chegada durante uma retenção: exercitam a regra P1) |

---

## B0 · Antes do M0: o comparador (seg 23/11, manhã)

A entrega o marco M0 até as 14h (repositório, headers, stubs, Makefile, casos). Enquanto isso, escreva `tests/compare.py` numa pasta local: ele só depende de arquivos de texto.

**Lembretes do enunciado**
- A comparação automática tolera: (i) desvio de até 150 ms nos instantes; (ii) permutação entre eventos cujos instantes diferem menos de 150 ms (por exemplo, simultâneos em CPUs diferentes); (iii) desvio de até 10% nas métricas. **A ordem dos eventos de uma mesma CPU, os PIDs, as CPUs e a contagem de eventos têm de coincidir** [E §4.6].
- "Métricas: desvio de até 10%. **A contagem de trocas de contexto deve ser exata**" [L].

**Regras do comparador**

Uso: `python3 tests/compare.py esperado.txt obtido.txt`. Sai com 0 se passar; se falhar, sai com 1 e imprime **a primeira diferença**.

1. Separar o log das métricas na linha `=== METRICAS ===`.
2. Ler cada evento: `t`, CPU (ou nenhuma) e a mensagem (tipo, PID e, na migração, a origem).
3. **Mesma quantidade de eventos**, no total.
4. **Por CPU:** a sequência de mensagens é idêntica, na mesma ordem, e cada `|t_esperado − t_obtido| ≤ 150`.
5. **Eventos sem CPU** (`Chegada`, `Escalonador terminou`): para cada esperado existe um obtido, ainda não usado, com a mesma mensagem e diferença de no máximo 150 ms.
6. **Métricas:** as linhas têm o mesmo texto fora dos números; os identificadores (PID, índice da CPU) são iguais; cada valor pode desviar até 10%. **Exceções:** `Trocas de contexto` é **exata** [L]; quando o valor esperado é 0, aceitar até 150 de diferença absoluta (decisão nossa, documentada no README).

**Teste (autoteste, com os casos de `casos_publicos_v1/`)**
- `saida_15.txt` contra ela mesma → passa.
- Cópia com um PID trocado numa linha `Executando` → falha.
- Cópia com um evento a mais → falha. Cópia com `Trocas de contexto` diferente em 1 → falha.
- Cópia com um `t` deslocado em 100 ms → passa; em 200 ms → falha.
- Cópia com duas linhas de CPUs diferentes, no mesmo instante, trocadas de lugar → passa.

**Pronto quando:** os seis autotestes dão o resultado esperado.

---

## B1 · `fila.c` (seg 23/11, depois do "M0 pronto")

**Lembretes do enunciado**
- "A fila de prontos pode ser implementada como lista ou fila circular protegida por mutex, com inserção no fim, remoção do início e remoção de elemento arbitrário" [E §2.5].
- Mutex da fila de prontos: **um por fila**; protege inserções e remoções [E §2.7].
- A migração do POR_CPU rouba o **último** elemento de uma fila [E §2.6]: por isso existe `fila_remover_fim`.
- Na PRIORIDADE, o empate é decidido pela ordem de entrada na fila [E §2.1, desempate 4].

**Contrato**
```c
/* fila.h (dono: B) - fila de prontos: lista duplamente encadeada intrusiva, sem travas internas. */
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
/* NENHUMA funcao abaixo trava: o chamador ja segura f->mutex. */
void fila_inserir_fim(Fila *f, PCB *p);   /* p->seq = f->proximo_seq++ */
void fila_remover(Fila *f, PCB *p);       /* inicio, meio ou fim, em O(1) */
PCB *fila_remover_inicio(Fila *f);        /* NULL se vazia */
PCB *fila_remover_fim(Fila *f);           /* NULL se vazia (usada na migracao) */
#endif
```
O nó da fila são os campos `ant`, `prox` e `seq` do próprio PCB (o `pcb.h` completo está na Parada 1).

**Pseudocódigo**
```
fila_iniciar(f):   f.inicio ← f.fim ← NULO;  f.tamanho ← 0;  f.proximo_seq ← 0;  iniciar f.mutex
fila_destruir(f):  destruir f.mutex

fila_inserir_fim(f, p):
    p.seq ← f.proximo_seq;  f.proximo_seq += 1
    p.prox ← NULO;  p.ant ← f.fim
    se f.fim ≠ NULO: f.fim.prox ← p    senão: f.inicio ← p
    f.fim ← p;  f.tamanho += 1

fila_remover(f, p):                                   (p está em f; início, meio ou fim)
    se p.ant ≠ NULO:  p.ant.prox ← p.prox    senão: f.inicio ← p.prox
    se p.prox ≠ NULO: p.prox.ant ← p.ant     senão: f.fim ← p.ant
    p.ant ← p.prox ← NULO;  f.tamanho −= 1

fila_remover_inicio(f):  p ← f.inicio;  se p ≠ NULO: fila_remover(f, p);  devolver p
fila_remover_fim(f):     p ← f.fim;     se p ≠ NULO: fila_remover(f, p);  devolver p
```

**Por que é assim**
- **Intrusiva:** os ponteiros `ant` e `prox` moram dentro do PCB. Não há `malloc` por inserção (nada para vazar) e remover um elemento qualquer custa O(1), que é o que a PRIORIDADE precisa.
- **Nenhuma função trava.** O mutex existe dentro da `Fila`, mas quem trava é o chamador (`cpus.c`). Motivo: o escalonador precisa fazer **várias** operações na mesma seção crítica (reinserir o preemptado e escolher o próximo), e no POR_CPU uma operação mexe em várias filas de uma vez.
- **As quatro operações** são as que o enunciado pede (inserção no fim, remoção do início, remoção de elemento arbitrário) mais a remoção do fim, usada pela migração ("rouba o último").
- **`seq`** numera a ordem de entrada. A ordem da lista já é essa ordem; o número serve para depurar e conferir.

**Armadilhas**
- Depois de remover, zere `ant` e `prox` do PCB: ele vai ser reinserido muitas vezes.
- Não inicie nem destrua o PCB aqui; a fila só liga e desliga ponteiros.

**Teste** `tests/test_fila.c` (PCBs zerados com `memset`, só o `pid` preenchido):
- inserir 1, 2, 3 → `tamanho = 3`, `inicio` é o 1, `fim` é o 3, `seq` 0, 1, 2;
- `fila_remover_inicio` → devolve o 1; fica [2, 3];
- em [1, 2, 3], `fila_remover` do 2 → fica [1, 3], com `1.prox = 3` e `3.ant = 1`;
- `fila_remover_fim` em [1] → devolve o 1; `inicio = fim = NULL`, `tamanho = 0`;
- remover de fila vazia (início e fim) → NULL;
- reinserir um PCB já removido → entra no fim com `seq` maior que todos os anteriores.

---

## B2 · `log.c` (seg 23/11)

**Lembretes do enunciado**
- O arquivo se chama `log_execucao_minikernel.txt`, **para qualquer política ou versão**, e contém os eventos em ordem cronológica, um por linha [E §4.2]. Quem abre o arquivo é a `main`; o seu módulo recebe o `FILE *`.
- Formato exato das linhas e do bloco de métricas (o bloco é da etapa B5):
```
[t=<t>ms] [<POL>] Chegada PID <p>
[t=<t>ms] [<POL>] [CPU<k>] Executando PID <p>
[t=<t>ms] [<POL>] [CPU<k>] Quantum esgotado PID <p>
[t=<t>ms] [<POL>] [CPU<k>] PID <p> finalizado
[t=<t>ms] [<POL>] [CPU<k>] Migracao PID <p> CPU<j> -> CPU<k>
[t=<t>ms] [<POL>] Escalonador terminou execução de todos processos
=== METRICAS ===
PID <p> | espera=<x>ms | turnaround=<x>ms | resposta=<x>ms        (PIDs em ordem crescente)
Espera media: <x.xx> ms
Turnaround medio: <x.xx> ms
Resposta media: <x.xx> ms
Trocas de contexto: <n>
Utilizacao CPU<k>: <x.xx>%                                         (uma linha por CPU, em ordem de índice)
Throughput: <x.xx> processos/s
Makespan: <n> ms
```
`<POL>` é `FCFS`, `RR` ou `PRIORIDADE`. Só "execução" tem acento (`ç` e `ã`, em UTF-8); "Migracao", "Utilizacao", "media" e "medio" não têm. O arquivo usa LF e termina com `\n`. Salve os `.c` em UTF-8.
- Na versão mono, a única CPU é `CPU0` [E §4.2].
- **Dentro de um mesmo instante**, as linhas aparecem na ordem: chegadas, finalizações, fim de quantum / migração / despacho [L].
- Mutex do log: protege o buffer, escrito por várias threads [E §2.7].
- O programa não imprime nada na tela [E §4.7].

**Contrato**
```c
/* log.h (dono: B) - buffer de eventos em memoria, ordenado so na gravacao. */
#ifndef LOG_H
#define LOG_H
#include <stdio.h>
enum { EV_CHEGADA = 0, EV_FINALIZADO = 1, EV_OUTRO = 2, EV_TERMINO = 3 };
void log_iniciar(const char *nome_politica);
/* cpu = -1: linha sem "[CPUk]". fmt e so a mensagem, ex.: "Executando PID %d".
 * t_logico = instante teorico do evento (multiplo de 100), usado para ORDENAR;
 * a linha impressa usa o tempo medido agora_ms(). */
void log_registrar(int t_logico, int cpu, int categoria, const char *fmt, ...);
void log_gravar(FILE *saida);   /* ordena por (t_logico, categoria, ordem de registro) e escreve */
void log_destruir(void);
#endif
```

**Pseudocódigo**
```
estado interno (static): eventos (vetor dinâmico), qtd, cap, nome da política, mutex_log
Evento { int t_logico, t_real, categoria;  long ordem;  char texto[128]; }

log_iniciar(nome_politica):  guardar o nome;  cap ← 256;  qtd ← 0;  alocar eventos[cap]

log_registrar(t_logico, cpu, categoria, fmt, ...):
    msg ← vsnprintf(fmt, ...)                                  (fora da trava)
    travar mutex_log
    t_real ← agora_ms()
    se qtd = cap: cap ← 2 × cap;  realloc
    e ← eventos[qtd]
    e.t_logico ← t_logico;  e.t_real ← t_real;  e.categoria ← categoria;  e.ordem ← qtd
    e.texto ← (cpu ≥ 0 ? "[t=%dms] [%s] [CPU%d] %s" : "[t=%dms] [%s] %s")  com (t_real, política, [cpu,] msg)
    qtd += 1
    destravar mutex_log

log_gravar(f):
    qsort(eventos) por (t_logico, categoria, ordem)            (ordem desempata: não precisa de sort estável)
    para cada evento: escrever texto + "\n"

log_destruir():  liberar eventos
```

**Por que é assim**
- **Em memória, gravado no fim.** Nenhuma thread faz E/S de arquivo durante a simulação; a seção crítica é curta.
- **Dois tempos por evento.** `t_real` é o que aparece na linha (`gettimeofday`, como pede o enunciado). `t_logico` é o instante teórico, passado por quem registra, e só serve para **ordenar**. Sem ele, dois eventos do mesmo instante registrados com 1 ms de diferença poderiam sair na ordem errada.
- **A categoria cria as faixas** que o LEIA-ME exige dentro de um instante: `EV_CHEGADA` (0) → `EV_FINALIZADO` (1) → `EV_OUTRO` (2: `Quantum esgotado`, `Migracao`, `Executando`) → `EV_TERMINO` (3).
- **Dentro de `EV_OUTRO`, vale a ordem de registro.** Na mesma CPU, `Quantum esgotado` ou `Migracao` sempre é registrado antes do `Executando` correspondente. Entre CPUs diferentes a ordem é livre (o comparador tolera).
- **Travas:** `mutex_log` é o último da ordem global. `log_registrar` pode ser chamado com filas ou com um processo travado, e não trava mais nada lá dentro.

Quem registra o quê:

| Linha | Quem registra | `t_logico` | `cpu` | Categoria |
|---|---|---|---|---|
| `Chegada PID p` | **você**, `cpus_inserir_lote` | `start_time` (o `t` do lote) | −1 | `EV_CHEGADA` |
| `Executando PID p` | A, `despachar` | instante do despacho | k | `EV_OUTRO` |
| `Quantum esgotado PID p` | A, laço do escalonador | instante da liberação | k | `EV_OUTRO` |
| `PID p finalizado` | A, líder da fatia | fim da fatia | `p.cpu_id` | `EV_FINALIZADO` |
| `Migracao PID p CPUj -> CPUk` | **você**, `cpus_tirar_proximo` | instante da escolha | k (destino) | `EV_OUTRO` |
| `Escalonador terminou...` | **você**, `cpus_verificar_termino` | liberação da última CPU | −1 | `EV_TERMINO` |

**Armadilhas**
- A função de comparação do `qsort` precisa usar os **três** campos; sem `ordem`, o resultado dentro de um instante fica indefinido (o `qsort` não é estável).
- `texto[128]` sobra (a linha mais longa tem cerca de 70 caracteres), mas use `snprintf`/`vsnprintf` com o tamanho do buffer.
- A mensagem de término é escrita com acento no fonte: salve `cpus.c` em UTF-8.

**Teste** `tests/test_log.c` (precisa de `tempo.c`): `tempo_iniciar`, `log_iniciar("RR")` e registrar **fora de ordem**:
```
(1000, cpu 0,  EV_OUTRO,      "Executando PID 7")
(1000, cpu −1, EV_TERMINO,    "Escalonador terminou execução de todos processos")
(1000, cpu −1, EV_CHEGADA,    "Chegada PID 7")
(0,    cpu 0,  EV_OUTRO,      "Executando PID 1")
(1000, cpu 0,  EV_FINALIZADO, "PID 1 finalizado")
(1000, cpu 1,  EV_OUTRO,      "Migracao PID 9 CPU0 -> CPU1")
```
O arquivo gravado tem de sair nesta ordem (o `t` impresso é o medido, perto de 0; compare ignorando o número):
```
[t=0ms] [RR] [CPU0] Executando PID 1
[t=0ms] [RR] Chegada PID 7
[t=0ms] [RR] [CPU0] PID 1 finalizado
[t=0ms] [RR] [CPU0] Executando PID 7
[t=0ms] [RR] [CPU1] Migracao PID 9 CPU0 -> CPU1
[t=0ms] [RR] Escalonador terminou execução de todos processos
```
Confira também que a última linha tem os bytes `c3 a7 c3 a3` em "execução" (`od -c`), que toda linha termina com `\n` e que registrar 1000 eventos funciona (força o `realloc`).

---

## B3 · Makefile: `teste_unidade` (seg 23/11) → PR `b/fila-log`

Acrescente ao Makefile do M0 o alvo que compila e roda cada teste em `tests/bin/`. Cada teste é ligado só com os módulos de que precisa:

| Teste | Dono | Liga com |
|---|---|---|
| `test_fila` | B | `fila.c` |
| `test_log` | B | `log.c tempo.c` |
| `test_cpus` | B | `cpus.c fila.c politica.c log.c tempo.c` (o teste define `g_cfg` e `g_pol`) |
| `test_metricas` | B | `metricas.c` |
| `test_barreira` | A | `barreira.c` |
| `test_pcb` | A | `pcb.c barreira.c tempo.c` (o teste define um `log_registrar` vazio) |
| `test_entrada` | A | `entrada.c pcb.c barreira.c tempo.c` (idem) |
| `test_politica` | A | `politica.c fila.c` (o teste define `g_cfg`) |

```
teste_unidade:
    criar tests/bin
    para cada linha da tabela: $(CC) $(CFLAGS) -I. -o tests/bin/<teste> tests/<teste>.c <módulos>
    rodar cada tests/bin/<teste>; parar no primeiro que sair com código diferente de 0
```
Ponha `teste_unidade` no `.PHONY`. O Makefile é seu a partir daqui: os alvos `monoprocessador` e `multiprocessador` continuam compilando tudo num comando só, e **nunca** entra `-Werror`.

**Pronto quando (segunda à noite):** `make teste_unidade` passa com `test_fila` e `test_log`; valgrind limpo nos dois; PR `b/fila-log` **mesclado ainda na segunda**, porque A usa a fila e o log na terça.

---

## Parada 1 · Antes do `cpus.c`: entenda o lado de A que ele serve

O `cpus.c` não tem laço próprio: ele é um conjunto de operações chamadas pela geradora e pelo laço do escalonador, os dois de A. Para implementá-lo você precisa saber quem chama cada função, com quais travas e em que momento. Leia esta parada inteira antes da etapa B4.

### O PCB e o protocolo da fatia (`pcb.c`)
```c
/* pcb.h (dono: A) - BCP, TCB e a thread trabalhadora (protocolo da fatia). */
#ifndef PCB_H
#define PCB_H
#include <pthread.h>
#include "barreira.h"
typedef enum { READY, RUNNING, FINISHED } ProcessState;
struct TCB;
typedef struct PCB {
    /* ---- campos do enunciado (secao 2.9), mesmos nomes ---- */
    int pid;
    int process_len;            /* duracao total (ms) */
    int remaining_time;         /* tempo restante (ms) */
    int priority;               /* 1 (mais alta) a 5 */
    int num_threads;
    int start_time;             /* chegada (ms) */
    int cpu_id;                 /* CPU atual; -1 se nenhuma */
    int first_dispatch;         /* instante LOGICO do 1o despacho; -1 se ainda nao */
    int finish_time;            /* instante LOGICO da finalizacao; -1 se ainda nao */
    ProcessState state;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
    pthread_t *thread_ids;
    /* ---- extras (justificados no README) ---- */
    Barreira barreira;              /* alinha as threads no fim de cada fatia */
    pthread_cond_t cv_escalonador;  /* lider da fatia -> escalonador; par: mutex */
    int orcamento;                  /* fatias que ainda pode usar neste despacho */
    int fatias_no_despacho;         /* fatias executadas neste despacho */
    int t_despacho;                 /* instante logico do despacho atual */
    int ultima_cpu;                 /* CPU do ultimo despacho (-1 = nunca); usado no bonus */
    struct TCB *tcbs;               /* vetor de num_threads TCBs (liberado no fim) */
    /* ---- no intrusivo da fila: so mexe quem segura o mutex da fila onde o PCB esta ---- */
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
Os campos de cima são os do enunciado, com os mesmos nomes. Os extras: `barreira` (alinha as threads), `cv_escalonador` (a líder avisa o escalonador), `orcamento` e `fatias_no_despacho` (quantas fatias este despacho pode e já usou), `t_despacho` (âncora dos prazos), `ultima_cpu` (você usa no bônus), `tcbs` e o nó da fila.

Uma fatia vista de perto (processo de 1000 ms, 3 threads, despachado em t=0 com orçamento 3):
```
t=0    escalonador: state=RUNNING, orcamento=3, fatias=0, t_despacho=0; broadcast(cv)
       as 3 threads acordam, calculam prazo = 0 + (0+1)×100 = 100 e dormem até lá
t=100  cada uma desconta a sua parte (34, 33, 33): remaining 1000 → 900
       1ª barreira: a que recebe SERIAL_THREAD é a líder → fatias=1, orcamento=2; não zerou nem esgotou
       2ª barreira: as três passam juntas, voltam ao topo, veem RUNNING → prazo = 200
t=200  idem → fatias=2, orcamento=1, remaining=800
t=300  líder: fatias=3, orcamento=0 → state=READY; signal(cv_escalonador)
       2ª barreira → topo → READY → as três dormem em cv até o próximo despacho
       o escalonador acorda: resultado READY, 3 fatias, t_fim = 0 + 300
```
```
pcb_iniciar(p):                                   (pid, process_len, priority, num_threads e start_time já preenchidos)
    p.remaining_time ← p.process_len;  p.state ← READY
    p.cpu_id ← p.first_dispatch ← p.finish_time ← p.ultima_cpu ← −1
    p.orcamento ← p.fatias_no_despacho ← p.t_despacho ← 0;  p.ant ← p.prox ← NULO;  p.seq ← 0
    iniciar p.mutex, p.cv e p.cv_escalonador;  barreira_iniciar(p.barreira, p.num_threads)
    p.thread_ids ← alocar num_threads pthread_t;  p.tcbs ← alocar num_threads TCB

pcb_destruir(p):                                  (depois do join das threads do processo)
    barreira_destruir(p.barreira);  destruir p.cv_escalonador, p.cv e p.mutex
    liberar p.thread_ids e p.tcbs

thread_trabalhadora(tcb):
    p ← tcb.pcb;  i ← tcb.thread_index;  n ← p.num_threads
    parte ← FATIA / n + (i < FATIA mod n ? 1 : 0)             (3 threads: 34, 33, 33)
    repita:
        travar p.mutex
        enquanto p.state = READY: esperar(p.cv, p.mutex)
        se p.state = FINISHED: destravar; sair do laço
        prazo ← p.t_despacho + (p.fatias_no_despacho + 1) × FATIA          ⚠️ prazo absoluto
        destravar p.mutex

        dormir_ate(prazo)                                     (a fatia; sem trava nenhuma)

        travar p.mutex;  p.remaining_time −= parte;  destravar p.mutex

        se barreira_esperar(p.barreira) = 1:                  (1ª barreira; só a líder entra aqui)
            travar p.mutex
            p.fatias_no_despacho += 1;  p.orcamento −= 1
            t_fim ← p.t_despacho + p.fatias_no_despacho × FATIA
            se p.remaining_time = 0:                          ⚠️ este teste vem ANTES do orçamento
                p.state ← FINISHED;  p.finish_time ← t_fim
                log_registrar(t_fim, p.cpu_id, EV_FINALIZADO, "PID %d finalizado", p.pid)
                broadcast(p.cv);  signal(p.cv_escalonador)
            senão se p.orcamento = 0:
                p.state ← READY                               (fim do quantum)
                signal(p.cv_escalonador)
            destravar p.mutex
        barreira_esperar(p.barreira)                          (2ª barreira: ninguém volta ao topo antes da decisão)
    devolver NULO
```
- **Por que duas barreiras.** Sem a 2ª, uma thread não líder voltaria ao topo, veria RUNNING e começaria uma fatia a mais antes de a líder gravar READY.
- **Por que só a líder decide.** Ninguém registra `finalizado` duas vezes nem desconta o orçamento duas vezes. O enunciado fala em "a thread que detectar primeiro"; a líder é exatamente uma thread bem definida que detecta (justificar no README).
- **Por que `remaining_time` nunca fica negativo.** As durações são múltiplas de 100 e cada fatia desconta exatamente 100 no total (as partes somam 100).
- **Por que zerar é testado antes do orçamento.** Quem termina exatamente no fim do quantum gera só `finalizado`, sem `Quantum esgotado` [L].
- **Por que `dormir_ate(prazo)` e não `usleep(FATIA*1000)`.** É a mesma fatia de 100 ms, mas ancorada em `t_despacho`: o atraso não se acumula. O prazo é igual para todas as threads porque `fatias_no_despacho` só muda entre as barreiras.
- **Por que as threads não perdem o despacho.** Elas testam o **estado** sob `p->mutex`, não o sinal. Se o broadcast chegar antes de a thread dormir, ela vê RUNNING e segue.

### Tempo (`tempo.c`)
```c
/* tempo.h (dono: A) - relogio da simulacao. */
#ifndef TEMPO_H
#define TEMPO_H
#define FATIA      100          /* ms: menor unidade de execucao simulada */
#define T_INFINITO 0x3fffffff   /* "nenhuma chegada pendente" */
void tempo_iniciar(void);       /* marca T0 com gettimeofday; chamar uma vez, logo apos ler a entrada */
int  agora_ms(void);            /* ms reais desde T0 */
void dormir_ate(int t_ms);      /* usleep em pedacos de no maximo 500 ms ate agora_ms() >= t_ms */
#endif
```
```
tempo_iniciar():   t0 ← gettimeofday()
agora_ms():        a ← gettimeofday()
                   devolver ((a.s − t0.s) × 1 000 000 + (a.us − t0.us)) / 1000        (conta em long)
dormir_ate(t):
    repita:
        resto ← t − agora_ms()
        se resto ≤ 0: sair
        usleep(min(resto, 500) × 1000)
```
- **Prazo absoluto.** `dormir_ate(t)` recebe um instante, não uma duração. Se a thread acordou 3 ms atrasada numa fatia, a próxima dura 97 ms: o atraso não se acumula. Com `usleep(100 ms)` solto, o atraso de cada fatia se somaria ao das anteriores (o caso 15 tem 74 fatias por CPU) até passar da tolerância de 150 ms.
- **Pedaços de 500 ms.** O POSIX permite que `usleep` com 1 000 000 µs ou mais falhe com `EINVAL`. Um quantum de 1000 ms ou um intervalo longo entre chegadas cairia nisso.
- **O laço reconfere o relógio**, então acordar cedo (sinal, arredondamento) não faz mal.

### Política (`politica.c`)
Você chama `g_pol->escolher(fila, k)` dentro de `cpus_tirar_proximo`, com a fila já travada, e edita o ponto do bônus no M5.
```c
/* politica.h (dono: A; bonus: B) - politicas de escalonamento atras de uma interface. */
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
```
escolher_fifo(f, cpu):                              (FCFS e RR)
    devolver fila_remover_inicio(f)                 (NULO se vazia)

escolher_prioridade(f, cpu):
    melhor ← NULO
    para cada p de f, do início para o fim:
        se melhor = NULO ou p.priority < melhor.priority: melhor ← p       ⚠️ "<" estrito
        /* BONUS AFINIDADE: B acrescenta aqui um "senão se", atrás de #ifdef AFINIDADE */
    se melhor ≠ NULO: fila_remover(f, melhor)
    devolver melhor

três instâncias constantes de Politica:
    { "FCFS", 0, escolher_fifo }    { "RR", 1, escolher_fifo }    { "PRIORIDADE", 1, escolher_prioridade }
politica_obter(tipo): devolve o ponteiro da instância correspondente
```
- **`<` estrito resolve o desempate 4.** Toda inserção é no fim, então a ordem da lista **é** a ordem de entrada na fila. Varrendo do início e trocando só por prioridade estritamente melhor, o empate fica com o mais antigo. (`seq` guarda essa mesma ordem em número; serve para depurar e para explicar no README.)
- **RR e FCFS usam a mesma função.** A diferença entre os dois é só `usa_quantum`, que o escalonador consulta para montar o orçamento.
- **O preemptado concorre de novo.** O escalonador reinsere o processo no fim da fila **antes** de chamar `escolher`. Na PRIORIDADE, se ele for o único com a melhor prioridade, é reescolhido; se houver outro igual, esse outro está há mais tempo na fila e ganha (os iguais se alternam como no RR).
- **A interface recebe `cpu`** só por causa do bônus de afinidade; FCFS, RR e a PRIORIDADE padrão ignoram o parâmetro.
- **2ª etapa:** política nova é mais uma instância de `Politica`. O escalonador não tem nenhum `if (politica == ...)`.

### Geradora (`geradora.c`)
É quem chama `cpus_inserir_lote`, **sem trava nenhuma**, uma vez por instante de chegada.
```c
/* geradora.h (dono: A) - thread geradora (chegadas em lote) e a espera por chegadas. */
#ifndef GERADORA_H
#define GERADORA_H
#include "pcb.h"
void  geradora_iniciar(PCB *pcbs, int n);  /* ANTES de criar qualquer thread: proxima_chegada = pcbs[0].start_time */
void *thread_geradora(void *arg);           /* arg ignorado */
void  esperar_chegadas_ate(int t);          /* bloqueia ate toda chegada com start_time <= t estar na fila */
void  geradora_destruir(void);
#endif
```
```
estado interno (static): pcbs, n, proxima_chegada, mutex_chegadas, cv_chegadas

geradora_iniciar(pcbs, n):
    guardar pcbs e n;  proxima_chegada ← (n > 0 ? pcbs[0].start_time : T_INFINITO)
    iniciar mutex_chegadas e cv_chegadas

thread_geradora():
    se n = 0: cpus_inserir_lote(NULO, 0, 0, 1);  terminar
    i ← 0
    enquanto i < n:
        t ← pcbs[i].start_time
        dormir_ate(t)
        j ← i
        enquanto j < n E pcbs[j].start_time = t:                       ⚠️ o lote do mesmo instante
            para x de 0 a pcbs[j].num_threads − 1:
                pcbs[j].tcbs[x] ← (pcb = &pcbs[j], thread_index = x)
                pthread_create(&pcbs[j].thread_ids[x], thread_trabalhadora, &pcbs[j].tcbs[x])
            j += 1
        cpus_inserir_lote(&pcbs[i], j − i, t, j = n)                   (o último lote liga generator_done)
        travar mutex_chegadas
        proxima_chegada ← (j < n ? pcbs[j].start_time : T_INFINITO)    ⚠️ só DEPOIS do lote inserido
        broadcast(cv_chegadas)
        destravar mutex_chegadas
        i ← j

esperar_chegadas_ate(t):
    travar mutex_chegadas
    enquanto proxima_chegada ≤ t: esperar(cv_chegadas, mutex_chegadas)
    destravar mutex_chegadas

geradora_destruir(): destruir mutex_chegadas e cv_chegadas
```
- **Lote, e não um por vez.** Se em t chegam o PID 8 (prioridade 2) e o PID 27 (prioridade 1) com uma CPU ociosa, inserir e acordar um por vez despacharia o 8. O certo é o 27: a CPU só pode escolher depois que **todos** os de t entraram. No POR_CPU a colocação também precisa ver o lote inteiro, em ordem de PID.
- **O vetor já vem ordenado por (chegada, PID)**, então o lote `pcbs[i..j−1]` já está em ordem crescente de PID (desempate 1).
- **Threads criadas antes da inserção.** Quando o processo for despachado, as threads já existem. Elas nascem bloqueadas porque o estado é READY.
- **`proxima_chegada` é a promessa da geradora:** "tudo que chega antes deste instante já está na fila". Ela só avança depois de `cpus_inserir_lote` voltar. `esperar_chegadas_ate(t)` espera a promessa passar de t. A espera dura só o atraso da geradora (poucos ms).
- **`mutex_chegadas` é folha:** nunca é segurado junto com outra trava. A geradora chama `cpus_inserir_lote` sem ele.

### O laço do escalonador (`escalonador.c`)
É quem chama todas as outras funções do `cpus.c`. Repare em três coisas: ele começa em `cpus_esperar_trabalho`; depois de um despacho ele chama `cpus_devolver` e `cpus_tirar_proximo` **sem soltar a trava entre os dois**; e `cpus_entregar_ociosas` só vem depois de ele já ter o próprio processo.
```c
/* escalonador.h (dono: A) - o unico laco de escalonador (mono, GLOBAL e POR_CPU). */
#ifndef ESCALONADOR_H
#define ESCALONADOR_H
void *thread_escalonador(void *arg_cpu);    /* arg = CPU* (uma thread por CPU) */
#endif
```
```
despachar(cpu, p, t) → (resultado, t_fim):
    se p.pid ≠ cpu.ultimo_pid: cpu.trocas += 1          (mesmo PID reescolhido não conta; o 1º despacho conta)
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
    cpu.ocupado_ms += fatias × FATIA                    (a retenção fica de fora: só fatias executadas entram)
    devolver (resultado, t + fatias × FATIA)

thread_escalonador(cpu):
    k ← cpu.indice
    cpus_travar()
    p ← cpus_esperar_trabalho(k, &t)                    ⚠️ toda CPU nasce ociosa: o 1º processo vem por entrega
    enquanto p ≠ NULO:
        cpus_entregar_ociosas(t)                        (só DEPOIS de esta CPU já ter o seu processo)
        cpus_destravar()

        (res, t_fim) ← despachar(cpu, p, t)             (bloqueia até o processo terminar ou esgotar o quantum)
        t_lib ← t_fim
        se res = FINISHED e g_pol.usa_quantum e t_fim < t + g_cfg.quantum:
            t_lib ← t + g_cfg.quantum                   (retenção: CPU presa e ociosa até o fim do quantum)
            dormir_ate(t_lib)                           (sem nenhuma trava; a CPU segue com executando ≠ NULO)
        esperar_chegadas_ate(t_lib)                     ⚠️ SEMPRE, qualquer que seja res
        se res = READY:
            log_registrar(t_lib, k, EV_OUTRO, "Quantum esgotado PID %d", p.pid)

        cpus_travar()
        cpus_devolver(k, res = READY ? p : NULO)        (libera a CPU; reinsere o preemptado no fim da fila dela)
        t ← t_lib
        p ← cpus_tirar_proximo(k, t)                    ⚠️ na MESMA seção crítica da reinserção
        se p = NULO:
            se cpus_verificar_termino(t): sair do laço
            p ← cpus_esperar_trabalho(k, &t)            (dorme; volta com a trava e com t = instante da entrega)
    cpus_destravar()
    devolver NULO
```
- **Desempate 2 e "chegadas antes de qualquer escolha".** `esperar_chegadas_ate(t_lib)` garante que toda chegada até `t_lib` já está na fila antes da reinserção e da escolha. Vale para fim de quantum, para finalização no FCFS e para fim de retenção: uma chegada mais prioritária em `t_lib` tem de concorrer.
- **Reinserir e escolher na mesma seção crítica.** Entre `cpus_devolver` e `cpus_tirar_proximo` a trava não é solta, e `cpus_entregar_ociosas` só roda depois da escolha. Nenhuma CPU ociosa "rouba" o processo recém-preemptado [E §2.6].
- **Desempate 3.** As ociosas recebem trabalho em ordem crescente de índice, escolhido por quem as acorda. Como toda CPU nasce ociosa, isso vale desde t=0. (Se cada escalonadora começasse tirando da fila por conta própria, a CPU1 poderia chegar à trava antes da CPU0 e levar o primeiro processo.)
- **PRIORIDADE.** O atual volta ao fim da fila **antes** da escolha e concorre com os outros. Reescolhido, saem `Quantum esgotado` e `Executando` com o mesmo PID, e `despachar` não conta troca.
- **Retenção.** O laço não avisa ninguém de que entrou em retenção: ele só dorme até `t_lib` antes de chamar `cpus_devolver`. Nesse intervalo a CPU continua com `executando ≠ NULO`, então ninguém entrega nada para ela, ninguém declara o término e, no POR_CPU, ela continua contando 1 na carga (regra P1). A utilização ignora a retenção porque só soma fatias.
- **Término.** Quem solta o último processo verifica o término em `t_lib`, que é o fim da retenção quando ela existe. Por isso `Escalonador terminou` pode sair depois do makespan.
- **FCFS.** Orçamento ilimitado: o processo só sai de RUNNING ao terminar, e `usa_quantum = 0` desliga a retenção.
- **Travas.** `despachar` trava só o processo (processo → log). As filas são soltas antes dele e retomadas depois. `dormir_ate` e `esperar_chegadas_ate` rodam sem nenhuma trava de fila ou de processo.

O laço no caso 05 (mono, RR, Q=300; processos `5 0 700`, `9 300 500`, `2 400 900`, `11 2500 200`):
```
t=0     geradora insere o PID 5 e entrega à CPU0 (ociosa desde o início) → despachar(5, t=0)
t=300   líder do 5: orçamento 0 → READY. Escalonador: res=READY, t_fim=300, t_lib=300
        esperar_chegadas_ate(300): o PID 9 chega em 300 → espera a geradora pôr o 9 na fila     fila=[9]
        "Quantum esgotado PID 5"; devolver(5) → fila=[9,5]; tirar_proximo → 9                    (desempate 2)
t=600 a 1200: mais três quanta sem novidade (PIDs 9, 5 e 2); em 1200 a fila é [9,5] e o 2 acaba de esgotar o quantum
t=1200  devolver(2) → fila=[9,5,2]; tirar_proximo → 9; despachar(9, t=1200) com remaining=200
t=1400  líder do 9: remaining=0 → FINISHED, "PID 9 finalizado" em 1400. res=FINISHED, t_fim=1400
        1400 < 1200+300 → t_lib=1500: retenção; dormir_ate(1500)                                 (CPU presa e ociosa)
t=1500  devolver(NULO); tirar_proximo → 5; "Executando PID 5" em 1500
t=1600  PID 5 finalizado; retenção até 1800; em 1800, "Executando PID 2"
t=2100  PID 2: quantum esgotado, fila=[2] → reescolhido: "Quantum esgotado PID 2" + "Executando PID 2", sem troca
t=2400  PID 2 termina exatamente no fim do quantum (2100+300): só "finalizado"; t_lib=2400, sem retenção
        fila vazia; verificar_termino(2400) = 0 (falta o PID 11) → esperar_trabalho
t=2500  geradora insere o 11 (último lote) e entrega → despachar(11, t=2500)
t=2700  PID 11 finalizado (makespan=2700); retenção até 2800
t=2800  devolver; fila vazia; verificar_termino(2800) = 1 → "Escalonador terminou" em 2800
        ocupado_ms = 700+500+900+200 = 2300 → utilização = 2300/2700 = 85,19%
```

---

## B4 · `cpus.c` (ter 24/11)

É o coração da sua metade. Implemente **todas** as funções com o código geral (o `nfilas` decide) e teste hoje com `nfilas = 1`, que é o que destrava o mono e o GLOBAL. Os caminhos de POR_CPU ficam escritos e ganham teste de integração no sábado (etapa B9), mas o `test_cpus` de hoje já os exercita.

**Lembretes do enunciado**
- Número de CPUs de 1 a 8. Um processo executa em **no máximo uma CPU por vez**. Cada CPU aplica a política de forma independente, e os quanta de CPUs diferentes não são sincronizados [E §2.6].
- **FILA GLOBAL:** uma única fila, protegida por mutex; qualquer CPU livre retira dela o próximo. Ao fim do quantum, a CPU reinsere o processo e seleciona o próximo **na mesma seção crítica, antes que CPUs ociosas sejam notificadas** [E §2.6].
- **FILA POR_CPU:** cada CPU tem a sua fila local e o seu mutex [E §2.6].
  - **Colocação:** quem chega vai para a CPU de **menor carga** = processos na fila local + processo em execução. Empate: menor índice.
  - **CPU em retenção conta 1 na carga** (confirmado em 24/11; o enunciado não diz). Ela terminou o processo antes do quantum, mas ainda não foi liberada. Exemplo: 2 CPUs, RR, Q=500; o PID 1 roda na CPU0 e termina em 200; em 300 chega o PID 2: cargas CPU0 = 1 (retida) e CPU1 = 0, então ele vai para a CPU1, sem `Migracao`.
  - **Migração:** quando uma CPU precisa selecionar e a fila local está vazia, ela **rouba o último** elemento da fila local **mais longa** (empate: menor índice), desde que essa fila tenha ao menos um processo. CPUs ociosas repetem a tentativa sempre que algum processo é inserido em qualquer fila.
  - Cada migração é registrada no log: `[CPU<k>] Migracao PID <p> CPU<j> -> CPU<k>`, na CPU de **destino** [E §4.2].
- **Desempate 3:** se mais de uma CPU está livre, o processo é atendido pela de menor índice [E §2.1; exemplo 4.5: em t=100 o PID 3 vai para a CPU1 porque a CPU0 está ocupada].
- **Desempate 1:** chegadas do mesmo instante entram em ordem crescente de PID (o lote já vem nessa ordem) [E §2.1].
- Uma condição por escalonador: dorme com fila vazia, acorda com nova inserção [E §2.7]. Sempre `while` em volta do `pthread_cond_wait` [E §5].
- Defina e documente uma ordem global de travamento; nunca mantenha o mutex da fila durante `usleep` [E §2.7].
- **Término:** cada escalonador termina quando, ao mesmo tempo, (i) todas as filas estão vazias, (ii) `generator_done` está ativa e (iii) nenhuma CPU está executando um processo [E §2.8]. O evento `Escalonador terminou...` sai quando a **última CPU é liberada**; com retenção, isso é depois do makespan [L; C 05: 2800 contra 2700; C 15: 7700 contra 7400].
- No mono, CPUS vale 1 e FILA é ignorado [E §2.6]: A já entrega `g_cfg` assim, e o seu código não precisa saber do alvo.

**Contrato**
```c
/* cpus.h (dono: B) - estado das CPUs e TODA a diferenca entre GLOBAL e POR_CPU. */
#ifndef CPUS_H
#define CPUS_H
#include <pthread.h>
#include "pcb.h"
typedef struct {
    int indice;
    pthread_t tid;
    /* --- protegidos pela trava de filas (cpus_travar) --- */
    pthread_cond_t cond;        /* par: mutex da fila desta CPU (GLOBAL: o da fila global) */
    int ociosa;                 /* 1 = sem trabalho, aceita entrega. NASCE 1 em cpus_iniciar */
    PCB *entregue;              /* processo escolhido PARA ela por quem a acordou (entrega direta) */
    int t_entrega;              /* instante logico dessa entrega = instante do despacho */
    PCB *executando;            /* processo despachado ou entregue; continua != NULL durante a retencao */
    /* --- so a propria thread escalonadora escreve; main le depois do join --- */
    int ultimo_pid;             /* -1 no inicio */
    int ocupado_ms;             /* soma de fatias x 100 */
    int trocas;                 /* trocas de contexto nesta CPU */
} CPU;
extern CPU *g_cpus;             /* vetor de g_cfg.num_cpus posicoes, criado em cpus_iniciar */

void cpus_iniciar(void);        /* usa g_cfg e g_pol; toda CPU nasce com ociosa = 1 */
void cpus_destruir(void);
/* GLOBAL: trava o mutex da fila global. POR_CPU: trava TODAS as filas, em ordem crescente. */
void cpus_travar(void);
void cpus_destravar(void);
/* ---- as funcoes abaixo exigem cpus_travar() ja feito ---- */
PCB *cpus_tirar_proximo(int k, int t);  /* politica na fila da CPU k; no POR_CPU, com fila vazia, migra
                                           (registra "Migracao" em t). Se devolver p: executando=p e
                                           cpus_executando++ */
void cpus_devolver(int k, PCB *p);      /* fim do despacho: executando=NULL, cpus_executando--;
                                           se p != NULL, reinsere p no fim da fila da CPU k */
void cpus_entregar_ociosas(int t);      /* para cada CPU ociosa, em ordem crescente de indice: escolhe por ela
                                           (cpus_tirar_proximo), guarda em entregue/t_entrega e da signal */
PCB *cpus_esperar_trabalho(int k, int *t); /* dorme ate receber uma entrega (devolve o PCB e poe o instante
                                           em *t) ou ate encerrar (devolve NULL); volta com cpus_travar() feito */
int  cpus_verificar_termino(int t);     /* se terminou: encerrar=1, registra o termino (uma vez) em t,
                                           acorda todas; devolve encerrar */
/* ---- chamada pela geradora SEM nenhuma trava ---- */
void cpus_inserir_lote(PCB *lote, int n, int t, int ultimo);
     /* trava; insere os n processos (ja em ordem de PID; no POR_CPU, cada um na CPU de menor
        carga); registra cada "Chegada" em t; se ultimo, generator_done=1; entrega as ociosas; destrava */
#endif
```
`CPU.ultimo_pid`, `ocupado_ms` e `trocas` são escritos só pela escalonadora da própria CPU (em `despachar`, de A); você só os inicia.

**Pseudocódigo**
```
estado interno (static):
    filas[nfilas]         nfilas = 1 no mono e no GLOBAL; nfilas = num_cpus no POR_CPU
    cpus_executando       quantas CPUs têm processo (entregue, executando ou em retenção)
    generator_done        a geradora já inseriu o último lote
    encerrar              o término já foi declarado
fila_da_cpu(k) = filas[nfilas = 1 ? 0 : k]

cpus_iniciar():
    nfilas ← (g_cfg.modo_fila = FILA_POR_CPU ? g_cfg.num_cpus : 1);  alocar e iniciar as filas
    g_cpus ← vetor ZERADO de g_cfg.num_cpus posições
    para cada k:  indice ← k;  ultimo_pid ← −1;  ociosa ← 1 ⚠️;  iniciar cond
    cpus_executando ← generator_done ← encerrar ← 0
cpus_destruir():  destruir as cond e as filas;  liberar g_cpus e filas

cpus_travar():     para q de 0 até nfilas−1: travar filas[q].mutex            (sempre em ordem crescente)
cpus_destravar():  para q de nfilas−1 até 0: destravar filas[q].mutex

carga(k):          (só POR_CPU)
    filas[k].tamanho + (g_cpus[k].executando ≠ NULO ? 1 : 0)
    (a CPU em retenção CONTA 1: executando só volta a NULO em cpus_devolver, no fim da retenção. Regra P1)

cpus_tirar_proximo(k, t):
    p ← g_pol.escolher(fila_da_cpu(k), k)
    se p = NULO E nfilas > 1:                                           (POR_CPU com fila local vazia: migração)
        origem ← índice j ≠ k com o MAIOR filas[j].tamanho > 0;  no empate, o menor j
        se existe origem:
            p ← fila_remover_fim(filas[origem])                         (rouba o ÚLTIMO)
            log_registrar(t, k, EV_OUTRO, "Migracao PID %d CPU%d -> CPU%d", p.pid, origem, k)
    se p ≠ NULO:  g_cpus[k].executando ← p;  cpus_executando += 1
    devolver p

cpus_devolver(k, p):
    g_cpus[k].executando ← NULO;  cpus_executando −= 1
    se p ≠ NULO: fila_inserir_fim(fila_da_cpu(k), p)                    (não acorda ninguém aqui)

cpus_entregar_ociosas(t):                                               ⚠️ entrega direta
    repita:
        mudou ← 0
        para k de 0 até num_cpus−1:                                     (menor índice primeiro)
            c ← g_cpus[k]
            se c.ociosa E c.entregue = NULO:
                p ← cpus_tirar_proximo(k, t)                            (fila própria; no POR_CPU, ou migração)
                se p ≠ NULO:  c.entregue ← p;  c.t_entrega ← t;  signal(c.cond);  mudou ← 1
    até mudou = 0                                                       (no POR_CPU uma entrega pode abrir outra)

cpus_esperar_trabalho(k, *t):                                           (entra e sai com cpus_travar() feito)
    c ← g_cpus[k];  m ← fila_da_cpu(k).mutex
    c.ociosa ← 1
    se nfilas > 1: destravar todas as filas EXCETO a de k, em ordem decrescente
    enquanto c.entregue = NULO E NÃO encerrar: esperar(c.cond, m)
    c.ociosa ← 0
    p ← c.entregue;  c.entregue ← NULO;  *t ← c.t_entrega
    se nfilas > 1:  destravar m;  cpus_travar()                         ⚠️ solta m ANTES de retravar tudo
    devolver p                                                          (NULO = encerrou)

cpus_verificar_termino(t):
    se NÃO encerrar E generator_done E cpus_executando = 0 E todas as filas vazias:
        encerrar ← 1
        log_registrar(t, −1, EV_TERMINO, "Escalonador terminou execução de todos processos")
        para cada k: broadcast(g_cpus[k].cond)
    devolver encerrar

cpus_inserir_lote(lote, n, t, ultimo):                                  (chamada pela geradora, SEM trava)
    cpus_travar()
    para i de 0 até n−1:                                                (lote já em ordem crescente de PID)
        destino ← (nfilas = 1 ? 0 : o k de menor carga(k); no empate, o menor k)
        fila_inserir_fim(filas[destino], lote[i])
        log_registrar(t, −1, EV_CHEGADA, "Chegada PID %d", lote[i].pid)
    se ultimo: generator_done ← 1
    cpus_entregar_ociosas(t)
    se ultimo: cpus_verificar_termino(t)                                (só faz diferença com PROCESSOS 0)
    cpus_destravar()
```

**Por que é assim**
- **Mono, GLOBAL e POR_CPU no mesmo código.** `nfilas` decide tudo. Com `nfilas = 1` não há carga nem migração, e `cpus_travar` trava um mutex só.
- **Entrega direta (desempate 3).** Se quem insere apenas acordasse as ociosas, elas disputariam a trava: a CPU1 poderia ganhar da CPU0 e levar o primeiro processo, e no POR_CPU uma ociosa de fila vazia roubaria o processo recém-colocado na fila de outra ociosa (uma `Migracao` que não deveria existir). Aqui quem insere escolhe por elas, em ordem de índice, e guarda o resultado em `entregue`. O instante da entrega (`t_entrega`) vira o instante lógico do despacho.
- **Toda CPU nasce ociosa (`ociosa = 1` em `cpus_iniciar`).** A geradora pode inserir o lote de t=0 antes de as escalonadoras começarem a rodar. Como o campo já nasce 1, a entrega funciona do mesmo jeito: a escalonadora, ao começar, entra em `cpus_esperar_trabalho`, encontra `entregue` preenchido e nem chega a dormir.
- **A entrega já marca `executando` e soma `cpus_executando`** (dentro de `cpus_tirar_proximo`). Entre o `signal` e o despacho, ninguém conclui que acabou nem põe outro processo nessa CPU; e no POR_CPU a carga dela já conta.
- **Sem lost wakeup.** A ociosa dorme segurando o mutex da própria fila. Quem entrega segura esse mutex também (no POR_CPU, todos). Então, ou ela já está em `wait` e recebe o `signal`, ou ainda vai testar `entregue` e vai achá-lo preenchido.
- **POR_CPU trava todas as filas.** `cpus_executando`, `executando` (carga), colocação, migração, entrega e término dependem de várias filas ao mesmo tempo. São no máximo 8 mutexes, sempre em ordem crescente: custo de microssegundos e helgrind limpo. Cada CPU continua com a sua fila e o seu mutex, como manda o enunciado.
- **`cpus_esperar_trabalho` no POR_CPU.** Para dormir, a CPU fica só com o próprio mutex (o parceiro da sua `cond`). Ao acordar ela segura só esse; para voltar a ter todos sem violar a ordem crescente, **solta o próprio e trava tudo de novo**.
- **Sem migração falsa.** Uma ociosa tem carga 0, então a chegada vai para a ociosa de menor índice; e na entrega cada CPU tenta primeiro a própria fila. A migração só acontece quando a fila local está mesmo vazia.
- **`cpus_devolver` não acorda ninguém.** Quem devolve vai escolher logo em seguida, na mesma seção crítica, e só então entrega às ociosas [E §2.6: evitar que a ociosa roube o recém-preemptado].
- **Término uma vez só.** `encerrar` impede o segundo registro; o broadcast acorda as ociosas, que saem de `cpus_esperar_trabalho` com NULO.

Exemplo POR_CPU para acompanhar à mão (é o caso `p01`): 2 CPUs, FCFS; processos `1 0 600`, `2 0 300`, `3 0 900`, `4 100 200`, `5 100 600`.
```
t=0    lote [1,2,3]. Cargas (0,0): 1→fila0. (1,0): 2→fila1. (1,1): 3→fila0.          filas: C0=[1,3]  C1=[2]
       entrega: CPU0 tira 1 da própria fila; CPU1 tira 2.                             filas: C0=[3]    C1=[]
t=100  lote [4,5]. Cargas C0=1+1=2, C1=0+1=1: 4→fila1. Depois (2,2): 5→fila0.         filas: C0=[3,5]  C1=[4]
       ninguém ociosa: nada é entregue
t=300  CPU1: PID 2 finalizado; devolver; tirar_proximo(1) → 4
t=500  CPU1: PID 4 finalizado; fila local vazia → rouba o ÚLTIMO da mais longa (C0=[3,5]) = 5
       "[CPU1] Migracao PID 5 CPU0 -> CPU1" e depois "[CPU1] Executando PID 5"         filas: C0=[3]    C1=[]
t=600  CPU0: PID 1 finalizado; tirar_proximo(0) → 3
t=1100 CPU1: PID 5 finalizado; nada a fazer; verificar_termino = 0 (CPU0 executando) → dorme
t=1500 CPU0: PID 3 finalizado; verificar_termino(1500) = 1 → "Escalonador terminou" e broadcast
```

**Armadilhas**
- ⚠️ `ociosa = 1` em `cpus_iniciar`. Sem isso, o lote de t=0 pode ser inserido antes de as escalonadoras entrarem em espera: ninguém recebe a entrega e o processo fica esquecido na fila. (No desenho anterior, em que cada escalonadora escolhia sozinha na partida, um caso mínimo saiu com a CPU errada em 32% das execuções.)
- ⚠️ A entrega percorre as CPUs em ordem crescente e dá `signal` na `cond` **da CPU que recebeu**.
- ⚠️ Em `cpus_esperar_trabalho` com POR_CPU: o `pthread_cond_wait` usa o mutex da **própria** fila; ao acordar, destrave-o e só então chame `cpus_travar()`. Travar os menores segurando um maior viola a ordem (deadlock, e o helgrind acusa).
- ⚠️ `cpus_verificar_termino` registra o término **uma vez** e faz broadcast em **todas** as `cond`.
- `cpus_inserir_lote` é a única função que trava sozinha. As outras supõem a trava feita por quem chama.
- `carga(k)` usa `filas[k]`, não `fila_da_cpu(k)`: ela só é chamada quando `nfilas > 1`.
- Na migração, a origem nunca é a própria CPU k (a fila dela está vazia, mas exclua `j = k` mesmo assim).
- `log_registrar` é chamado com as filas travadas. Está certo: a ordem é `filas → log`.

**Teste** `tests/test_cpus.c`: dá para testar o módulo inteiro **com uma thread só**, antes de o escalonador de A existir, porque `cpus_esperar_trabalho` não dorme quando já há algo em `entregue`. O teste define `Config g_cfg;` e `const Politica *g_pol;`, usa um vetor de PCBs zerados, em ordem de PID (preencha `pid`, `priority = 3` e `ultima_cpu = −1`; o lote é um trecho contíguo desse vetor), e chama `tempo_iniciar` e `log_iniciar`.

*Parte 1, GLOBAL, 2 CPUs, FCFS, processos 1, 2, 3:*
```
cpus_iniciar()                              → g_cpus[0].ociosa = g_cpus[1].ociosa = 1
cpus_inserir_lote([1,2,3], 3, t=0, ultimo=1) → g_cpus[0].entregue = P1, g_cpus[1].entregue = P2,
                                              g_cpus[0].t_entrega = 0, g_cpus[0].executando = P1
cpus_travar()
cpus_esperar_trabalho(0, &t)                → devolve P1 sem dormir; t = 0; g_cpus[0].ociosa = 0
cpus_esperar_trabalho(1, &t)                → devolve P2
cpus_devolver(0, NULL); cpus_tirar_proximo(0, 1000) → P3
cpus_devolver(1, NULL); cpus_tirar_proximo(1, 1600) → NULL;  cpus_verificar_termino(1600) → 0
cpus_devolver(0, NULL); cpus_tirar_proximo(0, 3000) → NULL;  cpus_verificar_termino(3000) → 1
cpus_destravar(); cpus_destruir()
```
*Parte 2, POR_CPU, 2 CPUs, FCFS: o exemplo acima (`p01`), processos 1 a 5:*
```
cpus_iniciar()
cpus_inserir_lote([1,2,3], 3, t=0, ultimo=0)   → entregue: CPU0 = P1, CPU1 = P2
cpus_travar(); cpus_esperar_trabalho(0, &t) → P1; cpus_esperar_trabalho(1, &t) → P2; cpus_destravar()
cpus_inserir_lote([4,5], 2, t=100, ultimo=1)   → nada entregue (ninguém ociosa)
cpus_travar()
cpus_devolver(1, NULL); cpus_tirar_proximo(1, 300)  → P4
cpus_devolver(1, NULL); cpus_tirar_proximo(1, 500)  → P5      (migração: registra "Migracao PID 5 CPU0 -> CPU1")
cpus_devolver(0, NULL); cpus_tirar_proximo(0, 600)  → P3
cpus_devolver(1, NULL); cpus_tirar_proximo(1, 1100) → NULL;  cpus_verificar_termino(1100) → 0
cpus_devolver(0, NULL); cpus_tirar_proximo(0, 1500) → NULL;  cpus_verificar_termino(1500) → 1
cpus_verificar_termino(1500)                        → 1 de novo, sem registrar a 2ª vez
cpus_destravar()
log_gravar(arquivo)   → 5 linhas "Chegada" (PIDs 1 a 5), 1 linha "[CPU1] Migracao PID 5 CPU0 -> CPU1",
                        1 linha "Escalonador terminou execução de todos processos", nesta ordem
```
Não chame `cpus_esperar_trabalho` sem entrega pendente neste teste: com uma thread só, ele dormiria para sempre. Rode o teste sob `valgrind --leak-check=full`: 0 erros.

**Pronto quando:** `test_cpus` passa nas duas partes.

---

## B5 · `metricas.c` (ter 24/11)

**Lembretes do enunciado** [E §4.3]
- Depois da última linha do log, o mesmo arquivo recebe o bloco de métricas: PIDs em **ordem crescente**, médias com **duas casas decimais**.
- **Espera** = turnaround − duração · **Turnaround** = instante de finalização − chegada · **Resposta** = instante do primeiro despacho − chegada · **Makespan** = instante da última finalização.
- **Troca de contexto** = despacho de um PID diferente do último executado naquela CPU (o primeiro despacho de cada CPU conta). Quem conta é A, em `despachar`; você soma as CPUs.
- **Utilização da CPU** = tempo efetivamente executando processos ÷ makespan; a retenção ociosa do RR e da PRIORIDADE não conta.
- **Throughput** = número de processos ÷ makespan em segundos.
- O relatório de métricas vale 1,0 ponto [E §8].

**Contrato**
```c
/* metricas.h (dono: B) - bloco "=== METRICAS ===". */
#ifndef METRICAS_H
#define METRICAS_H
#include <stdio.h>
#include "pcb.h"
#include "cpus.h"
void metricas_gravar(FILE *saida, const PCB *pcbs, int n, const CPU *cpus, int num_cpus);
#endif
```

**Pseudocódigo**
```
metricas_gravar(f, pcbs, n, cpus, ncpu):
    ord ← vetor de ponteiros para os PCBs, ordenado por pid
    makespan ← maior finish_time                               (0 se n = 0)
    escrever "=== METRICAS ==="
    para cada p em ord:
        ta ← p.finish_time − p.start_time                      (turnaround)
        es ← ta − p.process_len                                (espera)
        re ← p.first_dispatch − p.start_time                   (resposta)
        escrever "PID %d | espera=%dms | turnaround=%dms | resposta=%dms"
        somar es, ta, re
    escrever "Espera media: %.2f ms"       (soma / (double) n)
    escrever "Turnaround medio: %.2f ms"
    escrever "Resposta media: %.2f ms"
    escrever "Trocas de contexto: %d"      (soma de cpus[k].trocas)
    para cada k: escrever "Utilizacao CPU%d: %.2f%%"   (100.0 × cpus[k].ocupado_ms / makespan)
    escrever "Throughput: %.2f processos/s"   (n / (makespan / 1000.0))
    escrever "Makespan: %d ms"
    (se n = 0 ou makespan = 0: imprimir 0 em vez de dividir)
```

**Por que é assim**
- **Tudo sai de instantes lógicos** (`first_dispatch`, `finish_time`) e de contadores (`ocupado_ms`, `trocas`), então os números são exatos e iguais aos esperados, sem depender do relógio.
- **`ocupado_ms` só soma fatias executadas.** A retenção ociosa fica de fora sem nenhum código especial. Caso 05: 2300/2700 = 85,19%.
- **Makespan é a última finalização**, não o instante do `Escalonador terminou` (caso 05: 2700 contra 2800).
- **Troca de contexto** é contada por CPU em `despachar`: PID diferente do último daquela CPU; o primeiro despacho de cada CPU conta.

**Armadilhas**
- O vetor `pcbs` chega ordenado por (chegada, PID), não por PID: ordene uma cópia **de ponteiros**. O vetor original não pode ser movido.
- Médias em `double`; `%%` para imprimir o `%`.
- Uma linha `Utilizacao` por CPU, mesmo que a CPU nunca tenha rodado nada (0.00%).

**Teste** `tests/test_metricas.c`: preencha à mão os três PCBs do exemplo 4.4 e uma CPU, grave num arquivo e compare com o bloco de métricas de `saida_01.txt`. O `diff` tem de sair **vazio**.

| PID | `start_time` | `process_len` | `first_dispatch` | `finish_time` |
|---|---|---|---|---|
| 1 | 0 | 1000 | 0 | 1000 |
| 3 | 100 | 1500 | 1000 | 2500 |
| 2 | 200 | 2000 | 2500 | 4500 |

CPU0: `ocupado_ms = 4500`, `trocas = 3`.
```bash
sed -n '/=== METRICAS/,$p' tests/casos/saida_01.txt | diff - saida_do_teste.txt
```

---

## B6 · `tests/run_all.sh` → marco M1-B (ter 24/11, 23h)

Leve o `compare.py` da etapa B0 para `tests/` e escreva o script que roda tudo:
```
tests/run_all.sh [mono|multi|porcpu|todos]
    grupos: mono   = casos 01 a 06
            multi  = casos 07 a 18, ex45 e g*            (GLOBAL)
            porcpu = p*
            todos  = os três
    para cada grupo pedido:
        compilar o alvo certo UMA vez (mono → make monoprocessador; os outros → make multiprocessador)
        para cada tests/casos/entrada_X.txt do grupo que exista:
            ./trabSO tests/casos/entrada_X.txt
            python3 tests/compare.py tests/casos/saida_X.txt log_execucao_minikernel.txt
            imprimir "ok X" ou "FALHOU X: <primeira diferença>"
    no fim: "N/M ok"; código de saída 0 só se todos passarem
```
Só existem na pasta os casos 01, 03, 05, 13, 15 e 17 (mais os extras); o script tem de ignorar os números que faltam.

**Pronto quando (M1-B):** PR `M1-B: infraestrutura` aberto, com `make teste_unidade` passando (fila, log, cpus, métricas), os autotestes do `compare.py` passando e os dois alvos compilando sem warnings. O programa ainda não roda de ponta a ponta: isso é o M2, de A.

---

## B7 · Revisão do M2 (qua 25/11)

A integra hoje e fecha os casos 01, 03 e 05 no mono. A sua parte:
- Revisar o PR `M1-A` e, à noite, o `M2`. Foque no uso que o laço faz de `cpus_*`: trava antes, trava depois, `esperar_chegadas_ate` em todo fim de despacho, nada de `cpus_destravar` entre `cpus_devolver` e `cpus_tirar_proximo`.
- Se um caso mono falhar por causa do seu módulo, a tabela do apêndice B aponta a causa.
- Confira que `tests/run_all.sh multi` já enxerga `ex45` e `g01` a `g06` (estão em `tests/casos/` desde o M0).

## B8 · Multi GLOBAL → marco M3 (qui 26 e sex 27/11, 23h)

**Lembretes:** os do GLOBAL na etapa B4. O multiprocessador com fila global vale 2,0 pontos [E §8].

- `make multiprocessador` e os casos 13, 15, 17, `ex45` e `g01` a `g06`. Corrija `cpus.c` até passar.
- O que cada caso prova:

| Caso | Se passar, está certo |
|---|---|
| `ex45`, 13 (FCFS) | entrega em ordem de índice; processo que chega com a CPU0 ocupada vai para a CPU1 |
| 15 (RR) | reinserção e escolha na mesma seção crítica; em t=7300 a CPU0 reescolhe o PID 11 com a CPU1 ociosa; término em 7700 com makespan 7400 |
| 17 (PRIORIDADE) | chegada de t=1200 vista pela escolha de t=1200; em t=300 a CPU0 reescolhe o PID 49 com a CPU1 ociosa |
| `g01` a `g06` | vários processos em t=0 e no mesmo instante, com 2 e 4 CPUs |

- Problemas típicos do GLOBAL: o primeiro processo sai na CPU1 (entrega fora da ordem de índice ou CPU que não nasceu ociosa); ociosa que nunca acorda (entrega sem `signal`, ou com a `cond` errada); término antecipado (`cpus_executando` não somado na entrega).
- `tests/run_all.sh multi` **5 vezes seguidas**, tudo ok. PR `M3: multi GLOBAL`. A revisa e roda helgrind e estresse.

**Pronto quando (M3):** 3 casos oficiais multi + `ex45` + 6 extras ok, 5 vezes seguidas, e A aprovou.

---

## B9 · POR_CPU → marco M4 (sáb 28 e dom 29/11, 23h)

**Lembretes:** os do POR_CPU na etapa B4. Vale 1,5 ponto [E §8]. O LEIA-ME avisa que os casos oficiais 08, 10, 12, 14, 16 e 18 têm migrações; nenhum deles veio no pacote público, então os seus testes são `p01` a `p07`.

- Ative e teste os caminhos `nfilas > 1`: colocação por `carga(k)`, migração, espera e entrega com várias filas.
- Casos: `tests/run_all.sh porcpu`. Comece pelo `p01`, que você já acompanhou à mão. Depois `p02` a `p07` (2 migrações cada).
- Confira à mão as migrações de um caso com `python3 ferramentas/contar_migracoes.py log_execucao_minikernel.txt`.
- A regra P1 (CPU retida conta como carga) está confirmada desde 24/11; `p03`, `p06` e `p07` passam por ela. Os dois pontos ainda em aberto do POR_CPU (P2 e P8, no apêndice A) já estão decididos no pseudocódigo. Se aparecer no Classroom algum caso oficial POR_CPU, rode o oráculo nele: se não bater, o ponto errado é um desses dois; avise por issue antes de mexer.
- Problemas típicos: `Migracao` que não devia existir (ociosa roubando o que acabou de chegar para outra: falta a entrega direta); migração do primeiro em vez do último; deadlock ao acordar (retravou tudo sem soltar o próprio mutex); linha `Migracao` na CPU de origem em vez da de destino.
- Comente todos os seus arquivos: cabeçalho do arquivo com o papel do módulo; em cada função, o que faz, **quais travas supõe já tomadas e quais toma**. Código sem comentários dá desconto, e a organização vale 1,0 ponto [E §8].
- PR `M4: POR_CPU`. A roda helgrind e estresse com 8 CPUs; corrija o que vier por issue `bug`.

**Pronto quando (M4):** `p01` a `p07` ok 3 vezes seguidas, a `main` passa em `tests/run_all.sh todos`, e A aprovou.

---

## B10 · Bônus de afinidade (seg 30/11)

É o primeiro item a ser cortado se algo atrasar: o POR_CPU vale 1,5 ponto e o bônus, no máximo 0,5.

**Lembretes do enunciado** [E §9]
- Registrar no BCP a última CPU em que cada processo executou e usar esse campo como desempate (`ultima_cpu` já é gravado por A em `despachar`).
- Identificar e documentar em quais políticas e modos existe empate de fato.
- **O comportamento padrão não pode mudar**: os casos de teste esperam as saídas sem afinidade. O bônus liga e desliga por flag de compilação, sem alterar o formato da entrada nem da saída.
- Medir com e sem afinidade: número de migrações (POR_CPU) ou de despachos numa CPU diferente da anterior (GLOBAL, extraído do log), além das métricas. **Gerar um segundo arquivo de saída** com a análise exclusiva da afinidade.
- No README: o ganho observado, se as métricas mudaram ou não, e por quê.

Enunciado [E §9]: "em caso de empate na seleção, preferir o processo cuja última CPU seja a que está livre". O comportamento padrão **não pode mudar**: a afinidade fica atrás de uma flag de compilação (`make multiprocessador AFINIDADE=1` → `-DAFINIDADE`), sem mudar entrada nem saída. A medição vai num **segundo arquivo**, `ANALISE_AFINIDADE.md`.

Onde existe empate de fato:

| Política | Mono | GLOBAL (2+ CPUs) | POR_CPU (2+ CPUs) |
|---|---|---|---|
| FCFS, RR | não: fila FIFO estrita | não | só na **migração**, quando duas ou mais filas empatam como a mais longa |
| PRIORIDADE | há empate de prioridade, mas a afinidade não se aplica: com 1 CPU todo processo que já rodou tem `ultima_cpu = 0` | **sim**: mesma prioridade na fila global | **sim**: mesma prioridade na fila local, e também na migração |

As duas regras (ambas atrás de `#ifdef AFINIDADE` e só com `g_cfg.num_cpus > 1`):
```
[politica.c] escolher_prioridade(f, cpu), no ponto /* BONUS AFINIDADE */:
        se melhor = NULO ou p.priority < melhor.priority: melhor ← p
#ifdef AFINIDADE
        senão se g_cfg.num_cpus > 1 E p.priority = melhor.priority
                 E melhor.ultima_cpu ≠ cpu E p.ultima_cpu = cpu: melhor ← p
#endif

[cpus.c] cpus_tirar_proximo, ao escolher a origem da migração:
    maior ← maior tamanho entre as filas j ≠ k;  origem ← menor j com esse tamanho
#ifdef AFINIDADE
    para j crescente, j ≠ k, com filas[j].tamanho = maior:
        se filas[j].fim.ultima_cpu = k: origem ← j;  parar
#endif
    (o processo roubado continua sendo o ÚLTIMO da fila escolhida)
```
Em palavras: entre os processos de melhor prioridade, prefere o primeiro (na ordem da fila) cuja última CPU é a que está escolhendo; se nenhum tiver, vale o FIFO. Na migração, entre as filas empatadas como a mais longa, prefere a de menor índice cujo último processo já rodou na CPU que rouba.

Prévia com o oráculo (150 cargas aleatórias de 10 processos, Q=500, médias "sem → com"). **É só prévia**: os números do README têm de sair da execução real com `carga_analise.txt`.

| Configuração | logs que mudam | despachos em outra CPU | trocas | espera média | resposta média | makespan |
|---|---|---|---|---|---|---|
| FCFS e RR (GLOBAL e POR_CPU, 2 e 4 CPUs) | 0% | igual | igual | igual | igual | igual |
| PRIORIDADE GLOBAL 2 CPUs | 94% | 3,3 → 0,8 | 15,4 → 11,7 | 1478 → 1411 | 986 → 1115 | 6701 → 6732 |
| PRIORIDADE GLOBAL 4 CPUs | 59% | 2,1 → 1,0 | 12,3 → 11,2 | 186 → 179 | 94 → 113 | 4435 → 4455 |
| PRIORIDADE POR_CPU 2 CPUs | 86% | 0,4 → 0,1 | 15,3 → 11,5 | 1489 → 1393 | 907 → 1091 | 6724 → 6767 |
| PRIORIDADE POR_CPU 4 CPUs | 28% | 0,7 → 0,5 | 11,3 → 10,7 | 180 → 175 | 104 → 123 | 4422 → 4445 |

No caso público 17 (PRIORIDADE, GLOBAL, 2 CPUs): despachos em outra CPU 4 → 1, trocas 15 → 11, espera média 1475 → 1437,5, resposta média 1000 → 1050, makespan 6400 → 6600.

O que a discussão do README precisa cobrir, com os números reais:
1. **Onde há ganho:** só na PRIORIDADE com 2 ou mais CPUs. No FCFS e no RR não há empate na seleção, e o empate de filas na migração quase nunca tem candidato com afinidade: os logs saem idênticos. É o esperado, não é falha.
2. **O ganho:** caem os despachos em outra CPU e as trocas de contexto. Num processador real isso significa cache e TLB mais "quentes".
3. **Por que as trocas caem:** no fim do quantum o preemptado volta à fila com `ultima_cpu` igual à CPU que vai escolher; se houver outro de mesma prioridade, a afinidade reescolhe o preemptado. A alternância "como no RR" entre iguais deixa de acontecer.
4. **Métricas:** a espera média cai um pouco; a resposta média **piora**, porque processos novos (`ultima_cpu = −1`) perdem o empate para quem já rodou; o makespan sobe um pouco.
5. **Por que o POR_CPU ganha menos:** cada processo já volta para a fila da própria CPU, então quase todo candidato local já tem afinidade. As linhas `Migracao` praticamente não mudam, porque o roubado continua sendo o último da fila.
6. **Conclusão:** a afinidade troca justiça entre iguais e tempo de resposta por localidade. Por isso fica desligada por padrão.

**`ferramentas/medir_afinidade.sh`**
```
para cada política em (FCFS, RR, PRIORIDADE), modo em (GLOBAL, POR_CPU), CPUs em (2, 4):
    entrada ← cabeçalho + as linhas de processos de carga_analise.txt
    make multiprocessador             → rodar 3 vezes → contar_migracoes.py → mediana     (SEM afinidade)
    make multiprocessador AFINIDADE=1 → rodar 3 vezes → contar_migracoes.py → mediana     (COM afinidade)
    conferir as duas com o oráculo (sem e com --afinidade) quando a entrada não for ambígua
escrever ANALISE_AFINIDADE.md: uma tabela "sem | com | diferença" por configuração, com migracoes_log,
    despachos_outra, trocas, espera média, resposta média, turnaround médio, makespan e utilização
recompilar SEM afinidade no fim                                (para ninguém entregar o binário errado)
```
`ANALISE_AFINIDADE.md` vai no repositório e no pacote, ao lado do README. A tabela principal do README (de A) continua sem afinidade.

**Teste**
- Com a flag **desligada**: `tests/run_all.sh todos` continua 100% ok.
- Com a flag ligada: o mono fica idêntico; no multi, o caso 17 tem de bater com `python3 ferramentas/oraculo.py tests/casos/entrada_17.txt --afinidade` (use o `compare.py`).

## B11 · README: "Decisões de projeto" → marco M5 (seg 30/11, 23h)

**Lembretes do enunciado** [E §6, §8]: o README precisa ter as decisões de projeto (módulos, estruturas, **ordem de travamento dos mutexes** e **como cada regra da seção 2 foi implementada**) e a análise comparativa. Vale 1,5 ponto.

Estrutura, com o que cada item precisa conter:
1. **Como compilar e executar:** `make monoprocessador`, `make multiprocessador`, `./trabSO entrada.txt`, `make multiprocessador AFINIDADE=1`.
2. **Módulos:** a tabela da seção 1.2, com uma frase por arquivo. A política fica isolada em `politica.c`: na 2ª etapa, política nova é mais uma instância de `Politica`.
3. **Estruturas:** PCB e TCB do enunciado, mais cada campo extra e o seu porquê; `CPU` e `Fila`.
4. **Sincronização:** a ordem global de travamento e por que impede deadlock; os pares condição/mutex; o protocolo da fatia com duas barreiras (`pthread_barrier_t`, como o enunciado sugere) e a líder (a thread que recebe `PTHREAD_BARRIER_SERIAL_THREAD`); por que não há lost wakeup; por que o POR_CPU trava todas as filas.
5. **Como cada regra da seção 2 foi implementada** (tabela regra → onde está no código → como): desempate 1 (vetor ordenado e lote); desempate 2 e "chegadas antes de qualquer escolha" (`esperar_chegadas_ate`); desempate 3 (entrega direta, CPUs nascem ociosas); desempate 4 (varredura com `<` estrito); fronteiras de fatia; contabilização `100/n + resto`; retenção ociosa; reinserir e escolher na mesma seção crítica; colocação, migração e "ociosas tentam de novo"; término.
6. **Modelo de tempo:** instante lógico × tempo medido; prazos absolutos; o log imprime o medido e ordena pelo lógico; as métricas usam o lógico.
7. **Diferenças em relação à letra do enunciado**, com justificativa: `dormir_ate(prazo)` em vez de `usleep(FATIA*1000)`; a líder da fatia em vez de "a thread que detectar primeiro"; `generator_done` como variável interna de `cpus.c`.
8. **Decisões em pontos não especificados:** P1 a P8 (apêndice A; P1 foi confirmado em 24/11, os outros são leituras nossas) e a tolerância de 150 para métricas com valor esperado 0 no comparador.
9. **Testes:** como rodar `tests/run_all.sh`, o que o comparador verifica, os resultados de valgrind e helgrind, a rotina de estresse.
10. **Bônus: afinidade** (resumo e ponteiro para `ANALISE_AFINIDADE.md`) e **Autoria** (quem fez cada módulo).

Além disso, escreva o parágrafo **GLOBAL × POR_CPU** da discussão, a partir da tabela de A: utilização por CPU, número de migrações, equilíbrio de carga (o desbalanceamento aparece como diferença de utilização entre as CPUs) e o custo de sincronização de cada modelo.

**Pronto quando (M5):** PR `M5: bônus + decisões` aprovado por A.

---

## B12 · Congelamento → marco M6 (ter 01/12, 23h) e entrega

**Lembretes do enunciado**
- O professor roda `tar -xzvf <arquivo>.tar.gz`, `make <monoprocessador | multiprocessador>` e `./trabSO <entrada>`. **Siga esse padrão à risca** [E §4.7]. Os arquivos ficam na **raiz** do pacote.
- A submissão inclui todos os fontes e o Makefile. **Só um integrante envia**, e o nome tem as matrículas dos dois: `TP-SO-2026-2-<mat1>-<mat2>` [E §7].
- Erro de compilação: nota zero. `goto`: nota zero [E §8].
- Prazo: **04/12/2026, 07h59**, sem aceitação depois disso [E §7].

**`ferramentas/empacotar.sh <mat1> <mat2>`**
```
NOME ← "TP-SO-2026-2-<mat1>-<mat2>"
make clean
tar -czf NOME.tar.gz  Makefile *.c *.h README.md ANALISE_AFINIDADE.md tests/casos tests/compare.py tests/run_all.sh tests/test_*.c
extrair numa pasta temporária vazia e, lá dentro:
    make monoprocessador CFLAGS_EXTRA=-Werror  &&  ./trabSO tests/casos/entrada_01.txt
    make multiprocessador CFLAGS_EXTRA=-Werror &&  ./trabSO tests/casos/entrada_13.txt
imprimir "pacote ok: NOME.tar.gz"                 (o script para no primeiro erro)
```
Ficam **fora** do pacote: `STATUS.md`, `ferramentas/` (inclusive o oráculo), logs e binários. O `-Werror` entra só nesse teste.

**Checklist do congelamento**
- [ ] `grep -n goto *.c` vazio
- [ ] `grep -n "printf\|puts\|stderr" *.c` só mostra `fprintf`/`fputs` para o arquivo de saída e `snprintf`/`vsnprintf`
- [ ] os dois alvos sem warnings; `tests/run_all.sh todos` ok; valgrind e helgrind com 0 erros
- [ ] README completo; `ANALISE_AFINIDADE.md` gerado; binário recompilado **sem** afinidade
- [ ] tag `v1.0`. Depois dela, só correção de bug bloqueante, sempre por PR

**Qua 02/12:** fique disponível para corrigir o que o teste "de professor" de A achar no LabGrad.
**Qui 03/12:** depois que A enviar, **baixe o arquivo do Classroom**, extraia numa pasta limpa, rode os dois `make` e um caso de cada. Escreva "conferido" no `STATUS.md`.

**Se atrasar**
- Fila ou log (seg 23) são caminho crítico: merge até terça 12h no mais tardar; A pode usar a sua branch enquanto isso.
- Atrasou o GLOBAL: A ajuda na sexta, e o POR_CPU começa no sábado de qualquer jeito.
- Atrasou o POR_CPU: corte o bônus.

---

## Parada 2 · O resto da metade de A: barreira, entrada, `main`, estresse e análise

Você revisa esses PRs e pode ter de explicar qualquer um deles.

### `barreira.c`
```c
/* barreira.h (dono: A) - barreira reutilizavel com lider, sobre pthread_barrier_t. */
#ifndef BARREIRA_H
#define BARREIRA_H
#include <pthread.h>
typedef struct {
    pthread_barrier_t b;        /* barreira POSIX, reutilizavel; count = participantes */
} Barreira;
void barreira_iniciar(Barreira *b, int total);   /* total >= 1 (com 0 o init falha) */
int  barreira_esperar(Barreira *b);   /* devolve 1 para UMA thread por rodada (a lider) e 0 para as demais */
void barreira_destruir(Barreira *b);  /* so quando nenhuma thread esta esperando nela */
#endif
```
```
barreira_iniciar(b, total): pthread_barrier_init(b.b, NULL, total)         (total ≥ 1)
barreira_esperar(b):
    r ← pthread_barrier_wait(b.b)            (bloqueia até as total threads chegarem e solta todas juntas)
    devolver 1 se r = PTHREAD_BARRIER_SERIAL_THREAD, senão 0       (exatamente uma por rodada: a líder)
barreira_destruir(b): pthread_barrier_destroy(b.b)
```
- **Segue a sugestão do enunciado** [E §2.4] (decisão de 25/11; antes a barreira era própria, com mutex, cond e um contador de geração). A `pthread_barrier_t` já é reutilizável, então a 2ª barreira da fatia pode vir colada na 1ª, e não tem predicado nem contador nosso para errar: um wakeup espúrio não solta ninguém.
- **A líder é a thread que recebe `PTHREAD_BARRIER_SERIAL_THREAD`.** O POSIX garante exatamente uma por rodada, mas não diz qual (na glibc é a última a chegar). Para o protocolo da fatia tanto faz: quando qualquer thread sai da barreira, todas já descontaram a sua parte de `remaining_time` (sob `p->mutex`), e `pthread_barrier_wait` sincroniza a memória. A líder vê o valor final, seja ela quem for.
- **Com `total = 1`** a única thread recebe sempre o valor especial e nunca espera.
- **O invólucro `Barreira` fica.** As três funções escondem o tipo POSIX: `pcb.c`, `pcb.h` e o teste não mudam com a troca.
- ⚠️ Nada de `-std=c11`/`-std=c99` no Makefile nem nos testes: escondem `pthread_barrier_t`. Se precisar fixar, `-std=gnu11`.

### `entrada.c` e `config.h`
```c
/* config.h (dono: A) - configuracao lida do arquivo de entrada. */
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
extern Config g_cfg;            /* definido em main.c; so leitura depois de entrada_ler */
#endif
```
```c
/* entrada.h (dono: A) - leitura do arquivo de entrada. */
#ifndef ENTRADA_H
#define ENTRADA_H
#include "config.h"
#include "pcb.h"
/* Devolve n (ou -1 se nao abrir, sem imprimir nada). *pcbs = vetor alocado, ordenado por
 * (start_time, pid), com pcb_iniciar ja chamado em cada um.
 * Com -DMONO: num_cpus = 1 e modo_fila = FILA_GLOBAL, qualquer que seja o arquivo. */
int entrada_ler(const char *caminho, Config *cfg, PCB **pcbs);
#endif
```
```
entrada_ler(caminho, cfg, pcbs):
    f ← abrir(caminho, leitura);  se falhou: devolver −1                 (sem imprimir nada)
    ler "CPUS" n       → cfg.num_cpus
    ler "QUANTUM" q    → cfg.quantum
    ler "POLITICA" s   → cfg.politica   (FCFS | RR | PRIORIDADE)
    ler "FILA" s       → cfg.modo_fila  (GLOBAL | POR_CPU)
    ler "PROCESSOS" n  → cfg.n_processos
    #ifdef MONO:  cfg.num_cpus ← 1;  cfg.modo_fila ← FILA_GLOBAL
    v ← vetor ZERADO de n PCBs                                           (calloc; se n = 0, aloque 1 posição)
    para i de 0 a n−1: ler pid, start_time, process_len, priority, num_threads em v[i]
    fechar f
    ordenar v por (start_time, pid)                                      (qsort)
    para cada p de v: pcb_iniciar(p)                                     ⚠️ só DEPOIS de ordenar
    *pcbs ← v;  devolver n
```
- Ordenar por (chegada, PID) deixa a geradora trivial e resolve o desempate 1.
- `pcb_iniciar` vem depois do `qsort` porque um mutex já iniciado não pode ser movido. Depois que as threads existem, o vetor não muda mais de lugar: as suas filas guardam ponteiros para dentro dele.
- `-DMONO` é a única diferença de código entre os dois alvos.

### `main.c`
```
definir: Config g_cfg;  const Politica *g_pol;

main(argc, argv):
    se argc ≠ 2: devolver 1                                        (sem imprimir nada)
    n ← entrada_ler(argv[1], &g_cfg, &pcbs);  se n < 0: devolver 1
    tempo_iniciar()                                                (T0: logo após a leitura)
    g_pol ← politica_obter(g_cfg.politica)
    log_iniciar(g_pol.nome);  cpus_iniciar();  geradora_iniciar(pcbs, n)
    para cada k: pthread_create(&g_cpus[k].tid, thread_escalonador, &g_cpus[k])
    pthread_create(&tid_geradora, thread_geradora, NULO)
    join da geradora;  join de cada escalonadora;  join de cada thread de cada processo
    f ← abrir("log_execucao_minikernel.txt", escrita)
    log_gravar(f);  metricas_gravar(f, pcbs, n, g_cpus, g_cfg.num_cpus);  fechar f
    log_destruir();  cpus_destruir();  geradora_destruir()
    para cada p: pcb_destruir(p);  liberar pcbs
    devolver 0
```

### Estresse e invariantes (A, a partir de qui 26/11)
O enunciado avisa que há um caso oculto de estresse: dezenas de processos, até 4 threads por processo, 8 CPUs [E §8]. A escreve `ferramentas/gerar_estresse.py` (entradas aleatórias com muitos processos no mesmo instante) e `ferramentas/verificar_invariantes.py`, que confere num log o que vale em qualquer execução correta, mesmo em entrada ambígua:
1. cada PID tem exatamente uma `Chegada` e um `finalizado`; há exatamente um `Escalonador terminou`, na última linha;
2. nenhum PID executa antes de chegar, nem em duas CPUs ao mesmo tempo; nenhuma CPU tem dois processos;
3. em cada CPU, depois de `Executando PID p` vem `Quantum esgotado PID p` (Q depois) ou `PID p finalizado`;
4. no RR e na PRIORIDADE, depois de um `finalizado` a CPU só despacha de novo no fim do quantum (retenção);
5. o tempo executado de cada PID é igual à duração;
6. FCFS não tem `Quantum esgotado`; só POR_CPU tem `Migracao`;
7. as métricas batem com o próprio log, e `Trocas de contexto` é exata.

O programa roda com `timeout 120`: se estourar, é deadlock. As falhas chegam para você por issue com o rótulo `bug` e a entrada anexada.

```bash
valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./trabSO tests/casos/entrada_05.txt
valgrind --tool=helgrind --error-exitcode=1 ./trabSO caso_pequeno.txt
```
- **Objetivo: 0 erros nos dois.** Memcheck a partir do M2, helgrind a partir do M3.
- Sob o helgrind o programa fica dezenas de vezes mais lento e os tempos impressos saem errados. É normal: use casos de 8 a 12 processos e olhe só o relatório de erros.
- "lock order violated": alguém travou fila e processo juntos, ou as filas fora da ordem crescente.
- "Possible data race" em `ultimo_pid`, `ocupado_ms` ou `trocas`: a `main` leu antes do `pthread_join`.

### Análise comparativa (A, sáb 28 e seg 30/11)
A roda `carga_analise.txt` em: mono com as três políticas; RR mono com Q = 100, 500 e 1000; multi com 2 e 4 CPUs × GLOBAL e POR_CPU × as três políticas. Cada configuração 3 vezes, guardando a mediana. A tabela vai para o README, com a discussão das quatro perguntas do enunciado [E §6]: qual política minimizou a espera média e por quê; o que acontece com os processos de baixa prioridade; como o quantum afeta resposta e trocas de contexto; e o que muda entre fila global e filas por CPU (**este último parágrafo é seu**).

---

## Apêndice A · Pontos em aberto e o padrão adotado

Pontos que o enunciado não fecha. Cada um tem um **padrão adotado**; siga o padrão e registre no README (seção de decisões). **P1 foi confirmado em 24/11 e deixou de ser dúvida**; os demais continuam sendo leituras nossas.

| # | Pergunta | Padrão adotado | Por quê / como confirmar |
|---|---|---|---|
| P1 | POR_CPU: uma CPU **retida** (terminou antes do quantum) conta como carga na colocação? | **conta. Confirmado em 24/11** | durante a retenção a CPU ainda não foi liberada, então vale 1 na soma da carga. No código não há nada especial: `executando` só volta a NULO em `cpus_devolver`, no fim da retenção, e `carga(k)` soma 1 enquanto ele não é NULO. Os casos `p03`, `p06` e `p07` exercitam a regra |
| P2 | POR_CPU: quem esgota o quantum volta para qual fila? | a da **própria** CPU | "reinserido no fim da fila" [E §2.5]; migração só existe com fila local vazia |
| P3 | POR_CPU: chegada em t e uma CPU finaliza em t. Essa CPU ainda conta carga? | **conta** | chegadas vêm antes das finalizações no mesmo instante [L]. No código: `esperar_chegadas_ate` vem antes de `cpus_devolver` |
| P4 | Duas CPUs liberadas no mesmo instante disputam a mesma fila | qualquer ordem | o LEIA-ME garante que os casos oficiais não dependem disso |
| P5 | Onde o bônus de afinidade tem empate de fato? | PRIORIDADE com 2+ CPUs; e a origem da migração no POR_CPU | fila FIFO não tem empate; com 1 CPU a afinidade não muda nada |
| P6 | O log mostra o tempo medido ou o lógico? | o **medido**; o lógico só ordena e alimenta as métricas | o enunciado pede `gettimeofday`; com prazos absolutos o erro fica em poucos ms (tolerância: 150) |
| P7 | CPU1 já ociosa, CPU0 libera em t, e um processo chega em t. Quem pega? | a que **já estava ociosa** (recebe na inserção) | é consequência da entrega direta. A leitura alternativa (menor índice) daria CPU0. Nenhum caso público passa por aí, e o oráculo marca essas entradas como ambíguas |
| P8 | POR_CPU: no fim do quantum, uma CPU ociosa pode roubar o processo recém-reinserido? | **não**: reinserir e escolher ficam na mesma seção crítica, como no GLOBAL | o enunciado só diz isso para o GLOBAL; estendemos ao POR_CPU para não criar migração em pingue-pongue |

## Apêndice B · Sintoma → causa

| Sintoma | Causa mais provável |
|---|---|
| `Executando` antes de `Chegada` no mesmo instante | categoria errada em `log_registrar`, ou ordenação que ignora a categoria |
| caso 05, t=300: roda o PID 5 de novo em vez do 9 | faltou `esperar_chegadas_ate` antes de reinserir |
| caso 17, t=1200: não escolhe o PID 55 | idem: a chegada de t tem de estar na fila antes de **qualquer** escolha em t |
| uma fatia a mais depois do quantum | faltou a 2ª barreira, ou o orçamento não é decrementado pela líder |
| `finalizado` duplicado, ou tempo descontado a mais | mais de uma thread decide: só a líder mexe em `fatias`, `orcamento` e `state` |
| caso 05: término em 2700 em vez de 2800 | retenção pulada (`t_lib` tem de ser `t + Q`) |
| caso 05: `Utilizacao CPU0: 100.00%` em vez de 85.19% | a retenção entrou em `ocupado_ms` |
| caso 03: `Trocas de contexto` diferente de 4 | contou troca na reescolha do mesmo PID |
| `Quantum esgotado` junto com `finalizado` no mesmo instante | a líder testou o orçamento antes de `remaining_time = 0` |
| primeiro processo de t=0 sai na CPU1 | CPU que não nasce ociosa, escalonadora que escolhe sozinha na partida, ou entrega fora da ordem de índice |
| CPU ociosa nunca acorda | entrega sem `signal`, ou `signal` com o mutex errado |
| término antecipado, com processo ainda entregue | `cpus_executando` não foi somado na entrega |
| `Migracao` que não deveria existir | ociosas acordadas para disputar a fila, em vez de entrega direta |
| não termina (trava no fim) | `generator_done` nunca ligado (`ultimo`), `cpus_verificar_termino` não chamado quando a fila esvazia, ou falta o broadcast nas `cond` |
| trava com processo de 1 thread | a barreira com `total = 1` tem de devolver 1 na hora |
| `unknown type name 'pthread_barrier_t'` ao compilar | compilado com `-std=c11`/`-std=c99`; tire o `-std` ou use `-std=gnu11` |
| trava só no POR_CPU | filas travadas fora de ordem, ou `cpus_esperar_trabalho` retravou tudo sem soltar antes o próprio mutex |
| tempos derivando alguns ms a cada fatia | dormiu "100 ms a partir de agora" em vez de até o prazo absoluto |
| espera de 1 s ou mais que não acontece | `usleep` com 1 000 000 ou mais: use `dormir_ate` |
| valgrind: bytes perdidos | `tcbs`, `thread_ids`, filas ou o buffer do log não liberados |

## Apêndice C · Se o professor perguntar

O professor pode fazer entrevista, e a nota depende dela [E §8]. Cada um precisa saber responder a tudo isto, e não só à própria metade:

1. **Por que duas barreiras por fatia?** A 1ª junta todas as threads no fim da fatia e elege a líder (a única que recebe `PTHREAD_BARRIER_SERIAL_THREAD`). A 2ª segura todas até a líder decidir; sem ela, uma thread voltaria ao topo, veria RUNNING e começaria uma fatia a mais.
2. **Como a preempção só acontece em fronteira de fatia?** O escalonador não interrompe ninguém: dá um orçamento de fatias, e a líder põe READY quando ele zera.
3. **Qual é a ordem de travamento?** `filas (índice crescente) → log` e `processo → log`; fila e processo nunca juntos. Sem ciclo, sem deadlock.
4. **Como "quem chegou entra antes do preemptado" funciona com threads reais?** A escalonadora chama `esperar_chegadas_ate(t)` antes de reinserir e escolher; a geradora só libera essa espera depois de pôr o lote inteiro de t na fila.
5. **Os tempos do log são reais?** Sim, medidos com `gettimeofday`. O instante lógico só ordena as linhas e alimenta as métricas.
6. **O que é a retenção?** No RR e na PRIORIDADE a CPU fica presa até o fim do quantum mesmo que o processo termine antes. Não conta na utilização porque `ocupado_ms` só soma fatias executadas.
7. **Como a CPU de menor índice é garantida?** Entrega direta: quem insere escolhe pelas ociosas em ordem de índice; elas não disputam a trava. E toda CPU nasce ociosa.
8. **Por que, no POR_CPU, travar todas as filas?** Carga, migração, entrega e término olham várias filas ao mesmo tempo. Cada CPU continua com a sua fila e o seu mutex; são no máximo 8, sempre em ordem crescente.
9. **Como evitam lost wakeup?** Predicado em `while`, mudança sob o mutex parceiro, e a entrega fica guardada em `cpu.entregue`: quem ainda não dormiu encontra o campo preenchido.
10. **Onde entra uma política nova (2ª etapa)?** Em `politica.c`: mais uma instância de `Politica`. O escalonador não tem nenhum `if (politica == ...)`.
11. **Quando o programa termina?** Quando `generator_done`, todas as filas vazias e nenhuma CPU executando ou retida. Por isso `Escalonador terminou` pode sair depois do makespan (caso 05: 2800 contra 2700).
