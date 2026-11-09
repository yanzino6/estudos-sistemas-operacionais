# Parte 1 — Semáforos e raciocínio concorrente

Fontes: Aula 11 (23 páginas), Aula 11.2 (38 páginas) e Lista 6 (3 páginas). Os enunciados, inclusive os exemplos resolvidos, estão catalogados em [exercicios.md](exercicios.md). Esta parte prepara você para identificar **o que deve ser protegido** e **quem deve esperar por quem** antes de escrever código.

## 1. Comece por um intercalamento

Dois fluxos executam `contador++`. Essa expressão envolve ler, somar e gravar. Se ambos leem 7, ambos calculam 8 e ambos gravam 8, um incremento desaparece. A região crítica é a sequência inteira, não apenas a escrita final. Em C, acessos conflitantes sem sincronização constituem data race e produzem comportamento indefinido: o exemplo de intercalamento é um modelo didático, não uma lista completa dos resultados possíveis.

Concorrência significa progresso de várias atividades no mesmo intervalo. Paralelismo significa execução simultânea. Uma única CPU já permite corridas, pois pode interromper um fluxo entre duas instruções.

Antes de resolver um problema, anote: estado compartilhado, operações que alteram esse estado, condições para continuar e invariantes. Invariante é uma propriedade que deve permanecer verdadeira em todos os estados observáveis, como `0 <= itens <= capacidade`.

## 2. Espera ocupada e bloqueio

Na espera ocupada, um fluxo testa uma condição repetidamente, consumindo CPU. Uma variável `ocupado` comum não resolve: dois fluxos podem observar `false` antes de qualquer um escrever `true`. Alternância estrita impõe turnos mesmo quando o outro processo não deseja entrar, prejudicando progresso. Dekker e Peterson são soluções clássicas para dois participantes sob hipóteses específicas de atomicidade e ordenação de memória; sua tradução ingênua para variáveis comuns em C não é uma solução portátil.

TSL/test-and-set combina teste e alteração atomicamente e permite construir um spinlock. Desabilitar interrupções pode proteger determinadas regiões do núcleo em uma CPU, mas não impede outra CPU de acessar a memória. O slide sobre bloquear o barramento é uma simplificação de mecanismos de atomicidade: não é uma receita para código de usuário.

Bloquear retira o fluxo da disputa pela CPU até que um evento permita continuar. O desafio é evitar a janela entre testar e dormir: se a notificação ocorrer nessa janela e não for registrada, o fluxo pode dormir para sempre. Semáforos preservam permissões; variáveis de condição, na próxima parte, exigem outro protocolo.

## 3. O que P e V realmente fazem

Um semáforo possui um contador de permissões e um mecanismo de espera. `P`, `down` e `wait` são nomes da aquisição; `V`, `up` e `signal` são nomes da liberação. Cada operação é atômica.

- `P(S)`: se houver permissão, consome uma e continua; caso contrário, bloqueia.
- `V(S)`: entrega uma permissão a alguém que espera ou a deixa disponível para uma aquisição futura.

O pseudocódigo dos slides usa contadores não negativos. Existem apresentações com contador negativo para representar quantidade de bloqueados; não misture as duas convenções no mesmo rastreamento. A atomicidade é fornecida pela implementação, não por um `if` comum em C.

| Objetivo | Inicialização | Interpretação |
|---|---:|---|
| Exclusão mútua | 1 | Um participante por vez |
| Limitar uso de K recursos | K | Até K aquisições simultâneas |
| Aguardar evento ainda não ocorrido | 0 | Consumidor espera publicação |

Um semáforo binário pode servir à exclusão mútua, mas não é idêntico a um mutex POSIX: o mutex tem proprietário e normalmente deve ser liberado por quem o adquiriu. Um semáforo pode ser sinalizado por outro participante, como ocorre na precedência.

## 4. Precedência: transforme setas em permissões

Para garantir `A` antes de `B`, inicialize `s=0`, execute `A; V(s)` no primeiro fluxo e `P(s); B` no segundo. Se A terminar antes da criação de B, a permissão continuará guardada. Para B esperar A e C, B precisa obter duas notificações identificáveis. Para A liberar B e C, uma única chamada a V não basta: ela acorda apenas um consumidor de permissão.

Em um grafo, escreva todas as arestas, retire as já garantidas pela ordem sequencial dentro de cada processo e associe semáforos às restantes. Depois simule cada processo iniciando primeiro. Uma implementação pode impor dependências extras e ainda produzir uma ordem válida; isso reduz concorrência e deve ser discutido.

