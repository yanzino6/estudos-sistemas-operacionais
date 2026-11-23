# TP SO 2026/2 · Mini-kernel · Plano v2 (após revisão cega)

> Estes arquivos substituem os três planos anteriores (geral, Plano A e Plano B).
> Eles foram reescritos depois de uma revisão cega feita contra o PDF do enunciado
> e os 6 casos públicos. Quem implementar seguindo só estes arquivos, na ordem,
> não precisa de reunião.

## 1. Quem é quem

| Sigla | Pessoa | Trilha |
|---|---|---|
| **A** | Yan | Motor: tempo, entrada, PCB e threads, barreira, geradora, escalonador, políticas, `main`, estresse e análise comparativa |
| **B** | Dupla | Infraestrutura e multiprocessador: Makefile, fila, log, CPUs (GLOBAL e POR_CPU), métricas, comparador, bônus, empacotamento e seção de decisões do README |

A troca de papéis é permitida, desde que seja feita antes de 23/11 e escrita no `STATUS.md` do repositório.

## 2. Ordem de leitura

Leia na ordem. Cada arquivo supõe que você leu os anteriores.

| # | Arquivo | A | B | Tempo de leitura |
|---|---|---|---|---|
| 00 | `00_LEIA_PRIMEIRO.md` (este) | ✅ | ✅ | 10 min |
| 01 | `01_REVISAO_CEGA.md`: o que estava errado nos planos antigos e por quê | ✅ | ✅ | 15 min |
| 02 | `02_REGRAS_CONSOLIDADAS.md`: todas as regras do enunciado e dos casos num lugar só, com rastros | ✅ | ✅ | 30 min |
| 03 | `03_ARQUITETURA_E_CONTRATOS.md`: módulos, headers (contrato), travas, modelo de tempo, protocolo da fatia, laço do escalonador | ✅ | ✅ | 45 min |
| 04 | `04_TRILHA_A.md`: o dia a dia de A | ✅ | só os marcos | 20 min |
| 05 | `05_TRILHA_B.md`: o dia a dia de B | só os marcos | ✅ | 20 min |
| 06 | `06_TESTES_E_AMBIENTE.md`: ambiente Linux no Mac, oráculo, comparador, valgrind/helgrind, estresse | ✅ | ✅ | 20 min |
| 07 | `07_README_ANALISE_BONUS_ENTREGA.md`: README, análise comparativa, bônus, empacotamento e entrega | ✅ | ✅ | 15 min |

Junto vem `ferramentas/oraculo.py`: um simulador determinístico das regras que
reproduz **byte a byte** as saídas dos casos 01, 03, 05, 13, 15, 17 e do exemplo 4.5.
Ele gera a saída esperada de qualquer entrada nova. Ele não vai na entrega. `ferramentas/contar_migracoes.py` extrai de um log as medidas do bônus de afinidade.

## 3. Como trabalhamos sem reunião

### 3.1 Fonte única da verdade
- **Repositório privado no GitHub** (criado por A no marco M0). A `main` sempre compila nos dois alvos.
- **`STATUS.md`** na raiz do repositório. Fica fora do `.tar.gz`. Cada um atualiza a própria seção **ao fim de toda sessão de trabalho**, com o formato abaixo:

```
## A (Yan) · atualizado em 2026-11-24 22:10
Feito: geradora.c, escalonador.c (laço), politica.c
Próximo: integração M2 amanhã de manhã
Bloqueado por: nada
Perguntas para B: #7 (issue)
```

- **Contratos** (`*.h` listados no arquivo 03): só mudam por PR com o rótulo `contrato`, e o outro precisa aprovar. Mudança de contrato sem aprovação é proibida. É isso que permite trabalhar em paralelo sem conversar.

### 3.2 Branches e PRs
- Branches: `a/<tema>` e `b/<tema>`. PR para a `main`.
- Todo PR de marco tem o título `M<n>: ...` e, no corpo, a lista "Como verificar", com os comandos e a saída obtida.
- **Revisão em até 24 h.** Se o revisor não aparecer em 24 h e os comandos de verificação passarem, o autor pode fazer o merge e anota isso no `STATUS.md`. Ninguém fica parado esperando.

### 3.3 Dúvidas
- Abra uma issue com o rótulo `duvida`. O outro responde em até 12 h.
- **Enquanto a resposta não vem, siga o padrão escrito nestes arquivos.** Todo ponto em aberto tem um padrão definido (ver arquivo 02, seção 9). Se não houver padrão, escolha a opção mais simples, siga em frente e registre na issue.

