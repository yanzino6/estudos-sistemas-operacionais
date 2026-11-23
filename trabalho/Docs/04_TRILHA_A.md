# 04 · Trilha A (Yan): motor, políticas, estresse e análise

Pré-requisito: ter lido os arquivos 00 a 03 e ter o ambiente Linux pronto (arquivo 06,
seção 1). Cada dia tem **tarefas**, **teste do dia** e **pronto quando**. No fim de
toda sessão, atualize o `STATUS.md`.

Seus arquivos: `main.c`, `tempo.c/.h`, `config.h`, `entrada.c/.h`, `barreira.c/.h`,
`pcb.c/.h`, `geradora.c/.h`, `politica.c/.h`, `escalonador.c/.h`,
`tests/test_barreira.c`, `tests/test_pcb.c`, `ferramentas/gerar_estresse.py`,
`ferramentas/verificar_invariantes.py`, `ferramentas/rodar_analise.sh`.

---

## Seg 23/11 · M0 + tempo, barreira, entrada e threads 🔴

### Manhã (até 12h): marco **M0**
1. Criar o repositório **privado** no GitHub e adicionar B como colaborador. A branch padrão é a `main`.
2. Fazer o commit, na raiz:
   - todos os headers da seção 2 do arquivo 03, **copiados literalmente** (`tempo.h`, `config.h`, `barreira.h`, `pcb.h`, `fila.h`, `politica.h`, `log.h`, `cpus.h`, `geradora.h`, `entrada.h`, `escalonador.h`, `metricas.h`);
   - um `.c` para cada módulo, com as funções **vazias** (stubs que devolvem 0 ou NULL), para compilar;
   - `main.c` mínimo (`int main(void){ return 0; }`) e o Makefile da seção 2 do arquivo 03;
   - `.gitignore` com `trabSO`, `*.o`, `log_execucao_minikernel.txt` e `tests/bin/`;
   - `STATUS.md` com as seções "A" e "B" vazias;
   - `tests/casos/` com os casos públicos (`entrada_NN.txt`, `saida_NN.txt`) e `ferramentas/oraculo.py` e `ferramentas/contar_migracoes.py` (copiar da pasta `plano_v2/ferramentas/`).
3. Conferir: `make monoprocessador && make multiprocessador` **sem nenhum warning**. Atualizar o `STATUS.md` ("M0 ok") e avisar B por issue: "M0 pronto".

### Tarde: `tempo.c`, `barreira.c`, `entrada.c`, `pcb.c`
- `tempo.c`: arquivo 03, seção 3. `agora_ms` = `(s − s0)·1000 + (us − us0)/1000`. `dormir_ate` em pedaços de no máximo 500 ms.
- `barreira.c`: arquivo 03, seção 5 (versão com `geracao`).
- `entrada.c`: ler os 5 cabeçalhos com `fscanf("%s %d")` e `"%s %s"`, depois as n linhas. Preencher os campos e chamar `pcb_iniciar`. Com `#ifdef MONO`, `num_cpus = 1` e `modo_fila = FILA_GLOBAL`. Ordenar com `qsort` por `(start_time, pid)`. Se o arquivo não abrir, devolver −1 **sem imprimir nada**.
- `pcb.c`: `pcb_iniciar` (`remaining_time = process_len`, READY, `cpu_id = first_dispatch = finish_time = ultima_cpu = −1`, `ant = prox = NULL`, mutex, `cv`, `cv_escalonador`, barreira com `num_threads`, `malloc` de `thread_ids` e `tcbs`); `pcb_destruir` (destrói tudo e libera); `thread_trabalhadora` (arquivo 03, seção 5, **com prazo absoluto e duas barreiras**).

### Teste do dia
- `tests/test_barreira.c`: 4 threads × 1000 voltas na barreira, contando quantas vezes cada uma foi líder. Total de líderes = 1000. Nenhum travamento.
- `tests/test_pcb.c`: um PCB de 1000 ms com 3 threads. A thread principal faz o papel do escalonador (RUNNING, `orcamento = ORCAMENTO_ILIMITADO`, `t_despacho = 0`, broadcast em `cv`, espera em `cv_escalonador`). Defina no próprio teste um `log_registrar` vazio. Esperado: `state = FINISHED`, `finish_time = 1000`, `agora_ms()` entre 1000 e 1020, join das 3 threads, valgrind limpo.
- Teste de quantum: o mesmo PCB com `orcamento = 3` deve voltar READY com `fatias_no_despacho = 3` e `remaining_time = 700`.

