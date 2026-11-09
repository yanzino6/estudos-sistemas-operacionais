# Prova prática — 04_threads

Todas as questões pedem **programas C**. Os antigos pedidos de explicação/pseudocódigo foram convertidos em implementações. Os contratos finitos de entrada, saída e encerramento são adaptações de treino. Semáforos: use o adaptador descrito em [guia de modelos](../MODELOS_DE_RESPOSTA.md).

## t01 — Duas threads com a mesma rotina

**Origem:** Adaptação para programação a partir de: Professor — roteiro 4.1, A.1; Aula 17, exemplos 1–5

**Arquivo para escrever sua resposta:** [respostas/t01.c](respostas/t01.c).

### Implemente

Leia k (1<=k<=100). Crie duas threads com uma mesma função, argumentos id=0 e id=1. Cada uma imprime `id passo` para passo=0..k-1. Espere ambas. Ordem de linhas livre.

**Requisitos obrigatórios:** Duas criações antes dos joins; argumentos distintos, uma linha por printf e sem duplicações.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t01/01.in) · [saída de exemplo](testes/t01/01.out). [Todos os testes e regras](testes/t01/README.md).

O corretor compara o multiconjunto de linhas, aceitando qualquer ordem, mas preservando a quantidade de repetições.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t01/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t01/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t01.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t01/rubrica.md).

---

## t02 — Garantir término antes de sair da main

**Origem:** Adaptação para programação a partir de: Professor — roteiro 4.1, A.2

**Arquivo para escrever sua resposta:** [respostas/t02.c](respostas/t02.c).

### Implemente

Leia k (1<=k<=100). Crie uma thread trabalhadora que imprime `TRABALHO i` para i=0..k-1. Main só imprime `FIM` depois de todas essas linhas, usando pthread_join.

**Requisitos obrigatórios:** Trabalho executado na thread; FIM na main após join. Não usar sleep para esperar.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t02/01.in) · [saída de exemplo](testes/t02/01.out). [Todos os testes e regras](testes/t02/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t02/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t02/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t02.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t02/rubrica.md).

---

## t03 — Argumentos por estrutura

**Origem:** Adaptação para programação a partir de: Professor — roteiro 4.1, B1

**Arquivo para escrever sua resposta:** [respostas/t03.c](respostas/t03.c).

### Implemente

Leia quatro pares inicio,fim, com 0<=inicio<=fim<=1000. Crie quatro threads, cada uma recebe uma estrutura com id e intervalo [inicio,fim). Ela soma os inteiros desse intervalo e imprime `id soma`, ordem livre.

**Requisitos obrigatórios:** Quatro objetos distintos, preenchidos antes de create e vivos até join. Função recebe void* e converte ao tipo da estrutura.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t03/01.in) · [saída de exemplo](testes/t03/01.out). [Todos os testes e regras](testes/t03/README.md).

O corretor compara o multiconjunto de linhas, aceitando qualquer ordem, mas preservando a quantidade de repetições.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t03/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t03/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t03.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t03/rubrica.md).

---

## t04 — Corrigir passagem de &i

**Origem:** Adaptação para programação a partir de: Professor — roteiro 4.1, B2 (a–g); roteiro 4.2, exercício 6, parte 2

**Arquivo para escrever sua resposta:** [respostas/t04.c](respostas/t04.c).

### Implemente

Leia dez inteiros. Crie dez threads, cada uma imprime `indice valor`, ordem livre. Cada argumento deve ser alocado individualmente com malloc, preenchido antes de create e liberado pela thread após copiar seus dados. Use índice 0..9.

**Requisitos obrigatórios:** Não passar endereço da variável de controle; dez mallocs distintos, tratar falhas e fazer todos os joins.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t04/01.in) · [saída de exemplo](testes/t04/01.out). [Todos os testes e regras](testes/t04/README.md).

O corretor compara o multiconjunto de linhas, aceitando qualquer ordem, mas preservando a quantidade de repetições.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t04/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t04/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t04.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t04/rubrica.md).

---

## t05 — Retorno de dados alocados pela thread

**Origem:** Adaptação para programação a partir de: Professor — roteiro 4.1, B3 (a–d); Aula 17, exemplo 6

**Arquivo para escrever sua resposta:** [respostas/t05.c](respostas/t05.c).

### Implemente

Leia quatro inteiros x entre 0 e 1000000. Cada uma de quatro threads calcula um lançamento determinístico `1 + x % 6`, aloca o resultado e retorna o ponteiro. Main recolhe por join e imprime os quatro resultados na ordem original, liberando cada alocação.

**Requisitos obrigatórios:** Resultado alocado dentro da rotina; receber com void** no join; não retornar endereço de variável automática nem compartilhar uma única variável de retorno.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t05/01.in) · [saída de exemplo](testes/t05/01.out). [Todos os testes e regras](testes/t05/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t05/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t05/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t05.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t05/rubrica.md).

---

## t06 — Soma dos dez primos em duas threads

**Origem:** Professor — roteiro 4.1, exercício final

**Arquivo para escrever sua resposta:** [respostas/t06.c](respostas/t06.c).

### Implemente

Sem entrada. Use [2,3,5,7,11,13,17,19,23,29]. Primeira thread soma posições 0..4 e segunda 5..9. Retorne parciais por ponteiros recolhidos com join; main imprime soma total.

