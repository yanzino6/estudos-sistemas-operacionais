# Revisão manual — Pipeline ping | grep

Nenhum system/popen/shell para substituir a construção do pipeline. Conferir argumentos de exec, stdin/stdout redirecionados, pai sem extremidades abertas e waitpid para ambos. Não hardcodar a saída. A verificação de uso de chamadas é manual.

Marque cada requisito como atendido, ausente ou incorreto. Corrija os ausentes/incorretos e execute os casos novamente. A aprovação automática cobre somente o contrato objetivo; não atribui nota ao pseudocódigo ou à explicação.
