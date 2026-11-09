# Parte 4 — Threads: fundamentos, criação, argumentos e retorno

Fontes: Aula 16 inteira; Aula 17, páginas 1–20 e 29–33; roteiro 4.1 inteiro; roteiro 4.2, exercícios 1, 2, 3 e 6. A parte 5 aprofunda dados compartilhados e soma com muitas threads.

## 1. Processo possui recursos; thread executa

A separação central da aula é entre propriedade de recursos e escalonamento. O processo reúne espaço de endereçamento, código, dados, heap e descritores. Cada thread mantém seu fluxo: contador de programa, registradores, pilha, identificação, estado e atributos de escalonamento. O PCB/BCP representa o processo; TCB registra o contexto individual. Há ligação entre eles, não duplicação de todos os recursos para cada thread.

As pilhas são logicamente individuais, mas estão no espaço de endereçamento compartilhado: passar o endereço de uma variável de pilha a outra thread é possível, desde que o objeto permaneça vivo e o acesso esteja sincronizado. “Pilha própria” não significa proteção de memória contra as outras threads.

Estados básicos: pronta, executando e bloqueada. A discussão dos slides sobre suspensão refere-se ao modelo de swapping de processos; não é uma afirmação de que nenhuma API possa suspender threads.

## 2. Para que servem?

Um editor pode manter interação enquanto salva ou verifica texto. Um servidor pode atender outras requisições durante uma espera de rede. Em multiplicação de matrizes, células de saída independentes podem ser distribuídas entre trabalhadores. Esses exemplos mostram responsividade, compartilhamento econômico e aproveitamento de vários núcleos.

Criar/trocar threads de um mesmo processo tende a custar menos que criar/trocar processos isolados porque muitos recursos são reaproveitados. Isso não é uma promessa de aceleração em qualquer programa. Criação, escalonamento, sincronização e acesso à memória têm custo. Fork também pode usar copy-on-write, evitando copiar imediatamente toda a memória.

Uma sequência misturada de prints demonstra uma ordem observada de execução, não execução simultânea em dois núcleos. Sleep pode favorecer intercalamentos, mas não prova paralelismo e não corrige uma corrida.

## 3. ULT, KLT e modelos de mapeamento

| Modelo | Quem escalona | Vantagem | Limitação |
|---|---|---|---|
| M:1, ULT puro | Biblioteca em usuário gerencia M fluxos sobre uma entidade do núcleo | Troca local barata e política da aplicação | Sem paralelismo interno em várias CPUs; syscall bloqueante pode parar todos |
| 1:1 | Uma entidade do núcleo por thread de usuário | Bloqueio de uma não obriga as outras a parar; permite vários núcleos | Custo e limites de entidades no núcleo |
| M:N | Biblioteca e núcleo cooperam | Multiplexação e potencial paralelismo | Implementação e coordenação mais complexas |

ULT/KLT descrevem níveis de implementação; Pthreads é uma API e não obriga todos os sistemas a usar o mesmo modelo. No Linux/NPTL, o modelo estudado é 1:1. O núcleo trata threads como tasks, compartilhando estruturas conforme a criação via família clone. Threads do mesmo processo compartilham a identificação do processo vista por getpid, mas possuem identidades de thread distintas. `pthread_t` não deve ser confundido com PID ou com TID do núcleo.

O comentário do roteiro sobre escalonador O(1) é contexto histórico, não uma especificação do escalonador Linux atual. “Unbound” nos atributos não deve ser interpretado como uma implementação M:N do NPTL. Não confunda afinidade a CPU, escopo de contenção e mapeamento usuário/núcleo.

Nos slides, o teste de criação de 50 mil processos/threads ilustra uma medição particular, não um fator universal. Modelos de implementação e carga mudam os resultados.

## 4. Criar corretamente

```c
#include <pthread.h>
void *trabalho(void *arg);
pthread_t t;
int erro = pthread_create(&t, NULL, trabalho, argumento);
```

Os quatro argumentos são: endereço para receber a identificação, atributos (NULL significa padrões), função inicial e um ponteiro passado a ela. A função precisa ter assinatura `void *funcao(void *)`. Não force uma assinatura incompatível com cast.

Use `cc -std=c11 -Wall -Wextra -pthread solucao.c -o solucao`. O header correto é `<pthread.h>`, no singular. Funções Pthreads retornam um código de erro diretamente; diagnostique com `strerror(erro)`, em vez de assumir que errno foi atualizado para perror.

A thread pode começar antes que pthread_create retorne à criadora. Inicialize os argumentos antes da criação. Em falha parcial de um laço, não faça join em posições não criadas; recolha as threads já criadas e libere apenas os objetos cuja posse ainda pertence à main.

## 5. NULL em lugares diferentes

`pthread_create(&t, NULL, rotina, NULL)` tem dois NULL com significados diferentes: o segundo argumento seleciona atributos padrão; o quarto informa ausência de argumento específico. `pthread_join(t, NULL)` espera o término e descarta o valor retornado. Não significa “não esperar”.

A rotina pode não usar seu argumento; escreva `(void)arg;` para indicar isso. Um ponteiro void preserva o endereço; para usar os dados, converta para o tipo certo e desreferencie.

## 6. Espera, término e atributos

