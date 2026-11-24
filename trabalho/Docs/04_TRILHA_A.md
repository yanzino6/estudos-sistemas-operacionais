# Trilha A (Yan) · motor, políticas, estresse e análise

Este é o **único arquivo que você precisa abrir** para implementar a sua metade e para
entender o trabalho inteiro, inclusive a metade de B. Ele supõe que você já leu o
enunciado e o `LEIA-ME` dos casos; não repete o escopo, mas relembra a regra exata no
ponto em que ela vira código. Fontes dos lembretes: **[E §x]** = seção do enunciado,
**[L]** = LEIA-ME dos casos, **[C nn]** = caso público que prova a regra.

Como a trilha está organizada:
- **Etapas A0 a A14**, na ordem em que você implementa. Cada uma tem: *Lembretes do enunciado*, *Contrato* (o `.h`), *Pseudocódigo*, *Por que é assim*, *Armadilhas*, *Teste* e *Pronto quando*. O pseudocódigo foi validado: uma implementação feita linha a linha a partir dele passa nos 6 casos públicos, nos 14 casos extras, no valgrind e no helgrind. As linhas com ⚠️ são as que, se mudarem, quebram algum caso.
- **Paradas 1 a 3**: o código de B explicado (contrato, pseudocódigo e porquês), colocadas logo antes de você precisar dele. Você não implementa essas partes, mas chama, revisa e pode ter de explicar numa entrevista.
- **Apêndices**: pontos em aberto, tabela "sintoma → causa" e perguntas de entrevista.

**Índice:** 0. Calendário e combinados · 1. O desenho em uma página · 2. Ambiente e ferramentas · A0 M0 · A1 `tempo.c` · A2 `barreira.c` · A3 `entrada.c` · A4 `pcb.c` · *Parada 1: fila e log* · A5 `politica.c` · *Parada 2: `cpus.c`* · A6 `geradora.c` · A7 `escalonador.c` · A8 `main.c` (M1-A) · A9 integração (M2) · A10 estresse e helgrind · A11 M3 · A12 análise · A13 M4 · A14 M5, M6 e entrega · *Parada 3: métricas, comparador, bônus, pacote* · Apêndices A (pontos em aberto), B (sintoma → causa), C (entrevista)

Seus arquivos: `main.c`, `tempo.c`, `entrada.c`, `barreira.c`, `pcb.c`, `politica.c`, `geradora.c`, `escalonador.c`, `tests/test_barreira.c`, `tests/test_pcb.c`, `tests/test_entrada.c`, `tests/test_politica.c`, `ferramentas/gerar_estresse.py`, `ferramentas/verificar_invariantes.py`, `ferramentas/rodar_analise.sh` e a seção "Análise comparativa" do README.

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
| `barreira.c/.h` | A | barreira reutilizável com líder |
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
4. **Execução** (trabalhadoras de `p`): a cada fatia, todas dormem até o mesmo prazo, cada uma desconta a sua parte de `remaining_time` e todas se encontram numa barreira. A última a chegar (a líder) decide: zerou → FINISHED e `finalizado` no log; orçamento acabou → READY; senão, mais uma fatia.
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

**Ordem global (a frase vai para o README):** `filas (em ordem crescente de índice) → log` e `processo → log`. **Fila e processo nunca são travados ao mesmo tempo.** `mutex_chegadas` e o mutex da barreira são folhas: sempre travados sozinhos. Com uma ordem fixa não existe ciclo de espera e, portanto, não existe deadlock.

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

Pares condição/mutex (cada condição tem um único mutex parceiro): `p->cv` e `p->cv_escalonador` → `p->mutex` · `cpu.cond` → mutex da fila daquela CPU (no GLOBAL, o da fila global) · `cv_chegadas` → `mutex_chegadas` · `barreira.cond` → `barreira.mutex`.

**Três regras que valem para todo o código:** todo `pthread_cond_wait` fica dentro de um `while (condição)` · nenhum `usleep` com trava de fila ou de processo segurada · quem muda uma condição muda sob o mutex parceiro e acorda em seguida.

---

## 2. Ambiente e ferramentas

Você já está em Linux, que é onde o professor compila (Ubuntu do LabGrad). Falta só garantir os pacotes:
```bash
sudo apt install build-essential valgrind python3 git
gcc --version && valgrind --version && python3 --version
```
O teste final de quarta 02/12 é no LabGrad (ou numa VM Ubuntu limpa): é lá que vale.

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

## A0 · Marco M0: repositório, contrato e esqueleto (seg 23/11, até 14h)

B só consegue começar depois deste marco. Não escreva lógica nenhuma aqui.

1. Crie o repositório **privado** no GitHub (fora de `estudos-sistemas-operacionais`), adicione B como colaborador, branch padrão `main`.
2. Copie para a raiz os 12 headers da pasta `trabalho/contrato/` **sem alterar nada**: `tempo.h`, `config.h`, `barreira.h`, `pcb.h`, `fila.h`, `politica.h`, `log.h`, `cpus.h`, `geradora.h`, `entrada.h`, `escalonador.h`, `metricas.h`. São os mesmos headers que aparecem ao longo desta trilha. Crie o `Makefile` na raiz com o texto abaixo (as linhas de comando começam com TAB, não com espaços).
3. Crie um `.c` por módulo com **todas as funções do `.h` vazias** (as que devolvem valor devolvem 0 ou NULL): `tempo.c`, `barreira.c`, `entrada.c`, `pcb.c`, `geradora.c`, `politica.c`, `escalonador.c`, `fila.c`, `cpus.c`, `log.c`, `metricas.c`. O stub de `cpus.c` também define `CPU *g_cpus;`.
4. `main.c` mínimo: define `Config g_cfg;` e `const Politica *g_pol;` e tem um `main` que devolve 0.
5. `.gitignore` com `trabSO`, `*.o`, `log_execucao_minikernel.txt` e `tests/bin/`. `STATUS.md` com as seções "A" e "B".
6. `tests/casos/`: os 6 casos de `casos_publicos_v1/` e os 14 de `casos_extras/`. `ferramentas/`: `oraculo.py` e `contar_migracoes.py`.
7. Baixe do Classroom o **`carga_analise.txt`** (você precisa dele na etapa A12) e veja se há outro pacote de casos públicos além do v1. O LEIA-ME fala em 18 casos, a pasta tem 6, e o enunciado avisa que as saídas multi são só parcialmente divulgadas. Se aparecer caso oficial POR_CPU, avise B: ele resolve os pontos P2 e P8 (apêndice A).

