# Revisão manual — Implementar Hoare e Hansen com semáforos

Hoare: mutex=1,next=0,next_count=0; uma fila por condição e prioridade de next no epílogo/wait. Signal suspende sinalizador somente se há waiter. Sinaliza-e-continua: sinalizador mantém entrada e waiter readquire antes de retornar. Registro de espera deve ocorrer antes de liberar mutex; fila com semáforo privado evita roubo de sinal.

Marque cada requisito como atendido, ausente ou incorreto. Corrija os ausentes/incorretos e execute os casos novamente. A aprovação automática cobre somente o contrato objetivo; não atribui nota ao pseudocódigo ou à explicação.
