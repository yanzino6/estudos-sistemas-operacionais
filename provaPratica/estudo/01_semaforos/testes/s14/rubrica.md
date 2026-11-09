# Revisão manual — Buffer sem mutex e publicação fora de ordem

Declare hipótese de um único escritor por índice e semáforos com sincronização de memória. Não generalize SPSC para MPMC. No contraexemplo, P1 reserva slot0 e pausa, P2 escreve slot1 e publica item; consumidor tenta slot0. Também explique deadlock ao esperar vagas segurando mutex.

Marque cada requisito como atendido, ausente ou incorreto. Corrija os ausentes/incorretos e execute os casos novamente. A aprovação automática cobre somente o contrato objetivo; não atribui nota ao pseudocódigo ou à explicação.