O Makefile do M0:
```make
# Makefile (1a versao, do marco M0; B assume a partir dai)
CC     = gcc
CFLAGS = -Wall -pthread -O2 $(CFLAGS_EXTRA)
ifeq ($(AFINIDADE),1)
CFLAGS += -DAFINIDADE
endif
SRC = main.c tempo.c barreira.c entrada.c pcb.c geradora.c politica.c escalonador.c \
      fila.c cpus.c log.c metricas.c

.PHONY: monoprocessador multiprocessador clean

monoprocessador:
	$(CC) $(CFLAGS) -DMONO -o trabSO $(SRC)

multiprocessador:
	$(CC) $(CFLAGS) -o trabSO $(SRC)

clean:
	rm -f trabSO log_execucao_minikernel.txt tests/bin/*
```
- Compilar tudo num comando só evita misturar objetos de um alvo com os do outro.
- Não use `-std=c11` sozinho: ele esconde `usleep`. Se quiser fixar o padrão, `-std=gnu11`.
- **Nunca** `-Werror` no Makefile entregue (um warning novo do gcc do LabGrad viraria nota zero). Para treinar: `make multiprocessador CFLAGS_EXTRA=-Werror`.

**Pronto quando:** `make monoprocessador && make multiprocessador` compila **sem nenhum warning**; `STATUS.md` diz "M0 ok"; issue "M0 pronto" aberta para B.

---

## A1 · `tempo.c` (seg 23/11, tarde)

**Lembretes do enunciado**
- Tempo em milissegundos, medido com `gettimeofday` a partir de **T0**, marcado logo após a leitura do arquivo [E §2.1].
- FATIA = 100 ms; duração, chegada e quantum são sempre múltiplos de 100 [E §2.1].
- Nunca manter mutex de fila durante `usleep` [E §2.7].

**Contrato**
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

**Pseudocódigo**
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

**Por que é assim**
- **Prazo absoluto.** `dormir_ate(t)` recebe um instante, não uma duração. Se a thread acordou 3 ms atrasada numa fatia, a próxima dura 97 ms: o atraso não se acumula. Com `usleep(100 ms)` solto, o atraso de cada fatia se somaria ao das anteriores (o caso 15 tem 74 fatias por CPU) até passar da tolerância de 150 ms.
- **Pedaços de 500 ms.** O POSIX permite que `usleep` com 1 000 000 µs ou mais falhe com `EINVAL`. Um quantum de 1000 ms ou um intervalo longo entre chegadas cairia nisso.
- **O laço reconfere o relógio**, então acordar cedo (sinal, arredondamento) não faz mal.

**Armadilhas**
- Inclua `<sys/time.h>` e `<unistd.h>`. Faça a conta de `agora_ms` em `long` antes de converter para `int`.
- `T_INFINITO` e `ORCAMENTO_ILIMITADO` valem `0x3fffffff` de propósito: somar um pouco a eles não estoura `int`.

**Teste** (pode ir dentro de `tests/test_pcb.c` ou num teste só seu): `tempo_iniciar()`; `dormir_ate(250)` → `agora_ms()` entre 250 e 255; `dormir_ate(100)` volta na hora; `dormir_ate(1300)` → entre 1300 e 1305 (prova que passa de 1 s).

---

## A2 · `barreira.c` (seg 23/11, tarde)

**Lembretes do enunciado**
- "Uma thread conclui a fatia em andamento antes de bloquear. Sugestão: usar `pthread_barrier_t` para manter as threads de um processo alinhadas por fatia" [E §2.4].
- Sempre testar o predicado num `while` ao usar `pthread_cond_wait` [E §5].

**Contrato**
```c
/* barreira.h (dono: A) - barreira reutilizavel com lider, feita com mutex + cond. */
#ifndef BARREIRA_H
#define BARREIRA_H
#include <pthread.h>
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int total;                  /* participantes */
    int chegaram;               /* quantos ja chegaram nesta geracao */
    unsigned geracao;           /* muda a cada abertura; evita wakeup espurio e permite reuso */
} Barreira;
void barreira_iniciar(Barreira *b, int total);
int  barreira_esperar(Barreira *b);   /* devolve 1 para a ULTIMA thread a chegar (a lider) e 0 para as demais */
void barreira_destruir(Barreira *b);
#endif
```

**Pseudocódigo**
```
barreira_iniciar(b, total): iniciar mutex e cond;  b.total ← total;  b.chegaram ← 0;  b.geracao ← 0
barreira_esperar(b):
    travar b.mutex
    g ← b.geracao;  b.chegaram += 1
    se b.chegaram = b.total:                                (a última a chegar é a líder)
        b.chegaram ← 0;  b.geracao += 1;  broadcast(b.cond);  destravar;  devolver 1
    enquanto g = b.geracao: esperar(b.cond, b.mutex)        (predicado em while)
    destravar;  devolver 0
barreira_destruir(b): destruir mutex e cond
```

**Por que é assim**
- **`geracao`** é o predicado da espera: muda a cada abertura. Assim um wakeup espúrio não solta ninguém, e a barreira pode ser reusada logo em seguida (a 2ª barreira da fatia vem colada na 1ª).
- **Devolve quem é a líder** (1 para a última thread a chegar). É ela que toma as decisões da fatia.
- **Com `total = 1`** a única thread é sempre a líder e nunca espera.
- O enunciado só *sugere* `pthread_barrier_t`. A nossa tem 20 linhas, usa exatamente mutex e variável de condição (o assunto do trabalho) e é portável. Se um dia quiserem trocar, basta `barreira.c`: devolver 1 quando `pthread_barrier_wait` devolver `PTHREAD_BARRIER_SERIAL_THREAD`.

**Teste** `tests/test_barreira.c`: 4 threads, cada uma dá 1000 voltas chamando `barreira_esperar` e soma o valor devolvido no seu contador. No fim, a soma dos 4 contadores é **exatamente 1000** e o programa termina. Rode também sob `valgrind --tool=helgrind`: 0 erros.
```bash
mkdir -p tests/bin && gcc -Wall -pthread -I. -o tests/bin/test_barreira tests/test_barreira.c barreira.c && ./tests/bin/test_barreira
```

---

## A3 · `entrada.c` (seg 23/11, tarde)

**Lembretes do enunciado**
- Formato [E §4.1], cabeçalhos **sempre nesta ordem**, arquivos de teste sempre válidos:
  ```
  CPUS <1..8>
  QUANTUM <ms, múltiplo de 100>
  POLITICA <FCFS | RR | PRIORIDADE>
  FILA <GLOBAL | POR_CPU>
  PROCESSOS <n>
  <PID> <CHEGADA_ms> <DURACAO_ms> <PRIORIDADE 1..5> <NUM_THREADS 1..4>      (n linhas)
  ```
- Os processos **não têm ordem garantida** no arquivo. O PID é único, mas não é sequencial nem ordenado; não confundir com `getpid()` [E §2.1, §4.1].
- Na versão mono, CPUS vale 1 e FILA é ignorado. QUANTUM é ignorado no FCFS (quem cuida disso é `usa_quantum`, na política) [E §4.1].
- O programa não lê nada do teclado e não imprime nada na tela [E §4.7].

**Contrato**
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

**Pseudocódigo**
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

