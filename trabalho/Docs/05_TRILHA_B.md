# 05 · Trilha B (dupla): infraestrutura, multiprocessador, bônus e entrega

Pré-requisito: ter lido os arquivos 00 a 03 e ter o ambiente Linux pronto (arquivo 06,
seção 1). Cada dia tem **tarefas**, **teste do dia** e **pronto quando**. No fim de
toda sessão, atualize o `STATUS.md`.

Seus arquivos: `Makefile` (a partir do M0), `fila.c`, `log.c`, `cpus.c`, `metricas.c`,
`tests/compare.py`, `tests/run_all.sh`, `tests/test_fila.c`, `tests/test_log.c`,
`tests/test_metricas.c`, `ferramentas/empacotar.sh`, `ferramentas/medir_afinidade.sh`,
o trecho `/* BONUS AFINIDADE */` de `politica.c`, o desempate com afinidade na migração (`cpus.c`),
`ANALISE_AFINIDADE.md`, a seção "Decisões de projeto" e a seção "Bônus: afinidade" do README.
(`ferramentas/contar_migracoes.py` já vem pronto, junto com o oráculo.)

---

## Dom 22/11 (opcional) e Seg 23/11 de manhã: o que não depende do repositório
- Baixar do Classroom `carga_analise.txt` e os casos públicos que faltam (02, 04, 06 a 12, 14, 16 e 18). Assim que o M0 sair, fazer o commit deles em `tests/casos/`. **Se aparecerem casos com POR_CPU, resolver o ponto P1** (arquivo 02, seção 9): rodar `oraculo.py` com e sem `--retencao-nao-conta` e ver qual bate. Anotar o resultado no `STATUS.md`.
- Escrever `tests/compare.py` (arquivo 06, seção 3) numa pasta local. Ele só depende de arquivos de texto. Testar com `saida_15.txt` contra ela mesma (tem que passar) e contra uma cópia com um PID trocado (tem que falhar).

## Seg 23/11 · fila e log 🔴
(Começa quando A avisar "M0 pronto", até 12h.)
- `fila.c`: arquivo 03, seção 2 (`fila.h`) e o pseudocódigo do plano antigo, que continua correto: lista duplamente encadeada intrusiva, `fila_inserir_fim` atribui `p->seq = f->proximo_seq++`, `fila_remover` religa os vizinhos em O(1), `fila_remover_inicio` e `fila_remover_fim` devolvem NULL se a fila estiver vazia. **Nenhuma função trava.**
- `log.c`: arquivo 03, seção 9. `vsnprintf` fora da trava. Vetor que dobra de capacidade. `qsort` estável por `(t_logico, categoria, ordem)` (`ordem` desempata, então o `qsort` comum basta). Linha impressa com `t_real`.
- Makefile: alvo `teste_unidade`, que compila e roda `tests/test_*.c` em `tests/bin/`.

### Teste do dia
- `test_fila`: inserir 1, 2, 3 e remover do início (sai o 1); remover o do meio de [1, 2, 3] (fica [1, 3], com `ant`/`prox` corretos); `fila_remover_fim` de [1] (fila vazia, com `inicio = fim = NULL`); remover de fila vazia devolve NULL; os `seq` são crescentes.
- `test_log`: registrar fora de ordem `(1000, EV_OUTRO)`, `(1000, EV_CHEGADA)`, `(0, EV_OUTRO)` e `(1000, EV_FINALIZADO)` e conferir que o arquivo sai na ordem 0 → chegada → finalizado → outro; formato exato `[t=...ms] [RR] [CPU0] ...`; acentos de "execução" corretos em UTF-8.

**Pronto quando:** `make teste_unidade` passa e valgrind está limpo. PR `b/fila-log` mesclado ainda na segunda, porque A precisa disso na terça.

---

## Ter 24/11 · cpus (GLOBAL), métricas, comparador → marco **M1-B** (23h) 🔴
- `cpus.c`: implementar **todas** as funções do arquivo 03, seção 7, com o código geral (o `nfilas` decide). Testar hoje só com `nfilas = 1` (GLOBAL), que é o que destrava o mono. As partes de POR_CPU (colocação por carga e migração) podem ficar escritas mas sem teste até sábado.
  - ⚠️ Entrega direta às ociosas, em ordem de índice (R9).
  - ⚠️ `cpus_esperar_trabalho` com o protocolo de soltar as outras filas e retravar (R8). No GLOBAL não há outras filas.
  - ⚠️ `cpus_verificar_termino` registra o término uma única vez e faz broadcast em todas as `cond`.
- `metricas.c`: arquivo 03, seção 9. Médias com `double` e `%.2f`. Trocas = soma das CPUs. Uma linha de utilização por CPU.
- `tests/test_metricas.c`: preencher à mão os PCBs do exemplo 4.4 (`first_dispatch` 0/2500/1000, `finish_time` 1000/4500/2500) e uma CPU (`ocupado_ms = 4500`, `trocas = 3`). Gravar num arquivo e comparar com o bloco de métricas de `saida_01.txt` (`diff` tem que dar vazio).
- `tests/run_all.sh` (arquivo 06, seção 3).

**Pronto quando (M1-B):** PR `M1-B: infraestrutura` aberto, com `make teste_unidade` passando, `compare.py` passando nos autotestes e o Makefile compilando os dois alvos sem warnings.

---

