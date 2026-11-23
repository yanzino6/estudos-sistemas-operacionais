> **ARQUIVADO em 23/11/2026.** Este arquivo foi absorvido por `04_TRILHA_A.md` e `05_TRILHA_B.md` e não é mais atualizado. README, análise, bônus e empacotamento estão agora nas trilhas (A12 e A14 na Trilha A; B10 a B12 na Trilha B). Pode ser apagado.

# 07 · README, análise comparativa, bônus e entrega

O README vale 1,5 ponto, e a organização e os comentários valem mais 1,0. O professor pode
fazer entrevistas, e a nota depende delas [E §8]. **Os dois precisam saber explicar o
código inteiro**, não só a própria trilha. Por isso existe a revisão cruzada nos marcos
M5 e M6.

## 1. README, seção "Decisões de projeto" (B escreve; A revisa no M5)

Estrutura sugerida, com o que cada item precisa conter:

1. **Como compilar e executar:** `make monoprocessador` / `make multiprocessador`, `./trabSO entrada.txt`, `make multiprocessador AFINIDADE=1` (bônus).
2. **Módulos:** a tabela da seção 1 do arquivo 03, com uma frase por arquivo. Explicar que a política fica isolada em `politica.c`, pensando na 2ª etapa: uma política nova é só mais uma instância de `Politica`.
3. **Estruturas:** PCB e TCB do enunciado, mais os campos extras, com o porquê de cada um (barreira, `cv_escalonador`, orçamento, `fatias_no_despacho`, `t_despacho`, `ultima_cpu`, `tcbs`, nó intrusivo `ant/prox/seq`). `CPU` e `Fila`.
4. **Sincronização:**
   - a ordem global de travamento (`filas em ordem crescente → log`; `processo → log`; fila e processo nunca juntos; `chegadas` e barreira são folhas) e por que ela impede deadlock;
   - os pares condição/mutex;
   - o protocolo da fatia com duas barreiras e a líder;
   - por que não há lost wakeup (predicado em `while`, mudança sob o mutex parceiro, entrega direta com `entregue`);
   - por que, no POR_CPU, as operações de escalonamento travam todas as filas.
5. **Como cada regra da seção 2 foi implementada** (tabela regra → onde está no código → como):
   - desempate 1 (vetor ordenado por chegada e PID, lote em ordem);
   - desempate 2 e "chegadas antes de qualquer escolha" (`esperar_chegadas_ate`);
   - desempate 3 (entrega direta em ordem de índice);
   - desempate 4 (varredura com `<` estrito, mais `seq`);
   - fronteiras de fatia (barreiras);
   - contabilização `100/n + resto`;
   - retenção ociosa;
   - "reinserir e escolher na mesma seção crítica";
   - colocação, migração e "ociosas tentam de novo";
   - término.
6. **Modelo de tempo:** instante lógico × tempo medido. Prazos absolutos para não acumular deriva. O log imprime o tempo medido e ordena pelo lógico. As métricas usam os instantes lógicos (exatos).
7. **Decisões em pontos não especificados:** P1 a P6 do arquivo 02, seção 9, com o que foi escolhido e por quê. Inclua a tolerância de 150 ms para métricas cujo valor esperado é 0, no nosso comparador.
8. **Testes:** como rodar `tests/run_all.sh`, o que o comparador verifica, os resultados de valgrind e helgrind (0 erros) e a rotina de estresse com invariantes.
9. **Autoria:** quem fez cada módulo (a evidência também está nos commits).

## 2. Análise comparativa (A roda e escreve; B escreve o parágrafo GLOBAL × POR_CPU)

**Matriz obrigatória [E §6], com `carga_analise.txt`:**
| Grupo | Configurações |
|---|---|
| Mono | FCFS, RR (quantum do arquivo), PRIORIDADE |
| Multi | 2 e 4 CPUs × GLOBAL e POR_CPU × as três políticas (12 execuções; o enunciado não fixa a política, então rodar as três custa pouco e enriquece a discussão) |
| Quantum | RR mono com Q = 100, 500 e 1000 |
| Bônus | fora desta tabela: vai para `ANALISE_AFINIDADE.md` (seção 3.3) |

