# TP SO 2026/2 · Mini-kernel · Leia primeiro (revisão de 23/11, atualizado em 24/11)

## O que cada um lê

| Quem | Arquivo | E mais nada |
|---|---|---|
| **A** (Yan): motor, políticas, estresse, análise | `04_TRILHA_A.md` | a trilha traz o desenho, o contrato, o pseudocódigo, os testes e a metade de B explicada |
| **B** (dupla): infraestrutura, multiprocessador, bônus, entrega | `05_TRILHA_B.md` | idem, com a metade de A explicada |

Cada trilha é autossuficiente: dá para implementar a sua metade e entender o trabalho
inteiro sem abrir nenhum outro arquivo de `Docs/`. Os trechos comuns (desenho, contrato,
pseudocódigo, pontos em aberto) são **idênticos** nas duas. Elas supõem que você já leu o
enunciado e o `LEIA-ME` dos casos, e relembram cada regra no ponto em que ela vira código.

## O que há na pasta `trabalho/`

| Pasta | Conteúdo |
|---|---|
| `Docs/` | este arquivo, as duas trilhas e o PDF do enunciado |
| `contrato/` | os 12 headers (`.h`). No marco M0, A copia todos para a raiz do repositório, sem alterar. O Makefile inicial está escrito na etapa A0 da Trilha A |
| `ferramentas/` | `oraculo.py` (simulador de referência) e `contar_migracoes.py` (medidas do bônus). Não vão na entrega |
| `casos_publicos_v1/` | os 6 casos oficiais (01, 03, 05 mono; 13, 15, 17 multi GLOBAL) |
| `casos_extras/` | 14 casos gerados pelo oráculo (GLOBAL e POR_CPU, 2 e 4 CPUs), todos não ambíguos. Não são oficiais |

## Atualização de 24/11

**A CPU retida conta como carga (ponto P1): confirmado.** No POR_CPU, uma CPU que terminou
o processo antes do fim do quantum e ainda está em retenção vale 1 na soma da carga usada
para colocar quem chega. Era o comportamento que as trilhas já descreviam, então nenhum
resultado nem caso de teste mudou.

Com a regra fechada, o mecanismo que permitia a outra leitura saiu do desenho:

| Onde | O que saiu |
|---|---|
| `cpus.h` | a macro `CARGA_CONTA_RETENCAO`, o campo `retida` de `CPU` e a função `cpus_inicio_retencao` |
| `cpus.c` (B) | `carga(k)` agora é só `filas[k].tamanho + (executando ≠ NULO ? 1 : 0)`; `cpus_devolver` não mexe mais em `retida` |
| `escalonador.c` (A) | na retenção, o laço só faz `dormir_ate(t_lib)`; saiu a linha `cpus_travar(); cpus_inicio_retencao(k); cpus_destravar()` |
| `oraculo.py` | a opção `--retencao-nao-conta` |

Funciona sem marcação nenhuma porque `executando` só volta a NULO em `cpus_devolver`, que
o laço chama no fim da retenção. **Se o repositório já tem o `cpus.h` antigo ou stubs,
apliquem as três primeiras linhas da tabela** (é mudança de contrato: PR com o rótulo
`contrato`). Os arquivos antigos (`01`, `02`, `03`, `06`, `07`) já foram apagados.

## O que mudou nesta revisão

