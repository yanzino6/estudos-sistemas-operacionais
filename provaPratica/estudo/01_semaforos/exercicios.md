# Prova prática — 01_semaforos

Todas as questões pedem **programas C**. Os antigos pedidos de explicação/pseudocódigo foram convertidos em implementações. Os contratos finitos de entrada, saída e encerramento são adaptações de treino. Semáforos: use o adaptador descrito em [guia de modelos](../MODELOS_DE_RESPOSTA.md).

## s01 — Cinco blocos ordenados por semáforos

**Origem:** Adaptação para programação a partir de: Professor — Aula 11.2, pp. 3–4

**Arquivo para escrever sua resposta:** [respostas/s01.c](respostas/s01.c).

### Implemente

Leia k (1<=k<=200). Crie cinco threads. Thread i, com i=0..4, imprime os números de i*k+1 a (i+1)*k, um por linha. Use semáforos para imprimir os blocos em ordem. k=200 reproduz os cinco blocos da aula.

**Requisitos obrigatórios:** Quatro semáforos de precedência; cinco creates antes dos joins; cada thread imprime seu bloco.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s01/01.in) · [saída de exemplo](testes/s01/01.out). [Todos os testes e regras](testes/s01/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s01/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s01/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s01.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s01/rubrica.md).

---

## s02 — Grafo de nove funções

**Origem:** Adaptação para programação a partir de: Professor — Aula 11.2, pp. 5–6

**Arquivo para escrever sua resposta:** [respostas/s02.c](respostas/s02.c).

### Implemente

Implemente em C/Pthreads o grafo usando semáforos. Crie os trabalhadores com as sequências locais: A: F1,F2,F9,F4; B: F3,F7; C: F8,F5,F6. Cada função é representada por uma linha com seu nome, impressa quando executada. Sem entrada. Cada nó deve aparecer exatamente uma vez. Arestas obrigatórias: F1→F2, F1→F5, F1→F8, F2→F3, F2→F6, F5→F6, F5→F9, F8→F9, F6→F7, F9→F7, F3→F4, F7→F4, F2→F9, F8→F5, F3→F7. Qualquer ordenação que respeite essas dependências é aceita.

**Requisitos obrigatórios:** Criar todos os trabalhadores antes dos joins; semáforos inicialmente zero para precedência; sinalizar depois da ação; aguardar antes da dependente. Não imprimir tudo pela main nem serializar todos os trabalhadores com joins intermediários.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s02/01.in) · [saída de exemplo](testes/s02/01.out). [Todos os testes e regras](testes/s02/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s02/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s02/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s02.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s02/rubrica.md).

---

## s03 — Junção e bifurcação com semáforos

**Origem:** Adaptação para programação a partir de: Professor — Aula 11.2, pp. 7–9

**Arquivo para escrever sua resposta:** [respostas/s03.c](respostas/s03.c).

### Implemente

Implemente em C/Pthreads o grafo usando semáforos. Crie os trabalhadores com as sequências locais: uma thread para cada nó A,B,C,D,E. Cada função é representada por uma linha com seu nome, impressa quando executada. Sem entrada. Cada nó deve aparecer exatamente uma vez. Arestas obrigatórias: A→C, B→C, C→D, C→E. Qualquer ordenação que respeite essas dependências é aceita.

**Requisitos obrigatórios:** Criar todos os trabalhadores antes dos joins; semáforos inicialmente zero para precedência; sinalizar depois da ação; aguardar antes da dependente. Não imprimir tudo pela main nem serializar todos os trabalhadores com joins intermediários.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s03/01.in) · [saída de exemplo](testes/s03/01.out). [Todos os testes e regras](testes/s03/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s03/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s03/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s03.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s03/rubrica.md).

---

## s04 — Pombo correio em lotes de 20

**Origem:** Adaptação para programação a partir de: Professor — Aula 11.2, pp. 10–12

**Arquivo para escrever sua resposta:** [respostas/s04.c](respostas/s04.c).

### Implemente

Leia v (1<=v<=4), número de viagens. Crie 20*v threads usuárias, cada uma cola uma mensagem de id 0..20*v-1, e uma thread pombo. Cada mensagem aparece uma vez.

Registre uma linha por evento: `ENTRA id` ao reservar/colar; `INICIO` com lote cheio; `FIM` ao terminar a viagem/exibição. O pombo retorna no evento FIM, que libera o lote e permite novas mensagens; não imprima SAI.