### 3.4 Regra de ouro
Quem quebra a `main` conserta antes de qualquer outra coisa. Erro de compilação vale nota zero.

## 4. Marcos (pontos de entrega e verificação)

Cada marco tem **dono**, **prazo**, **o que entra na `main`** e **como o outro verifica**. Os detalhes estão nas trilhas (arquivos 04 e 05).

| Marco | Prazo | Dono | Entrega na `main` | Verificação (quem não é o dono roda) |
|---|---|---|---|---|
| **M0** | seg 23/11, 12h | A | repositório, `.gitignore`, `STATUS.md`, **todos os headers do arquivo 03 copiados literalmente**, `.c` vazios (stubs), Makefile do arquivo 03, `tests/casos/` com os casos públicos, `ferramentas/oraculo.py` e `ferramentas/contar_migracoes.py` | `make monoprocessador && make multiprocessador` sem nenhum warning |
| **M1-B** | ter 24/11, 23h | B | `fila.c`, `log.c`, `metricas.c`, `cpus.c` (só modo GLOBAL), `tests/compare.py`, `tests/run_all.sh`, testes unitários | `make teste_unidade` passa; `python3 tests/compare.py tests/casos/saida_01.txt tests/casos/saida_01.txt` sai com 0 |
| **M1-A** | ter 24/11, 23h | A | `tempo.c`, `barreira.c`, `entrada.c`, `pcb.c`, `geradora.c`, `politica.c`, `escalonador.c`, `main.c` | compila; o teste de barreira passa; o teste de threads (arquivo 04, dia 1) passa |
| **M2** | qua 25/11, 23h | A (integra) | merge de M1-A e M1-B; **casos 01, 03 e 05 passando** no mono | `tests/run_all.sh mono` mostra 3/3 ok, 3 vezes seguidas; valgrind com 0 erros no caso 05 |
| **M3** | sex 27/11, 23h | B (multi) e A (estresse) | **casos 13, 15, 17 e exemplo 4.5 passando** no multi GLOBAL; gerador de estresse e verificador de invariantes | `tests/run_all.sh` mostra 7/7 ok, 5 vezes seguidas; helgrind limpo em 3 casos pequenos |
| **M4** | dom 29/11, 23h | B | `cpus.c` com POR_CPU (colocação, migração); casos POR_CPU gerados pelo oráculo passando | `tests/run_all.sh porcpu` ok; A roda helgrind com POR_CPU e 8 CPUs, sem erros |
| **M5** | seg 30/11, 23h | A e B | bônus de afinidade atrás de `AFINIDADE=1` + `ANALISE_AFINIDADE.md` (B), tabela da análise sem afinidade (A), README completo (decisões e bônus: B; análise: A) | o outro lê o README e marca "ok" no PR |
| **M6** | ter 01/12, 23h | B | **código congelado**, tag `v1.0`, `ferramentas/empacotar.sh`, teste "de professor" numa pasta limpa | A repete o teste de professor no LabGrad (ou numa VM Ubuntu) na quarta |
| Entrega | qui 03/12 | A envia, B confere | `TP-SO-2026-2-<mat1>-<mat2>.tar.gz` no Classroom | B baixa o arquivo enviado e roda `make` nos dois alvos |

**Prazo oficial: sexta 04/12/2026, até 07h59. Não há aceitação com atraso.** A meta interna é quinta 03/12, deixando uma noite de folga.

## 5. Caminho crítico

`M0 → M1-A/M1-B → M2 (mono) → M3 (GLOBAL) → M4 (POR_CPU) → M6 (congelamento)`

Se um marco atrasar mais de 24 h, o outro integrante assume a próxima tarefa 🔴 do atrasado e registra no `STATUS.md`. **O bônus é o primeiro a ser cortado. O helgrind nunca é cortado.**

## 6. Antes de começar (domingo 22/11, cada um sozinho, 30 min)
1. Ler os arquivos 00 a 03.
2. Montar o ambiente Linux (arquivo 06, seção 1). **No macOS não existe `pthread_barrier_t` e o valgrind não roda.** Todo teste é feito em Linux.
3. Baixar do Classroom **`carga_analise.txt`** e **os casos públicos que faltam** (02, 04, 06 a 12, 14, 16 e 18). Na pasta local só existem 01, 03, 05, 13, 15 e 17. Quem achar primeiro faz o commit em `tests/casos/`.
