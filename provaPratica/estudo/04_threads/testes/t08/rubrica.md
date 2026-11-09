# Revisão manual — Posição de pthread_join

Até dez trabalhadores vivos no primeiro desenho; no máximo um trabalhador do laço no segundo. Main continua sendo uma thread, mas espera no join. Hardware pode limitar execução simultânea; não aferir paralelismo apenas por ordem dos prints.

Marque cada requisito como atendido, ausente ou incorreto. Corrija os ausentes/incorretos e execute os casos novamente. A aprovação automática cobre somente o contrato objetivo; não atribui nota ao pseudocódigo ou à explicação.