**Requisitos obrigatórios:** Use semáforos e contadores protegidos. Eventos representam alterações reais do estado, impressos na mesma região crítica da alteração. Sem sleep para garantir ordem; número finito de rodadas e join em todas as threads.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s04/01.in) · [saída de exemplo](testes/s04/01.out). [Todos os testes e regras](testes/s04/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s04/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s04/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s04.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s04/rubrica.md).

---

## s05 — Montanha-russa com desembarque

**Origem:** Adaptação para programação a partir de: Professor — Aula 11.2, pp. 13–14

**Arquivo para escrever sua resposta:** [respostas/s05.c](respostas/s05.c).

### Implemente

Leia n,C,v (2<=n<=16,1<=C<n,1<=v<=4). Crie n threads passageiras e uma thread carro. Faça exatamente v viagens. Passageiros podem repetir viagens; cada passageiro ocupa no máximo um assento por viagem. Encerre também os passageiros que estão esperando quando todas as viagens acabarem.

Registre uma linha por evento: `ENTRA id` ao reservar/colar; `INICIO` com lote cheio; `FIM` ao terminar a viagem/exibição. Cada participante registra `SAI id` ao sair; só reabra a entrada quando o lote inteiro sair.

**Requisitos obrigatórios:** Use semáforos e contadores protegidos. Eventos representam alterações reais do estado, impressos na mesma região crítica da alteração. Sem sleep para garantir ordem; número finito de rodadas e join em todas as threads. Use no máximo quatro semáforos; o protocolo de encerramento deve usar esses mesmos objetos.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s05/01.in) · [saída de exemplo](testes/s05/01.out). [Todos os testes e regras](testes/s05/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s05/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s05/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s05.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s05/rubrica.md).

---

## s06 — Canibais e cozinheiro com encerramento

**Origem:** Adaptação para programação a partir de: Professor — Aula 11.2, pp. 15–17

**Arquivo para escrever sua resposta:** [respostas/s06.c](respostas/s06.c).

### Implemente

Leia N,M,k (1<=N<=8,1<=M<=8,1<=k<=8). Travessa começa com M porções. Crie N canibais, cada um come k porções, e um cozinheiro. Registre `COME id` na retirada e `REPOE` ao repor M porções. Só reponha quando vazio e ainda houver refeições pendentes. Encerre o cozinheiro após todas as refeições.

**Requisitos obrigatórios:** Semáforos para pedido/retorno, contador protegido; um pedido por reposição; não repor antes de vazio. Ids de 0 a N-1.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s06/01.in) · [saída de exemplo](testes/s06/01.out). [Todos os testes e regras](testes/s06/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s06/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s06/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s06.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s06/rubrica.md).

---

## s07 — Sessões de filme com término sincronizado

**Origem:** Adaptação para programação a partir de: Professor — Aula 11.2, pp. 18–20

**Arquivo para escrever sua resposta:** [respostas/s07.c](respostas/s07.c).

### Implemente

Leia v (1<=v<=3). Crie 10*v threads fãs distintas e uma thread demonstradora. Cada fã participa exatamente uma vez. Execute v sessões com dez espectadores e não libere saída antes do fim.

Registre uma linha por evento: `ENTRA id` ao reservar/colar; `INICIO` com lote cheio; `FIM` ao terminar a viagem/exibição. Cada participante registra `SAI id` ao sair; só reabra a entrada quando o lote inteiro sair.

**Requisitos obrigatórios:** Use semáforos e contadores protegidos. Eventos representam alterações reais do estado, impressos na mesma região crítica da alteração. Sem sleep para garantir ordem; número finito de rodadas e join em todas as threads.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s07/01.in) · [saída de exemplo](testes/s07/01.out). [Todos os testes e regras](testes/s07/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s07/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s07/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s07.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s07/rubrica.md).

---

## s08 — Base compartilhada com leitores e escritores

**Origem:** Adaptação para programação a partir de: Professor — Aula 11.2, pp. 21–33; Lista 6, questão 2

**Arquivo para escrever sua resposta:** [respostas/s08.c](respostas/s08.c).

### Implemente

Leia R,W,k (1..8 cada). Crie R leitores e W escritores. Cada um acessa a base k vezes. Base inicia em zero; cada escrita soma um. Leitor registra `RI id valor` na entrada e `RF id` na saída; escritor registra `WI id` na entrada e `WF id novo_valor` após incremento, antes de liberar a base. Ids de leitores/escritores são independentes, ambos começam em zero.

**Requisitos obrigatórios:** Implemente a catraca R e proteção M/W da Lista 6 usando semáforos. Permitir múltiplos leitores; nenhum escritor junto com qualquer outro acesso. Todas as threads terminam. Mutex de log não deve manter a base serializada durante toda a leitura.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s08/01.in) · [saída de exemplo](testes/s08/01.out). [Todos os testes e regras](testes/s08/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s08/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s08/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s08.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s08/rubrica.md).

