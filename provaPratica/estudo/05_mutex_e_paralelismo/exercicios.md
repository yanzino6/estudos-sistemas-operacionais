# Prova prática — 05_mutex_e_paralelismo

Todas as questões pedem **programas C**. Os antigos pedidos de explicação/pseudocódigo foram convertidos em implementações. Os contratos finitos de entrada, saída e encerramento são adaptações de treino. Semáforos: use o adaptador descrito em [guia de modelos](../MODELOS_DE_RESPOSTA.md).

## x01 — Distribuição dinâmica com índice global seguro

**Origem:** Adaptação para programação a partir de: Professor — roteiro 4.2, exercício 4 (j–l); Aula 17, pp.21–23

**Arquivo para escrever sua resposta:** [respostas/x01.c](respostas/x01.c).

### Implemente

Leia n (0<=n<=1000) e n inteiros entre -1000000 e 1000000. Crie quatro threads; elas reservam o próximo índice usando variável global protegida por mutex. Cada reserva imprime `indice valor`, ordem livre. Cada posição deve ser processada exatamente uma vez.

**Requisitos obrigatórios:** Teste de limite e incremento na mesma região crítica. Cada trabalhador usa cópia local do índice. Não usar divisão estática: o objetivo é implementar corretamente a reserva global.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/x01/01.in) · [saída de exemplo](testes/x01/01.out). [Todos os testes e regras](testes/x01/README.md).

O corretor compara o multiconjunto de linhas, aceitando qualquer ordem, mas preservando a quantidade de repetições.

Da raiz `provaPratica`:

```bash
python3 estudo/05_mutex_e_paralelismo/testes/x01/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/05_mutex_e_paralelismo/testes/x01/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/x01.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/x01/rubrica.md).

---

## x02 — Soma com reserva de índice protegida

**Origem:** Professor — roteiro 4.2, exercício 5 (m–o)

**Arquivo para escrever sua resposta:** [respostas/x02.c](respostas/x02.c).

### Implemente

Leia dez inteiros entre -1000000000 e 1000000000. Duas threads retiram índices de um índice global protegido por mutex e somam parciais. Main imprime o total long long após joins.

**Requisitos obrigatórios:** mutex_init/lock/unlock/destroy; decisão de limite e reserva indivisíveis; não serializar create/join.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/x02/01.in) · [saída de exemplo](testes/x02/01.out). [Todos os testes e regras](testes/x02/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/05_mutex_e_paralelismo/testes/x02/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/05_mutex_e_paralelismo/testes/x02/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/x02.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/x02/rubrica.md).

---

## x03 — Soma de mil elementos com N threads

**Origem:** Professor — roteiro 4.2, exercício 7; Aula 17, exercício de soma, pp.21–23

**Arquivo para escrever sua resposta:** [respostas/x03.c](respostas/x03.c).

### Implemente

Leia t (1<=t<=1000), seguido de exatamente 1000 inteiros entre -1000000 e 1000000. Divida em intervalos de tamanhos que diferem no máximo em um. Use argumentos alocados dinamicamente e colete parciais com join. Imprima soma long long. t inválido: stdout vazio, retorno 1.

**Requisitos obrigatórios:** Cobrir todas as posições sem sobreposição, tratar resto, liberar argumentos/resultados, criar todos antes dos joins.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/x03/01.in) · [saída de exemplo](testes/x03/01.out). [Todos os testes e regras](testes/x03/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/05_mutex_e_paralelismo/testes/x03/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/05_mutex_e_paralelismo/testes/x03/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/x03.c`. Tempo máximo por execução: 10 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/x03/rubrica.md).

---

## x04 — Partição com vetor de tamanho variável

**Origem:** Criado para este estudo

**Arquivo para escrever sua resposta:** [respostas/x04.c](respostas/x04.c).

### Implemente

Leia n,t (0<=n<=1000,1<=t<=32) e n inteiros entre -1000000 e 1000000. Divida equilibradamente entre t threads, permitindo intervalos vazios; imprima soma long long. n/t fora do contrato: stdout vazio, retorno 1.

**Requisitos obrigatórios:** Tratar n=0, t>n e restos, não dividir antes de validar.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/x04/01.in) · [saída de exemplo](testes/x04/01.out). [Todos os testes e regras](testes/x04/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/05_mutex_e_paralelismo/testes/x04/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/05_mutex_e_paralelismo/testes/x04/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/x04.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/x04/rubrica.md).

---

## x05 — Trylock e sincronização sem sleep

**Origem:** Adaptação para programação a partir de: Criado para este estudo — revisão da Aula 17

**Arquivo para escrever sua resposta:** [respostas/x05.c](respostas/x05.c).

### Implemente

Sem entrada. Main inicializa e adquire um mutex de recurso antes de criar uma thread. Trabalhadora tenta pthread_mutex_trylock, confirma EBUSY e imprime OCUPADO. Em seguida avisa main por uma condição protegida por outro mutex. Main só então libera o recurso. Trabalhadora faz lock normal, imprime ADQUIRIDO, libera e termina. Main faz join e destrói os objetos.

**Requisitos obrigatórios:** Handshake com mutex de controle distinto do mutex de recurso. Verificar EBUSY, não confundir erro com posse. A thread que adquire o mutex é quem o libera.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/x05/01.in) · [saída de exemplo](testes/x05/01.out). [Todos os testes e regras](testes/x05/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/05_mutex_e_paralelismo/testes/x05/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/05_mutex_e_paralelismo/testes/x05/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/x05.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/x05/rubrica.md).