## Qua 25/11 · Revisão do M2 e casos multi extras
- Revisar o PR `M1-A` (e, à noite, o `M2`) de A. Foque no uso que o escalonador faz de `cpus_*`: trava antes, trava depois, `esperar_chegadas_ate` sempre.
- Criar `tests/casos/entrada_ex45.txt` (o exemplo 4.5 do PDF: os processos do caso 01 com `CPUS 2`) e `saida_ex45.txt` com `ferramentas/oraculo.py` (a saída bate com o PDF).
- Gerar com o oráculo **6 casos GLOBAL extras sem ambiguidade** (`oraculo.py --ambiguo` sai com 0): 2 e 4 CPUs, as três políticas, com várias chegadas em t=0 (`ferramentas/gerar_estresse.py --sem-ambiguidade`, de A; enquanto ele não existe, edite à mão uma entrada e confira com `--ambiguo`). Com 8 CPUs quase todo caso aleatório é ambíguo: ali vale o verificador de invariantes. Salvar como `tests/casos/entrada_g01.txt` e seguintes, com as saídas.

## Qui 26/11 · multi GLOBAL
- `make multiprocessador` e os casos 13, 15, 17, ex45 e g01 a g06. Corrigir `cpus.c` até passar.
- Os problemas típicos do GLOBAL são: CPU1 despacha o primeiro processo (a entrega não está em ordem de índice); ociosa que nunca acorda (entrega sem `signal` ou com o mutex errado); término antecipado (`cpus_executando` não foi somado na entrega).

## Sex 27/11 · marco **M3** 🔴
- `tests/run_all.sh` multi **5 vezes**: todos ok.
- PR `M3: multi GLOBAL`. A revisa e roda o helgrind.

**Pronto quando (M3):** 7/7 casos oficiais (mais os extras) ok 5 vezes, e A aprovou.

---

## Sáb 28/11 · POR_CPU 🔴
- Ativar e testar os caminhos `nfilas > 1` de `cpus.c`: colocação por `carga(k)` com `CARGA_CONTA_RETENCAO`, migração (rouba o **último** da fila **mais longa**, empate pelo menor índice, registro `Migracao PID p CPUj -> CPUk` em t na CPU de destino) e esperar/entregar com várias filas.
- Casos de teste: o exemplo POR_CPU do arquivo 02 (seção 7) e mais 6 gerados pelo oráculo sem ambiguidade (2 e 4 CPUs, as três políticas; com 8 CPUs, só invariantes). Salvar como `tests/casos/entrada_p01.txt` e seguintes.
- Conferir à mão as migrações de um caso pequeno com `ferramentas/contar_migracoes.py` (conta as linhas `Migracao` e, por PID, os despachos numa CPU diferente da anterior).

**Pronto quando:** todos os casos `p*` ok, 3 vezes seguidas.

## Dom 29/11 · marco **M4**
- PR `M4: POR_CPU`. A roda helgrind e estresse com 8 CPUs. Corrigir o que ele reportar (issues com o rótulo `bug`).
- Comentar todos os seus arquivos (o que faz, quais travas assume e quais trava).

**Pronto quando (M4):** A aprovou e a `main` passa em todos os casos.

---

## Seg 30/11 · Bônus, README (decisões) → marco **M5**
- **Bônus de afinidade** (arquivo 07, seção 3, siga as 5 orientações do grupo):
  - implementar os dois desempates (prioridade em `politica.c`; origem da migração em `cpus.c`) atrás de `#ifdef AFINIDADE` e só com `g_cfg.num_cpus > 1`;
  - conferir que, **sem** a flag, `tests/run_all.sh todos` continua 100% ok e que, com a flag, a saída bate com `oraculo.py --afinidade`;
  - escrever `ferramentas/medir_afinidade.sh` e gerar `ANALISE_AFINIDADE.md` (as três políticas × GLOBAL/POR_CPU × 2 e 4 CPUs, com e sem afinidade);
  - escrever a seção "Bônus: afinidade" do README, cobrindo os 6 pontos da seção 3.4 do arquivo 07 com os números reais.
- **README, seção "Decisões de projeto"** (arquivo 07, seção 1).
- Escrever o parágrafo GLOBAL × POR_CPU da discussão (utilização, migrações, equilíbrio de carga), a partir da tabela de A.

**Pronto quando (M5):** PR `M5: bônus + decisões` aprovado por A.

---

## Ter 01/12 · marco **M6**: congelamento 🔴
- `ferramentas/empacotar.sh` (arquivo 07, seção 4): gera `TP-SO-2026-2-<mat1>-<mat2>.tar.gz` com os arquivos **na raiz**, extrai numa pasta temporária vazia, compila os dois alvos com `-Werror` (só nesse teste), roda um caso de cada e imprime "pacote ok".
- Conferir que não existe `goto` (`grep -n goto *.c`), `printf` ou `fprintf(stderr`, nem warning.
- Tag `v1.0`. Depois disso, só correção de bug bloqueante, sempre por PR.

## Qua 02/12 e Qui 03/12
- Qua: ficar disponível para corrigir o que o teste de professor de A achar no LabGrad.
- Qui: depois que A enviar, **baixar o arquivo do Classroom**, extrair numa pasta limpa, rodar `make monoprocessador` e `make multiprocessador` e um caso de cada. Escrever "conferido" no `STATUS.md`.

## Se atrasar
- Atrasou a fila ou o log (seg 23): é caminho crítico. Faça o merge até terça 12h, no mais tardar. A pode usar a sua branch enquanto isso.
- Atrasou o GLOBAL: A ajuda na sexta, e o POR_CPU começa no sábado de qualquer jeito.
- Atrasou o POR_CPU: corte o bônus. O POR_CPU vale 1,5 ponto, e o bônus vale no máximo 0,5.
