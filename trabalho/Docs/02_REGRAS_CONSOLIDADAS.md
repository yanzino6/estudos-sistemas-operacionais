# 02 · Regras consolidadas (enunciado + LEIA-ME + casos públicos)

Tudo o que o programa precisa respeitar está aqui, num lugar só. Cada regra tem a
fonte: **[E §x]** é a seção do enunciado, **[L]** é o LEIA-ME dos casos e **[C nn]** é
o caso público que confirma a regra. Todas as regras abaixo foram conferidas pelo
oráculo, que reproduz os casos byte a byte.

## 1. Execução e entrega
- `make monoprocessador` e `make multiprocessador` geram o executável `trabSO`, compilado com `-pthread -Wall` [E §5].
- O programa roda como `./trabSO entrada.txt`. Não lê nada do teclado e **não imprime nada na tela** [E §4.7].
- Saída única: `log_execucao_minikernel.txt`, com o log e depois o bloco de métricas [E §4.2, §4.3].
- `goto` vale nota zero. Erro de compilação vale nota zero. Segfault, corrida e deadlock têm penalização severa. Vazamento dá desconto [E §8].

## 2. Entrada [E §4.1]
```
CPUS <1..8>
QUANTUM <ms, múltiplo de 100>
POLITICA <FCFS | RR | PRIORIDADE>
FILA <GLOBAL | POR_CPU>
PROCESSOS <n>
<PID> <CHEGADA_ms> <DURACAO_ms> <PRIORIDADE 1..5> <NUM_THREADS 1..4>   (n linhas, sem ordem garantida)
```
- Os cabeçalhos aparecem sempre nessa ordem, e os arquivos são válidos.
- Na versão mono, CPUS vale 1 e FILA é ignorado. No FCFS, QUANTUM é ignorado.
- O PID é único, mas não é sequencial nem vem ordenado.

## 3. Tempo [E §2.1]
- Milissegundos desde **T0**, medidos com `gettimeofday`. T0 é marcado logo após a leitura da entrada.
- **FATIA = 100 ms.** Chegada, duração e quantum são múltiplos de 100. Logo, **todo evento acontece num múltiplo de 100**. É isso que permite calcular o "instante lógico" de cada evento (arquivo 03, seção 3).

## 4. Processos e threads [E §2.2 a §2.4]
- O PCB começa com `remaining_time = process_len`, estado READY e `cpu_id`, `first_dispatch` e `finish_time` iguais a −1.
- Na chegada, as threads do processo são criadas e ficam bloqueadas na `cv` do processo até o estado virar RUNNING.
- Em cada fatia, **todas** as threads do processo dormem em paralelo e juntas descontam exatamente 100 ms. A thread i desconta `100/n + (i < 100%n ? 1 : 0)`. Um processo de duração D ocupa a CPU por exatamente D ms.
- O escalonador só muda o estado de um processo em **fronteiras de fatia**. Uma thread termina a fatia em andamento antes de bloquear.
- Quando `remaining_time` chega a 0: estado FINISHED, a finalização é registrada e todas as threads encerram.

## 5. Políticas [E §2.5]
| | FCFS | RR | PRIORIDADE |
|---|---|---|---|
| Quantum | não | Q da entrada | Q da entrada |
| Escolha | início da fila | início da fila | menor número de prioridade; empate: quem está há mais tempo na fila (FIFO) |
| Fim do quantum com tempo restante | — | `Quantum esgotado`, reinsere no fim da fila e escolhe o próximo | igual ao RR: reinsere no fim e **depois** escolhe |
| Termina antes do fim do quantum | libera a CPU na hora | `finalizado` sai na hora, mas **a CPU fica retida e ociosa até o fim do quantum** | igual ao RR |
| Termina exatamente no fim do quantum | — | só `finalizado`, sem `Quantum esgotado` [L] | igual |
| Preempção | nunca | só no fim do quantum | **só no fim do quantum**, nem mesmo quando chega alguém mais prioritário |

- Se, no fim do quantum, o processo atual é o único (ou o melhor) da fila, ele é escolhido de novo: sai `Quantum esgotado` seguido de `Executando` para o mesmo PID, **sem contar troca de contexto** [E §2.5, L, C 03 em t=1000, C 05 em t=2100, C 17 em t=300].
- A retenção ociosa **não conta** como CPU ocupada na utilização [E §2.5, §4.3, C 05: 2300/2700 = 85,19%].

## 6. Desempates obrigatórios [E §2.1] e a ordem dentro de um instante [L]
1. Chegadas no mesmo instante entram na fila em **ordem crescente de PID**.
2. Se no instante t um processo chega e outro volta por fim de quantum, **o que chegou entra primeiro** [C 05 em t=300: o PID 9 chega, o PID 5 esgota o quantum, e o PID 9 roda].
3. Se mais de uma CPU está livre, atende **a de menor índice** [E §4.5: em t=100, o PID 3 vai para a CPU1 porque a CPU0 está ocupada].
4. Na Prioridade, empate de prioridade é decidido pela ordem de entrada na fila.

**Consequência obrigatória (erro R1 dos planos antigos):** no instante t, primeiro **todas
as chegadas de t entram na fila**, e só depois qualquer CPU escolhe, seja por fim de
quantum, por finalização ou por fim de retenção. Prova: [C 17 em t=1200], onde o PID 55
(prioridade 1) chega e é escolhido na mesma hora pela CPU0, que acabou de esgotar o
quantum do PID 25.

