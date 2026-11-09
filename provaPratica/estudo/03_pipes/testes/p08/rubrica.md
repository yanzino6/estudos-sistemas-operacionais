# Revisão manual — FIFO com dois escritores

Criar com mkfifo, open apropriado, leitura com delimitação limitada, close e unlink. Distinguir persistência do nome de persistência de dados. Ler até EOF sem coordenação pode perder uma sessão futura. Abertura não bloqueante de leitura sem escritor pode ter sucesso; escrita sem leitor dá ENXIO.

Marque cada requisito como atendido, ausente ou incorreto. Corrija os ausentes/incorretos e execute os casos novamente. A aprovação automática cobre somente o contrato objetivo; não atribui nota ao pseudocódigo ou à explicação.
