# Exercícios — 04_threads

Comandos abaixo são executados a partir da raiz `provaPratica`. As adaptações de entrada/saída servem ao treino; as exigências originais e questões discursivas continuam na rubrica. Nos casos de análise, os cenários são complementos didáticos criados para avaliar a compreensão do enunciado original.

## t01 — Criando threads e interpretando a saída

**Origem:** Professor — roteiro 4.1, A.1; Aula 17, exemplos 1–5

O roteiro pede compilar cria_threads com -pthread, explicar NULL em create/join, observar a saída e retirar comentários para comparar. O arquivo cria_threads não foi fornecido. Variante de treino: escreva duas threads com a mesma rotina, cada uma emitindo dez registros; compare com/sem pausas e responda (a) significado de NULL, (b) se saída prova paralelismo, (c) se a conclusão muda com pausas. Analise ainda os exemplos global 20→40 após join, x/o infinitos, cinco Hello e criação de nomes com malloc da Aula 17.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/t01/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/04_threads/testes/t01/respostas.modelo.json /tmp/t01.json
# edite /tmp/t01.json
python3 estudo/04_threads/testes/t01/corrigir.py /tmp/t01.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/t01/gabarito.md) e [rubrica](testes/t01/rubrica.md) para concluir a correção manual.

---

## t02 — Joining threads e saída da main

**Origem:** Professor — roteiro 4.1, A.2

O roteiro pede executar joining_thread, com myturn em newthread e yourturn na main, explicar a saída e depois comentar o join. O fonte não está incluído. Recrie uma variante com cinco iterações de cada rotina. Compare com/sem join e explique por que não há uma quantidade universal de linhas garantida na versão sem espera.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/t02/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/04_threads/testes/t02/respostas.modelo.json /tmp/t02.json
# edite /tmp/t02.json
python3 estudo/04_threads/testes/t02/corrigir.py /tmp/t02.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/t02/gabarito.md) e [rubrica](testes/t02/rubrica.md) para concluir a correção manual.

---

## t03 — Passagem de parâmetros

**Origem:** Professor — roteiro 4.1, B1

Explique a transformação de passing_parameter em relação à tarefa A.2: último argumento de create torna-se o endereço de uma variável. Fonte ausente. Variante: passe estrutura {inicio,fim} a uma rotina, explique void*, conversão e vida útil; crie antes de join e analise a saída sem presumir ordem.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/t03/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/04_threads/testes/t03/respostas.modelo.json /tmp/t03.json
# edite /tmp/t03.json
python3 estudo/04_threads/testes/t03/corrigir.py /tmp/t03.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/t03/gabarito.md) e [rubrica](testes/t03/rubrica.md) para concluir a correção manual.

---

## t04 — Primos: &i versus malloc

**Origem:** Professor — roteiro 4.1, B2 (a–g); roteiro 4.2, exercício 6, parte 2

Analise primos_malloc_v1 (fonte não fornecido): dez threads recebem índice por referência à variável do laço. (a) Simule criação/join; (b) explique impressão incorreta possível; (c) identifique causa; (d) explique efeito de sleep(1). Na v2 cada argumento tem malloc próprio: (e) compare saída; (f) explique a correção; (g) resuma por que v1 falha. Responda também às quatro perguntas de investigação do roteiro 4.2: resultados sempre corretos? repetição de valores? acesso inválido? diferença entre &i e objeto dinâmico?

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/t04/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/04_threads/testes/t04/respostas.modelo.json /tmp/t04.json
# edite /tmp/t04.json
python3 estudo/04_threads/testes/t04/corrigir.py /tmp/t04.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/t04/gabarito.md) e [rubrica](testes/t04/rubrica.md) para concluir a correção manual.

---

## t05 — Receber valores e exemplo do dado

**Origem:** Professor — roteiro 4.1, B3 (a–d); Aula 17, exemplo 6

Explique alocação do resultado na rotina, retorno void*, variável void* na main e join com endereço desse ponteiro. O roteiro pede compilar receiving_values, observar saída e analisar receiving_values_dice, mas fontes não estão em materiais. Variante: rotina retorna um inteiro alocado com valor 99; depois adapte para um dado 1–6, sem esperar sequência aleatória fixa.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/t05/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/04_threads/testes/t05/respostas.modelo.json /tmp/t05.json
# edite /tmp/t05.json
python3 estudo/04_threads/testes/t05/corrigir.py /tmp/t05.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/t05/gabarito.md) e [rubrica](testes/t05/rubrica.md) para concluir a correção manual.

