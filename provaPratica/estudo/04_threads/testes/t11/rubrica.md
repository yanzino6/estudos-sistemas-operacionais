# Revisão manual — ULT, KLT e recursos compartilhados

Cada thread tem contexto/pilha e compartilha espaço de endereçamento. M:1 puro não explora simultaneidade de vários núcleos internamente; syscall bloqueante pode parar toda a entidade. 1:1 permite kernel escalonar cada thread. M:N combina níveis e adiciona complexidade.

Marque cada requisito como atendido, ausente ou incorreto. Corrija os ausentes/incorretos e execute os casos novamente. A aprovação automática cobre somente o contrato objetivo; não atribui nota ao pseudocódigo ou à explicação.
