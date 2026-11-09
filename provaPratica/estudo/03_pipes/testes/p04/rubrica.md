# Revisão manual — Experimento pipe1.c e EOF

Distinguir leitor aberto de processo efetivamente lendo. Escrita pequena cabe, repetição pode encher. Sem leitores: SIGPIPE/EPIPE; fd inválido: EBADF; EOF exige buffer drenado e zero escritores. Corrigir fd[2] fora do vetor no código de erro do slide.

Marque cada requisito como atendido, ausente ou incorreto. Corrija os ausentes/incorretos e execute os casos novamente. A aprovação automática cobre somente o contrato objetivo; não atribui nota ao pseudocódigo ou à explicação.
