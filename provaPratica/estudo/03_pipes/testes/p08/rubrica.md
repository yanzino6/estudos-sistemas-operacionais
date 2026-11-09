# Requisitos de implementação — p08

mkfifo com permissões octais, open/read/write/close/unlink. Registros pequenos com escrita atômica e leitura recomposta. Coordenação de abertura, por exemplo canal auxiliar de prontidão, sem sleep. O corretor também verifica que canal.fifo não permaneceu.

O script compila e executa todos os casos. Confira estes requisitos no código: resultados corretos não provam que você usou a primitiva exigida. Não há entrega de resposta teórica/JSON; a entrega é o programa C.