**`ferramentas/rodar_analise.sh`:**
```
para cada configuração:
    montar a entrada: cabeçalho (CPUS, QUANTUM, POLITICA, FILA) + as linhas de processos de carga_analise.txt
    compilar o alvo certo (mono ou multi; AFINIDADE=1 quando for o caso)
    rodar 3 vezes; de cada log extrair as médias de espera, turnaround e resposta, as trocas, a utilização
        (média e por CPU), o throughput, o makespan e as migrações (contar_migracoes.py)
    guardar a mediana das 3 execuções
    conferir com oraculo.py (deve bater dentro da tolerância; se não bater, investigar antes de publicar)
gerar a tabela em Markdown: ferramentas/tabela_analise.md
```

**Perguntas que a discussão precisa responder [E §6.3]**, com um parágrafo cada, sempre citando números da tabela:
1. **Qual política minimizou a espera média, e por quê.** Relacione com a ordem das durações na carga: o FCFS penaliza os curtos que chegam depois dos longos; o RR divide a CPU e aumenta o turnaround dos longos; a PRIORIDADE favorece só quem tem prioridade alta, sem considerar a duração.
2. **O que acontece com os processos de baixa prioridade.** Espera e resposta dos processos de prioridade 5. Sem envelhecimento (aging), a inanição é possível sob chegada contínua de processos prioritários.
3. **Como o quantum afeta a resposta e as trocas de contexto.** Q pequeno dá resposta melhor e muitas trocas. Q grande aproxima o RR do FCFS. Some ainda o efeito da retenção ociosa: com Q grande, a CPU fica mais tempo retida quando o processo termina antes do quantum, e a utilização cai.
4. **GLOBAL × POR_CPU** (parágrafo de B): utilização por CPU, número de migrações, equilíbrio de carga (o desbalanceamento aparece como diferença de utilização entre as CPUs) e o custo de sincronização de cada modelo.

## 3. Bônus: afinidade de CPU (B, até 0,5 ponto)

**Enunciado [E §9]:** "em caso de empate na seleção, preferir o processo cuja última CPU
seja a que está livre. Meça e discuta no README o impacto no número de migrações e nas
métricas."

**Orientações do grupo (obrigatórias):**
1. O BCP registra a última CPU em que cada processo executou (`ultima_cpu`, gravado em `despachar`), e esse campo é o critério de desempate na seleção.
2. Identificar e documentar **em quais políticas e modos existe empate de fato** (tabela 3.1).
3. **O comportamento padrão não muda.** A afinidade é ligada e desligada por flag de compilação, sem mudar o formato da entrada nem o da saída: `make multiprocessador` (sem afinidade, o que vai para os testes) e `make multiprocessador AFINIDADE=1` (com afinidade).
4. Medir com e sem afinidade: migrações (POR_CPU), despachos numa CPU diferente da anterior (GLOBAL, extraído do log) e as métricas de desempenho. **O resultado vai num segundo arquivo, `ANALISE_AFINIDADE.md`.** A tabela principal da análise (seção 2) continua sem afinidade.
5. No README: discutir o ganho observado, se as métricas mudaram ou não, e por quê.

### 3.1 Onde existe empate de fato

| Política | Mono | GLOBAL (2+ CPUs) | POR_CPU (2+ CPUs) |
|---|---|---|---|
| FCFS | não: fila FIFO estrita | não: a CPU livre sempre pega a cabeça da fila | só na **migração**, quando duas ou mais filas empatam como a mais longa (o enunciado desempata pelo menor índice) |
| RR | não: fila FIFO estrita | não | igual ao FCFS: só na migração |
| PRIORIDADE | existe empate de prioridade, mas **a afinidade não se aplica**: com uma CPU só, todo processo que já rodou tem `ultima_cpu = 0`, e "preferir a última CPU" viraria só "preferir quem já rodou", sem nenhum ganho de cache. O mono fica idêntico com ou sem a flag. | **sim**: processos de mesma prioridade na fila global | **sim**: mesma prioridade na fila local, e também na migração (filas de mesmo tamanho) |

Regras de desempate com afinidade (as duas atrás de `#ifdef AFINIDADE` e só quando `g_cfg.num_cpus > 1`):
- **Seleção na PRIORIDADE** (`politica.c`, ponto `/* BONUS AFINIDADE */`): entre os processos com a melhor prioridade, prefere o primeiro, na ordem da fila, cujo `ultima_cpu` é a CPU que está escolhendo. Se nenhum tiver, vale o FIFO (o mais antigo).
- **Origem da migração no POR_CPU** (`cpus.c`, em `cpus_tirar_proximo`): entre as filas empatadas como a mais longa, prefere a de menor índice **cujo último processo** tem `ultima_cpu` igual à CPU que rouba. Se nenhuma tiver, vale a de menor índice. O processo roubado continua sendo o **último**, como manda o enunciado.