**Por que é assim**
- **Ordenar por (chegada, PID)** deixa a geradora trivial e já resolve o desempate 1: processos do mesmo instante ficam vizinhos, em PID crescente.
- **`-DMONO`** é a única diferença de código entre os dois alvos: força 1 CPU e fila global. O resto do programa não sabe em que alvo está.

**Armadilhas**
- ⚠️ `pcb_iniciar` só depois do `qsort`. O `qsort` move as structs com cópia de bytes, e um `pthread_mutex_t` já iniciado não pode ser movido.
- ⚠️ Depois que as threads existirem, o vetor **não pode mais mudar de lugar** (nada de `realloc`): há ponteiros `&pcbs[i]` nas filas, nos TCBs e nas CPUs.
- Arquivo que não abre: devolver −1 em silêncio.

**Teste** `tests/test_entrada.c` (só precisa de `pcb.c`, `barreira.c`, `tempo.c` e de um `log_registrar` vazio definido no próprio teste): ler `tests/casos/entrada_05.txt` → n = 4, ordem dos PIDs **5, 9, 2, 11**, `quantum = 300`, política RR. Ler `entrada_13.txt` compilado com `-DMONO` → `num_cpus = 1`; sem `-DMONO` → 2.

---

## A4 · `pcb.c`: o protocolo da fatia (seg 23/11, noite)

É o coração da sua metade. Leia o traço da fatia antes do pseudocódigo.

**Lembretes do enunciado**
- Cada BCP começa com `remaining_time = process_len`, estado READY, mutex e variável de condição iniciados e o vetor de threads alocado [E §2.2].
- As threads são criadas na chegada, mas **não trabalham**: ficam bloqueadas em `cv` até o escalonador pôr o processo em RUNNING [E §2.3].
- O laço de cada thread [E §2.4]: espera RUNNING; simula uma fatia; desconta o tempo sob mutex; se zerou, FINISHED, registra a finalização e faz broadcast para as demais encerrarem.
- **Contabilização:** em cada fatia, todas as threads executam em paralelo e, juntas, consomem exatamente 100 ms. A thread i desconta `FATIA / num_threads + (i < FATIA % num_threads ? 1 : 0)`. Um processo de duração D ocupa a CPU por exatamente D ms, com qualquer número de threads [E §2.4].
- **Fronteiras de fatia:** o escalonador só altera o estado em fronteiras de fatia; a thread conclui a fatia em andamento antes de bloquear [E §2.4].
- Transições válidas: READY → RUNNING (escalonado), RUNNING → FINISHED (`remaining_time` zerou), RUNNING → READY (fim de quantum) [E §2.9].
- Processo que termina exatamente no fim do quantum gera só `finalizado` [L].

**Contrato**
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
Os campos de cima são os do enunciado, com os mesmos nomes. Os extras: `barreira` (alinha as threads), `cv_escalonador` (a líder avisa o escalonador), `orcamento` e `fatias_no_despacho` (quantas fatias este despacho pode e já usou), `t_despacho` (âncora dos prazos), `ultima_cpu` (bônus), `tcbs` (para liberar no fim) e o nó da fila (`ant`, `prox`, `seq`).

Uma fatia vista de perto (processo de 1000 ms, 3 threads, despachado em t=0 com orçamento 3):
```
t=0    escalonador: state=RUNNING, orcamento=3, fatias=0, t_despacho=0; broadcast(cv)
       as 3 threads acordam, calculam prazo = 0 + (0+1)×100 = 100 e dormem até lá
t=100  cada uma desconta a sua parte (34, 33, 33): remaining 1000 → 900
       1ª barreira: a última a chegar é a líder → fatias=1, orcamento=2; não zerou nem esgotou
       2ª barreira: as três passam juntas, voltam ao topo, veem RUNNING → prazo = 200
t=200  idem → fatias=2, orcamento=1, remaining=800
t=300  líder: fatias=3, orcamento=0 → state=READY; signal(cv_escalonador)
       2ª barreira → topo → READY → as três dormem em cv até o próximo despacho
       o escalonador acorda: resultado READY, 3 fatias, t_fim = 0 + 300
```

**Pseudocódigo**
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

**Por que é assim**
- **Por que duas barreiras.** Sem a 2ª, uma thread não líder voltaria ao topo, veria RUNNING e começaria uma fatia a mais antes de a líder gravar READY.
- **Por que só a líder decide.** Ninguém registra `finalizado` duas vezes nem desconta o orçamento duas vezes. O enunciado fala em "a thread que detectar primeiro"; a líder é exatamente uma thread bem definida que detecta (justificar no README).
- **Por que `remaining_time` nunca fica negativo.** As durações são múltiplas de 100 e cada fatia desconta exatamente 100 no total (as partes somam 100).
- **Por que zerar é testado antes do orçamento.** Quem termina exatamente no fim do quantum gera só `finalizado`, sem `Quantum esgotado` [L].
- **Por que `dormir_ate(prazo)` e não `usleep(FATIA*1000)`.** É a mesma fatia de 100 ms, mas ancorada em `t_despacho`: o atraso não se acumula. O prazo é igual para todas as threads porque `fatias_no_despacho` só muda entre as barreiras.
- **Por que as threads não perdem o despacho.** Elas testam o **estado** sob `p->mutex`, não o sinal. Se o broadcast chegar antes de a thread dormir, ela vê RUNNING e segue.

**Armadilhas**
- ⚠️ Nunca saia do laço sem passar pelas **duas** barreiras: as outras threads ficariam presas.
- ⚠️ `dormir_ate` e `barreira_esperar` sempre **sem** `p->mutex`.
- `log_registrar` é chamado com `p->mutex` travado. Está certo: a ordem é `processo → log`.
- A líder usa `signal` em `cv_escalonador` (só o escalonador espera ali) e `broadcast` em `cv` (todas as threads do processo).
- Quem preenche `tcbs[x]` e cria as threads é a geradora (A6), não o `pcb_iniciar`.

**Teste** `tests/test_pcb.c`. A thread principal faz o papel do escalonador; defina no próprio teste um `log_registrar` vazio.
1. Um PCB com `process_len = 1000`, `num_threads = 3`; `pcb_iniciar`; `tempo_iniciar`; preencher os TCBs e criar as 3 threads.
2. **Quantum:** travar; `state = RUNNING`, `orcamento = 3`, `fatias_no_despacho = 0`, `t_despacho = 0`; broadcast em `cv`; esperar em `cv_escalonador` enquanto RUNNING. Esperado: `state = READY`, `fatias_no_despacho = 3`, `remaining_time = 700`, `agora_ms()` entre 300 e 310.
3. **Até o fim:** de novo, com `orcamento = ORCAMENTO_ILIMITADO` e `t_despacho = 300`. Esperado: `state = FINISHED`, `finish_time = 1000`, `remaining_time = 0`, `agora_ms()` entre 1000 e 1020.
4. `join` das 3 threads, `pcb_destruir`. `valgrind --leak-check=full`: 0 erros, 0 bytes perdidos.
5. Repita com `num_threads = 1` e com 4.
```bash
gcc -Wall -pthread -I. -o tests/bin/test_pcb tests/test_pcb.c pcb.c barreira.c tempo.c && ./tests/bin/test_pcb
```

