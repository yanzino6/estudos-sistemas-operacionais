# Revisão manual — Primos: &i versus malloc

Mostrar endereço compartilhado, leitura tardia, valor 10 fora dos índices 0..9 e data race. Malloc deve ser um por thread ou objetos independentes; alocar uma vez não resolve. Definir quem libera. Sleep revela, não corrige. Não prometer uma saída errada específica.

Marque cada requisito como atendido, ausente ou incorreto. Corrija os ausentes/incorretos e execute os casos novamente. A aprovação automática cobre somente o contrato objetivo; não atribui nota ao pseudocódigo ou à explicação.