Na Lista 6, o grafo representa `F1→F2,F3,F4`, `F2→F5`, `F3→F5,F6`, `F4→F6`, `F5→F7`, `F6→F7`. P1 já garante F1 antes de F3; P2 garante F2 antes de F5 antes de F7; P3 garante F4 antes de F6. Restam cinco comunicações entre processos. O desenho original foi preservado em `referencias/` para conferência.

No problema de imprimir 1 a 1000, os cinco participantes imprimem blocos de 200. Quatro semáforos iniciados em zero encadeiam os blocos; o último não deve sinalizar um inexistente quinto semáforo.

## 5. Produtor-consumidor, passo a passo

Use um vetor circular de N posições, `in` para próxima inserção e `out` para próxima retirada. Avançar significa `(indice + 1) % N`. Inicialize `vagas=N`, `itens=0`, `mutex=1`.

```text
produzir(x):                 consumir():
  P(vagas)                    P(itens)
  P(mutex)                    P(mutex)
  buffer[in] = x              x = buffer[out]
  in = (in+1) % N             out = (out+1) % N
  V(mutex)                    V(mutex)
  V(itens)                    V(vagas)
```

Vagas/itens coordenam disponibilidade; mutex protege a atualização do buffer e dos índices. Eles resolvem problemas diferentes. Com N=2, duas produções sem consumo esgotam vagas. A terceira produção bloqueia **antes** de tomar mutex, permitindo ao consumidor abrir espaço.

Se o produtor adquirir mutex antes de esperar vaga, poderá dormir com o buffer cheio segurando o recurso que o consumidor precisa. Forma-se uma espera circular. O mesmo cuidado vale para o consumidor esperando item com mutex adquirido.

No caso restrito de **um produtor e um consumidor**, cada índice tem um único escritor; as permissões impedem acesso prematuro ou reutilização de uma posição. Nesse modelo e com sincronização de memória adequada das primitivas, o mutex pode ser dispensado. Isso não se generaliza automaticamente para vários produtores.

### Atenção à otimização da página 22

O slide reserva o índice sob mutex, solta o mutex e só depois escreve no slot. Com dois produtores, P1 reserva slot 0 e pausa; P2 reserva slot 1, escreve e publica um item; um consumidor pode então ler o slot 0 ainda não preenchido. Logo, a contagem de itens não identifica qual slot está pronto. Dois locks separados podem permitir produção e consumo simultâneos, mas a publicação precisa preservar a ordem dos slots; não copie a otimização sem esse protocolo. Um contraexemplo vale mais que uma execução que parece correta.

## 6. Problemas de lotes

**Pombo correio:** capacidade 20, contador protegido e semáforo `cheia=0`. Usuários adquirem uma das 20 vagas antes de colar a mensagem. O vigésimo sinaliza cheia. O pombo viaja somente com exatamente 20 mensagens e repõe as 20 vagas somente após voltar. Preparar mensagens fica fora da região crítica. Um lote incompleto deve continuar esperando; em um teste finito, forneça múltiplos de 20 ou defina explicitamente uma política de encerramento.

**Montanha-russa:** capacidade C e n>C passageiros. Separar embarque, passeio e desembarque evita que uma vaga liberada seja usada enquanto o carro ainda viaja. Com até quatro semáforos, use `embarque=C`, `cheio=0`, `fim=0`, `mutex=1`, além de contadores. O último embarque sinaliza cheio. Ao terminar, o carro libera C permissões de fim. Cada passageiro consome uma e desembarca; o último desembarque reabre as C vagas. Enquanto o lote desembarca, nenhum passageiro pode iniciar outra viagem. Reutilize o contador somente em uma transição protegida.

**Canibais:** travessa com M porções. Apenas um faminto deve solicitar reposição quando vazia; os demais precisam esperar. Uma solução didática simples mantém mutex enquanto o primeiro espera `reposto`: isso é seguro somente se o cozinheiro não precisar desse mesmo mutex para repor e sinalizar. O cozinheiro espera `pedido`, coloca M porções e sinaliza `reposto`; o canibal decrementa a quantidade sob mutex. Comer ocorre fora. No código dos slides, analise a distinção entre reservar uma porção e efetivamente servir-se: o cozinheiro não pode sobrescrever a travessa enquanto alguém ainda retira a porção reservada.