---

## s09 — Barbeiro com semáforos

**Origem:** Adaptação para programação a partir de: Professor — Aula 11.2, pp. 34–35

**Arquivo para escrever sua resposta:** [respostas/s09.c](respostas/s09.c).

### Implemente

Leia q (1<=q<=100), depois q comandos `A id` (chegada de cliente) ou `T` (autoriza concluir um corte). Ids de clientes são distintos e não negativos. Crie uma thread barbeiro e uma thread despachante (pode ser a main). A despachante só envia o próximo comando após confirmação do anterior. A chegada ocupa a cadeira livre (`ATENDE id`), entra na fila FIFO de cinco lugares (`ESPERA id`) ou é recusada (`RECUSA id`). T sem cliente imprime `LIVRE`; T com cliente imprime `FIM id` seguido de `ATENDE proximo` ou `LIVRE`. Os testes encerram sem clientes pendentes. Esse relógio de comandos torna reproduzível o atendimento concorrente.

**Requisitos obrigatórios:** Implemente fila/cadeira usando semáforos. O barbeiro executa as conclusões de corte; despachante envia comandos e aguarda confirmação sem busy wait. Não simular toda a barbearia sequencialmente na main.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s09/01.in) · [saída de exemplo](testes/s09/01.out). [Todos os testes e regras](testes/s09/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s09/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s09/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s09.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s09/rubrica.md).

---

## s10 — Cinco filósofos usando semáforos

**Origem:** Adaptação para programação a partir de: Professor — Aula 11.2, pp. 36–38

**Arquivo para escrever sua resposta:** [respostas/s10.c](respostas/s10.c).

### Implemente

Leia k (1<=k<=8). Crie cinco filósofos; cada um come k vezes. Ids 0..4 formam ciclo; 0 e 4 são vizinhos. Registre `IN id` após adquirir os recursos e `OUT id` antes de liberá-los. Todos devem terminar. O corretor verifica contagem, aquisições/liberações e vizinhos nunca ativos simultaneamente.

**Requisitos obrigatórios:** Use semáforos para controlar as aquisições. Crie todos antes dos joins. Os eventos devem corresponder à posse real, não a uma simulação na main.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s10/01.in) · [saída de exemplo](testes/s10/01.out). [Todos os testes e regras](testes/s10/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s10/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s10/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s10.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s10/rubrica.md).

---

## s11 — Grafo da Lista 6

**Origem:** Adaptação para programação a partir de: Professor — Lista 6, questão 1

**Arquivo para escrever sua resposta:** [respostas/s11.c](respostas/s11.c).

### Implemente

Implemente em C/Pthreads o grafo usando semáforos. Crie os trabalhadores com as sequências locais: P1: F1,F3; P2: F2,F5,F7; P3: F4,F6. Cada função é representada por uma linha com seu nome, impressa quando executada. Sem entrada. Cada nó deve aparecer exatamente uma vez. Arestas obrigatórias: F1→F2, F1→F3, F1→F4, F2→F5, F3→F5, F3→F6, F4→F6, F5→F7, F6→F7. Qualquer ordenação que respeite essas dependências é aceita.

**Requisitos obrigatórios:** Criar todos os trabalhadores antes dos joins; semáforos inicialmente zero para precedência; sinalizar depois da ação; aguardar antes da dependente. Não imprimir tudo pela main nem serializar todos os trabalhadores com joins intermediários.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s11/01.in) · [saída de exemplo](testes/s11/01.out). [Todos os testes e regras](testes/s11/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s11/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s11/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s11.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s11/rubrica.md).

---

## s12 — Aquisição múltipla tudo-ou-nada

**Origem:** Adaptação para programação a partir de: Professor — Lista 6, questão 3

**Arquivo para escrever sua resposta:** [respostas/s12.c](respostas/s12.c).

### Implemente

Leia m,q (1<=m<=8,1<=q<=100), m contadores iniciais não negativos e q comandos. Cada comando tem letra D ou U seguida de m demandas não negativas. D tenta adquirir o vetor de demandas atomicamente, imprime OK se conseguiu ou FALHA sem mudar nada. U soma o vetor e imprime OK. Ao final imprima `ESTADO` e os m contadores. Todos os valores e somas ficam abaixo de 1000000. Faça down_multiplo/up_multiplo sob um mutex; a main aplica os comandos na ordem da entrada.

