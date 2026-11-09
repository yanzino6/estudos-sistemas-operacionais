# Prova prática — 02_monitores

Todas as questões pedem **programas C**. Os antigos pedidos de explicação/pseudocódigo foram convertidos em implementações. Os contratos finitos de entrada, saída e encerramento são adaptações de treino. Semáforos: use o adaptador descrito em [guia de modelos](../MODELOS_DE_RESPOSTA.md).

## m01 — Corrigir buffer para vários produtores e consumidores

**Origem:** Adaptação para programação a partir de: Professor — Lista 7, questão 1; Aula 12, p.13

**Arquivo para escrever sua resposta:** [respostas/m01.c](respostas/m01.c).

### Implemente

Leia N,P,C,k (1<=N<=8,1<=P,C<=8,1<=k<=20). Crie P produtores: produtor i insere os valores i*k até (i+1)*k-1. Crie C consumidores e um buffer circular de N posições. Cada item é retirado exatamente uma vez; consumidores imprimem `ITEM valor`. Ordem livre. Use sentinelas -1, uma por consumidor, publicadas pela main somente após joins dos produtores; sentinelas não são impressas. Faça join de todos e termine.

**Requisitos obrigatórios:** Use mutex e duas condições; espere em while, nunca if. Cada rotina do monitor realiza uma operação por chamada. Não substituir por um vetor pronto de resultados. Testes cobrem buffer de uma posição, vários produtores/consumidores e totais não divisíveis por C.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/m01/01.in) · [saída de exemplo](testes/m01/01.out). [Todos os testes e regras](testes/m01/README.md).

O corretor compara o multiconjunto de linhas, aceitando qualquer ordem, mas preservando a quantidade de repetições.

Da raiz `provaPratica`:

```bash
python3 estudo/02_monitores/testes/m01/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/02_monitores/testes/m01/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/m01.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/m01/rubrica.md).

---

## m02 — Porto com prioridade de saída em C

**Origem:** Adaptação para programação a partir de: Professor — Lista 7, questão 2

**Arquivo para escrever sua resposta:** [respostas/m02.c](respostas/m02.c).

### Implemente

Leia N,n (1<=N<=n<=16), capacidade do porto e número de navios. Crie uma thread por navio id=0..n-1. Cada um entra e sai uma vez. Implemente pedido_entrada, entrada_OK, pedido_saida e saida_OK usando monitor. Eventos: PE id registra pedido de entrada; EN id concede entrada e reserva vaga/canal; AT id confirma atracação e libera canal; PS id registra pedido de saída; SA id concede saída e ocupa canal; FI id conclui saída e libera canal/vaga. Imprima cada evento dentro da proteção que altera o estado.

**Requisitos obrigatórios:** Apenas um navio no canal; ocupação inclui reservas de entrada; EN não pode ocorrer enquanto há PS ainda não atendido por SA. Um pedido não interrompe travessia já concedida. Use while nos predicados e termine todas as threads.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/m02/01.in) · [saída de exemplo](testes/m02/01.out). [Todos os testes e regras](testes/m02/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/02_monitores/testes/m02/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/02_monitores/testes/m02/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/m02.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/m02/rubrica.md).

---

## m03 — Buffer com monitor Hoare implementado por semáforos

**Origem:** Adaptação para programação a partir de: Professor — Lista 7, questão 3; Aula 12, pp.24–26

**Arquivo para escrever sua resposta:** [respostas/m03.c](respostas/m03.c).

### Implemente

Leia N,P,C,k (1<=N<=8,1<=P,C<=8,1<=k<=20). Crie P produtores: produtor i insere os valores i*k até (i+1)*k-1. Crie C consumidores e um buffer circular de N posições. Cada item é retirado exatamente uma vez; consumidores imprimem `ITEM valor`. Ordem livre. Use sentinelas -1, uma por consumidor, publicadas pela main somente após joins dos produtores; sentinelas não são impressas. Faça join de todos e termine.

**Requisitos obrigatórios:** Implemente entrada/saída, wait e signal de monitor Hoare usando mutex semafórico, fila urgente next e fila por condição; não use pthread_cond_t. Acordado recebe monitor antes do sinalizador continuar. Não substituir por um vetor pronto de resultados. Testes cobrem buffer de uma posição, vários produtores/consumidores e totais não divisíveis por C.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/m03/01.in) · [saída de exemplo](testes/m03/01.out). [Todos os testes e regras](testes/m03/README.md).

O corretor compara o multiconjunto de linhas, aceitando qualquer ordem, mas preservando a quantidade de repetições.

Da raiz `provaPratica`:

```bash
python3 estudo/02_monitores/testes/m03/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/02_monitores/testes/m03/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/m03.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/m03/rubrica.md).