**Pronto quando:** os três testes passam, `valgrind --leak-check=full` não mostra erros, e tudo está na branch `a/motor`.

---

## Ter 24/11 · política, geradora, escalonador e main 🔴 → marco **M1-A** (23h)

- `politica.c`: três instâncias de `Politica`. FCFS e RR usam `fila_remover_inicio`. PRIORIDADE percorre do início ao fim e troca de candidato só com `<` **estrito** (o empate fica com o mais antigo, que é o FIFO), depois `fila_remover`. `politica_obter(tipo)` devolve o ponteiro. **Deixe um ponto marcado `/* BONUS AFINIDADE */`** no empate da prioridade: B vai mexer ali.
- `geradora.c`: arquivo 03, seção 8. ⚠️ Lote do mesmo instante (R2). ⚠️ `proxima_chegada` atualizado depois do lote.
- `escalonador.c`: `despachar` e `thread_escalonador` do arquivo 03, seção 6. ⚠️ `esperar_chegadas_ate(t_lib)` **sempre** (R1). ⚠️ Retenção com `cpus_inicio_retencao` e `dormir_ate(t + Q)`. ⚠️ Reinserir e escolher sem soltar a trava.
- `main.c`: arquivo 03, seção 10.
- Enquanto `fila.c`, `log.c` e `cpus.c` de B não chegam à `main`, compile contra os stubs. Não escreva versões próprias desses módulos: se precisar testar antes, use as versões da branch de B (`git fetch` e `git merge origin/b/...` na sua branch local, sem subir).

**Pronto quando (M1-A):** PR `M1-A: motor` aberto, compilando nos dois alvos sem warnings, com os testes de segunda passando. Atualizar o `STATUS.md`.

---

## Qua 25/11 · Integração → marco **M2** (23h) 🔴

Você integra, porque o escalonador é a cola entre os módulos.
1. Pela manhã, confira se o `M1-B` de B está na `main` (veja o `STATUS.md`). Se não estiver, revise o PR dele primeiro: isso destrava os dois.
2. Faça o merge de `a/motor` com a `main` atualizada.
3. `make monoprocessador` e `tests/run_all.sh mono` (script de B): casos **01, 03 e 05**.
4. Erros típicos, nesta ordem de probabilidade:
   - `Executando` antes de `Chegada` no mesmo instante: categoria errada no `log_registrar`;
   - fatia a mais depois do quantum: faltou a 2ª barreira;
   - travamento no fim: `cpus_verificar_termino` não foi chamado, ou falta broadcast;
   - término registrado em 2700 em vez de 2800 no caso 05: retenção pulada.
5. `valgrind --leak-check=full ./trabSO tests/casos/entrada_05.txt` com 0 erros e 0 bytes perdidos.
6. Já tente `make multiprocessador` com os casos 13, 15 e 17. Pelo desenho, o GLOBAL deve funcionar sem mudanças. Anote no `STATUS.md` o que falhar, para B.

**Pronto quando (M2):** `run_all.sh mono` 3/3 ok, **3 vezes seguidas**, e valgrind limpo. PR `M2: integração mono` mesclado.

---

## Qui 26/11 · Estresse e helgrind

- `ferramentas/gerar_estresse.py` (arquivo 06, seção 5): de 30 a 60 processos, PIDs embaralhados, chegadas múltiplas de 100 entre 0 e 5000 (com **vários no mesmo instante**), durações de 100 a 2000, prioridades de 1 a 5, de 1 a 4 threads, CPUS de 1 a 8, as três políticas e os dois modos.
- `ferramentas/verificar_invariantes.py` (arquivo 06, seção 5).
- **Mono é determinístico:** para entradas mono, compare o `trabSO` com o oráculo (`compare.py`). Rode 20 casos mono gerados.
- `valgrind --tool=helgrind` em 3 casos pequenos (de 8 a 12 processos), no mono e no GLOBAL. **Objetivo: 0 erros.** Os tempos do log saem errados sob o helgrind, e isso é esperado.