**Requisitos obrigatórios:** O teste de todos os contadores e a alteração inteira ocorrem na mesma região crítica. Não consumir parcialmente. API deve ser segura para chamadas concorrentes; os casos ordenados tornam o resultado verificável.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s12/01.in) · [saída de exemplo](testes/s12/01.out). [Todos os testes e regras](testes/s12/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s12/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s12/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s12.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s12/rubrica.md).

---

## s13 — Agente e fumantes por rodadas

**Origem:** Adaptação para programação a partir de: Professor — Lista 6, questão 4

**Arquivo para escrever sua resposta:** [respostas/s13.c](respostas/s13.c).

### Implemente

Leia q (1<=q<=100), seguido de q pares a,b de ingredientes distintos entre 0,1,2. 0=tabaco,1=papel,2=fósforo. Crie um agente e três fumantes. Em cada rodada, agente disponibiliza o par; só o fumante que possui o ingrediente faltante pode atender. Imprima uma linha `FUMA id` por rodada, na ordem dos pares. Agente espera conclusão antes de publicar outro par; encerre os três fumantes ao final.

**Requisitos obrigatórios:** Semáforo individual por fumante e confirmação ao agente; publicar somente para o correto. Main não calcula nem imprime as linhas no lugar das threads.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s13/01.in) · [saída de exemplo](testes/s13/01.out). [Todos os testes e regras](testes/s13/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s13/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s13/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s13.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s13/rubrica.md).

---

## s14 — Buffer circular SPSC sem mutex

**Origem:** Adaptação para programação a partir de: Professor — Aula 11, pp. 14–22; casos de análise criados para o treino

**Arquivo para escrever sua resposta:** [respostas/s14.c](respostas/s14.c).

### Implemente

Leia N,k e k inteiros (1<=N<=8,0<=k<=1000). Use uma thread produtora e uma consumidora. Implemente buffer circular usando somente semáforos vagas/itens: cada índice tem um único escritor. A consumidora imprime cada valor retirado, um por linha, preservando a ordem de entrada.

**Requisitos obrigatórios:** Sem mutex no buffer SPSC; publicar item somente após escrever; liberar vaga somente após ler; não imprimir diretamente da entrada.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s14/01.in) · [saída de exemplo](testes/s14/01.out). [Todos os testes e regras](testes/s14/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s14/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s14/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s14.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s14/rubrica.md).

---

## s15 — Reserva de recursos limitados

**Origem:** Adaptação para programação a partir de: Criado para este estudo

**Arquivo para escrever sua resposta:** [respostas/s15.c](respostas/s15.c).

### Implemente

Leia n,C,k (1<=C<=n<=16,1<=k<=8). Crie n trabalhadores, cada um usa um dos C recursos idênticos k vezes. Registre `IN id` após adquirir os recursos e `OUT id` antes de liberá-los. Todos devem terminar. O corretor verifica contagem, aquisições/liberações e no máximo C usuários ativos.

**Requisitos obrigatórios:** Use semáforos para controlar as aquisições. Crie todos antes dos joins. Os eventos devem corresponder à posse real, não a uma simulação na main.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s15/01.in) · [saída de exemplo](testes/s15/01.out). [Todos os testes e regras](testes/s15/README.md).

O corretor valida eventos e invariantes; `.out` é **um exemplo válido**, não uma ordem única exigida. Veja os parâmetros de cada caso no JSON.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s15/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s15/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s15.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s15/rubrica.md).

---

## s16 — Contador protegido por semáforo

**Origem:** Criado para este estudo — implementação prática

**Arquivo para escrever sua resposta:** [respostas/s16.c](respostas/s16.c).

### Implemente

Leia t,k (1<=t<=16,0<=k<=10000). Crie t threads, cada uma incrementa k vezes um contador global usando semáforo binário. Imprima somente o total.

**Requisitos obrigatórios:** Semáforo iniciado em 1; criar todos antes dos joins; contador atualizado realmente pelas threads.

### Entrada, saída e correção

Primeiro caso: [entrada](testes/s16/01.in) · [saída de exemplo](testes/s16/01.out). [Todos os testes e regras](testes/s16/README.md).

O corretor compara os tokens em ordem, ignorando diferenças de espaços e quebras de linha.

Da raiz `provaPratica`:

```bash
python3 estudo/01_semaforos/testes/s16/corrigir.py
# Para corrigir outro arquivo C:
python3 estudo/01_semaforos/testes/s16/corrigir.py /caminho/solucao.c
```

O comando sem argumentos usa seu modelo em `respostas/s16.c`. Tempo máximo por execução: 5 segundos. Código 0 = testes passaram; 1 = reprovação; 2 = erro de uso/configuração. [Requisitos para conferir no código](testes/s16/rubrica.md).