**Pronto quando (fim de segunda):** os testes de A1 a A4 passam, valgrind limpo, tudo na branch `a/motor`, `STATUS.md` atualizado.

---

## Parada 1 · O que B entrega na segunda: `fila.c` e `log.c`

Você usa os dois a partir de amanhã. O PR `b/fila-log` deve estar mesclado ainda na segunda; se não estiver, use a branch de B localmente (`git fetch` + `git merge origin/b/fila-log` na sua branch, sem subir). **Não escreva versões próprias desses módulos.**

### `fila.c`
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
- **Intrusiva:** os ponteiros `ant` e `prox` moram dentro do PCB. Não há `malloc` por inserção (nada para vazar) e remover um elemento qualquer custa O(1), que é o que a PRIORIDADE precisa.
- **Nenhuma função trava.** O mutex existe dentro da `Fila`, mas quem trava é o chamador (`cpus.c`). Motivo: o escalonador precisa fazer **várias** operações na mesma seção crítica (reinserir o preemptado e escolher o próximo), e no POR_CPU uma operação mexe em várias filas de uma vez.
- **As quatro operações** são as que o enunciado pede (inserção no fim, remoção do início, remoção de elemento arbitrário) mais a remoção do fim, usada pela migração ("rouba o último").
- **`seq`** numera a ordem de entrada. A ordem da lista já é essa ordem; o número serve para depurar e conferir.

### `log.c`
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
- **Em memória, gravado no fim.** Nenhuma thread faz E/S de arquivo durante a simulação; a seção crítica é curta.
- **Dois tempos por evento.** `t_real` é o que aparece na linha (`gettimeofday`, como pede o enunciado). `t_logico` é o instante teórico, passado por quem registra, e só serve para **ordenar**. Sem ele, dois eventos do mesmo instante registrados com 1 ms de diferença poderiam sair na ordem errada.
- **A categoria cria as faixas** que o LEIA-ME exige dentro de um instante: `EV_CHEGADA` (0) → `EV_FINALIZADO` (1) → `EV_OUTRO` (2: `Quantum esgotado`, `Migracao`, `Executando`) → `EV_TERMINO` (3).
- **Dentro de `EV_OUTRO`, vale a ordem de registro.** Na mesma CPU, `Quantum esgotado` ou `Migracao` sempre é registrado antes do `Executando` correspondente. Entre CPUs diferentes a ordem é livre (o comparador tolera).
- **Travas:** `mutex_log` é o último da ordem global. `log_registrar` pode ser chamado com filas ou com um processo travado, e não trava mais nada lá dentro.

Quem registra o quê (para você conferir as suas três chamadas):

| Linha | Quem registra | `t_logico` | `cpu` | Categoria |
|---|---|---|---|---|
| `Chegada PID p` | B, `cpus_inserir_lote` | `start_time` | −1 | `EV_CHEGADA` |
| `Executando PID p` | **você**, `despachar` | instante do despacho | k | `EV_OUTRO` |
| `Quantum esgotado PID p` | **você**, laço do escalonador | `t_lib` | k | `EV_OUTRO` |
| `PID p finalizado` | **você**, líder da fatia | `t_fim` da fatia | `p.cpu_id` | `EV_FINALIZADO` |
| `Migracao PID p CPUj -> CPUk` | B, `cpus_tirar_proximo` | instante da escolha | k (destino) | `EV_OUTRO` |
| `Escalonador terminou...` | B, `cpus_verificar_termino` | `t_lib` da última CPU | −1 | `EV_TERMINO` |

O formato final do arquivo, para referência:
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

---

## A5 · `politica.c` (ter 24/11)

**Lembretes do enunciado**
- **FCFS:** sem quantum e sem preempção; a ordem de execução é a ordem de entrada na fila [E §2.5].
- **RR:** quantum Q da entrada; no fim do quantum, com tempo restante, o processo vai para o **fim** da fila [E §2.5].
- **PRIORIDADE:** 1 é a mais alta, 5 a mais baixa [E §2.1]. Seleciona o menor número; no empate, vence quem está **há mais tempo na fila** [E §2.5, desempate 4]. A preempção só ocorre no fim do quantum, nunca no meio, nem quando chega alguém mais prioritário.
- "Mantenha a política de escalonamento isolada do restante do sistema": a 2ª etapa traz políticas novas [E §1].

**Contrato**
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

**Pseudocódigo**
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

**Por que é assim**
- **`<` estrito resolve o desempate 4.** Toda inserção é no fim, então a ordem da lista **é** a ordem de entrada na fila. Varrendo do início e trocando só por prioridade estritamente melhor, o empate fica com o mais antigo. (`seq` guarda essa mesma ordem em número; serve para depurar e para explicar no README.)
- **RR e FCFS usam a mesma função.** A diferença entre os dois é só `usa_quantum`, que o escalonador consulta para montar o orçamento.
- **O preemptado concorre de novo.** O escalonador reinsere o processo no fim da fila **antes** de chamar `escolher`. Na PRIORIDADE, se ele for o único com a melhor prioridade, é reescolhido; se houver outro igual, esse outro está há mais tempo na fila e ganha (os iguais se alternam como no RR).
- **A interface recebe `cpu`** só por causa do bônus de afinidade; FCFS, RR e a PRIORIDADE padrão ignoram o parâmetro.
- **2ª etapa:** política nova é mais uma instância de `Politica`. O escalonador não tem nenhum `if (politica == ...)`.

**Armadilhas**
- `escolher` é chamado com a fila **já travada** (por `cpus_tirar_proximo`, de B). Não trave nada aqui.
- Deixe o comentário `/* BONUS AFINIDADE */` no lugar indicado: B vai editar exatamente ali no M5.
- O parâmetro `cpu` fica sem uso por enquanto; `-Wall` não reclama de parâmetro não usado.

**Teste** `tests/test_politica.c` (precisa de `fila.c`; defina `Config g_cfg;` no teste). Monte uma fila com A (prioridade 3), B (1), C (1), D (2), nessa ordem:
- PRIORIDADE escolhe **B, C, D, A** (B antes de C: mesmo nível, B entrou primeiro).
- FCFS e RR escolhem **A, B, C, D**.
- Fila vazia → NULL nas três. `politica_obter(POL_RR)->usa_quantum = 1`, `POL_FCFS` → 0, e os nomes são `"FCFS"`, `"RR"`, `"PRIORIDADE"`.

---

## Parada 2 · Antes da geradora e do escalonador: entenda o `cpus.c`, de B

A geradora e o seu laço só conversam com este módulo. B entrega a versão testada no GLOBAL na terça à noite (PR `M1-B`); até lá você compila contra os stubs. Você não implementa nada daqui, mas precisa saber exatamente o que cada função garante, porque o seu laço depende disso.

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

