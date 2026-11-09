# Exercícios — 02_monitores

Comandos abaixo são executados a partir da raiz `provaPratica`. As adaptações de entrada/saída servem ao treino; as exigências originais e questões discursivas continuam na rubrica. Nos casos de análise, os cenários são complementos didáticos criados para avaliar a compreensão do enunciado original.

## m01 — Buffer com if e sinaliza-e-continua

**Origem:** Professor — Lista 7, questão 1; Aula 12, p.13

O monitor da lista tem itens=0 e condições temItens/temEspacos. Pega executa while(1){if(!itens) wait(temItens); retira; itens--; signal(temEspacos); return item;}. Coloca executa while(1){if(itens==MAX) wait(temEspacos); insere; itens++; signal(temItens);}. Explique por que não funciona sob sinaliza-e-continua e reescreva os dois procedimentos.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/m01/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/02_monitores/testes/m01/respostas.modelo.json /tmp/m01.json
# edite /tmp/m01.json
python3 estudo/02_monitores/testes/m01/corrigir.py /tmp/m01.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/m01/gabarito.md) e [rubrica](testes/m01/rubrica.md) para concluir a correção manual.

---

## m02 — Porto com prioridade de saída

**Origem:** Professor — Lista 7, questão 2

Um porto comporta N navios, canal permite um por vez e saídas têm prioridade. Implemente pedido_entrada(), entrada_OK(), pedido_saida(), saida_OK(). Cada navio chama pedido_entrada, atravessa, confirma entrada, atraca, pede saída, atravessa e confirma saída. Declare se ocupação inclui reserva de entrada.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/m02/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/02_monitores/testes/m02/respostas.modelo.json /tmp/m02.json
# edite /tmp/m02.json
python3 estudo/02_monitores/testes/m02/corrigir.py /tmp/m02.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/m02/gabarito.md) e [rubrica](testes/m02/rubrica.md) para concluir a correção manual.

---

## m03 — Implementar Hoare e Hansen com semáforos

**Origem:** Professor — Lista 7, questão 3; Aula 12, pp.24–26

Desenvolva entrada/saída do monitor, wait e signal usando semáforos para Hoare e para sinaliza-e-continua (Hansen no material). Explique mutex, fila de condições, fila urgente e seus contadores. Declare a disciplina adotada.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/m03/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/02_monitores/testes/m03/respostas.modelo.json /tmp/m03.json
# edite /tmp/m03.json
python3 estudo/02_monitores/testes/m03/corrigir.py /tmp/m03.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/m03/gabarito.md) e [rubrica](testes/m03/rubrica.md) para concluir a correção manual.

---

## m04 — Barbeiro com monitor

**Origem:** Professor — Lista 7, questão 4

Um barbeiro, uma cadeira de corte e cinco de espera. Cliente sem vaga sai; barbeiro sem clientes dorme; uma pessoa por vez ocupa a cadeira de corte. Modele entrada, chamada, término e saída com monitor.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/m04/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/02_monitores/testes/m04/respostas.modelo.json /tmp/m04.json
# edite /tmp/m04.json
python3 estudo/02_monitores/testes/m04/corrigir.py /tmp/m04.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/m04/gabarito.md) e [rubrica](testes/m04/rubrica.md) para concluir a correção manual.

---

## m05 — Filósofos no monitor

**Origem:** Professor — Aula 12, pp.20–23

Explique e reescreva pickup, putdown e test usando estados e uma condição por filósofo. Compare o monitor Hoare do slide com uma tradução para Pthreads. Avalie as afirmações de ausência de deadlock e starvation.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/m05/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/02_monitores/testes/m05/respostas.modelo.json /tmp/m05.json
# edite /tmp/m05.json
python3 estudo/02_monitores/testes/m05/corrigir.py /tmp/m05.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/m05/gabarito.md) e [rubrica](testes/m05/rubrica.md) para concluir a correção manual.

---

## m06 — Notificação perdida e despertar espúrio

**Origem:** Criado para este estudo

Analise dois erros: usar condição como contador de eventos e trocar while por if porque “só uma thread sinaliza”. Explique como predicado protegido evita ambos.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/m06/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/02_monitores/testes/m06/respostas.modelo.json /tmp/m06.json
# edite /tmp/m06.json
python3 estudo/02_monitores/testes/m06/corrigir.py /tmp/m06.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/m06/gabarito.md) e [rubrica](testes/m06/rubrica.md) para concluir a correção manual.

---

## m07 — Buffer limitado em C

**Origem:** Criado para este estudo — implementação de monitor com Pthreads

Leia capacidade N, quantidade k e k inteiros (1<=N<=16,0<=k<=1000). Use uma thread produtora e uma consumidora, mutex e duas condições. Produza/consuma exatamente k itens por buffer circular. Consumidora acumula soma; main imprime a soma após joins. Inclua caso k=0 sem bloquear.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/02_monitores/testes/m07/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/02_monitores/testes/m07/corrigir.py ./solucao
```

Casos: [casos.json](testes/m07/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/m07/rubrica.md).
