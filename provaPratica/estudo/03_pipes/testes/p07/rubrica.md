# Revisão manual — Capacidade do pipe e alarm

Capacidade é finita e dependente do sistema. PIPE_BUF não é capacidade. Alarm interno é reagendado a cada passo; externo limita o tempo desde início. Printf/exit no handler não são uma estratégia robusta de sinal.

Marque cada requisito como atendido, ausente ou incorreto. Corrija os ausentes/incorretos e execute os casos novamente. A aprovação automática cobre somente o contrato objetivo; não atribui nota ao pseudocódigo ou à explicação.