**Ordem das linhas do log dentro de um mesmo instante [L]:** chegadas → finalizações →
(fim de quantum / migração / despacho). Entre CPUs diferentes, eventos com menos de
150 ms de diferença podem aparecer em qualquer ordem. Dentro de uma mesma CPU, a ordem
é rígida: `Quantum esgotado` (ou `Migracao`) e só depois `Executando`.

## 7. Multiprocessador [E §2.6]
- Um processo roda em no máximo uma CPU por vez. Cada CPU aplica a política de forma independente, e os quanta de CPUs diferentes não são sincronizados.
- **GLOBAL:** uma fila só, com mutex. Qualquer CPU livre retira dela o próximo. **Ao fim do quantum, a CPU reinsere o processo e escolhe o próximo na mesma seção crítica, antes de acordar as ociosas.**
- **POR_CPU:** uma fila e um mutex por CPU.
  - **Colocação:** quem chega vai para a CPU de menor **carga** = processos na fila local + processo em execução. Empate: menor índice.
  - **Migração:** quando uma CPU precisa escolher e a fila local está vazia, ela **rouba o último** elemento da fila local **mais longa** (empate: menor índice), desde que essa fila tenha ao menos um processo. CPUs ociosas tentam de novo sempre que algum processo é inserido em qualquer fila.
  - Cada migração gera a linha `Migracao PID p CPUj -> CPUk` na CPU de **destino** k, seguida de `Executando` [E §4.2].
  - Ao fim do quantum, o processo volta para o fim da **fila da própria CPU** (padrão adotado; ver seção 9).
- **Término [E §2.8]:** quando (i) todas as filas estão vazias, (ii) `generator_done` está ativo e (iii) nenhuma CPU está executando ou retida. O evento `Escalonador terminou...` sai **quando a última CPU é liberada**, podendo ser depois do makespan [L, C 05: término em 2800, makespan 2700; C 15: 7700 e 7400].

### Exemplo POR_CPU (gerado pelo oráculo; não depende da ordem entre CPUs simultâneas)
Entrada: 2 CPUs, FCFS, POR_CPU; processos `1 0 600`, `2 0 300`, `3 0 900`, `4 100 200`, `5 100 600`.
```
t=0   chegam 1,2,3. Cargas (0,0): 1→CPU0. (1,0): 2→CPU1. (1,1): 3→CPU0.   filas: C0=[1,3]  C1=[2]
      CPU0 executa 1, CPU1 executa 2.                                     filas: C0=[3]    C1=[]
t=100 chegam 4,5. Cargas C0=1+1=2, C1=0+1=1: 4→CPU1. Depois (2,2): 5→CPU0. filas: C0=[3,5]  C1=[4]
t=300 CPU1: 2 finalizado; executa 4.
t=500 CPU1: 4 finalizado; fila local vazia → rouba o ÚLTIMO da mais longa (C0=[3,5]) = 5
      [CPU1] Migracao PID 5 CPU0 -> CPU1 ; [CPU1] Executando PID 5
t=600 CPU0: 1 finalizado; executa 3.
```

## 8. Saída [E §4.2, §4.3]
Formato exato. Atenção aos espaços e acentos:
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
Utilizacao CPU<k>: <x.xx>%            (uma linha por CPU, em ordem de índice)
Throughput: <x.xx> processos/s
Makespan: <n> ms
```
`<POL>` é `FCFS`, `RR` ou `PRIORIDADE`. O arquivo é UTF-8, com LF, e termina com `\n`.

**Definições [E §4.3]:** turnaround = finalização − chegada · espera = turnaround −
duração · resposta = primeiro despacho − chegada · makespan = instante da última
finalização · troca de contexto = despacho de um PID diferente do último executado
**naquela CPU** (o primeiro despacho de cada CPU conta) · utilização da CPU k = tempo
executando processos / makespan · throughput = n / (makespan em segundos).

## 9. Pontos sem resposta explícita e o padrão adotado
| # | Pergunta | Padrão adotado | Por quê | Como confirmar |
|---|---|---|---|---|
| P1 | No POR_CPU, uma CPU **retida** (RR/Prioridade, processo terminou antes do quantum) conta como carga na colocação? | **Conta** (`CARGA_CONTA_RETENCAO 1`) | do ponto de vista do escalonamento, a CPU ainda não foi liberada | quando os casos 08 a 18 aparecerem, rodar o oráculo com e sem `--retencao-nao-conta` e ver qual bate; trocar a macro |
| P2 | No POR_CPU, o processo que esgota o quantum volta para qual fila? | a fila da **própria** CPU | "reinserido no fim da fila" (E §2.5) e a migração só ocorre com fila local vazia | idem |
| P3 | No POR_CPU, uma chegada no mesmo instante em que uma CPU finaliza: a CPU que finaliza ainda conta carga? | **conta**: chegadas são processadas antes das finalizações do mesmo instante (ordem do L) | coerente com a ordem chegadas → finalizações → resto | idem |
| P4 | Duas CPUs livres no mesmo instante disputando a mesma fila | a de menor índice escolhe primeiro (regra 3) quando dá; caso contrário, aceitar | o L garante que os casos oficiais não dependem disso | — |
| P5 | Onde o bônus de afinidade tem empate de fato? | PRIORIDADE com 2+ CPUs (mesma prioridade); POR_CPU em qualquer política, só na escolha da fila de origem da migração; nunca no mono | FIFO não tem empate; com 1 CPU a afinidade não traz ganho | tabela no arquivo 07, seção 3.1 |
| P6 | O log mostra o tempo medido ou o lógico? | o **medido** (`gettimeofday`), que é o que o enunciado pede; o lógico só ordena e alimenta as métricas | tolerância de 150 ms; com prazos absolutos o erro fica em poucos ms | — |
