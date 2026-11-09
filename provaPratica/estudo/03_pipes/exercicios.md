# Exercícios — 03_pipes

Comandos abaixo são executados a partir da raiz `provaPratica`. As adaptações de entrada/saída servem ao treino; as exigências originais e questões discursivas continuam na rubrica. Nos casos de análise, os cenários são complementos didáticos criados para avaliar a compreensão do enunciado original.

## p01 — Comunicação circular entre três processos

**Origem:** Professor — roteiro_lab03, exercício 1, p.1

Pai lê um inteiro, envia ao primeiro filho, que soma 5; segundo filho recebe e soma 10; devolve ao pai pelo terceiro pipe. Pai imprime “[PARENT] Resultado Final: valor!”. Use dois filhos irmãos, três pipes, read/write e waitpid. Contrato de treino: entrada é um inteiro entre -1000000 e 1000000, saída exatamente a frase com o valor e exclamação; nenhum prompt.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/03_pipes/testes/p01/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/03_pipes/testes/p01/corrigir.py ./solucao
```

Casos: [casos.json](testes/p01/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/p01/rubrica.md).

---

## p02 — Pipeline ping | grep

**Origem:** Professor — roteiro_lab03, exercício 2, pp.2–3

Implemente ping -c 5 google.com | grep rtt usando pipe, fork, dup2 e execlp, com dois filhos. Teste offline: o corretor põe um ping controlado no PATH e executa grep real. Seu stdout contém somente o que grep imprime. Para o caso sem rtt, o controlador deve terminar normalmente após recolher os filhos (exit 0), mesmo que grep retorne 1 por ausência de seleção. Essa política de retorno é uma adaptação de teste, não uma exigência textual do professor.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/03_pipes/testes/p02/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/03_pipes/testes/p02/corrigir.py ./solucao
```

Casos: [casos.json](testes/p02/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/p02/rubrica.md).

---

## p03 — Avô, pai e neto

**Origem:** Professor — roteiro_lab03, exercício 3, pp.3–4

Avô lê vetor, filho multiplica cada valor por três, neto inverte, filho devolve ao avô e avô imprime. Máximo 100 elementos. Contrato de treino: primeira entrada n (1<=n<=100), seguida por n inteiros entre -1000000 e 1000000. Saída “[GRANDPARENT] Vetor final:” seguida dos valores separados por espaços, sem prompts. O exemplo original n=5 deve resultar em 15 12 9 6 3.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/03_pipes/testes/p03/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/03_pipes/testes/p03/corrigir.py ./solucao
```

Casos: [casos.json](testes/p03/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/p03/rubrica.md).

---

## p04 — Experimento pipe1.c e EOF

**Origem:** Professor — Aula 14, pp.24–27; cenários controlados de treino

Reconstrua o envio de hello world do pai ao filho. Depois, conforme p.26, mantenha a ponta de leitura no pai, retire operações do filho e observe uma escrita pequena. Explique também as variantes com todos os leitores fechados e com uma ponta de escrita esquecida. A previsão deve declarar quais descritores ainda existem, sem depender apenas de sleep.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/p04/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/03_pipes/testes/p04/respostas.modelo.json /tmp/p04.json
# edite /tmp/p04.json
python3 estudo/03_pipes/testes/p04/corrigir.py /tmp/p04.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/p04/gabarito.md) e [rubrica](testes/p04/rubrica.md) para concluir a correção manual.

---

## p05 — Dois filhos escrevendo para o pai

**Origem:** Professor — Aula 14, exemplo double.c/son.c, pp.33–36

Analise double.c: dois filhos executam son, cada qual recebe o número de um descritor e escreve “A: Bom dia!” ou “B: Bom dia!” em registro de 11 bytes. Explique herança através de exec, fechamento da escrita no pai, leitura de dois registros e espera dos filhos. Proponha versão robusta a reads parciais.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/p05/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/03_pipes/testes/p05/respostas.modelo.json /tmp/p05.json
# edite /tmp/p05.json
python3 estudo/03_pipes/testes/p05/corrigir.py /tmp/p05.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/p05/gabarito.md) e [rubrica](testes/p05/rubrica.md) para concluir a correção manual.

---

## p06 — Explicar ls | wc e dup2

**Origem:** Professor — Aula 14, exemplo 3, pp.37–41

Diga o que faz o programa que cria pipe, no filho duplica leitura em 0 e exec wc, no pai duplica escrita em 1 e exec ls. Desenhe tabelas de descritores antes/depois de dup2/exec e reescreva com um controlador que recolha ambos.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/p06/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/03_pipes/testes/p06/respostas.modelo.json /tmp/p06.json
# edite /tmp/p06.json
python3 estudo/03_pipes/testes/p06/corrigir.py /tmp/p06.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/p06/gabarito.md) e [rubrica](testes/p06/rubrica.md) para concluir a correção manual.

---

## p07 — Capacidade do pipe e alarm

**Origem:** Professor — Aula 14, exemplo 4, p.42

Analise o programa que escreve bytes em um pipe sem leitor ativo, reagendando alarm(20) a cada iteração. Explique o bloqueio e a diferença de colocar alarm fora do for. Discuta segurança do handler e limite de atomicidade versus capacidade.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/p07/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/03_pipes/testes/p07/respostas.modelo.json /tmp/p07.json
# edite /tmp/p07.json
python3 estudo/03_pipes/testes/p07/corrigir.py /tmp/p07.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/p07/gabarito.md) e [rubrica](testes/p07/rubrica.md) para concluir a correção manual.

---

## p08 — FIFO com dois escritores

**Origem:** Professor — Aula 14, pp.43–55, exemplos reader/writer e comunicação bidirecional

Explique dois pipes para ida/volta pai-filho e uma FIFO nomeada para processos independentes. Analise os writers que enviam três mensagens e o reader que lê até NUL. Descreva criação, abertura, permissões, limites do buffer, EOF e remoção do nome. Explique por que 0660 difere de 660.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/p08/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/03_pipes/testes/p08/respostas.modelo.json /tmp/p08.json
# edite /tmp/p08.json
python3 estudo/03_pipes/testes/p08/corrigir.py /tmp/p08.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/p08/gabarito.md) e [rubrica](testes/p08/rubrica.md) para concluir a correção manual.

---

## p09 — Filtro de soma por pipe

**Origem:** Criado para este estudo

Leia n (0<=n<=1000) e n inteiros entre -1000000 e 1000000. Pai envia tamanho e valores por pipe; filho soma em long long e devolve por outro pipe; pai imprime somente a soma. Não usar memória compartilhada.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/03_pipes/testes/p09/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/03_pipes/testes/p09/corrigir.py ./solucao
```

Casos: [casos.json](testes/p09/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/p09/rubrica.md).
