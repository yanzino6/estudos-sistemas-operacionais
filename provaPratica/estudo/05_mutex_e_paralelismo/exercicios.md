# Exercícios — 05_mutex_e_paralelismo

Comandos abaixo são executados a partir da raiz `provaPratica`. As adaptações de entrada/saída servem ao treino; as exigências originais e questões discursivas continuam na rubrica. Nos casos de análise, os cenários são complementos didáticos criados para avaliar a compreensão do enunciado original.

## x01 — Investigar índice global sem proteção

**Origem:** Professor — roteiro 4.2, exercício 4 (j–l); Aula 17, pp.21–23

Crie duas threads que somam partes de um vetor usando um índice global compartilhado e gravam parciais em vetor global. Execute várias vezes. Responda (j) risco de corrida, (k) quando mutex é necessário, (l) como garantir consistência. A versão propositalmente incorreta não tem saída errada obrigatória; por isso estes testes avaliam intercalamentos e a correção exige inspecionar o código.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/x01/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/05_mutex_e_paralelismo/testes/x01/respostas.modelo.json /tmp/x01.json
# edite /tmp/x01.json
python3 estudo/05_mutex_e_paralelismo/testes/x01/corrigir.py /tmp/x01.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/x01/gabarito.md) e [rubrica](testes/x01/rubrica.md) para concluir a correção manual.

---

## x02 — Corrigir índice global com mutex

**Origem:** Professor — roteiro 4.2, exercício 5 (m–o)

Corrija o exercício 4 usando mutex_init/lock/unlock/destroy. A referência do PDF ao próprio exercício 5 é uma autorreferência; aqui usamos o anterior. Contrato de treino: leia dez inteiros entre -1000000000 e 1000000000; duas threads reservam índices de um índice global protegido, acumulam parciais e main imprime soma. Responda (m) determinismo, (n) impacto no desempenho, (o) custo de sincronização.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/05_mutex_e_paralelismo/testes/x02/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/05_mutex_e_paralelismo/testes/x02/corrigir.py ./solucao
```

Casos: [casos.json](testes/x02/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/x02/rubrica.md).

---

## x03 — Soma de 1000 elementos com N threads

**Origem:** Professor — roteiro 4.2, exercício 7; Aula 17, exercício de soma, pp.21–23

Vetor de exatamente 1000 inteiros e número t de threads. Divida em partes aproximadamente iguais, parâmetros alocados dinamicamente, retorno de parciais por join e liberação. Compare tempos para 1,2,4,8 threads em relatório. Contrato: primeira entrada t (1<=t<=1000), seguida por 1000 inteiros entre -1000000 e 1000000; stdout somente soma long long. Entrada t fora do intervalo: exit 1, stdout vazio. Também explique a variante de índice global da aula, comparando-a à divisão estática.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 10 segundos.

```bash
python3 estudo/05_mutex_e_paralelismo/testes/x03/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/05_mutex_e_paralelismo/testes/x03/corrigir.py ./solucao
```

Casos: [casos.json](testes/x03/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/x03/rubrica.md).

---

## x04 — Partição variável e casos de fronteira

**Origem:** Criado para este estudo

Leia n,t (0<=n<=1000,1<=t<=32), seguidos por n inteiros entre -1000000 e 1000000. Distribua intervalos aproximadamente iguais entre t threads, permitindo intervalos vazios. Imprima soma. Rejeite n/t fora do contrato com exit 1 e stdout vazio.

### Como testar e corrigir

Implemente `solucao.c` conforme o contrato abaixo. O script compila com `cc -std=c11 -Wall -Wextra -pthread`, executa os casos em diretório temporário e limita cada execução a 5 segundos.

```bash
python3 estudo/05_mutex_e_paralelismo/testes/x04/corrigir.py /caminho/solucao.c
# ou compile e passe o executável:
cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread solucao.c -o solucao
python3 estudo/05_mutex_e_paralelismo/testes/x04/corrigir.py ./solucao
```

Casos: [casos.json](testes/x04/casos.json), arquivos `.in`/`.out` ao lado. Saída sem prompts extras; espaços e quebras são normalizados. Confira também [rubrica.md](testes/x04/rubrica.md).

---

## x05 — Desempenho, atributos e revisão de mutex

**Origem:** Criado para este estudo — revisão da Aula 17

Explique init/destroy, trylock, tipos normal/recursivo/error-checking, robustez, compartilhamento entre processos, contenção, acumulação local e false sharing. Calcule um limite ideal de aceleração e planeje a comparação de 1/2/4/8 trabalhadores.

### Como testar e corrigir

Escreva a resolução e o pseudocódigo pedido em `resposta.md`. Leia [os cenários](testes/x05/cenarios.md), copie `respostas.modelo.json` para um arquivo seu e substitua cada `null` pela resposta pedida (booleanos JSON são `true`/`false`).

```bash
cp estudo/05_mutex_e_paralelismo/testes/x05/respostas.modelo.json /tmp/x05.json
# edite /tmp/x05.json
python3 estudo/05_mutex_e_paralelismo/testes/x05/corrigir.py /tmp/x05.json
```

O script corrige as previsões dos cenários, **não o texto nem o pseudocódigo**. Depois da tentativa, use [gabarito](testes/x05/gabarito.md) e [rubrica](testes/x05/rubrica.md) para concluir a correção manual.
