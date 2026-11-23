> **ARQUIVADO em 23/11/2026.** Este arquivo foi absorvido por `04_TRILHA_A.md` e `05_TRILHA_B.md` e não é mais atualizado. É o histórico da revisão anterior; os erros R1 a R18 descritos aqui já estão corrigidos no pseudocódigo das trilhas. Pode ser apagado.

# 01 · Revisão cega dos três planos

**Método.** Primeiro o enunciado e os casos públicos foram lidos do zero. Depois as
regras foram reimplementadas num simulador determinístico (`ferramentas/oraculo.py`),
sem consultar os planos. O simulador reproduz **byte a byte** as saídas dos casos 01,
03, 05, 13, 15, 17 e do exemplo 4.5 do PDF. Só então os planos foram comparados
contra o enunciado, os casos e o simulador.

**Veredito.** A espinha dorsal dos planos antigos está certa e foi mantida: orçamento
de fatias, duas barreiras por fatia, nó da fila embutido no PCB, funções de fila que
não travam, log em memória ordenado no final, política atrás de uma interface,
utilização contada em fatias e um único código para os dois alvos. Foram encontrados
**5 erros que gerariam saída errada ou travamento** (R1 a R4 e R9), **7 problemas
importantes** e **6 menores**. Também havia um conflito com o objetivo do grupo: os planos previam
reuniões.

Legenda: 🔴 saída errada, deadlock ou nota zero · 🟠 risco real · 🟡 clareza ou robustez.

---

## Erros que gerariam saída errada

### 🔴 R1. As chegadas do instante t precisam ser vistas por **toda** escolha feita em t, e não só pela reinserção
*Planos afetados: A (Round Robin, `tratar_fim_do_despacho`) e B (laço GLOBAL).*

O Plano A só espera a geradora (`esperar_chegadas_ate`) quando o processo volta por
fim de quantum. Quando a CPU fica livre por **finalização** (FCFS, ou fim da retenção
no RR e na Prioridade), ela escolhe o próximo sem esperar. Se um processo chega
naquele mesmo instante, ele pode ficar de fora da escolha.

**Prova nos casos.** No caso 17, em t=1200, chegam o PID 39 (prioridade 5) e o PID 55
(prioridade 1), e a CPU0 esgota o quantum do PID 25 (prioridade 2). A saída esperada é
`Executando PID 55`. Logo, a chegada de t=1200 já estava na fila no momento da escolha.
O LEIA-ME confirma: dentro de um instante, a ordem é *chegadas, finalizações, resto*.
Com a Prioridade, o mesmo acontece depois de uma finalização: uma chegada mais
prioritária no mesmo instante precisa ganhar.

**Correção.** Antes de **qualquer** escolha feita no instante lógico t depois de um
despacho (fim de quantum, finalização sem retenção ou fim de retenção), o escalonador
chama `esperar_chegadas_ate(t)`. Ver o arquivo 03, seção 6.

### 🔴 R2. Chegadas do mesmo instante precisam entrar na fila **em lote**, antes de acordar qualquer CPU
*Planos afetados: A (geradora) e B (`inserir_chegada`).*

A geradora insere um processo por vez e acorda CPUs ociosas a cada inserção. Com a
Prioridade e uma CPU ociosa, se chegam em t o PID 8 (prioridade 2) e o PID 27
(prioridade 1), a CPU acorda com o PID 8 já na fila e o despacha antes de o PID 27
entrar. O correto é despachar o 27. No POR_CPU, a colocação também precisa ver o lote
inteiro na ordem de PID.

**Correção.** A geradora cria as threads de **todos** os processos com o mesmo
`start_time`, depois insere todos numa única seção crítica (`cpus_inserir_lote`) e só
então acorda as CPUs.

### 🔴 R3. Sem um ambiente Linux, nada compila nem é testado
*Planos afetados: os três (sugeriam `pthread_barrier_t` e valgrind, mas não diziam onde rodar).*

O Yan usa macOS. No macOS **não existe `pthread_barrier_t`**, e o valgrind/helgrind
não roda nas versões atuais. O professor compila no Ubuntu do LabGrad.

**Correção.** (a) Usar uma **barreira própria** com mutex e variável de condição (30
linhas). Ela é portável, o helgrind entende, e devolve explicitamente qual thread é a
líder. (b) Todo teste roda em Linux (Docker, VM ou LabGrad). Ver o arquivo 06,
seção 1.