```
[politica.c] escolher_prioridade(f, cpu):
    melhor ← NULO
    para cada p da fila, do início para o fim:
        se melhor = NULO ou p.priority < melhor.priority: melhor ← p
#ifdef AFINIDADE
        senão se g_cfg.num_cpus > 1 E p.priority = melhor.priority
                 E melhor.ultima_cpu ≠ cpu E p.ultima_cpu = cpu: melhor ← p
#endif
    fila_remover(f, melhor);  devolver melhor

[cpus.c] dentro de cpus_tirar_proximo, ao escolher a origem da migração:
    maior ← maior tamanho entre as filas;  origem ← menor j com filas[j].tamanho = maior
#ifdef AFINIDADE
    para j crescente com filas[j].tamanho = maior:
        se filas[j].fim.ultima_cpu = k: origem ← j; parar
#endif
```

### 3.2 Flag de compilação
Makefile (já previsto no arquivo 03): `ifeq ($(AFINIDADE),1) CFLAGS += -DAFINIDADE endif`.
- `make monoprocessador` / `make multiprocessador`: **sem** afinidade. É o que o professor roda, e as saídas dos casos oficiais não mudam.
- `make multiprocessador AFINIDADE=1`: com afinidade. Mesmo executável `trabSO`, mesma entrada, mesmo `log_execucao_minikernel.txt` no mesmo formato.
- Verificação obrigatória antes do M5: com a flag **desligada**, `tests/run_all.sh todos` continua 100% ok; com a flag ligada, o mono continua idêntico (`--mono --afinidade` no oráculo dá a mesma saída), e o multi bate com `oraculo.py --afinidade`.

### 3.3 Medição e o segundo arquivo de saída
Ferramentas: `ferramentas/contar_migracoes.py` (já pronto, junto com o oráculo) lê um log e
imprime `migracoes_log` (linhas `Migracao`), `despachos_outra` (despachos de um PID numa
CPU diferente do seu despacho anterior, a "migração implícita" do GLOBAL), `retornos`,
`trocas`, as médias de espera, turnaround e resposta, o `makespan` e a utilização média.

`ferramentas/medir_afinidade.sh` (B escreve):
```
para cada política em (FCFS, RR, PRIORIDADE), modo em (GLOBAL, POR_CPU), CPUs em (2, 4):
    entrada ← cabeçalho + processos de carga_analise.txt
    make multiprocessador          → rodar 3x → contar_migracoes.py → mediana  (SEM afinidade)
    make multiprocessador AFINIDADE=1 → rodar 3x → contar_migracoes.py → mediana (COM afinidade)
    conferir as duas com oraculo.py (sem e com --afinidade)
escrever ANALISE_AFINIDADE.md: uma tabela "sem | com | diferença" por configuração, com
    migracoes_log, despachos_outra, trocas, espera média, resposta média, turnaround médio, makespan, utilização
voltar a compilar SEM afinidade no fim (para ninguém entregar o binário errado)
```
`ANALISE_AFINIDADE.md` vai no repositório e no pacote, ao lado do README. A tabela
principal do README não muda, e o README ganha uma seção curta "Bônus: afinidade" que
resume o resultado e aponta para esse arquivo.

### 3.4 O que esperar (prévia com o oráculo) e o que discutir no README
Prévia: 150 cargas aleatórias de 10 processos, Q = 500, médias "sem → com". **Vale só como
prévia.** Os números do README têm que sair da execução real com `carga_analise.txt`.

| Configuração | logs que mudam | despachos em outra CPU | trocas | espera média | resposta média | makespan |
|---|---|---|---|---|---|---|
| FCFS e RR (GLOBAL e POR_CPU, 2 e 4 CPUs) | 0% | igual | igual | igual | igual | igual |
| PRIORIDADE GLOBAL 2 CPUs | 99% | 5,5 → 0,5 | 18,2 → 11,6 | 2336 → 2269 | 1595 → 1852 | 7751 → 7781 |
| PRIORIDADE GLOBAL 4 CPUs | 87% | 4,4 → 1,7 | 15,0 → 12,2 | 562 → 545 | 296 → 339 | 4641 → 4663 |
| PRIORIDADE POR_CPU 2 CPUs | 96% | 0,4 → 0,1 | 18,1 → 11,5 | 2377 → 2252 | 1450 → 1812 | 7742 → 7785 |
| PRIORIDADE POR_CPU 4 CPUs | 60% | 0,9 → 0,5 | 13,5 → 11,4 | 558 → 532 | 280 → 354 | 4610 → 4639 |

