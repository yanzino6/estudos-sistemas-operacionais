# Requisitos de implementação — p07

Não fixar capacidade do pipe nem tratar EAGAIN como perda de dados. Não usar alarm/printf em handler. Resultado deve vir dos bytes recebidos, não de n*120 na main.

O script compila e executa todos os casos. Confira estes requisitos no código: resultados corretos não provam que você usou a primitiva exigida. Não há entrega de resposta teórica/JSON; a entrega é o programa C.