O contrato em uma frase por função, do ponto de vista de quem chama:

| Função | Pré-condição | O que garante |
|---|---|---|
| `cpus_travar` / `cpus_destravar` | — | exclusão sobre todas as filas e sobre o estado das CPUs |
| `cpus_tirar_proximo(k, t)` | travado | devolve o próximo da CPU k segundo a política (com migração no POR_CPU) e já marca a CPU como ocupada; NULL se não há nada |
| `cpus_devolver(k, p)` | travado | a CPU deixa de estar ocupada (até aqui, inclusive durante a retenção, ela conta como ocupada e como carga); se `p ≠ NULL`, ele volta ao fim da fila dela. Não acorda ninguém |
| `cpus_entregar_ociosas(t)` | travado | toda CPU ociosa que puder receber algo recebe, em ordem de índice, com instante `t` |
| `cpus_esperar_trabalho(k, &t)` | travado | dorme até receber um processo (devolve-o, com `t` = instante da entrega) ou até o término (NULL); volta travado |
| `cpus_verificar_termino(t)` | travado | se não sobrou nada, declara o término em `t` (uma vez) e acorda todas; devolve 1 se encerrou |
| `cpus_inserir_lote(lote, n, t, ultimo)` | **sem** trava | insere o lote, registra as chegadas, entrega às ociosas |

**Pseudocódigo de B**
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

---

## A6 · `geradora.c` (ter 24/11)

**Lembretes do enunciado**
- Uma thread geradora aguarda até o tempo atual alcançar o tempo de chegada de cada processo, **em ordem de chegada**. `usleep` em intervalos curtos é aceitável; espera ocupada pura é desencorajada [E §2.3].
- Ao chegar, o processo tem as threads criadas, entra na fila de prontos e a chegada vai para o log [E §2.3].
- Quando todos os processos já chegaram, `generator_done` é ativada [E §2.3]. (No nosso desenho ela mora dentro de `cpus.c`; você a liga passando `ultimo = 1` no último lote.)
- **Desempate 1:** processos que chegam no mesmo instante entram na fila em ordem crescente de PID [E §2.1].
- **Desempate 2:** se no instante t um processo chega e outro é reinserido por fim de quantum, **o que chegou entra primeiro** [E §2.1; C 05 em t=300].
- No POR_CPU, quem chega vai para a CPU de menor carga (isso é com B, dentro de `cpus_inserir_lote`) [E §2.6].

**Contrato**
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

**Pseudocódigo**
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

**Por que é assim**
- **Lote, e não um por vez.** Se em t chegam o PID 8 (prioridade 2) e o PID 27 (prioridade 1) com uma CPU ociosa, inserir e acordar um por vez despacharia o 8. O certo é o 27: a CPU só pode escolher depois que **todos** os de t entraram. No POR_CPU a colocação também precisa ver o lote inteiro, em ordem de PID.
- **O vetor já vem ordenado por (chegada, PID)**, então o lote `pcbs[i..j−1]` já está em ordem crescente de PID (desempate 1).
- **Threads criadas antes da inserção.** Quando o processo for despachado, as threads já existem. Elas nascem bloqueadas porque o estado é READY.
- **`proxima_chegada` é a promessa da geradora:** "tudo que chega antes deste instante já está na fila". Ela só avança depois de `cpus_inserir_lote` voltar. `esperar_chegadas_ate(t)` espera a promessa passar de t. A espera dura só o atraso da geradora (poucos ms).
- **`mutex_chegadas` é folha:** nunca é segurado junto com outra trava. A geradora chama `cpus_inserir_lote` sem ele.

**Armadilhas**
- ⚠️ `proxima_chegada` só é atualizada **depois** de `cpus_inserir_lote` voltar. Se atualizar antes, um escalonador passa por `esperar_chegadas_ate` com o lote ainda fora da fila, e o caso 05 quebra em t=300.
- ⚠️ `esperar_chegadas_ate` usa `≤`: uma chegada exatamente em t tem de ser esperada.
- Não chame `cpus_inserir_lote` com `mutex_chegadas` travado.
- Com `PROCESSOS 0`, o lote vazio com `ultimo = 1` é o que permite ao programa terminar.

**Teste:** só na integração (A9). O ponto a observar é o caso 05 em t=300 e o caso 17 em t=1200.

---

## A7 · `escalonador.c` (ter 24/11)

**Lembretes do enunciado**
- Cada CPU é simulada por uma thread escalonadora própria; no mono há uma só. Ela seleciona o próximo, muda o estado para RUNNING, registra o evento, faz broadcast na `cv` do processo e aguarda o fim do processo ou do quantum [E §2.5].
- **FCFS:** executa até finalizar; a CPU é liberada nesse instante.
- **RR:** ao ser despachado, o processo detém a CPU por Q ms. Se terminar antes, `finalizado` é registrado no instante em que `remaining_time` zera, **mas a CPU só é liberada no fim do quantum**: fica retida e ociosa, e esse tempo não conta como ocupado [E §2.5, §4.3; C 05: 85,19%].
- Fim do quantum com `remaining_time > 0`: o processo volta a READY, `Quantum esgotado` é registrado e ele vai para o fim da fila [E §2.5].
- **PRIORIDADE:** mesma mecânica de quantum e retenção. No fim do quantum: (1) o atual, se não finalizou, vai para o fim da fila; (2) seleciona-se o de menor número de prioridade [E §2.5].
- Se o atual for reescolhido, saem `Quantum esgotado` e `Executando` com o mesmo PID, **sem contar troca de contexto** [E §2.5; C 03 em t=1000; C 05 em t=2100].
- Quem termina exatamente no fim do quantum só gera `finalizado` [L].
- **GLOBAL:** ao fim do quantum, a CPU reinsere o processo e seleciona o próximo **na mesma seção crítica**, antes que as ociosas sejam notificadas [E §2.6].
- **Desempate 3:** se mais de uma CPU está livre, atende a de menor índice [E §2.1].
- **Troca de contexto:** despacho de um PID diferente do último executado **naquela CPU**; o primeiro despacho de cada CPU conta [E §4.3].
- **Término:** todas as filas vazias, `generator_done` ativa e nenhuma CPU executando [E §2.8]. O evento sai quando a **última CPU é liberada**, o que pode ser depois do makespan [L; C 05: 2800 contra 2700].
- Dentro de um mesmo instante, as linhas saem na ordem chegadas → finalizações → resto [L]. Quem garante é a categoria do log; a sua parte é passar o instante lógico certo.

**Contrato**
```c
/* escalonador.h (dono: A) - o unico laco de escalonador (mono, GLOBAL e POR_CPU). */
#ifndef ESCALONADOR_H
#define ESCALONADOR_H
void *thread_escalonador(void *arg_cpu);    /* arg = CPU* (uma thread por CPU) */
#endif
```

**Pseudocódigo**
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

**Por que é assim** (como o laço cumpre cada regra)
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

