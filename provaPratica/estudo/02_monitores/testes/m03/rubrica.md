# Requisitos de implementação — m03

Implemente entrada/saída, wait e signal de monitor Hoare usando mutex semafórico, fila urgente next e fila por condição; não use pthread_cond_t. Acordado recebe monitor antes do sinalizador continuar. Não substituir por um vetor pronto de resultados. Testes cobrem buffer de uma posição, vários produtores/consumidores e totais não divisíveis por C.

O script compila e executa todos os casos. Confira estes requisitos no código: resultados corretos não provam que você usou a primitiva exigida. Não há entrega de resposta teórica/JSON; a entrega é o programa C.