1. **Corrida na partida (corrigida).** No desenho anterior a geradora era criada antes das escalonadoras e cada escalonadora começava escolhendo sozinha da fila. Com poucos processos em t=0, a CPU1 podia chegar à trava antes da CPU0 e levar o primeiro processo (viola o desempate 3). Medido num caso mínimo (2 processos em t=0, 8 CPUs): CPU errada em 48 de 150 execuções. Correção: toda CPU **nasce ociosa** e a escalonadora começa em `cpus_esperar_trabalho`; o primeiro processo sempre chega por entrega direta, em ordem de índice. Depois da correção: 0 de 250.
2. **O oráculo não estava na pasta.** Os arquivos antigos diziam que `ferramentas/oraculo.py` e `contar_migracoes.py` "vinham junto", mas não existiam aqui. Foram recriados. O oráculo reproduz byte a byte os 6 casos públicos e o exemplo 4.5, e os números de afinidade do caso 17 que os arquivos antigos citavam.
3. **Contrato como arquivos.** Os headers saíram do markdown e viraram `contrato/*.h`, compilados com `-Wall` sem warnings. `entrada.h`, `escalonador.h` e `metricas.h` estavam incompletos (sem guarda nem includes) e foram completados. `arred100` e `cpus_encerrado` não eram usados por ninguém e saíram.
4. **Trilhas autossuficientes.** Antes eram listas de tarefas que apontavam para quatro outros arquivos. Agora cada etapa traz os lembretes do enunciado, o header, o pseudocódigo, os porquês, as armadilhas e um teste com os valores esperados.
5. **Armadilhas que não estavam escritas:** `pcb_iniciar` só depois do `qsort` (mutex iniciado não pode ser movido); `agora_ms` com a conta em microssegundos; `proxima_chegada` só avança depois do lote inserido.
6. **Teste novo para B:** `test_cpus`, que exercita o `cpus.c` inteiro (GLOBAL e POR_CPU, com migração e término) usando uma thread só, antes de o escalonador de A existir.
7. **Pontos em aberto novos:** P7 (CPU já ociosa × CPU que libera no mesmo instante de uma chegada) e P8 (no POR_CPU, a ociosa não rouba o processo recém-reinserido). Estão no apêndice A das trilhas, com o padrão adotado.
8. **Ambiente.** Os arquivos antigos supunham macOS para o Yan; a máquina dele é Linux, então a seção de ambiente da Trilha A ficou em duas linhas.
9. **Datas.** O M0 passou das 12h para as **14h de segunda 23/11**. O resto do calendário não mudou.

## Como o pseudocódigo das trilhas foi conferido

Ele foi implementado linha a linha num protótipo de teste (que **não** está na pasta: o
código da entrega é de vocês). Resultados:
- os 6 casos públicos e os 14 extras passam, em 3 execuções seguidas;
- mais de 60 entradas aleatórias não ambíguas saem iguais ao oráculo (8 delas com o bônus de afinidade ligado);
- mais de 300 entradas ambíguas, 64 delas no formato do caso oculto de estresse (8 CPUs, 40 processos, até 4 threads), terminam sem violar nenhum invariante, com o tempo medido a menos de 40 ms do instante lógico;
- `valgrind --leak-check=full` e `valgrind --tool=helgrind` com 0 erros em mono, GLOBAL e POR_CPU (inclusive com migração);
- os valores esperados dos testes unitários descritos nas trilhas (`test_pcb`, `test_barreira`, `test_politica`, `test_log`, `test_cpus`, `test_metricas`) foram obtidos nesse protótipo.

O que **não** dá para conferir daqui: os casos ocultos e os casos oficiais POR_CPU, que não
vieram no pacote público. Os pontos P2, P7 e P8 são leituras nossas do enunciado (P1 foi
confirmado em 24/11).

## Calendário

| Marco | Prazo | A (Yan) | B (dupla) |
|---|---|---|---|
| M0 | seg 23/11, 14h | repositório, contrato, stubs | (adianta o comparador) |
| M1 | ter 24/11, 23h | motor completo (PR `M1-A`) | infraestrutura e `cpus.c` (PR `M1-B`) |
| M2 | qua 25/11, 23h | integra: mono passando | revisa |
| M3 | sex 27/11, 23h | estresse e helgrind | multi GLOBAL |
| M4 | dom 29/11, 23h | estresse POR_CPU | POR_CPU |
| M5 | seg 30/11, 23h | análise no README | bônus e decisões no README |
| M6 | ter 01/12, 23h | teste de professor (qua 02) | congelamento e pacote |
| Entrega | qui 03/12 | envia | confere |

**Prazo oficial: sexta 04/12/2026, 07h59, sem atraso.**