**Armadilhas**
- ⚠️ `esperar_chegadas_ate(t_lib)` em **todo** fim de despacho, não só no fim de quantum. Prova: no caso 17, em t=1200, chegam os PIDs 39 e 55 e a CPU0 esgota o quantum do 25; a saída esperada é `Executando PID 55` (prioridade 1), ou seja, a chegada de 1200 já estava na fila na hora da escolha.
- ⚠️ A escalonadora **não** escolhe sozinha na partida: começa em `cpus_esperar_trabalho`. Medido: com a escolha direta no início, um caso mínimo com 2 processos em t=0 e 8 CPUs saiu com a CPU errada em 32% das execuções.
- ⚠️ Entre `cpus_devolver` e `cpus_tirar_proximo`, nada de `cpus_destravar`.
- ⚠️ `cpus_entregar_ociosas` vem **depois** de esta CPU já ter o seu processo, nunca antes.
- A condição da retenção é `t_fim < t + quantum`. Com `t_fim = t + quantum` (terminou no fim exato) não há retenção.
- `cpu.ultimo_pid`, `cpu.ocupado_ms` e `cpu.trocas` são só da própria thread: não precisam de trava.
- `despachar` pode ser `static`. Como C não devolve dois valores, devolva o estado e passe `t_fim` por ponteiro.

**Teste:** na integração (A9).

---

## A8 · `main.c` → marco M1-A (ter 24/11, 23h)

**Lembretes do enunciado**
- `./trabSO <arquivo>`: único argumento; nada de entrada pelo teclado, **nada na tela** [E §4.7].
- No fim: `pthread_join` em **todas** as threads, gravar log e métricas em `log_execucao_minikernel.txt` (este é o único nome, para qualquer política ou versão), liberar toda a memória [E §2.8, §4.2].
- Variáveis globais só quando estritamente necessário [E §2.9]. As nossas: `g_cfg`, `g_pol`, `g_cpus`; o resto é `static` dentro de cada módulo.

**Pseudocódigo**
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

**Armadilhas**
- O `join` das threads dos processos vem depois do `join` da geradora: só então todas foram criadas.
- A `main` lê `g_cpus[k].trocas` e `ocupado_ms` (dentro de `metricas_gravar`) só depois do `join` das escalonadoras.
- Ordem de destruição: log, cpus, geradora, PCBs, vetor.

**Pronto quando (M1-A):** PR `M1-A: motor` aberto, compilando nos dois alvos sem warnings, com os testes de A1 a A5 passando. Enquanto `cpus.c` de B não chega à `main`, o programa compila contra o stub e ainda não roda de ponta a ponta: é o esperado.

---

## A9 · Integração → marco M2 (qua 25/11, 23h)

Você integra, porque o escalonador é a cola entre os módulos.

1. Confira no `STATUS.md` se o `M1-B` está na `main`. Se não estiver, revise o PR de B primeiro: isso destrava os dois.
2. Merge de `a/motor` com a `main` atualizada.
3. `make monoprocessador` e `tests/run_all.sh mono` (script de B): casos **01, 03 e 05**.
4. Se falhar, a primeira diferença impressa pelo comparador mais a tabela do apêndice B resolvem quase tudo. O que cada caso prova:

| Caso | Política | Se passar, está certo |
|---|---|---|
| 01 | FCFS | ordem de chegada, chegadas durante a execução, métricas básicas |
| 03 | PRIORIDADE | preempção só no fim do quantum (o PID 2 chega em 200 e só entra em 500), reescolha do mesmo PID sem troca (4 trocas no total) |
| 05 | RR | desempate 2 em t=300, retenção em 1400 e 1600, reescolha em 2100, fim exato de quantum em 2400, término em 2800 com makespan 2700, utilização 85,19% |

5. Memcheck:
```bash
valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./trabSO tests/casos/entrada_05.txt
valgrind --tool=helgrind --error-exitcode=1 ./trabSO caso_pequeno.txt
```
- **Objetivo: 0 erros nos dois.** Memcheck a partir do M2, helgrind a partir do M3.
- Sob o helgrind o programa fica dezenas de vezes mais lento e os tempos impressos saem errados. É normal: use casos de 8 a 12 processos e olhe só o relatório de erros.
- "lock order violated": alguém travou fila e processo juntos, ou as filas fora da ordem crescente.
- "Possible data race" em `ultimo_pid`, `ocupado_ms` ou `trocas`: a `main` leu antes do `pthread_join`.
6. Já tente `make multiprocessador` com os casos 13, 15, 17 e `ex45`. Pelo desenho, o GLOBAL deve funcionar sem mudança no seu código. Anote no `STATUS.md` o que falhar, para B.

**Pronto quando (M2):** `tests/run_all.sh mono` mostra 3/3 ok **3 vezes seguidas**; valgrind com 0 erros e 0 bytes perdidos no caso 05; PR `M2: integração mono` mesclado.

---

## A10 · Estresse, invariantes e helgrind (qui 26/11)

**Lembretes do enunciado**
- Há casos ocultos, incluindo um de **estresse**: dezenas de processos, até 4 threads por processo, 8 CPUs, feito para detectar condições de corrida [E §8].
- Segfault, condição de corrida e deadlock: penalização severa. Vazamento: desconto [E §8].
- "Use `valgrind --tool=helgrind` [...]. O ideal é zero erros" [E §5].

**`ferramentas/gerar_estresse.py <semente> [--cpus N] [--politica P] [--fila F] [--n N] [--sem-ambiguidade]`**
```
r ← gerador aleatório iniciado com a semente
n ← 30 a 60 (ou --n; use 8 a 12 para o helgrind)
PIDs ← n valores distintos em 1..999, embaralhados                       (não sequenciais)
instantes ← cerca de n/3 múltiplos de 100 em [0, 5000], sempre incluindo o 0
para cada PID: chegada ← sorteio em instantes                            (de propósito: vários no mesmo instante)
               duração ← múltiplo de 100 em [100, 2000];  prioridade ← 1..5;  threads ← 1..4
cabeçalho: CPUS (1..8), QUANTUM (100..1000, múltiplo de 100), POLITICA, FILA   (sorteados, ou os pedidos)
imprimir o cabeçalho e as linhas dos processos em ordem aleatória
--sem-ambiguidade: tentar semente, semente+1, ... até "oraculo.py --ambiguo" sair com 0
                   (com muitos processos no mesmo instante quase tudo é ambíguo: use 2 CPUs e n pequeno;
                    pode levar dezenas de sementes)
```

