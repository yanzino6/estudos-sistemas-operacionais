# Parte 3 — Processos, pipes, redirecionamento e FIFOs

Fontes: Aula 14 (55 páginas) e roteiro_lab03 (4 páginas). Meta prática: desenhar o fluxo de dados, criar exatamente os processos necessários e fechar cada descritor no lugar certo.

## 1. Por que IPC?

Processos têm espaços de endereçamento separados. Após fork, alterar uma variável no filho não altera a variável do pai. O sistema oferece comunicação entre processos (IPC): memória compartilhada, pipes, sinais, mensagens e RPC são alternativas apresentadas na aula. Memória compartilhada evita cópias em muitos cenários, mas requer protocolo de sincronização. Comunicação pelo núcleo oferece operações de espera e transporte; não elimina a necessidade de definir mensagens e ordem.

Um pipe é um fluxo de bytes FIFO, com uma ponta de leitura e outra de escrita. Ele não possui cursor reposicionável e não preserva, para read, os limites de cada chamada write. Dois writes podem ser recebidos juntos, ou uma mensagem pode exigir várias leituras.

## 2. Descritores e fork

Cada processo tem uma tabela de descritores: 0 é entrada padrão, 1 saída padrão, 2 erro padrão. Um descritor é um índice dessa tabela, não o dado armazenado no pipe. `pipe(fd)` preenche `fd[0]` para leitura e `fd[1]` para escrita e retorna zero em sucesso.

```c
int fd[2];
if (pipe(fd) == -1) { perror("pipe"); return 1; }
pid_t pid = fork();
if (pid == -1) { perror("fork"); return 1; }
if (pid == 0) {
    close(fd[1]); /* filho leitor */
    /* read(fd[0], ...) */
    close(fd[0]);
    _exit(0);
}
close(fd[0]);     /* pai escritor */
/* write(fd[1], ...) */
close(fd[1]);
/* waitpid(pid, &status, 0), tratando erros */
```

Crie o pipe antes de fork para que ambos herdem referências aos mesmos objetos do núcleo. Fechar um descritor em um processo não fecha automaticamente a cópia do outro. O pipe persiste enquanto houver referências abertas; não necessariamente morre junto de seu criador. O padrão de ancestral comum dos slides é o uso clássico por herança; descritores também podem ser transferidos por outros mecanismos fora do escopo da prova.

Fork retorna zero no filho e o PID do filho no pai. Cuidado com `pid=fork()<0`: pela precedência de operadores, isso atribui o resultado da comparação. Use `pid = fork()` e depois teste, ou `if ((pid = fork()) < 0)`.

## 3. Read, write e encerramento

Assinaturas úteis: `ssize_t read(int fd, void *buf, size_t n)` e `ssize_t write(int fd, const void *buf, size_t n)`. O retorno é a quantidade efetivamente transferida; -1 indica erro, com detalhe em errno. Errno não é uma variável de ambiente do shell.

| Situação | Resultado esperado no modo bloqueante |
|---|---|
| Pipe vazio, mas ainda há escritores abertos | read espera |
| Pipe esvaziado e nenhuma ponta de escrita aberta | read retorna 0: EOF |
| Há bytes no pipe, mesmo após último escritor fechar | read ainda entrega esses bytes antes do EOF |
| Pipe cheio com leitores existentes | write pode bloquear |
| Nenhuma ponta de leitura aberta | write provoca SIGPIPE; se tratado/ignorado, falha com EPIPE |
| Descritor inválido/fechado | falha com EBADF; não é EOF |

Essas distinções corrigem simplificações da página 27. Deixar uma ponta de escrita esquecida no pai impede o filho de observar EOF. Deixar uma ponta de leitura aberta pode impedir SIGPIPE, mesmo que ninguém vá ler.

Para transmitir exatamente sizeof(int) ou um vetor, use laços que avancem o ponteiro pelos bytes já transferidos. Se ocorrer EINTR, repita a operação; se read retornar 0 antes do tamanho esperado, detecte mensagem truncada. Envie valores, não ponteiros: um endereço não representa conteúdo transferível entre espaços de memória.

O limite `PIPE_BUF` refere-se à atomicidade de escritas concorrentes, não à capacidade total do pipe. `_POSIX_PIPE_BUF` é um mínimo definido pelo padrão. Não codifique “o pipe comporta 512 bytes”. Escritas maiores que o limite atômico podem se intercalar; leituras não passam a ser mensagens só porque a escrita foi atômica.

## 4. Um protocolo simples evita ambiguidades

Para um vetor, combine: primeiro tamanho n, depois n inteiros. Valide n antes de usar memória. Os processos do mesmo programa podem trocar a representação binária dos inteiros; para máquinas/formatos diferentes seria preciso definir representação e endianness.

Se preferir texto, delimite linhas e trate números incompletos entre reads. Não use strlen em bytes lidos sem garantir terminador NUL. No exemplo `double.c/son.c`, cada filho escreve 11 bytes; o leitor precisa acumular um registro completo e imprimir somente bytes válidos. Não assuma que uma única read entrega exatamente os 11 bytes solicitados.

## 5. Dup, dup2 e exec

`dup(fd)` devolve o menor descritor livre apontando para o mesmo recurso aberto. `dup2(oldfd,newfd)` faz newfd apontar para o recurso de oldfd, fechando a associação anterior de newfd quando necessário. Portanto, `dup2(fd[1], STDOUT_FILENO)` redireciona a saída padrão para a escrita do pipe.

Exec substitui a imagem do processo; não cria outro PID. Descritores abertos são normalmente preservados, salvo os marcados close-on-exec. Em sucesso, exec não retorna. Em erro, imprima diagnóstico em stderr e encerre o filho com `_exit(127)`; não deixe que ele continue a lógica do pai.