---

## m04 — Barbearia encapsulada em monitor

**Origem:** Adaptação para programação a partir de: Professor — Lista 7, questão 4

**Arquivo para escrever sua resposta:** [respostas/m04.c](respostas/m04.c).

### Implemente

Leia q (1<=q<=100), depois q comandos `A id` (chegada de cliente) ou `T` (autoriza concluir um corte). Ids de clientes são distintos e não negativos. Crie uma thread barbeiro e uma thread despachante (pode ser a main). A despachante só envia o próximo comando após confirmação do anterior. A chegada ocupa a cadeira livre (`ATENDE id`), entra na fila FIFO de cinco lugares (`ESPERA id`) ou é recusada (`RECUSA id`). T sem cliente imprime `LIVRE`; T com cliente imprime `FIM id` seguido de `ATENDE proximo` ou `LIVRE`. Os testes encerram sem clientes pendentes. Esse relógio de comandos torna reproduzível o atendimento concorrente.

**Requisitos obrigatórios:** Implemente fila/cadeira usando mutex e variáveis de condição. O barbeiro executa as conclusões de corte; despachante envia comandos e aguarda confirmação sem busy wait. Não simular toda a barbearia sequencialmente na main.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/m04/01.in) · [saída de exemplo](testes/m04/01.out). [Todos os testes e regras](testes/m04/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/02_monitores/testes/m04/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/02_monitores/testes/m04/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/m04.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/m04/rubrica.md).

---

## m05 — Cinco filósofos usando monitor

**Origem:** Adaptação para programação a partir de: Professor — Aula 12, pp.20–23

**Arquivo para escrever sua resposta:** [respostas/m05.c](respostas/m05.c).

### Implemente

Leia k (1<=k<=8). Crie cinco filósofos; cada um come k vezes. Ids 0..4 formam ciclo; 0 e 4 são vizinhos. Registre `IN id` após adquirir os recursos e `OUT id` antes de liberá-los. Todos devem terminar. O corretor verifica contagem, aquisições/liberações e vizinhos nunca ativos simultaneamente.

**Requisitos obrigatórios:** Use estados e uma condição por filósofo, sob monitor. Crie todos antes dos joins. Os eventos devem corresponder à posse real, não a uma simulação na main.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/m05/01.in) · [saída de exemplo](testes/m05/01.out). [Todos os testes e regras](testes/m05/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/02_monitores/testes/m05/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/02_monitores/testes/m05/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/m05.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/m05/rubrica.md).

---

## m06 — Barreira reutilizável com condição

**Origem:** Adaptação para programação a partir de: Criado para este estudo

**Arquivo para escrever sua resposta:** [respostas/m06.c](respostas/m06.c).

### Implemente

Leia n,r (1<=n<=16,1<=r<=8). Crie n threads. Em cada rodada j=0..r-1, cada uma registra CHEGA j id, espera todas chegarem e registra PASSA j id. Uma thread pode chegar à próxima rodada após seu próprio PASSA, mesmo se outra ainda não imprimiu PASSA da anterior. Use contador e geração para distinguir rodadas.

**Requisitos obrigatórios:** Mutex, condição, while e broadcast. Não reaproveitar contador sem geração. Registro CHEGA é feito com a contagem de chegada; PASSA só após condição da geração satisfeita.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/m06/01.in) · [saída de exemplo](testes/m06/01.out). [Todos os testes e regras](testes/m06/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/02_monitores/testes/m06/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/02_monitores/testes/m06/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/m06.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/m06/rubrica.md).

---

## m07 — Buffer limitado e soma

**Origem:** Criado para este estudo — implementação de monitor com Pthreads

**Arquivo para escrever sua resposta:** [respostas/m07.c](respostas/m07.c).

### Implemente

Leia N,k e k inteiros (1<=N<=16,0<=k<=1000). Uma thread produz os valores num buffer circular e outra os consome somando. Main imprime a soma após joins.

**Requisitos obrigatórios:** Mutex e duas condições, waits em while, k=0 termina sem bloqueio.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/m07/01.in) · [saída de exemplo](testes/m07/01.out). [Todos os testes e regras](testes/m07/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/02_monitores/testes/m07/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/02_monitores/testes/m07/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/m07.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/m07/rubrica.md).
