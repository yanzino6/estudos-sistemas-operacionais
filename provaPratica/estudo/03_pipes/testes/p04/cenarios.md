# Casos para responder

## a

Pai mantém fd de leitura aberto, filho terminou; escrita pequena que cabe necessariamente gera SIGPIPE? Booleano.

## b

Buffer vazio, pai ainda mantém escrita aberta: leitor recebe EOF imediatamente? Booleano.

## c

Todos os escritores fecharam e buffer foi drenado: retorno de read?

## d

Read num descritor fechado representa EOF? Booleano.