**Pronto quando:** 20/20 casos mono iguais ao oráculo e helgrind limpo.

---

## Sex 27/11 · marco **M3** (com B) e script da análise

- Rodar `tests/run_all.sh` (mono e multi) **5 vezes**: 7/7 ok em todas (casos 01, 03, 05, 13, 15, 17 e o exemplo 4.5).
- Estresse multi GLOBAL: 20 casos com o verificador de invariantes. Também compare com o oráculo os casos sem ambiguidade (`oraculo.py --ambiguo` sai com 0; ver o arquivo 06).
- Começar `ferramentas/rodar_analise.sh` (arquivo 07, seção 2).

**Pronto quando (M3):** o PR `M3` de B está aprovado por você, e o seu PR `M3-A: estresse` está mesclado.

---

## Sáb 28/11 · Análise (mono e GLOBAL) e comentários

- Rodar a matriz da análise com `carga_analise.txt`: mono FCFS/RR/PRIORIDADE, RR com Q = 100/500/1000, e GLOBAL com 2 e 4 CPUs. Cada configuração 3 vezes, usando a mediana.
- Rascunhar no README a seção "Análise comparativa" com a tabela e as respostas às 4 perguntas (arquivo 07, seção 2).
- Comentar todos os seus arquivos: cabeçalho do arquivo e, em cada função, o que faz, quais travas assume e quais trava.

---

## Dom 29/11 · Estresse com POR_CPU

- Assim que o `M4` de B estiver na `main`: helgrind com POR_CPU e **8 CPUs** (3 casos pequenos), e estresse com invariantes (20 casos).
- `valgrind --leak-check=full` em todos os modos e políticas.
- Reportar problemas a B por issue com o rótulo `bug`, anexando a entrada que falha.

**Pronto quando:** você aprovou o PR `M4` e o helgrind está limpo com POR_CPU.

---

## Seg 30/11 · marco **M5**: análise completa

- Rodar POR_CPU com 2 e 4 CPUs na matriz e completar a tabela.
- O bônus **não** entra na sua tabela: B gera `ANALISE_AFINIDADE.md` separado (arquivo 07, seção 3.3). Revise esse arquivo e a seção "Bônus: afinidade" do README.
- Terminar a discussão: espera média, baixa prioridade, efeito do quantum e GLOBAL × POR_CPU (este último em parceria com o texto de B; ver o arquivo 07).
- Revisão cruzada: no PR `M5` de B, procurar `goto`, warnings, funções sem comentário, travas fora da ordem e `printf`.

**Pronto quando (M5):** README com a análise completa, e você aprovou a seção de decisões de B.

---

## Ter 01/12 a Sex 04/12 · congelamento e entrega

- **Ter 01:** revisar e aprovar o PR `M6` (congelamento) de B. Rodar `ferramentas/empacotar.sh` no Docker.
- **Qua 02:** teste "de professor" no **LabGrad** (ou numa VM Ubuntu limpa): copiar só o `.tar.gz`, `tar -xzvf`, `make monoprocessador`, rodar os casos mono; `make multiprocessador`, rodar os casos multi. Zero warnings. Só corrigir o que bloquear, e toda correção repete o teste completo.
- **Qui 03:** enviar o `TP-SO-2026-2-<mat1>-<mat2>.tar.gz` no Classroom e compartilhar o repositório com o professor. Escrever "enviado" no `STATUS.md`. B confere.
- **Sex 04, 07h59:** prazo final. Não mexer em nada depois do envio, a não ser que B ache um erro que impeça a compilação.

## Se atrasar
- Atrasou o escalonador (ter 24): é caminho crítico. Escreva no `STATUS.md` às 23h. B assume a integração do M2 na quarta de manhã, e você fica com a correção dos testes.
- Atrasou o estresse: corte o bônus antes, nunca o helgrind.
