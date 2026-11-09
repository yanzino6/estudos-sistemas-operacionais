# Ressalvas e correções didáticas

Use estas notas para não transformar erros de transcrição ou simplificações dos slides em regras de implementação. Os enunciados originais continuam identificados no catálogo.

| Local | Ressalva usada no estudo |
|---|---|
| Aula 11, p.22 | Reservar índice e publicar fora da ordem pode falhar com múltiplos produtores; há contraexemplo no conteúdo, não uma recomendação para copiar a otimização |
| Aula 11.2, p.6 | C faz dois P(s1), mas A publica apenas um V(s1); a solução apresentada pode bloquear. s02 descreve correção mantendo as arestas |
| Aula 11.2, p.13 | Montanha-russa contém nomes e trechos do pombo; usar o enunciado da p.14 e separar fases |
| Aula 11.2, p.20 | A própria aula pergunta pelo defeito do filme; a autorização de entrada não comprova fim da exibição |
| Aula 12, pp.20–23 | Ciclo de cinco filósofos tem cinco garfos; ausência de starvation precisa de política/hipótese de justiça, não só teste dos vizinhos |
| Aula 12 / Lista 7 | Para perguntas, manter a disciplina “sinaliza e continua” chamada Hansen no material; outros textos usam Mesa. Declarar semântica evita ambiguidade |
| Aula 14, p.22 | `pid=fork()<0` precisa de parênteses ou atribuição separada |
| Aula 14, p.24 | `fd[2]` está fora de um vetor de dois elementos; mensagens de erro devem usar stderr/descritor 2, não essa posição |
| Aula 14, p.27 | FD inválido produz EBADF; EOF exige pipe drenado e nenhum escritor. SIGPIPE depende de ausência de leitores, não de qualquer FD fechado |
| Aula 14, p.27 | PIPE_BUF é limite de atomicidade e não capacidade total do pipe |
| Aula 14, pp.49–50 | FIFO O_RDONLY|O_NONBLOCK pode abrir sem escritor; ENXIO vale para abertura de escrita sem leitor. É write, não read, que reporta EPIPE nessa situação |
| Aula 14, p.53 | Permissão octal é `0660`, não o inteiro decimal `660` |
| Aula 17, p.3 | Header é `<pthread.h>`, não `<pthreads.h>` |
| Aula 17, pp.9–12 | Conferir tipos reais: pthread_t, void** no join, size_t para tamanho de pilha. Joins simultâneos do mesmo alvo têm resultado indefinido |
| Aula 17, p.10 | pthread_exit encerra uma thread; retornar da main encerra o processo. Não equivale a liberar todos os mallocs e mutexes dela |
| Aula 17, exemplos | Evitar retorno de int por cast direto para ponteiro e impressão não portátil de pthread_t; usar objetos com vida útil válida e IDs lógicos para testes |
| Roteiro 4.1 | Referência ao escalonador O(1) é histórica; não é requisito de NPTL nem descrição atual universal |
| Roteiro 4.2, Ex5 | “Use o programa do Exercício 5” é autorreferência; adotado Ex4 pela sequência da atividade |
| Roteiro 4.2, Ex6 | `[2,..,27]` não descreve dez primos; adotado o vetor explícito do roteiro 4.1, terminando em 29 |
| Aula 17, Java/C | JVM gerenciar threads não impede criação de processos externos; desempenho C/Java depende da carga e implementação |

Documentação usada para conferir os detalhes de API: [join no manual OpenBSD](https://man.openbsd.org/OpenBSD-7.8/pthread_join.3), [open POSIX](https://man7.org/linux/man-pages/man3/open.3p.html), [pipe no projeto Linux man-pages](https://man7.org/linux/man-pages/man7/pipe.7.html) e [condition wait no manual illumos/OmniOS](https://man.omnios.org/man3c/pthread_cond_wait). As análises de intercalamento são raciocínios explicitados no conteúdo, não citações dessas APIs.
