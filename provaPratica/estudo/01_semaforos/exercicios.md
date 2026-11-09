# Exercícios — 01_semaforos

Comandos abaixo são executados a partir da raiz `provaPratica`. As adaptações de entrada/saída servem ao treino; as exigências originais e questões discursivas continuam na rubrica. Nos casos de análise, os cenários são complementos didáticos criados para avaliar a compreensão do enunciado original.

## s01 — Impressão ordenada de cinco blocos

**Origem:** Professor — Aula 11.2, pp. 3–4

Crie cinco processos lógicos: o primeiro imprime 1–200, o segundo 201–400, até 801–1000. Acrescente semáforos para garantir essa ordem. Dê inicialização e operações de cada participante. A numeração P0–P4 dos códigos equivale aos processos 1–5 do texto.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s01/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s01/respostas.modelo.json /tmp/s01.json
# edite /tmp/s01.json
python3 estudo/01_semaforos/testes/s01/corrigir.py /tmp/s01.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s01/gabarito.md) e [rubrica](testes/s01/rubrica.md) para concluir a correção manual.

---

## s02 — Grafo de nove funções

**Origem:** Professor — Aula 11.2, pp. 5–6

Acrescente no máximo seis semáforos aos processos A: F1,F2,F9,F4; B: F3,F7; C: F8,F5,F6. O grafo exige F1→F2,F5,F8; F2→F3,F6; F5→F6,F9; F8→F9; F6→F7; F9→F7; F3→F4; F7→F4. Veja a figura em referencias/grafo_nove_funcoes.png. Preserve as ordens locais. Analise também o código apresentado como solução na página 6.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s02/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s02/respostas.modelo.json /tmp/s02.json
# edite /tmp/s02.json
python3 estudo/01_semaforos/testes/s02/corrigir.py /tmp/s02.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s02/gabarito.md) e [rubrica](testes/s02/rubrica.md) para concluir a correção manual.

---

## s03 — Junção A/B e bifurcação D/E

**Origem:** Professor — Aula 11.2, pp. 7–9

Modele A→C, B→C, C→D e C→E. Use os três semáforos sugeridos nos slides e mostre os cinco processos.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s03/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s03/respostas.modelo.json /tmp/s03.json
# edite /tmp/s03.json
python3 estudo/01_semaforos/testes/s03/corrigir.py /tmp/s03.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s03/gabarito.md) e [rubrica](testes/s03/rubrica.md) para concluir a correção manual.

---

## s04 — Pombo correio

**Origem:** Professor — Aula 11.2, pp. 10–12

Modele muitos usuários e um pombo. A mochila comporta exatamente 20 mensagens; o vigésimo usuário acorda o pombo; ninguém cola mensagens durante a viagem; após levar as mensagens de A a B ele volta e aceita novo lote. Identifique regiões críticas.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s04/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s04/respostas.modelo.json /tmp/s04.json
# edite /tmp/s04.json
python3 estudo/01_semaforos/testes/s04/corrigir.py /tmp/s04.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s04/gabarito.md) e [rubrica](testes/s04/rubrica.md) para concluir a correção manual.

---

## s05 — Montanha-russa

**Origem:** Professor — Aula 11.2, pp. 13–14

Há n passageiros que repetem embarque, passeio e retorno à fila; um carro comporta C<n pessoas e só parte cheio. Várias portas permitem embarques concorrentes. Resolva usando no máximo quatro semáforos, distinguindo embarque, fim da viagem e desembarque. A p. 13 contém nomes copiados do pombo; use o enunciado da p. 14.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s05/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s05/respostas.modelo.json /tmp/s05.json
# edite /tmp/s05.json
python3 estudo/01_semaforos/testes/s05/corrigir.py /tmp/s05.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s05/gabarito.md) e [rubrica](testes/s05/rubrica.md) para concluir a correção manual.

---

## s06 — Canibais e cozinheiro

**Origem:** Professor — Aula 11.2, pp. 15–17

Modele N canibais e uma travessa de M porções. Quem encontra a travessa vazia acorda o cozinheiro e espera reposição; ele deve ser acordado apenas quando necessário. Desenvolva os dois processos, sem deadlock. Declare estado inicial; nos cenários abaixo a travessa começa cheia.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s06/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s06/respostas.modelo.json /tmp/s06.json
# edite /tmp/s06.json
python3 estudo/01_semaforos/testes/s06/corrigir.py /tmp/s06.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s06/gabarito.md) e [rubrica](testes/s06/rubrica.md) para concluir a correção manual.

---

## s07 — Filme sobre a vida de Hoare

**Origem:** Professor — Aula 11.2, pp. 18–20

Modele um demonstrador e muitos fãs. Auditório comporta dez; exibição começa lotada; fãs só saem para telefonar depois do filme completo e então retornam à fila. Analise o problema da solução proposta e apresente protocolo corrigido.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s07/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s07/respostas.modelo.json /tmp/s07.json
# edite /tmp/s07.json
python3 estudo/01_semaforos/testes/s07/corrigir.py /tmp/s07.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s07/gabarito.md) e [rubrica](testes/s07/rubrica.md) para concluir a correção manual.

---

## s08 — Leitores e escritores: três políticas

**Origem:** Professor — Aula 11.2, pp. 21–33; Lista 6, questão 2

Explique preferência dos leitores, preferência dos escritores e a solução com R,M,W. Na Lista 6: leitor 2:P(R),3:P(M),4:rc++,5:se rc==1 P(W),6:V(M),7:V(R),8:LÊ,9:P(M),10:rc--,11:se rc==0 V(W),12:V(M). Escritor 3:P(R),4:P(W),5:ESCREVE,6:V(W),7:V(R). Todos os semáforos começam em 1, rc=0. (a) Discuta starvation citando linhas; (b) explique M e dê um intercalamento ruim sem ele.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s08/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s08/respostas.modelo.json /tmp/s08.json
# edite /tmp/s08.json
python3 estudo/01_semaforos/testes/s08/corrigir.py /tmp/s08.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s08/gabarito.md) e [rubrica](testes/s08/rubrica.md) para concluir a correção manual.