No caso público 17 (PRIORIDADE, GLOBAL, 2 CPUs): despachos em outra CPU 4 → 1, trocas
15 → 11, espera média 1475 → 1437,5, resposta média 1000 → 1050, makespan 6400 → 6600.

**Pontos que a discussão do README precisa cobrir (com os números reais):**
1. **Onde há ganho:** só na PRIORIDADE com 2 ou mais CPUs. No FCFS e no RR não existe empate na seleção, e o empate de filas na migração quase nunca tem um candidato com afinidade, então os logs saem idênticos. Isso é esperado, não é falha.
2. **O ganho:** caem os despachos em outra CPU (no GLOBAL, quase todos somem) e caem as trocas de contexto. Num processador real, isso significa cache e TLB mais "quentes".
3. **Por que as trocas caem (efeito colateral importante):** no fim do quantum, o processo preemptado volta para a fila com `ultima_cpu` igual à CPU que vai escolher. Se houver outro de mesma prioridade, a afinidade escolhe o preemptado de novo, e o mesmo PID continua (sem troca). A alternância "como no RR" entre processos de mesma prioridade, que o enunciado descreve, deixa de acontecer, e os iguais passam a ser servidos quase em FCFS.
4. **Métricas:** a espera média cai um pouco (menos alternância reduz o tempo de espera de quem termina primeiro). A resposta média **piora**, porque processos novos (`ultima_cpu = −1`) perdem o empate para quem já rodou. O makespan sobe um pouco: escolher pela CPU, e não pela ordem, deixa CPUs com cargas menos equilibradas no fim. A utilização acompanha o makespan.
5. **Por que o POR_CPU ganha menos:** cada processo já volta para a fila da própria CPU, então quase todos os candidatos na fila local já têm afinidade com ela. A afinidade só reordena "quem já rodou aqui" contra "quem acabou de chegar". As linhas `Migracao` praticamente não mudam, porque o processo roubado continua sendo o último da fila.
6. **Conclusão:** a afinidade troca justiça entre iguais e tempo de resposta por localidade (menos migrações e menos trocas). Por isso ela fica desligada por padrão.

## 4. Empacotamento e entrega (B prepara; A envia)

**`ferramentas/empacotar.sh <mat1> <mat2>`:**
```sh
#!/bin/sh
set -e
NOME="TP-SO-2026-2-$1-$2"
make clean
tar -czf "$NOME.tar.gz" Makefile *.c *.h README.md ANALISE_AFINIDADE.md tests/casos tests/compare.py tests/run_all.sh tests/test_*.c
TMP=$(mktemp -d)
tar -xzf "$NOME.tar.gz" -C "$TMP"
cd "$TMP"
make monoprocessador CFLAGS_EXTRA=-Werror && ./trabSO tests/casos/entrada_01.txt
make multiprocessador CFLAGS_EXTRA=-Werror && ./trabSO tests/casos/entrada_13.txt
echo "pacote ok: $NOME.tar.gz"
```
- Os arquivos vão **na raiz** do pacote: o professor roda `tar -xzvf` e `make` na mesma pasta.
- **Fora do pacote:** `STATUS.md`, `ferramentas/` (inclusive o oráculo), logs e binários.
- `-Werror` entra só nesse teste, nunca no Makefile entregue.

**Checklist do envio (qui 03/12):**
- [ ] `grep -n goto *.c` vazio; `grep -n "printf\|puts\|stderr" *.c` só mostra `fprintf` para o arquivo de log ou `snprintf`
- [ ] `make monoprocessador` e `make multiprocessador` sem warnings no LabGrad
- [ ] `tests/run_all.sh todos` ok; valgrind e helgrind com 0 erros nos casos oficiais
- [ ] README com decisões, tabela, discussão, seção do bônus e autoria; `ANALISE_AFINIDADE.md` gerado sem a flag padrão ter mudado
- [ ] nome exato `TP-SO-2026-2-<mat1>-<mat2>.tar.gz`, enviado por **um** integrante (A)
- [ ] repositório compartilhado com o professor
- [ ] B baixou o arquivo enviado do Classroom, compilou e rodou: "conferido" no `STATUS.md`
- [ ] tudo isso antes de **sexta 04/12, 07h59**
