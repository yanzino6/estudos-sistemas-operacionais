# Revisão manual — Buffer limitado em C

Estado encapsulado, while para vazio/cheio, atualizações sob mutex, signal para condição correspondente. Consumidora realmente passa pelo buffer; não somar diretamente na main. Ambas threads terminam; destruir condições/mutex após joins.

Marque cada requisito como atendido, ausente ou incorreto. Corrija os ausentes/incorretos e execute os casos novamente. A aprovação automática cobre somente o contrato objetivo; não atribui nota ao pseudocódigo ou à explicação.