---

## s09 — Barbeiro dorminhoco com semáforos

**Origem:** Professor — Aula 11.2, pp. 34–35

Modele um barbeiro, sua cadeira e cinco cadeiras de espera. Sem clientes ele dorme; cliente que encontra espera cheia vai embora; caso contrário aguarda atendimento. Explique customers, barbers, mutex e waiting; acrescente conclusão se desejar saída somente após corte.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s09/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s09/respostas.modelo.json /tmp/s09.json
# edite /tmp/s09.json
python3 estudo/01_semaforos/testes/s09/corrigir.py /tmp/s09.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s09/gabarito.md) e [rubrica](testes/s09/rubrica.md) para concluir a correção manual.

---

## s10 — Filósofos e deadlock

**Origem:** Professor — Aula 11.2, pp. 36–38

Cinco filósofos alternam pensar/comer e precisam dos dois garfos adjacentes. Explique a espera circular de pegar sempre o esquerdo e desenvolva a solução por estados, mutex e semáforo por filósofo. Discuta justiça.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s10/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s10/respostas.modelo.json /tmp/s10.json
# edite /tmp/s10.json
python3 estudo/01_semaforos/testes/s10/corrigir.py /tmp/s10.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s10/gabarito.md) e [rubrica](testes/s10/rubrica.md) para concluir a correção manual.

---

## s11 — Grafo da Lista 6

**Origem:** Professor — Lista 6, questão 1

Inicialize semáforos e acrescente P/V a P1: F1,F3; P2: F2,F5,F7; P3: F4,F6. Dependências: F1→F2,F3,F4; F2→F5; F3→F5,F6; F4→F6; F5→F7; F6→F7. Consulte referencias/lista6_grafo.png.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s11/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s11/respostas.modelo.json /tmp/s11.json
# edite /tmp/s11.json
python3 estudo/01_semaforos/testes/s11/corrigir.py /tmp/s11.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s11/gabarito.md) e [rubrica](testes/s11/rubrica.md) para concluir a correção manual.

---

## s12 — Semáforo múltiplo

**Origem:** Professor — Lista 6, questão 3

Implemente down(S1,...,Sn) e up(S1,...,Sn) atômicos sobre o conjunto. Você pode consultar os contadores. O esboço da lista devolve booleano: use tentativa tudo-ou-nada e explique como uma variante bloqueante seria diferente. Inclua liberação e sincronização de todos os acessos.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s12/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s12/respostas.modelo.json /tmp/s12.json
# edite /tmp/s12.json
python3 estudo/01_semaforos/testes/s12/corrigir.py /tmp/s12.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s12/gabarito.md) e [rubrica](testes/s12/rubrica.md) para concluir a correção manual.

---

## s13 — Três fumantes

**Origem:** Professor — Lista 6, questão 4

Agente fornece dois dos ingredientes tabaco, papel e fósforo. Cada fumante possui ilimitadamente um deles. Desbloqueie exatamente o fumante que tem o ingrediente faltante; ele termina e acorda o agente para nova rodada. Desenvolva os processos usando semáforos.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s13/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s13/respostas.modelo.json /tmp/s13.json
# edite /tmp/s13.json
python3 estudo/01_semaforos/testes/s13/corrigir.py /tmp/s13.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s13/gabarito.md) e [rubrica](testes/s13/rubrica.md) para concluir a correção manual.

---

## s14 — Buffer sem mutex e publicação fora de ordem

**Origem:** Professor — Aula 11, pp. 14–22; casos de análise criados para o treino

Explique a solução com vagas/itens/mutex. Responda ao exercício da p.20: um produtor e um consumidor permitem retirar mutex? Depois analise a otimização da p.22, em que vários produtores reservam índice sob lock, liberam lock e só depois escrevem. Produza um contraexemplo se necessário.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s14/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s14/respostas.modelo.json /tmp/s14.json
# edite /tmp/s14.json
python3 estudo/01_semaforos/testes/s14/corrigir.py /tmp/s14.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s14/gabarito.md) e [rubrica](testes/s14/rubrica.md) para concluir a correção manual.

---

## s15 — Semáforos: diagnóstico de recursos

**Origem:** Criado para este estudo

Uma oficina tem três ferramentas idênticas, cinco trabalhadores e contador semafórico iniciado em três. Simule aquisições e liberações. Compare recurso limitado, exclusão mútua, precedência, deadlock e starvation.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/s15/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/01_semaforos/testes/s15/respostas.modelo.json /tmp/s15.json
# edite /tmp/s15.json
python3 estudo/01_semaforos/testes/s15/corrigir.py /tmp/s15.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/s15/gabarito.md) e [rubrica](testes/s15/rubrica.md) para concluir a correção manual.

---

## s16 — Contador protegido por semáforo

**Origem:** Criado para este estudo — implementação prática

Leia t e k (1<=t<=16, 0<=k<=10000). Crie t threads; cada uma incrementa k vezes um contador compartilhado protegido por semáforo binário. Imprima somente o total. No Linux pode usar sem_init; no macOS use um semáforo nomeado com sem_open e limpeza adequada ou execute em Linux. Não substituir a atividade por multiplicação direta.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/01_semaforos/testes/s16/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/01_semaforos/testes/s16/corrigir.py ./solucao
```

Casos: [casos.json](testes/s16/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/s16/rubrica.md).