### 🔴 R4. `usleep` com 1 segundo ou mais é comportamento não especificado
*Plano afetado: A (retenção: "dormir (fim_quantum − agora) ms").*

O POSIX permite que `usleep(≥ 1.000.000)` falhe com `EINVAL`. Com quantum de 1000 ms,
ou com intervalos longos entre chegadas, uma espera única pode simplesmente não
acontecer.

**Correção.** Toda espera passa por `dormir_ate(t)`, que chama `usleep` em pedaços de
no máximo 500 ms até `agora_ms() ≥ t`.

---

## Problemas importantes

### 🟠 R5. Deriva de tempo acumulada: dormir "100 ms a partir de agora" em vez de "até o instante t"
Cada fatia dorme `usleep(100 ms)` a partir do momento em que a thread acordou. Os
atrasos de acordar, das barreiras e do escalonador **se somam** a cada fatia. O caso 15
tem 74 fatias por CPU. No caso oculto de estresse (8 CPUs, dezenas de processos, até
4 threads), a deriva pode passar de 50 ms. A partir daí, o "instante lógico por
arredondamento" erra o grupo e quebra a ordenação do log e a regra 2. Acima de 150 ms,
os eventos saem da tolerância.

**Correção.** **Prazos absolutos.** Cada fatia termina no instante lógico
`t_despacho + 100·k`, e a thread dorme até esse instante (`dormir_ate`). Os atrasos
deixam de se acumular: cada fatia corrige o atraso da anterior.

### 🟠 R6. O instante lógico deve ser **calculado**, não adivinhado pelo arredondamento do relógio
Os planos arredondavam `agora_ms()` para descobrir o instante lógico de cada evento
dentro do log. Quase todo instante, porém, é conhecido exatamente: a chegada é
`start_time`, o fim da fatia k é `t_despacho + 100·k`, o fim da retenção é
`t_despacho + Q`. **Correção:** quem registra o evento passa o instante lógico
explicitamente (`log_registrar(t_logico, ...)`). Com a entrega direta (R9), até o
despacho de uma CPU que estava ociosa tem instante exato. As métricas usam esses
instantes exatos, o que dispensa o arredondamento posterior do Plano B.

### 🟠 R7. Três laços de escalonador diferentes (mono do A, GLOBAL do B, POR_CPU do B)
Três laços quase iguais multiplicam o risco de divergência e o trabalho de integração.
**Correção:** **um único laço** (de A) que só fala com o módulo `cpus.c` (de B). Mono
é GLOBAL com 1 CPU. A diferença entre GLOBAL e POR_CPU fica toda dentro de `cpus.c`:
quais mutexes travar, onde inserir, como migrar e quem acordar. Ver o arquivo 03.

### 🟠 R8. Proteção inconsistente de `cpus_executando`, `executando` e `encerrar` no POR_CPU
O Plano B travava só a fila local para tirar o próximo, mas o contador global
`cpus_executando`, o campo `executando` (usado no cálculo de carga) e o teste de
término dependem de **todas** as filas. Isso é corrida de dados, e o helgrind acusaria.
**Correção:** no POR_CPU, toda operação do escalonador sobre filas trava **todas** as
filas, em ordem crescente (no máximo 8 mutexes, custo de microssegundos). A CPU ociosa
dorme segurando só o próprio mutex. Ver o protocolo de espera no arquivo 03, seção 7.

### 🔴 R9. Acordar CPUs ociosas para "disputarem" a fila não garante a regra 3 e gera migração falsa
*Plano afetado: B (`acordar_ociosas` no GLOBAL e `signal` em todas as ociosas no POR_CPU).*

No GLOBAL, quando chegam dois processos em t=0 com duas CPUs ociosas, o Plano B acorda
a CPU0 e a CPU1, e as duas correm para a trava da fila. Se a CPU1 ganhar, ela pega o
PID menor, e o log sai com as CPUs trocadas. Pela regra 3, o primeiro processo vai para
a CPU0. Chegadas simultâneas em t=0 são comuns nos casos ocultos e no de estresse. No
POR_CPU, acordar todas as ociosas deixa uma CPU de fila vazia "roubar" o processo que
acabou de ser colocado na fila de outra ociosa, gerando uma `Migracao` que não deveria
existir.

