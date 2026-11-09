# Requisitos de implementação — x05

Handshake com mutex de controle distinto do mutex de recurso. Verificar EBUSY, não confundir erro com posse. A thread que adquire o mutex é quem o libera.

O script compila e executa todos os casos. Confira estes requisitos no código: resultados corretos não provam que você usou a primitiva exigida. Não há entrega de resposta teórica/JSON; a entrega é o programa C.
