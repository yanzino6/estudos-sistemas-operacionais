# Requisitos de implementação — s05

Use semáforos e contadores protegidos. Eventos representam alterações reais do estado, impressos na mesma região crítica da alteração. Sem sleep para garantir ordem; número finito de rodadas e join em todas as threads. Use no máximo quatro semáforos; o protocolo de encerramento deve usar esses mesmos objetos.

O script compila e executa todos os casos. Confira estes requisitos no código: resultados corretos não provam que você usou a primitiva exigida. Não há entrega de resposta teórica/JSON; a entrega é o programa C.
