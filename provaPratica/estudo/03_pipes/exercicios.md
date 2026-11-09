# Prova prática — 03_pipes

Todas as questões pedem **programas C**. Os antigos pedidos de explicação/pseudocódigo foram convertidos em implementações. Os contratos finitos de entrada, saída e encerramento são adaptações de treino. Semáforos: use o adaptador descrito em [guia de modelos](../MODELOS_DE_RESPOSTA.md).

## p01 — Anel pai e dois filhos

**Origem:** Professor — roteiro_lab03, exercício 1, p.1

**Arquivo para escrever sua resposta:** [respostas/p01.c](respostas/p01.c).

### Implemente

Leia um inteiro x entre -1000000 e 1000000. Pai envia ao filho 1, que soma 5 e envia ao filho 2; este soma 10 e devolve ao pai. Imprima `[PARENT] Resultado Final: valor!`.

**Requisitos obrigatórios:** Dois filhos irmãos, três pipes, read/write, fechamentos e waitpid; operações nos processos pedidos.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/p01/01.in) · [saída de exemplo](testes/p01/01.out). [Todos os testes e regras](testes/p01/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/03_pipes/testes/p01/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/03_pipes/testes/p01/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/p01.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/p01/rubrica.md).

---

## p02 — Pipeline ping | grep

**Origem:** Professor — roteiro_lab03, exercício 2, pp.2–3

**Arquivo para escrever sua resposta:** [respostas/p02.c](respostas/p02.c).

### Implemente

Implemente `ping -c 5 google.com | grep rtt` com dois filhos, pipe, dup2 e execlp. Sem entrada; stdout contém apenas linhas selecionadas. Main retorna zero após recolher ambos, inclusive se grep não selecionar linhas. Os testes fornecem ping controlado no PATH, mantendo grep real.

**Requisitos obrigatórios:** Sem system/popen; argumentos corretos, ambos criados antes da espera; fechar pontas. Não usar caminho absoluto para ping.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/p02/01.in) · [saída de exemplo](testes/p02/01.out). [Todos os testes e regras](testes/p02/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/03_pipes/testes/p02/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/03_pipes/testes/p02/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/p02.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/p02/rubrica.md).

---

## p03 — Avô, pai e neto

**Origem:** Professor — roteiro_lab03, exercício 3, pp.3–4

**Arquivo para escrever sua resposta:** [respostas/p03.c](respostas/p03.c).

### Implemente

Leia n e n inteiros (1<=n<=100, valores entre -1000000 e 1000000). Avô envia ao filho; filho multiplica por três e envia ao neto; neto inverte e devolve ao filho, que devolve ao avô. Saída: `[GRANDPARENT] Vetor final:` seguida do vetor final.

**Requisitos obrigatórios:** Neto criado pelo filho, quatro canais direcionais ou protocolo equivalente; operações nos processos especificados; waitpid na hierarquia.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/p03/01.in) · [saída de exemplo](testes/p03/01.out). [Todos os testes e regras](testes/p03/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/03_pipes/testes/p03/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/03_pipes/testes/p03/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/p03.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/p03/rubrica.md).

---

## p04 — Ler um pipe até EOF

**Origem:** Adaptação para programação a partir de: Professor — Aula 14, pp.24–27; cenários controlados de treino

**Arquivo para escrever sua resposta:** [respostas/p04.c](respostas/p04.c).

### Implemente

Leia n (0<=n<=200000). Crie um filho que escreve n bytes x no pipe e fecha sua escrita. Pai fecha sua cópia de escrita, lê até read retornar zero e imprime `BYTES n`. Filho deve escrever em blocos e tratar retornos parciais; pai não pode depender de conhecer n para encerrar a leitura.

**Requisitos obrigatórios:** Fechar pontas corretamente e esperar filho. Sem ponta de escrita esquecida. read()==0 é critério de término.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/p04/01.in) · [saída de exemplo](testes/p04/01.out). [Todos os testes e regras](testes/p04/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/03_pipes/testes/p04/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/03_pipes/testes/p04/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/p04.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/p04/rubrica.md).

---

## p05 — Dois filhos enviando registros

**Origem:** Adaptação para programação a partir de: Professor — Aula 14, exemplo double.c/son.c, pp.33–36

**Arquivo para escrever sua resposta:** [respostas/p05.c](respostas/p05.c).

### Implemente

Leia k (1<=k<=100). Crie dois filhos irmãos; cada um escreve k registros no mesmo pipe. Registro tem origem 0 ou 1 e sequência 0..k-1. Pai lê, recompõe os registros e imprime `origem sequencia` por registro, em qualquer ordem. Use struct de dois ints e um write por registro (menor que PIPE_BUF), tratando erros.

**Requisitos obrigatórios:** Pai não fabrica registros; filhos compartilham escrita do pipe e pai lê até EOF. Reconstruir mensagens mesmo com leitura parcial; fechar pontas e waitpid em ambos.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/p05/01.in) · [saída de exemplo](testes/p05/01.out). [Todos os testes e regras](testes/p05/README.md).

