# Parte 2 — Monitores e variáveis de condição

Fontes: Aula 12 (28 páginas) e Lista 7 (2 páginas). Pré-requisito: distinguir exclusão mútua de notificação. Objetivo: escrever procedimentos cujo estado compartilhado permaneça consistente, mesmo quando uma chamada precisa esperar.

## 1. Por que encapsular a sincronização?

Com semáforos espalhados por vários processos, qualquer caminho que esqueça uma liberação pode bloquear o sistema. Um monitor reúne estado privado, procedimentos de entrada e sincronização. Só se acessa o estado por esses procedimentos, e apenas um participante executa ativamente dentro do monitor por vez. Isso é exclusão mútua implícita na abstração; em C, você a implementará explicitamente com mutex.

Exemplo: um monitor Buffer mantém vetor, índices e quantidade. Produtores chamam `coloca(x)` e consumidores `retira()`. Eles não alteram diretamente quantidade. Uma função auxiliar chamada de dentro do monitor já está sob proteção; adquirir de novo um mutex normal pode causar autotravamento.

## 2. Esperar é liberar o monitor

Se um consumidor encontra o buffer vazio, não pode simplesmente dormir segurando a entrada: nenhum produtor conseguiria entrar. `wait(cond)` registra a espera e libera a exclusão do monitor como uma operação coordenada. Ao retornar, a execução volta sob proteção.

`signal(cond)` notifica um participante que já espera. Se não há ninguém esperando, normalmente a notificação não é acumulada. Isso difere de `V(sem)`. O estado compartilhado representa a verdade; a notificação apenas anuncia que vale a pena testá-la novamente. `broadcast` acorda todos os candidatos, que ainda precisam disputar acesso e verificar a própria condição.

O predicado não é a variável de condição: `temItens` é uma fila de espera; `quantidade > 0` é uma expressão sobre o estado. Confundir os dois leva a tratar signal como entrega garantida de um item.

## 3. Quem executa depois do signal?

| Disciplina | Quem continua imediatamente? | Consequência |
|---|---|---|
| Hoare, sinaliza e espera | O acordado recebe o monitor | Sinalizador fica na fila urgente |
| Sinaliza e continua, chamada Hansen no material | O sinalizador continua | Acordado deve revalidar o estado |
| Sinaliza e sai | A operação do sinalizador termina | Restrição simplifica transferência |

A terminologia varia entre livros. A disciplina de continuar executando também é frequentemente chamada Mesa. Nas questões, declare a semântica adotada pelo professor em vez de depender só do nome. O material mostra Concurrent Pascal como caso com signal no encerramento da operação.

Sob Hoare, a fila urgente dos sinalizadores tem prioridade sobre novos entrantes; isso preserva a transferência prevista pelo algoritmo. Sob sinaliza-e-continua, alguém pode consumir o recurso antes de o acordado retomar. Em Pthreads, use sempre um laço de predicado: também são permitidos despertares espúrios.

## 4. O erro de if na Lista 7

Considere buffer vazio e consumidores C1 e C2. C1 executa `if (!itens) wait(...)`. Um produtor insere X e sinaliza C1. Antes que C1 readquira o monitor, C2 entra e retira X. Quando C1 continua, o `if` já foi ultrapassado: ele tenta retirar de um buffer vazio.

```text
retira():
  enquanto quantidade == 0: wait(temItens)
  x = buffer[out]
  out = (out + 1) % N
  quantidade--
  signal(temEspacos)
  retorna x

coloca(x):
  enquanto quantidade == N: wait(temEspacos)
  buffer[in] = x
  in = (in + 1) % N
  quantidade++
  signal(temItens)
  retorna
```

O `while(1)` externo da lista não substitui o laço em torno de wait: depois de acordar, o código ainda executa a retirada antes de voltar ao início. Além disso, `coloca()` não retorna no pseudocódigo original; uma chamada pode continuar inserindo repetidamente. Para uma API que insere um item por chamada, remova o laço infinito. O laço de produção fica no produtor, fora do monitor.

## 5. Tradução para C/Pthreads

```c
pthread_mutex_lock(&m);
while (quantidade == 0)
    pthread_cond_wait(&tem_itens, &m);
/* retirar item e atualizar quantidade, sob m */
pthread_cond_signal(&tem_espacos);
pthread_mutex_unlock(&m);
```

Inicialize mutex e condições antes das threads. Todos os acessos ao predicado usam o mesmo mutex. `pthread_cond_wait` libera m ao bloquear e o readquire antes do retorno; não faça unlock antes de chamá-la. Destrua os objetos somente depois de todas as threads terminarem. Um programa finito precisa combinar quantidades de produção/consumo ou ter um protocolo explícito de encerramento.