A assinatura correta é `int pthread_join(pthread_t thread, void **retval)`. Join espera uma thread joinable específica. Ele não escolhe arbitrariamente a próxima thread terminada. Cada thread deve ter um único responsável por join. Chamadas simultâneas de join para o mesmo alvo têm resultado indefinido; não se deve depender da promessa de ESRCH feita no material. Veja a documentação de [`pthread_join`](https://man.openbsd.org/OpenBSD-7.8/pthread_join.3).

Retornar da rotina de uma thread equivale ao término dessa thread com o ponteiro de retorno. `pthread_exit(valor)` encerra a chamadora. Retornar de main ou chamar exit encerra o processo, inclusive outras threads. Chamar pthread_exit na main pode deixar as demais vivas, mas não substitui a organização da coleta de resultados e da vida útil dos dados.

Uma thread detached tem recursos de execução recuperados automaticamente ao terminar e não pode ser recolhida com join. Isso não libera automaticamente todo malloc feito por ela nem destrava arbitrariamente seus mutexes.

Para atributos: declare `pthread_attr_t`, inicialize com `pthread_attr_init`, ajuste com setters, use em create e destrua com `pthread_attr_destroy`. A aula lista estado detached, tamanho/endereço de pilha, escopo e política/parâmetros/herança de escalonamento. Tipos e suporte variam; consulte os headers para a assinatura real, como `size_t` em tamanho de pilha. Um atributo destruído após create não destrói a thread criada.

## 7. O famoso erro de passar &i

```c
for (int i = 0; i < 10; ++i)
    pthread_create(&t[i], NULL, rotina, &i); /* incorreto */
```

Todas as threads recebem o mesmo endereço. O valor pode mudar antes de uma thread lê-lo; duas podem observar o mesmo índice; a última pode observar 10 e acessar fora do vetor. Há ainda concorrência entre escrita de i e leitura pelas threads, além de possíveis problemas de vida útil. O argumento de create não é uma cópia automática do objeto apontado.

Duas correções:

- Vetor estável de argumentos, um elemento por thread, mantido vivo até os joins.
- Um malloc por argumento, preenchido antes de create; a thread copia os campos e libera o objeto, se esse for o acordo de propriedade.

Malloc resolve o problema quando cria **objetos distintos e estáveis**. Alocar uma vez e continuar sobrescrevendo o mesmo objeto mantém o problema. Sleep pode torná-lo mais visível; não fornece sincronização.

## 8. Retornar resultados sem ponteiro pendurado

Uma variável local automática deixa de existir quando a rotina retorna. Portanto, `return &soma_local` é inválido. Alternativas: resultado alocado no heap e liberado pela main após join; ou campo de uma estrutura externa que permaneça viva até o join.

```c
void *resultado = NULL;
int erro = pthread_join(t, &resultado);
if (erro == 0 && resultado != NULL) {
    long long soma = *(long long *)resultado;
    free(resultado);
    /* usar soma */
}
```

A rotina aloca `long long *p = malloc(sizeof *p)`, calcula `*p` e retorna p. Verifique falha de malloc e defina como relatá-la. Evite converter diretamente int para ponteiro como nos exemplos de aula: não é uma forma geral de retornar dados portavelmente. Para exercícios de soma, um objeto real é mais claro.

O exemplo do dado ilustra retorno, mas números aleatórios não fornecem um gabarito fixo; valide a faixa e o protocolo de memória. Os arquivos `receiving_values` e `receiving_values_dice` citados no roteiro não estão em materiais. As atividades aqui preservam as perguntas e oferecem variantes de treino explicitamente identificadas.

## 9. Soma de dez primos e posição do join

Os primos são `[2,3,5,7,11,13,17,19,23,29]`. Primeira metade soma 28, segunda 101, total 129. A anotação `[2,..,27]` no roteiro 4.2 é inconsistente: 27 não é primo.

Crie as duas threads antes de recolher resultados. Fazer create seguido imediatamente de join dentro do mesmo laço permite no máximo um trabalhador daquele laço por vez. A main ainda existe, mas fica bloqueada durante o join. Com dois laços, todos os trabalhadores podem estar vivos ao mesmo tempo; a execução realmente simultânea depende das CPUs e do escalonador.

Para dez threads imprimindo elementos, a ordem de linhas é livre. Faça um printf por registro e inclua índice lógico para reconhecer duplicações. Pthread_self serve para identificar a thread, mas a representação imprimível de pthread_t depende da plataforma. Nos testes, coloque a identificação real em stderr e use índice lógico/valor em stdout, conforme o contrato. O corretor compara linhas sem impor ordem de criação.

## 10. Leitura dos exemplos de Aula 17

O global que começa em 20, é alterado para 40 na thread e lido após join deve ser observado como 40 pela main. Os loops de x/o ilustram fluxos concorrentes sem término, não um bom programa de teste finito. O exemplo de cinco Hello mostra criação e identificação lógica; evite casts diretos int/ponteiro. O exemplo interativo que cria até 100 threads precisa limitar a quantidade, limitar scanf e liberar os nomes; um laço infinito com vetor de 100 posições não é seguro. O exemplo que “retorna 99” pode ser reescrito retornando um objeto alocado para eliminar a dependência de conversões de ponteiros.

## 11. Threads Java e comparação com C

O material apresenta herdar Thread ou implementar Runnable, com gerenciamento pela JVM. Isso é suficiente para entender o paralelo conceitual com uma rotina Pthreads. A frase “a JVM só suporta um processo” não impede programas Java de criarem processos externos. Também não se pode concluir que C sempre vence Java em desempenho: JIT, GC, carga, algoritmos e bibliotecas mudam o resultado. Compare programas equivalentes e medições; não use a linguagem como prova.

Antes de seguir, você deve conseguir explicar a vida útil de cada ponteiro, a função de cada NULL, a ordem dos joins e a diferença entre concorrência, paralelismo e saída intercalada.