---

## t06 — Soma dos dez primos em duas threads

**Origem:** Professor — roteiro 4.1, exercício final

Use o vetor fixo [2,3,5,7,11,13,17,19,23,29]. Primeira thread soma posições 0–4, segunda 5–9. Retorne parciais para main por retorno da thread e join; main soma e imprime. Contrato de treino: sem entrada, stdout somente 129.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/04_threads/testes/t06/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/04_threads/testes/t06/corrigir.py ./solucao
```

Casos: [casos.json](testes/t06/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/t06/rubrica.md).

---

## t07 — Criação e identificação de dez threads

**Origem:** Professor — roteiro 4.2, exercício 1 (a–c)

Crie dez threads, cada uma recebe uma posição de vetor de dez inteiros e imprime pthread_self() e valor. Main espera todas. Discuta (a) passar &i, (b) ordem de impressão, (c) papel do join. Adaptação testável: leia os dez valores; stdout tem dez linhas “indice valor”, índices 0–9, ordem livre; escreva identificação real pthread_self() em stderr com representação adequada ao sistema.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/04_threads/testes/t07/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/04_threads/testes/t07/corrigir.py ./solucao
```

Casos: [casos.json](testes/t07/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/t07/rubrica.md).

---

## t08 — Posição de pthread_join

**Origem:** Professor — roteiro 4.2, exercício 2 (d–f)

Compare a versão com todas as criações antes dos joins com a versão create/join no mesmo laço. Execute ambas. (d) Quantos trabalhadores podem estar ativos em cada versão? (e) Qual permite paralelismo? (f) Por que a segunda se parece com execução sequencial? Diferencie trabalhadores vivos, executáveis e efetivamente em CPU.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/t08/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/04_threads/testes/t08/respostas.modelo.json /tmp/t08.json
# edite /tmp/t08.json
python3 estudo/04_threads/testes/t08/corrigir.py /tmp/t08.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/t08/gabarito.md) e [rubrica](testes/t08/rubrica.md) para concluir a correção manual.

---

## t09 — Soma paralela de dez inteiros

**Origem:** Professor — roteiro 4.2, exercício 3 (g–i)

Leia dez inteiros. Duas threads somam metades de cinco, retornam parciais pelo retorno da rotina e main recupera por join. Imprima somente a soma em long long. Discuta (g) independência das partes, (h) coleta dos resultados, (i) consequência de não liberar memória. Valores entre -1000000000 e 1000000000.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/04_threads/testes/t09/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/04_threads/testes/t09/corrigir.py ./solucao
```

Casos: [casos.json](testes/t09/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/t09/rubrica.md).

---

## t10 — Impressão concorrente de primos

**Origem:** Professor — roteiro 4.2, exercício 6, parte 1

Crie dez threads, uma por primo, com parâmetros próprios e liberação adequada. Use [2,3,5,7,11,13,17,19,23,29]: o roteiro escreve [2,..,27], mas 27 não é primo. Sem entrada. Stdout: linhas “indice primo”, ordem livre; pthread_self() em stderr. Parte 2 investigativa está em t04.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/04_threads/testes/t10/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/04_threads/testes/t10/corrigir.py ./solucao
```

Casos: [casos.json](testes/t10/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/t10/rubrica.md).

---

## t11 — ULT, KLT e recursos compartilhados

**Origem:** Criado para este estudo — revisão integral da Aula 16

Compare M:1, 1:1 e M:N, papel do PCB/TCB, bloqueio, pilhas, heap, descritores, escalonamento, responsividade e economia. Use os exemplos editor, servidor e multiplicação de matrizes. Explique por que a API Pthreads não determina universalmente o modelo.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/t11/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/04_threads/testes/t11/respostas.modelo.json /tmp/t11.json
# edite /tmp/t11.json
python3 estudo/04_threads/testes/t11/corrigir.py /tmp/t11.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/t11/gabarito.md) e [rubrica](testes/t11/rubrica.md) para concluir a correção manual.

---

## t12 — Quadrados retornados por threads

**Origem:** Criado para este estudo

Leia quatro inteiros entre -10000 e 10000. Crie quatro threads, cada uma retorna o quadrado de seu valor em memória com vida útil válida. Após os joins, main imprime os quatro quadrados na ordem original. Não precisa mutex para resultados independentes.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/04_threads/testes/t12/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/04_threads/testes/t12/corrigir.py ./solucao
```

Casos: [casos.json](testes/t12/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/t12/rubrica.md).