A especificação de [`pthread_cond_wait`](https://man.omnios.org/man3c/pthread_cond_wait) documenta a liberação/readquisição do mutex e os despertares espúrios. Signal não implica que a thread tenha terminado sua operação.

## 6. Porto com prioridade para saída

Estado sugerido: `ocupacao`, `canalOcupado`, `saidasEsperando`, capacidade N e condições `podeEntrar`, `podeSair`. Defina ocupação incluindo a reserva de uma entrada que já recebeu autorização: isso impede admitir N+1 navios enquanto um ainda atravessa.

```text
pedido_entrada():
  enquanto canalOcupado ou ocupacao == N ou saidasEsperando > 0:
    wait(podeEntrar)
  canalOcupado = verdadeiro
  ocupacao++                 // reserva antes da travessia

entrada_OK():
  canalOcupado = falso
  acordar_candidatos()

pedido_saida():
  saidasEsperando++
  enquanto canalOcupado: wait(podeSair)
  saidasEsperando--
  canalOcupado = verdadeiro

saida_OK():
  ocupacao--
  canalOcupado = falso
  acordar_candidatos()
```

`acordar_candidatos` sinaliza uma saída se houver; caso contrário, sinaliza uma entrada se existir vaga. Todas essas funções são procedimentos do mesmo monitor. Travessia e atracação acontecem fora, conforme o enunciado, mas a reserva do canal permanece marcada. Assuma que cada navio chama as funções na ordem correta, com exatamente uma confirmação por autorização.

Invariantes: ocupação entre 0 e N, no máximo uma travessia, nenhuma nova entrada admitida quando há saída aguardando. Prioridade não interrompe uma entrada já autorizada. Um fluxo contínuo de saídas pode adiar entradas: prioridade não é promessa de justiça universal.

## 7. Construindo Hoare com semáforos

Use `mutex=1` na entrada, `next=0` para sinalizadores urgentes e `next_count=0`. Cada condição c tem `c.sem=0`, `c.count=0`. Um procedimento faz P(mutex), executa seu corpo e termina por `sair()`.

```text
sair():
  se next_count > 0: V(next)
  senão: V(mutex)

wait(c):
  c.count++
  sair()
  P(c.sem)
  c.count--

signal(c):
  se c.count > 0:
    next_count++
    V(c.sem)
    P(next)
    next_count--
```

Acordado não faz P(mutex) de novo: recebeu a posse lógica do monitor. O sinalizador só retoma quando alguém entrega next. Todos os contadores são manipulados sob essa exclusão lógica. Retornos antecipados também devem usar sair; esquecer esse epílogo prende o monitor.

## 8. Construindo sinaliza-e-continua

Aqui o sinalizador não libera a posse. Uma implementação robusta com semáforos pode usar **uma fila de registros de espera**, cada registro com semáforo privado iniciado em zero.

```text
wait(c):
  criar registro r com semáforo zero
  enfileirar r em c                 // ainda dentro do monitor
  V(mutex)
  P(r.sem)
  P(mutex)
  destruir r após retomar

signal(c):
  se fila c não vazia:
    r = desenfileirar c
    V(r.sem)                       // sinalizador continua
```

Usar um registro por espera impede que um novo waiter roube uma notificação destinada a alguém já na fila. A fila é alterada apenas sob mutex; não há janela entre registro e liberação na qual se perca a notificação. O retorno de wait sempre exige reavaliar o predicado.

## 9. Barbeiro como monitor

Mantenha fila com até cinco clientes aguardando e uma identificação do cliente em corte. Um cliente chega: se o barbeiro está livre, ocupa/reserva a cadeira e acorda o barbeiro; se ocupado e há vaga, entra na fila; caso contrário, vai embora. O barbeiro chama exatamente um cliente, corta fora do monitor e registra conclusão antes de aceitar o próximo. Cada cliente espera por sua própria condição de atendimento e conclusão, ou por um predicado que compare seu identificador.

Se todos esperarem numa única condição e você sinalizar apenas um arbitrário, pode acordar o cliente errado. Use condições individuais, ou broadcast com um predicado por cliente. Os testes distinguem cinco em espera de seis pessoas no estabelecimento: uma pode estar no corte e cinco na fila.

## 10. Filósofos e monitores Java

O monitor dos slides usa um estado por filósofo, `pickup`, `putdown` e `test`. `pickup` marca FAMINTO e tenta reservar os garfos; `putdown` marca PENSANDO e testa os vizinhos; `test` só concede quando nenhum vizinho COME. No modelo de Hoare a passagem de controle é imediata. Numa implementação Pthreads, traduza a espera para while.

A frase dos slides que promete ausência de starvation exige uma política adicional de justiça: os testes dos vizinhos, por si, não impõem uma ordem justa entre todos os famintos. Também há cinco garfos no ciclo de cinco filósofos, e não quatro como aparece em uma anotação.

Em Java, cada objeto possui monitor; `synchronized` protege método ou bloco. `wait`, `notify` e `notifyAll` são usados com a posse do monitor correspondente. Wait libera esse monitor, enquanto notify apenas torna uma espera elegível para continuar após a liberação. Use while pelo mesmo motivo estudado. Não confunda `Thread.sleep` com wait: dormir não libera automaticamente o monitor.

## Como estudar esta parte

Para cada exercício, desenhe o estado antes e depois de wait/signal. Pergunte quem possui a exclusão em cada etapa. Escreva o invariante ao lado do procedimento e teste: estado vazio, estado cheio, dois acordados disputando uma vaga e prioridade de entrada. Os casos objetivos verificam essas decisões; a rubrica exige revisar também o pseudocódigo completo.