**Filme de Hoare:** esperar dez fãs não garante que todos assistiram ao filme inteiro. São necessárias fases de admissão, exibição concluída e saída. O código questionado na página 20 permite que fãs avancem para telefonar sem uma confirmação do término da exibição; também lê `nFans` fora da proteção. A solução deve separar permissões de entrada e de conclusão, impedir sobreposição indevida de sessões e liberar a próxima turma no momento correto.

## 7. Leitores e escritores

Leituras podem coexistir; uma escrita exige exclusividade. Na solução com preferência de leitores, `rc` conta leitores ativos, `mutex` protege rc e `db` controla a base. O primeiro leitor adquire db; o último libera. Enquanto um leitor novo consegue juntar-se ao grupo antes da saída do último, um escritor pode esperar indefinidamente.

A variante com preferência dos escritores acrescenta `wc` e uma porta de entrada dos leitores. O primeiro escritor fecha essa porta, o último a reabre. `mutex_rc` e `mutex_wc` protegem seus contadores; `w_db` protege a base; `r_db` fecha a entrada de leitores; o mutex adicional serializa a tentativa de entrada de leitores. Isso não significa FIFO global: preferência de escritores pode prejudicar leitores.

Na Lista 6, o semáforo R é uma catraca comum a leitores e escritores. O escritor adquire R na linha 3 e mantém R enquanto espera W na linha 4. Assim, depois de adquirir R, ele impede novos leitores e os antigos podem sair (linhas 9–12, sem precisar de R). M protege rc, incluindo a decisão de primeiro/último leitor. R sozinho não protege a saída dos leitores. A conclusão sobre ausência de starvation **antes da aquisição de R** depende da política de atendimento do semáforo e da justiça do escalonamento. Não prometa justiça que a primitiva não oferece.

Sem M, um leitor entrando e outro saindo podem perder atualizações de rc ou liberar W em um instante inadequado. Mesmo que entradas passem uma por vez por R, saídas continuam concorrentes com entradas.

## 8. Semáforo múltiplo e fumantes

Aquisição múltipla deve ser tudo ou nada. Fazer `P(A); P(B)` não é atômico como conjunto: o fluxo pode reter A enquanto espera B. No modelo booleano fornecido na Lista 6, proteja todos os contadores com um mutex global, teste a disponibilidade de todos e só então decremente todos. Se algum não puder ser adquirido, retorne falso sem alterar nenhum. A liberação múltipla também usa esse mutex. Todos os acessos a esses recursos devem respeitar o protocolo; misturar operações externas desprotegidas invalida o teste. Se um recurso aparecer duas vezes, é necessário somar a demanda por ele.

Uma versão bloqueante precisa registrar espera, liberar mutex e dormir atomicamente em relação às notificações, reavaliando o predicado ao acordar. Não aguarde um recurso indisponível segurando o mutex que protege a reposição.

Nos fumantes, um agente disponibiliza dois ingredientes e sabe qual fumante possui o terceiro. Use um semáforo por fumante, todos zero, e uma confirmação do agente. Publicar tabaco e papel acorda quem tem fósforo. O fumante confirma ao terminar; só então o agente publica o próximo par. É um problema de despacho e confirmação, não de sorte na aquisição de ingredientes isolados.

## 9. Barbeiro, filósofos e progresso

No barbeiro, há cinco cadeiras de espera além da cadeira de corte. O contador `waiting` não inclui quem já está sendo atendido. Clientes que não cabem saem; o barbeiro dorme esperando clientes. Os semáforos `customers`, `barbers` e `mutex` dos slides coordenam a seleção; se o exercício exigir que um cliente só saia após o corte, inclua uma confirmação de conclusão, pois ser chamado não equivale a terminar o corte.

Cinco filósofos formam um ciclo com cinco garfos. Se todos pegarem primeiro o garfo esquerdo, cada um espera pelo vizinho: deadlock. A solução por estados mantém PENSANDO/FAMINTO/COMENDO sob mutex. `test(i)` autoriza i somente se os dois vizinhos não comem. Ao liberar, testa os vizinhos e sinaliza seus semáforos individuais. Reservar o estado COMENDO dentro da região crítica impede concessões conflitantes. Segurança e ausência de deadlock não demonstram, sozinhas, ausência de starvation; uma fila ou outra política justa precisa ser acrescentada se essa garantia for exigida.

**Como revisar qualquer solução:** prove exclusão/limites, verifique quem libera cada espera, teste inicialização, enumere estados extremos e discuta justiça separadamente. Um timeout sugere falta de progresso, mas não prova matematicamente deadlock. Um teste que termina também não prova correção para todos os escalonamentos.