O corretor compara o multiconjunto de linhas, aceitando qualquer ordem, mas preservando a quantidade de repetições.

Da raiz `provaPratica`:

```bash
python3 estudo/03_pipes/testes/p05/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/03_pipes/testes/p05/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/p05.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/p05/rubrica.md).

---

## p06 — Pipeline de filtro e contagem

**Origem:** Adaptação para programação a partir de: Professor — Aula 14, exemplo 3, pp.37–41

**Arquivo para escrever sua resposta:** [respostas/p06.c](respostas/p06.c).

### Implemente

Leia n (0<=n<=100), seguido de n linhas de até 100 caracteres. Monte dois filhos e dois pipes: main fornece as linhas a `grep SO`; a saída do grep alimenta `wc -l`. Os programas são executados por execlp e conectados por dup2. Stdout é somente a quantidade de linhas que contêm a substring maiúscula SO. Main recolhe ambos e retorna zero, inclusive quando grep não seleciona linhas. Esta é a versão determinística de treino do exemplo ls | wc.

**Requisitos obrigatórios:** Criar ambos antes de enviar/esperar; não usar system ou popen; não contar as linhas na main; fechar todas as escritas para propagar EOF.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/p06/01.in) · [saída de exemplo](testes/p06/01.out). [Todos os testes e regras](testes/p06/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/03_pipes/testes/p06/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/03_pipes/testes/p06/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/p06.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/p06/rubrica.md).

---

## p07 — Pipe com escrita não bloqueante

**Origem:** Adaptação para programação a partir de: Professor — Aula 14, exemplo 4, p.42

**Arquivo para escrever sua resposta:** [respostas/p07.c](respostas/p07.c).

### Implemente

Leia n (1<=n<=1000000). Filho escreve n bytes x, em blocos, numa ponta marcada O_NONBLOCK via fcntl. Pai drena o pipe até EOF e acumula número de bytes e soma dos valores dos bytes. Imprima `n soma`, onde x possui valor 120. Ao receber EAGAIN, filho deve aguardar disponibilidade via poll e continuar; tratar também EINTR e writes parciais.

**Requisitos obrigatórios:** Não fixar capacidade do pipe nem tratar EAGAIN como perda de dados. Não usar alarm/printf em handler. Resultado deve vir dos bytes recebidos, não de n*120 na main.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/p07/01.in) · [saída de exemplo](testes/p07/01.out). [Todos os testes e regras](testes/p07/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/03_pipes/testes/p07/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/03_pipes/testes/p07/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/p07.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/p07/rubrica.md).

---

## p08 — FIFO com dois escritores e limpeza

**Origem:** Adaptação para programação a partir de: Professor — Aula 14, pp.43–55, exemplos reader/writer e comunicação bidirecional

**Arquivo para escrever sua resposta:** [respostas/p08.c](respostas/p08.c).

### Implemente

Leia k (1<=k<=100). Crie uma FIFO chamada canal.fifo no diretório de execução e dois filhos escritores. Cada filho envia k registros {origem,sequencia}; pai lê 2*k registros e imprime `origem sequencia`, ordem livre. Use protocolo para que ambos abram antes de começar a escrita e para não confundir o intervalo entre aberturas com fim definitivo. Feche descritores, recolha filhos e remova a FIFO antes de terminar.

**Requisitos obrigatórios:** mkfifo com permissões octais, open/read/write/close/unlink. Registros pequenos com escrita atômica e leitura recomposta. Coordenação de abertura, por exemplo canal auxiliar de prontidão, sem sleep. O corretor também verifica que canal.fifo não permaneceu.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/p08/01.in) · [saída de exemplo](testes/p08/01.out). [Todos os testes e regras](testes/p08/README.md).

O corretor compara o multiconjunto de linhas, aceitando qualquer ordem, mas preservando a quantidade de repetições.

Da raiz `provaPratica`:

```bash
python3 estudo/03_pipes/testes/p08/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/03_pipes/testes/p08/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/p08.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/p08/rubrica.md).

---

## p09 — Soma pelo processo filho

**Origem:** Criado para este estudo

**Arquivo para escrever sua resposta:** [respostas/p09.c](respostas/p09.c).

### Implemente

Leia n e n inteiros (0<=n<=1000, valores entre -1000000 e 1000000). Pai envia por pipe; filho soma em long long e devolve por outro pipe; pai imprime só soma.

**Requisitos obrigatórios:** Transferências completas, n=0, fechamento e waitpid; soma no filho.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/p09/01.in) · [saída de exemplo](testes/p09/01.out). [Todos os testes e regras](testes/p09/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/03_pipes/testes/p09/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/03_pipes/testes/p09/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/p09.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/p09/rubrica.md).