**`ferramentas/verificar_invariantes.py entrada.txt log_execucao_minikernel.txt`**: confere o que vale em **qualquer** execução correta, mesmo nas entradas ambíguas. Arredonde cada `t` do log para o múltiplo de 100 mais próximo antes de conferir (e acuse se o medido estiver a mais de 50 ms do arredondado). Sai com 0 ou imprime a primeira violação.
1. Cada PID tem exatamente uma `Chegada`, no seu instante de chegada, e exatamente um `finalizado`. Existe exatamente um `Escalonador terminou`, e ele é a última linha do log.
2. Nenhum `Executando` de um PID antes da sua `Chegada` nem depois do seu `finalizado`. Um PID nunca está em duas CPUs ao mesmo tempo. Uma CPU nunca tem dois processos ao mesmo tempo.
3. Em cada CPU, depois de `Executando PID p`, o próximo evento de execução dela é `Quantum esgotado PID p` (exatamente Q depois) ou `PID p finalizado`.
4. RR e PRIORIDADE: depois de um `finalizado`, a CPU só despacha de novo a partir de (instante do despacho + Q). É a retenção.
5. Para cada PID, a soma dos intervalos executados é igual à duração.
6. FCFS não tem `Quantum esgotado`. Só POR_CPU tem `Migracao`.
7. Métricas: cada linha `PID` bate com o log (turnaround = finalizado − chegada; espera = turnaround − duração), e `Trocas de contexto` é a contagem de `Executando` com PID diferente do anterior na mesma CPU.

**Rotina**
```bash
# mono é determinístico: 20 entradas contra o oráculo
for s in $(seq 1 20); do
  python3 ferramentas/gerar_estresse.py $s --cpus 1 > /tmp/e.txt
  python3 ferramentas/oraculo.py /tmp/e.txt --mono > /tmp/esp.txt
  timeout 120 ./trabSO /tmp/e.txt && python3 tests/compare.py /tmp/esp.txt log_execucao_minikernel.txt || echo "FALHOU semente $s"
done
# multi: invariantes (qualquer entrada) e, quando não for ambígua, também o oráculo
for s in $(seq 1 20); do
  python3 ferramentas/gerar_estresse.py $s --cpus 8 > /tmp/e.txt
  timeout 120 ./trabSO /tmp/e.txt || echo "TRAVOU ou falhou: semente $s"
  python3 ferramentas/verificar_invariantes.py /tmp/e.txt log_execucao_minikernel.txt || echo "INVARIANTE: semente $s"
done
```
`timeout` estourado é deadlock. Quando uma semente falhar, guarde a entrada em `tests/estresse/falha_<s>.txt` e abra issue com o rótulo `bug`.

**Helgrind:** 3 entradas pequenas (8 a 12 processos), no mono e no GLOBAL. Objetivo: 0 erros.

**Pronto quando:** 20/20 entradas mono iguais ao oráculo; 20/20 multi sem violação; helgrind limpo.

---

## A11 · Marco M3, com B (sex 27/11, 23h)

- `tests/run_all.sh todos` **5 vezes seguidas**: casos 01, 03, 05 (mono) e 13, 15, 17, `ex45`, `g01` a `g06` (multi GLOBAL), todos ok.
- Estresse multi GLOBAL com 2, 4 e 8 CPUs (rotina de A10).
- Revise o PR `M3: multi GLOBAL` de B. O que olhar: entrega em ordem de índice, `cpus_executando` somado na entrega, término declarado uma vez só.
- Seu PR: `M3-A: estresse` (gerador, verificador e a rotina num script).
- Comece `ferramentas/rodar_analise.sh` (etapa A12).

**Pronto quando (M3):** você aprovou o PR de B e o seu está mesclado.

---

## A12 · Análise comparativa (sáb 28/11; completa na seg 30/11)

**Lembretes do enunciado** [E §6]
- Usar a carga `carga_analise.txt` (do Classroom, cerca de 10 processos), executada com: as três políticas no mono; o multi com 2 e 4 CPUs nos modos GLOBAL e POR_CPU; RR com quantum de 100, 500 e 1000 ms.
- Uma tabela com as métricas e uma discussão que responda a quatro perguntas (abaixo).
- O README vale 1,5 ponto.

**Matriz**

| Grupo | Configurações |
|---|---|
| Mono | FCFS, RR (quantum do arquivo), PRIORIDADE |
| Quantum | RR mono com Q = 100, 500 e 1000 |
| Multi | 2 e 4 CPUs × GLOBAL e POR_CPU × as três políticas (12 execuções; o enunciado não fixa a política, e rodar as três custa pouco) |
| Bônus | **fora** desta tabela: vai para `ANALISE_AFINIDADE.md`, de B |

**`ferramentas/rodar_analise.sh`**
```
para cada configuração da matriz:
    montar a entrada: cabeçalho (CPUS, QUANTUM, POLITICA, FILA) + as linhas de processos de carga_analise.txt
    compilar o alvo certo (mono ou multi), uma vez por grupo
    rodar 3 vezes; de cada log extrair: médias de espera, turnaround e resposta, trocas, utilização
        (por CPU e média), throughput, makespan e migrações (contar_migracoes.py)
    guardar a mediana das 3
    conferir com o oráculo quando a entrada não for ambígua; se não bater, investigar antes de publicar
gerar a tabela em Markdown: ferramentas/tabela_analise.md
```
No sábado rode mono e GLOBAL; as linhas POR_CPU entram na segunda, depois do M4.

**As quatro perguntas** (um parágrafo cada, sempre citando números da tabela):
1. **Qual política minimizou a espera média, e por quê.** Relacione com a ordem das durações na carga: o FCFS penaliza os curtos que chegam depois dos longos; o RR divide a CPU e alonga o turnaround dos longos; a PRIORIDADE favorece quem tem prioridade alta, sem olhar a duração.
2. **O que acontece com os processos de baixa prioridade.** Espera e resposta dos de prioridade 5. Sem envelhecimento (aging), a inanição é possível sob chegada contínua de processos prioritários.
3. **Como o quantum afeta a resposta e as trocas de contexto.** Q pequeno: resposta melhor, muitas trocas. Q grande: o RR se aproxima do FCFS. Some o efeito da retenção: com Q grande, a CPU fica mais tempo presa quando o processo termina antes do quantum, e a utilização cai.
4. **GLOBAL × POR_CPU** (o parágrafo é de B, a partir da sua tabela): utilização por CPU, número de migrações, equilíbrio de carga e custo de sincronização de cada modelo.

**Também no sábado:** comente todos os seus arquivos. Cabeçalho do arquivo dizendo o papel do módulo; em cada função, o que faz, **quais travas supõe já tomadas e quais toma**. Código sem comentários dá desconto, e a organização vale 1,0 ponto [E §8].

---

## A13 · POR_CPU sob estresse → marco M4 (dom 29/11, 23h)

Assim que o PR `M4: POR_CPU` de B abrir:
- `tests/run_all.sh todos`, agora com `p01` a `p07`.
- Helgrind com POR_CPU e **8 CPUs**, 3 entradas pequenas. Estresse com invariantes, 20 entradas com `--fila POR_CPU --cpus 8`.
- `valgrind --leak-check=full` numa entrada de cada combinação de política e modo.
- Problemas vão para B por issue com o rótulo `bug`, com a entrada que falha anexada.