Para `produtor | consumidor`:

1. Pai cria um pipe e dois filhos.
2. Filho produtor duplica a escrita para stdout, fecha ambas as pontas originais e executa o produtor.
3. Filho consumidor duplica a leitura para stdin, fecha as pontas originais e executa o consumidor.
4. Pai fecha ambas as pontas e espera os dois filhos.

Crie ambos antes de esperar. Esperar o produtor terminar antes de criar o consumidor pode travar quando o pipe encher. O exemplo de aula que substitui o próprio pai por `ls` também conecta ls a wc, mas um controlador separado facilita recolher e verificar o término de todos.

## 6. Os três exercícios do laboratório

### Anel com pai e dois filhos

```text
pai --x--> filho1 --x+5--> filho2 --x+15--> pai
```

São três pipes unidirecionais. Prepare uma tabela de posse:

| Processo | Lê | Escreve |
|---|---|---|
| Pai | pipe filho2→pai | pipe pai→filho1 |
| Filho1 | pipe pai→filho1 | pipe filho1→filho2 |
| Filho2 | pipe filho1→filho2 | pipe filho2→pai |

Cada processo fecha as outras quatro extremidades. Pai imprime `[PARENT] Resultado Final: valor!`. Um segundo fork só deve ocorrer no pai para que os dois filhos sejam irmãos. Trate retornos parciais e espere ambos.

### Ping filtrado por grep

O pedido original é `ping -c 5 google.com | grep rtt`. O primeiro exec recebe argumentos separados: `execlp("ping", "ping", "-c", "5", "google.com", (char *)NULL)`. O segundo executa grep com o argumento `rtt`. As aspas faltando no roteiro são erro tipográfico.

Esse comando depende de rede e da implementação do ping. No macOS, o resumo costuma usar outro rótulo. Os testes fornecem um comando ping controlado via PATH, mantendo grep real: isso testa a conexão sem depender de DNS, rede ou latência. Não use caminho absoluto para ping na versão de treino. Faça a execução real em Linux se quiser reproduzir o ambiente do enunciado. Uma saída correta não demonstra uso de fork/dup2; a rubrica exige revisar isso.

### Avô, pai e neto

Avô lê n e o vetor; pai multiplica por três; neto inverte; pai devolve; avô imprime. O neto deve ser criado pelo pai intermediário. O resultado de `[1,2,3,4,5]` é `[15,12,9,6,3]`. Use quatro canais direcionais para avô→pai, pai→neto, neto→pai e pai→avô, ou uma organização equivalente com protocolo explícito. O pai deve recolher o neto e o avô deve recolher seu filho. O limite original é 100 elementos.

## 7. Experimentos da aula

Em `pipe1.c`, retirar a leitura do filho não provoca imediatamente bloqueio se a mensagem ainda cabe no pipe. Se o pai mantém uma ponta de leitura aberta, não há “zero leitores” para disparar SIGPIPE. Ao repetir escritas, a capacidade eventualmente se esgota. Não use sleep como garantia de que o filho terminou; para uma experiência determinística, coordene ou espere o filho.

No exemplo de alarm, recolocar `alarm(20)` em cada iteração significa esperar vinte segundos desde o último agendamento: o alarme tende a aparecer quando uma escrita bloqueia. Colocar o alarme fora do laço limita o tempo total. `printf` e `exit` dentro de um tratador de sinal não são uma boa implementação robusta: prefira uma flag `volatile sig_atomic_t` e tratamento fora do handler, ou operações apropriadas a contexto de sinal.

Antes de fork, descarregue buffers de stdio quando necessário. A memória do buffer também é duplicada; `exit` em ambos pode duplicar texto pendente. `_exit` não descarrega stdio: se você usou printf no filho, deve descarregar explicitamente antes ou escolher outra organização de saída.

## 8. FIFOs: pipes com nome

Uma FIFO possui entrada no sistema de arquivos, permitindo que processos independentes a encontrem. Crie com `mkfifo("canal", 0600)`; abra com open; use read/write; feche e remova com unlink. O nome persiste até remoção, mas não é um arquivo regular que armazena permanentemente as mensagens.

Permissões são octais: `0660` permite leitura/escrita ao dono e grupo; `660` sem zero inicial é decimal e não expressa esse modo. A umask ainda pode retirar permissões. O primeiro caractere `p` em `ls -l` identifica a FIFO.

A abertura bloqueante espera a contraparte. No modo não bloqueante, abrir somente para leitura pode ter sucesso sem escritor; abrir somente para escrita sem leitor falha com ENXIO. A página 49 não distingue corretamente esses dois casos. A leitura vazia depende também de ainda existir escritor: sem escritores, EOF; com escritor e sem dados no modo não bloqueante, EAGAIN.

No exemplo writer/reader, mensagens incluem NUL como delimitador. A função de leitura deve respeitar o limite do buffer e detectar mensagem incompleta. O leitor não pode concluir que outro writer futuro nunca aparecerá só porque recebeu EOF de uma sessão; defina o ciclo de vida pretendido. Ao terminar, unlink remove o nome, independentemente de a FIFO ter sido criada via shell ou C.

## Checklist de prova

Desenhe processos e setas antes do código. Para cada ponta, identifique dono e momento de fechamento. Confira todos os retornos. Não aguarde um processo que depende de alguém ainda não criado. Não use o tamanho esperado como substituto do retorno de read. Não confunda fim de mensagem, fechamento e fim de arquivo. Reserve stdout para o resultado do protocolo de testes e stderr para diagnóstico.