**Correção: entrega direta.** Quem insere ou libera trabalho (geradora ou CPU que acabou
de escolher), ainda com a trava das filas, percorre as CPUs ociosas **em ordem crescente
de índice** e escolhe por elas: `p = cpus_tirar_proximo(k, t)`, incluindo a migração no
POR_CPU. Guarda `p` em `cpu.entregue`, com o instante `t`, e só então dá `signal`. A
CPU acorda e despacha o que recebeu. O resultado fica determinístico, cumpre a regra 3,
coincide com o oráculo e ainda dá o instante lógico exato do despacho (sem
arredondamento). Ver o arquivo 03, seção 7.

### 🟠 R10. O comparador aceitava 10% nas trocas de contexto
O LEIA-ME diz: "A contagem de trocas de contexto deve ser exata". **Correção** no
arquivo 06.

### 🟠 R11. Ordem de travamento contraditória entre os planos
O plano geral dizia `filas → processo → log`. A Fase 0 e os Planos A e B diziam
`fila → CPU → processo → log`, mas não existe mutex de CPU, e `mutex_chegadas` não
aparecia em lugar nenhum. **Correção (única e definitiva):** `filas (índice
crescente) → log` e `processo → log`. **Fila e processo nunca são travados juntos.**
`chegadas` e a barreira são folhas, sempre travadas sozinhas. Ver o arquivo 03,
seção 4.

### 🟠 R12. A interface da política não recebia a CPU, então o bônus exigiria mudar o contrato depois
**Correção:** `escolher(Fila *f, int cpu)` desde o início.

---

## Menores

- 🟡 **R13. Nomes divergentes** (`rq_push_back` no B, `inserir_no_fim` no pseudocódigo, `select_next` no A). **Correção:** um único conjunto de headers em português, no arquivo 03, que vira contrato no M0.
- 🟡 **R14. Makefile:** sem `.PHONY`, e o risco de alguém colocar `-Werror`. Se o gcc do LabGrad emitir um warning novo, `-Werror` vira erro de compilação, que vale nota zero. **Correção:** `.PHONY`, rebuild completo em cada alvo e **nunca** `-Werror` no Makefile entregue (só localmente, com `make CFLAGS_EXTRA=-Werror`).
- 🟡 **R15. Texto exato:** "Escalonador terminou execução de todos processos" tem `ç` e `ã` em UTF-8. "Migracao" e "Utilizacao" não têm acento. Os arquivos esperados usam LF e terminam com `\n`. Os `.c` precisam ser salvos em UTF-8.
- 🟡 **R16. Nada na tela:** nem `stderr`. Prints de depuração só com uma macro `DEBUG` desligada por padrão.
- 🟡 **R17. Bônus sem interpretação definida.** Em FCFS e RR não existe empate na seleção (a fila é FIFO estrita). **Padrão:** a afinidade vale, só com 2 ou mais CPUs, no empate de prioridade e no empate entre filas de mesmo tamanho na migração do POR_CPU. Fica atrás de `-DAFINIDADE`, desligada por padrão, para não alterar as saídas oficiais. A medição vai para um arquivo separado, `ANALISE_AFINIDADE.md`. Ver o arquivo 07, seção 3.
- 🟡 **R18. Casos faltando e `carga_analise.txt`:** a pasta só tem 6 dos 18 casos, e nenhum de POR_CPU. **Ação** no dia 0 (arquivo 00, seção 6). Enquanto isso, o oráculo gera casos de POR_CPU.

## Conflito com o objetivo do grupo

Os planos antigos tinham uma "reunião de 1h" no kickoff, "check-in de 15 min a cada 2
dias" e várias tarefas "juntos". **Correção:** os headers já estão fechados no arquivo
03 (são o contrato e substituem o kickoff). A comunicação é por `STATUS.md`, PRs e
issues, com prazos de resposta e padrões para todo ponto em aberto (arquivo 00,
seção 3). A discussão do README foi dividida por tema (arquivo 07).

## O que foi mantido (e continua valendo)

Orçamento de fatias definido pelo escalonador e gasto pelas threads · duas barreiras por
fatia, com só a líder decidindo · nó da fila embutido no PCB (sem `malloc` por
inserção) · funções da fila que não travam · log em memória, ordenado por (instante,
categoria, ordem de registro) · utilização = fatias × 100 / makespan · interface de
política sem `if (politica == ...)` no escalonador · `-DMONO` no mesmo código · vetor de
PCBs ordenado por (chegada, PID) · "Escalonador terminou" no fim da retenção ·
desempate da Prioridade pelo menor `seq`.