O que revisar no código de B: filas sempre travadas em ordem crescente; em `cpus_esperar_trabalho`, soltar o próprio mutex antes de retravar tudo; migração rouba o **último** da **mais longa**, empate no menor índice; a linha `Migracao` sai na CPU de destino, antes do `Executando`.

**Pronto quando (M4):** você aprovou o PR e o helgrind está limpo com POR_CPU.

---

## A14 · Marcos M5 e M6, e a entrega (seg 30/11 a sex 04/12)

- **Seg 30 (M5):** rodar as linhas POR_CPU da matriz e fechar a tabela e a discussão no README. Revisar o PR `M5: bônus + decisões` de B: procurar `goto`, warnings, função sem comentário, trava fora de ordem, `printf`. Conferir que, **sem** `AFINIDADE=1`, `tests/run_all.sh todos` continua 100%.
- **Ter 01 (M6):** revisar e aprovar o PR de congelamento. Rodar `ferramentas/empacotar.sh`.
- **Qua 02:** teste "de professor" no **LabGrad** (ou VM Ubuntu limpa). Copiar só o `.tar.gz`; `tar -xzvf`; `make monoprocessador`; rodar os casos mono; `make multiprocessador`; rodar os casos multi. Zero warnings. Só corrigir o que bloquear, e toda correção repete o teste inteiro.
- **Qui 03:** enviar `TP-SO-2026-2-<mat1>-<mat2>.tar.gz` no Classroom (só um integrante envia; o nome tem as duas matrículas) e compartilhar o repositório com o professor. Escrever "enviado" no `STATUS.md`. B baixa e confere.
- **Sex 04, 07h59:** prazo final. Depois do envio, só mexer se B achar erro que impeça a compilação.

**Se atrasar**
- O escalonador (ter 24) é caminho crítico. Escreva no `STATUS.md` às 23h; B assume a integração do M2 na quarta de manhã e você fica com a correção dos testes.
- Atrasou o estresse: corte o bônus antes, nunca o helgrind.

---

## Parada 3 · O resto da metade de B: métricas, comparador, bônus e pacote

Você revisa esses PRs e pode ter de explicar qualquer um deles.

### `metricas.c`
**Lembretes do enunciado** [E §4.3]: espera = turnaround − duração · turnaround = finalização − chegada · resposta = primeiro despacho − chegada · makespan = instante da última finalização · utilização da CPU = tempo executando ÷ makespan · throughput = nº de processos ÷ makespan em segundos · PIDs em ordem crescente, médias com duas casas.
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
- **Tudo sai de instantes lógicos** (`first_dispatch`, `finish_time`) e de contadores (`ocupado_ms`, `trocas`), então os números são exatos e iguais aos esperados, sem depender do relógio.
- **`ocupado_ms` só soma fatias executadas.** A retenção ociosa fica de fora sem nenhum código especial. Caso 05: 2300/2700 = 85,19%.
- **Makespan é a última finalização**, não o instante do `Escalonador terminou` (caso 05: 2700 contra 2800).
- **Troca de contexto** é contada por CPU em `despachar`: PID diferente do último daquela CPU; o primeiro despacho de cada CPU conta.

### `tests/compare.py` e `tests/run_all.sh`
Lembrete [E §4.6, L]: tolera-se desvio de até 150 ms nos instantes, permutação entre eventos a menos de 150 ms (CPUs diferentes) e 10% nas métricas; a ordem dos eventos de uma mesma CPU, os PIDs, as CPUs e a contagem de eventos têm de coincidir; a contagem de trocas de contexto é exata.

Uso: `python3 tests/compare.py esperado.txt obtido.txt`. Sai com 0 se passar; se falhar, sai com 1 e imprime **a primeira diferença**.

1. Separar o log das métricas na linha `=== METRICAS ===`.
2. Ler cada evento: `t`, CPU (ou nenhuma) e a mensagem (tipo, PID e, na migração, a origem).
3. **Mesma quantidade de eventos**, no total.
4. **Por CPU:** a sequência de mensagens é idêntica, na mesma ordem, e cada `|t_esperado − t_obtido| ≤ 150`.
5. **Eventos sem CPU** (`Chegada`, `Escalonador terminou`): para cada esperado existe um obtido, ainda não usado, com a mesma mensagem e diferença de no máximo 150 ms.
6. **Métricas:** as linhas têm o mesmo texto fora dos números; os identificadores (PID, índice da CPU) são iguais; cada valor pode desviar até 10%. **Exceções:** `Trocas de contexto` é **exata** [L]; quando o valor esperado é 0, aceitar até 150 de diferença absoluta (decisão nossa, documentada no README).

`tests/run_all.sh [mono|multi|porcpu|todos]` compila o alvo certo uma vez por grupo (casos 01 a 06 → mono; 07 a 18, `ex45`, `g*`, `p*` → multi), roda cada entrada, compara e imprime `ok X` ou `FALHOU X: <primeira diferença>`; no fim, `N/M ok`, com código de saída 0 só se todos passarem.

### Bônus de afinidade (até 0,5 ponto; é o primeiro item a ser cortado)
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

### README de B e o pacote
- **Decisões de projeto** (B escreve, você revisa): como compilar; módulos; estruturas e o porquê de cada campo extra; sincronização (ordem de travamento, pares condição/mutex, duas barreiras, lost wakeup, por que o POR_CPU trava todas as filas); tabela "regra da seção 2 → onde está no código"; modelo de tempo; decisões P1 a P8 (P1 confirmado em 24/11, os outros são leituras nossas); testes; autoria.
- **`ferramentas/empacotar.sh <mat1> <mat2>`:** `make clean`; gera `TP-SO-2026-2-<mat1>-<mat2>.tar.gz` com `Makefile`, `*.c`, `*.h`, `README.md`, `ANALISE_AFINIDADE.md` e `tests/` **na raiz do pacote**; extrai numa pasta temporária; compila os dois alvos com `CFLAGS_EXTRA=-Werror` (só nesse teste); roda um caso de cada; imprime "pacote ok". Ficam **fora**: `STATUS.md`, `ferramentas/`, logs e binários.

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
| trava só no POR_CPU | filas travadas fora de ordem, ou `cpus_esperar_trabalho` retravou tudo sem soltar antes o próprio mutex |
| tempos derivando alguns ms a cada fatia | dormiu "100 ms a partir de agora" em vez de até o prazo absoluto |
| espera de 1 s ou mais que não acontece | `usleep` com 1 000 000 ou mais: use `dormir_ate` |
| valgrind: bytes perdidos | `tcbs`, `thread_ids`, filas ou o buffer do log não liberados |

## Apêndice C · Se o professor perguntar

O professor pode fazer entrevista, e a nota depende dela [E §8]. Cada um precisa saber responder a tudo isto, e não só à própria metade:

1. **Por que duas barreiras por fatia?** A 1ª junta todas as threads no fim da fatia e elege a líder. A 2ª segura todas até a líder decidir; sem ela, uma thread voltaria ao topo, veria RUNNING e começaria uma fatia a mais.
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