**Requisitos obrigatórios:** Criar duas antes dos joins; retornar objetos vivos e liberar resultados.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t06/01.in) · [saída de exemplo](testes/t06/01.out). [Todos os testes e regras](testes/t06/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t06/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t06/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t06.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t06/rubrica.md).

---

## t07 — Dez threads identificadas

**Origem:** Professor — roteiro 4.2, exercício 1 (a–c)

**Arquivo para escrever sua resposta:** [respostas/t07.c](respostas/t07.c).

### Implemente

Leia dez inteiros. Crie uma thread por elemento, que imprime `indice valor` em stdout, ordem livre. Imprima também pthread_self() em stderr numa representação adequada à plataforma.

**Requisitos obrigatórios:** Índices 0..9, argumentos independentes, todos os joins.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t07/01.in) · [saída de exemplo](testes/t07/01.out). [Todos os testes e regras](testes/t07/README.md).

O corretor compara o multiconjunto de linhas, aceitando qualquer ordem, mas preservando a quantidade de repetições.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t07/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t07/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t07.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t07/rubrica.md).

---

## t08 — Dois modos de criação e join

**Origem:** Adaptação para programação a partir de: Professor — roteiro 4.2, exercício 2 (d–f)

**Arquivo para escrever sua resposta:** [respostas/t08.c](respostas/t08.c).

### Implemente

Leia modo,n (modo 0 ou 1,1<=n<=16). Modo 0: para cada id, crie e faça join imediatamente; trabalhador imprime INICIO id e FIM id. Modo 1: crie todos antes dos joins; cada trabalhador imprime CHEGA 0 id, aguarda uma barreira de n participantes e imprime PASSA 0 id. A barreira torna verificável a coexistência dos trabalhadores sem medir tempo.

**Requisitos obrigatórios:** No modo 0 no máximo um trabalhador do laço vivo. No modo 1 use mutex/condição para barreira; não faça join antes de criar todos.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t08/01.in) · [saída de exemplo](testes/t08/01.out). [Todos os testes e regras](testes/t08/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t08/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t08/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t08.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t08/rubrica.md).

---

## t09 — Soma paralela de dez inteiros

**Origem:** Professor — roteiro 4.2, exercício 3 (g–i)

**Arquivo para escrever sua resposta:** [respostas/t09.c](respostas/t09.c).

### Implemente

Leia dez inteiros entre -1000000000 e 1000000000. Duas threads somam cinco elementos cada; retornam parciais via retorno da rotina. Main recupera com join, soma e imprime long long.

**Requisitos obrigatórios:** Não retornar endereço local nem substituir retorno por variável global; liberar heap.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t09/01.in) · [saída de exemplo](testes/t09/01.out). [Todos os testes e regras](testes/t09/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t09/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t09/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t09.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t09/rubrica.md).

---

## t10 — Impressão concorrente de primos

**Origem:** Professor — roteiro 4.2, exercício 6, parte 1

**Arquivo para escrever sua resposta:** [respostas/t10.c](respostas/t10.c).

### Implemente

Sem entrada. Crie dez threads para [2,3,5,7,11,13,17,19,23,29]. Stdout tem `indice primo`, ordem livre; pthread_self() em stderr.

**Requisitos obrigatórios:** Parâmetro próprio por thread, liberação e join em todas; 27 não é primo.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t10/01.in) · [saída de exemplo](testes/t10/01.out). [Todos os testes e regras](testes/t10/README.md).

O corretor compara o multiconjunto de linhas, aceitando qualquer ordem, mas preservando a quantidade de repetições.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t10/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t10/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t10.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t10/rubrica.md).

---

## t11 — Partilhar heap entre trabalhadores

**Origem:** Adaptação para programação a partir de: Criado para este estudo — revisão integral da Aula 16

**Arquivo para escrever sua resposta:** [respostas/t11.c](respostas/t11.c).

### Implemente

Leia n (0<=n<=1000) e n inteiros entre -10000 e 10000. Aloque vetor no heap. Crie duas threads: a primeira dobra elementos de índice par; a segunda dobra elementos de índice ímpar. Após ambos os joins, main imprime o vetor transformado na ordem original. Cada posição tem um único escritor.

**Requisitos obrigatórios:** Compartilhar o mesmo vetor alocado; não copiar todo o vetor para cada thread; main só lê depois dos joins. Sem mutex necessário para posições distintas.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t11/01.in) · [saída de exemplo](testes/t11/01.out). [Todos os testes e regras](testes/t11/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t11/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t11/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t11.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t11/rubrica.md).

---

## t12 — Quadrados retornados por threads

**Origem:** Criado para este estudo

**Arquivo para escrever sua resposta:** [respostas/t12.c](respostas/t12.c).

### Implemente

Leia quatro inteiros entre -10000 e 10000. Crie quatro threads, cada uma retorna o quadrado de seu valor em objeto de vida útil válida. Main recolhe e imprime os quatro quadrados na ordem original.

**Requisitos obrigatórios:** Quatro creates antes dos joins; recuperar resultados por join, liberar alocações.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/t12/01.in) · [saída de exemplo](testes/t12/01.out). [Todos os testes e regras](testes/t12/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/04_threads/testes/t12/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/04_threads/testes/t12/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/t12.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/t12/rubrica.md).
