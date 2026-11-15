# Plano de desenvolvimento do Mini-Kernel - 1ª etapa

Plano preparado em 05/11/2026, a partir das dez páginas do PDF de especificação presente nesta pasta. Este documento contém orientações, entregas e verificações; não contém implementação.

## Prazo, disponibilidade e limites do plano

- Prazo do enunciado: **04/12/2026, às 07h59**.
- Meta de conclusão de todos os requisitos, inclusive análise e README: **26/11/2026**.
- Reserva exclusiva para revisar, corrigir e entregar: **27/11 a 03/12**. São sete dias corridos e cinco sessões em dias de semana.
- Envio planejado: **03/12**, sem depender da manhã de 04/12.
- Disponibilidade informada: **2 horas por dia, de segunda a sexta, desenvolvimento individual**.
- Planejamento conservador: 20/11 fica livre por ser feriado nacional. Fonte: [calendário oficial de 2026](https://www.gov.br/gestao/pt-br/assuntos/noticias/2025/dezembro/confira-o-calendario-oficial-de-feriados-nacionais-e-pontos-facultativos-em-2026).
- Capacidade: **15 sessões de desenvolvimento, totalizando 30 horas**, e **5 sessões de revisão, totalizando 10 horas**. Fins de semana livres. O dia 04/12 serve somente para conferir a submissão antes do prazo, se necessário.

O cronograma é apertado para um projeto com concorrência real. As duas horas de cada dia incluem compreensão, implementação, testes e registro. O plano não pressupõe horas extras, trabalho no feriado ou uso da semana de revisão para terminar funcionalidades. A viabilidade deve ser conferida nos marcos de 11, 13, 19 e 26/11; os resultados desses marcos valem mais que uma estimativa inicial.

**Exigência formal a resolver no primeiro dia:** a seção 7 determina trabalho em dupla. Desenvolver individualmente é a preferência informada, mas o enunciado não confirma que isso seja permitido. Confirme a autorização com o professor e o formato de identificação da entrega individual. Não invente uma segunda matrícula nem suponha uma dispensa.

## Como usar este guia comigo

No início da sessão, indique a data do plano, o último objetivo concluído e o problema atual. Meu papel será explicar o objetivo, ajudar a raciocinar sobre estruturas e sincronização, revisar suas decisões e interpretar os resultados. As orientações devem continuar sem código, salvo se você mudar explicitamente essa preferência.

Rotina sugerida de 120 minutos:

1. **10 minutos:** retomar o registro anterior e descrever o comportamento esperado do dia.
2. **70 minutos:** desenvolver a entrega principal, em passos pequenos.
3. **30 minutos:** executar os casos direcionados e investigar diferenças.
4. **10 minutos:** atualizar o README e registrar o que passou, o que falhou e o próximo passo.

Cada sessão termina com uma evidência concreta: uma entrada e seu resultado, um trecho de log validado, um cálculo conferido ou um diagnóstico de ferramenta. Não marque uma tarefa como concluída apenas porque ela compila. Faça um registro de versão recuperável após cada entrega validada.

Se um critério de conclusão falhar, registre a falha para abrirmos a próxima sessão por ela. Não construa a etapa seguinte sobre uma base de concorrência sabidamente incorreta. Se houver atraso, corte primeiro bônus, ferramentas auxiliares e melhorias estéticas. Se os requisitos obrigatórios deixarem de caber, revisaremos o cronograma e explicitaremos o risco; a reserva não deve desaparecer silenciosamente.

## Escopo obrigatório e relação com a nota

| Entrega | Conteúdo que precisa funcionar | Peso |
| --- | --- | --- |
| Monoprocessador | FCFS, RR e Prioridade Preemptiva | 3,0 |
| Multiprocessador GLOBAL | Uma fila compartilhada e de 1 a 8 CPUs | 2,0 |
| Multiprocessador POR_CPU | Filas locais, colocação por carga e migração | 1,5 |
| Métricas | Medidas individuais, médias e medidas do sistema | 1,0 |
| README e análise | Decisões, regras de sincronização, experimentos e discussão | 1,5 |
| Organização | Módulos, clareza, comentários e consistência | 1,0 |

O bônus de afinidade de CPU fica fora das 30 horas planejadas. Ele pode interferir nos desempates obrigatórios e exige medição própria. Só deve ser considerado depois de todo o obrigatório aprovado, sem consumir a reserva. O PDF anuncia até 1,0 de bônus, mas o único item descrito nas páginas fornecidas é afinidade, até 0,5; não há outro bônus especificado para planejar.

## Decisões que precisam estar claras antes da implementação

### Separação de responsabilidades

Organize o projeto em responsabilidades de entrada/configuração, processos e threads, filas de prontos, políticas, coordenação dos escalonadores, tempo, log/métricas e ciclo de vida. Isso não exige um arquivo para cada responsabilidade. O objetivo é evitar duplicar todo o sistema para cada política ou versão e permitir extensão na segunda etapa.

Defina contratos em português: quem cria e destrói cada objeto, quem pode alterar cada campo, qual mutex protege cada dado e quais condições acordam cada thread. As políticas escolhem o próximo processo; a infraestrutura controla execução, quantum e sincronização compartilhada.

### Modelo de dados e de execução

- O BCP representa o processo: PID, chegada, duração, prioridade, quantidade de threads, tempo restante, estado, CPU, primeiro despacho, finalização e recursos de sincronização.
- O TCB identifica a qual BCP uma thread pertence e seu índice local. Os argumentos precisam permanecer válidos enquanto a thread os utilizar.
- PID vem da entrada; não é o identificador do processo do sistema operacional nem uma posição garantida em vetor.
- Os processos começam com dados inicializados, mas só entram na fila quando chega seu instante de chegada.
- Cada CPU simulada possui sua própria thread escalonadora. Um processo pode executar em somente uma CPU simulada por vez, mesmo tendo várias threads POSIX.
- O programa usa pthreads reais e sincronização real; as CPUs do trabalho são recursos simulados.
- As threads de um processo só são criadas na chegada e ficam bloqueadas até o despacho.
- O início do tempo, T0, ocorre logo após a leitura do arquivo, conforme o PDF. Todas as medições usam a mesma origem em milissegundos, com gettimeofday.

### Regras de tempo que precisam ser preservadas

1. A fatia mínima é 100 ms. Duração, chegada e quantum das entradas de teste são múltiplos de 100.
2. Todas as threads de um processo participam da mesma fatia. **Juntas descontam exatamente 100 ms**, independentemente de serem 1, 2, 3 ou 4 threads.
3. Para três threads, por exemplo, a divisão de uma fatia é 34, 33 e 33 ms; as três simulam a mesma passagem de 100 ms. Não transforme três threads em três CPUs para esse processo.
4. Controle as fronteiras entre fatias: uma thread não pode iniciar a próxima enquanto outra ainda fecha a anterior. Uma barreira é sugerida pelo enunciado; outro mecanismo equivalente precisa garantir o mesmo alinhamento.
5. As alterações do escalonador respeitam fronteiras de fatia. A preempção de RR e Prioridade ocorre somente no fim do quantum.
6. Na finalização, a primeira detecção registra o evento uma única vez e acorda as demais threads. A saída precisa ser compatível com a sincronização da fatia: ninguém pode ficar esperando participantes que já encerraram.
7. No RR e na Prioridade, finalizar antes do quantum gera o evento imediatamente, mas a CPU continua reservada até o fim daquele quantum. Esse intervalo ocioso não aumenta a utilização.
8. Quanta de CPUs diferentes começam nos respectivos despachos e são independentes. Não crie um quantum global sincronizado para todas as CPUs.
9. Não substitua os tempos medidos por valores artificiais para igualar exemplos. Pequenos atrasos de execução real são previstos pelo enunciado.

### Desempates e operações que precisam ser coordenados

- Chegadas simultâneas entram em ordem crescente de PID, mesmo que o arquivo venha desordenado.
- No mesmo instante previsto de chegada e fim de quantum, a chegada entra antes da reinserção. Não dependa de qual thread o Linux acordar primeiro.
- Quando mais de uma CPU estiver livre, a de menor índice atende primeiro.
- Em Prioridade, menor número significa maior prioridade. Igualdade usa ordem FIFO de entrada na fila, não PID.
- Na fila GLOBAL, reinserção ao fim do quantum e escolha do próximo processo ocorrem na mesma seção crítica, antes de notificar CPUs ociosas.
- No modo POR_CPU, a carga para colocação é quantidade de processos na fila local mais o processo em execução, e não soma das durações.
- A migração retira o último processo da fila local mais longa, considerando apenas processos à espera. O processo em execução não é roubado. Empate entre doadoras usa o menor índice.
- Toda inserção em qualquer fila deve permitir nova tentativa de roubo pelas CPUs ociosas.

### Sincronização e encerramento

Documente uma ordem global de aquisição dos mutexes, incluindo a ordem entre filas quando uma operação acessar duas. Aplique-a também à migração, ao log e à consulta de estado. O mutex do BCP protege estado e tempo restante; cada fila tem seu mutex; o buffer de log tem proteção própria.

Cada espera por variável de condição verifica novamente seu predicado em laço. Notificações não substituem o estado compartilhado protegido. Nunca durma simulando execução enquanto mantém o mutex da fila. Escalonadores sem trabalho aguardam uma condição em vez de girar em espera ocupada; a geradora pode aguardar com pausas curtas.

Encerrar exige simultaneamente todas as filas vazias, geradora concluída e nenhuma CPU executando processo. Preserve também a retenção de quantum prevista antes de liberar uma CPU. Separe conceitualmente processo finalizado, CPU reservada e CPU executando: eles não são equivalentes.

A thread principal aguarda geradora, escalonadores e todas as threads de processos; grava o resultado completo; destrói recursos de sincronização quando ninguém mais os usa; libera BCPs, TCBs, vetores, filas e log. O evento de encerramento global deve aparecer uma única vez.

## Calendário de desenvolvimento: 15 sessões de duas horas

### 05/11, quinta-feira - contrato do projeto e ambiente

**Objetivo:** eliminar dúvidas de entrega e preparar um ambiente reproduzível.

- [ ] Ler as seções 2, 4, 5, 6 e 7 com atenção às regras destacadas neste plano.
- [ ] Resolver com o professor a questão do desenvolvimento individual e registrar a resposta.
- [ ] Obter do Classroom a carga de análise, os testes públicos e eventuais instruções adicionais. Eles não estavam na pasta ao preparar este plano.
- [ ] Definir as responsabilidades dos módulos e uma primeira ordem global de mutexes.
- [ ] Preparar compilação dos alvos monoprocessador e multiprocessador, produzindo trabSO, com pthreads e os avisos exigidos pelo PDF.
- [ ] Confirmar acesso a Ubuntu/LabGrad ou ambiente Linux equivalente e às ferramentas Valgrind e Helgrind. O ambiente atual é macOS; a validação de entrega precisa ocorrer em Linux desde cedo.
- [ ] Criar a estrutura inicial do README e o registro de progresso.

**Verificação e conclusão:** ambos os alvos são construídos no Linux; há uma lista de responsabilidades e de recursos compartilhados. A indisponibilidade de ambiente ou arquivos externos fica registrada como pendência com ação definida, sem substituir material oficial por suposição.

### 06/11, sexta-feira - entrada, BCP, TCB e relógio

**Objetivo:** carregar qualquer entrada válida do formato exigido.

- [ ] Ler o único argumento de entrada e os cabeçalhos na ordem especificada: CPUs, quantum, política, fila e quantidade de processos.
- [ ] Carregar os processos sem assumir ordem de chegada nem sequência de PID; preparar a ordem por chegada e PID para a geradora.
- [ ] Inicializar duração restante, estado, identificação de CPU, indicadores de primeiro despacho/finalização e recursos do BCP.
- [ ] Planejar a vida útil dos TCBs e vetores de identificadores de threads; a criação das threads fica para o dia 10.
- [ ] Estabelecer o relógio em milissegundos com origem única logo após a leitura.
- [ ] Respeitar os valores ignorados: quantum em FCFS e modo de fila na versão monoprocessador.

**Verificação e conclusão:** conferir uma entrada desordenada com PIDs não consecutivos, prioridades de 1 a 5 e 1 a 4 threads. Confirmar que cada dado vai ao processo certo e fazer a primeira checagem de liberação de memória. Entradas de teste são garantidamente válidas; não gastar a sessão com um validador genérico de formatos inválidos.

### 09/11, segunda-feira - filas e registro de eventos

**Objetivo:** disponibilizar as operações usadas por todas as políticas.

- [ ] Criar as operações de inserir ao fim, remover do início, selecionar/remover um elemento e retirar o último; definir como consultar tamanho com segurança.
- [ ] Preservar a ordem de entrada, que será usada nos desempates de prioridade.
- [ ] Implementar o buffer de eventos protegido e o destino único log_execucao_minikernel.txt.
- [ ] Registrar tempo, tipo de evento, política, PID e CPU quando aplicável; assegurar saída cronológica e preservar a sequência causal de uma mesma CPU.
- [ ] Planejar os registros de primeiro despacho, finalização, execução por CPU e último PID por CPU para as métricas futuras.

**Verificação e conclusão:** exercitar fila vazia, um elemento, remoção intermediária e retirada do último, sem perda ou duplicação. Conferir nomes e estrutura dos eventos com a seção 4.2. A saída final não deve depender de mensagens de depuração no terminal.

### 10/11, terça-feira - geradora e execução por fatias

**Objetivo:** executar um processo multithread corretamente antes de disputar a CPU.

- [ ] Criar a geradora que aguarda as chegadas, cria as threads no momento previsto e insere processos prontos.
- [ ] Manter as threads bloqueadas até o despacho e ativar a indicação de geradora concluída após a última chegada.
- [ ] Desenvolver uma fatia de 100 ms com divisão exata do desconto entre as threads.
- [ ] Alinhar início/fim das fatias e proteger estado e tempo restante.
- [ ] Garantir finalização única, despertar de todos os participantes e saída sem bloqueio em barreira ou condição.

**Verificação e conclusão:** um processo de 300 ms deve consumir três fatias tanto com 1 como com 4 threads; com 3 threads, cada fatia desconta 34, 33 e 33 ms. Não deve haver trabalho antes do despacho, tempo restante negativo ou múltiplas finalizações. Fazer uma primeira inspeção com Helgrind; medições instrumentadas não servem para comparar tolerância temporal.

### 11/11, quarta-feira - FCFS monoprocessador e ciclo completo

**Objetivo:** concluir a primeira execução completa do sistema.

- [ ] Criar o escalonador CPU0 e o despacho FIFO sem quantum nem preempção.
- [ ] Liberar a CPU assim que o processo terminar e aguardar novas chegadas quando a fila estiver vazia.
- [ ] Registrar primeiro despacho, execução, finalização e encerramento global.
- [ ] Encerrar corretamente com filas vazias, geradora concluída e ausência de execução; fazer todas as esperas de término e a limpeza de recursos disponíveis até aqui.
- [ ] Documentar o caminho completo da entrada até o log.

**Verificação e conclusão:** reproduzir a sequência do exemplo FCFS monoprocessador do PDF: PID 1, depois 3, depois 2; finalizações próximas de 1000, 2500 e 4500 ms. Testar também uma primeira chegada tardia e um intervalo sem processos prontos. Rodar Valgrind no ciclo completo.

**Marco 1:** FCFS monoprocessador funciona do início ao fim, sem travar. Se isso não estiver estável, a próxima sessão começa pela correção.

### 12/11, quinta-feira - Round Robin

**Objetivo:** adicionar quantum sem quebrar a execução por fatias.

- [ ] Controlar o quantum a partir de cada despacho.
- [ ] Ao fim do quantum, pausar nas fronteiras de fatia, registrar esgotamento e reinserir apenas processos ainda não finalizados.
- [ ] Garantir que chegadas no mesmo instante previsto entrem antes do processo reinserido.
- [ ] Preservar a CPU reservada até o fim do quantum se o processo finalizar antes; registrar a finalização no momento real e excluir a retenção ociosa da execução efetiva.
- [ ] Registrar novo despacho mesmo quando o único processo for selecionado novamente.

**Verificação e conclusão:** testar dois processos alternando, chegada no fim exato do quantum, duração menor/igual/maior que quantum e um único processo de vários quanta. Um processo de 100 ms com quantum de 500 ms finaliza perto de 100 ms, mas o próximo só recebe aquela CPU perto de 500 ms. Não emitir Quantum esgotado para processo já finalizado.

### 13/11, sexta-feira - Prioridade Preemptiva

**Objetivo:** completar as três políticas monoprocessador.

- [ ] Reutilizar o controle de quantum e de retenção do RR.
- [ ] Selecionar o menor número de prioridade; em empate, usar o processo há mais tempo na fila.
- [ ] Recolocar o processo ainda ativo no fim da fila antes da nova seleção.
- [ ] Impedir interrupção no meio do quantum mesmo que chegue prioridade superior.
- [ ] Manter eventos de esgotamento e despacho quando o mesmo PID for selecionado novamente.
- [ ] Reexecutar os casos essenciais de FCFS e RR e atualizar o README das políticas.

**Verificação e conclusão:** uma chegada mais prioritária espera o fim do quantum; prioridades iguais se alternam como RR; o único processo de maior prioridade pode executar vários quanta consecutivos. Conferir que a futura contagem de contexto não confunde novo despacho do mesmo PID com troca.

**Marco 2:** as três políticas monoprocessador estão estáveis. Evitar expandir para várias CPUs com defeitos conhecidos nessa base.

### 16/11, segunda-feira - multiprocessador com fila GLOBAL

**Objetivo:** permitir execução de processos distintos em paralelo.

- [ ] Criar a quantidade de escalonadores indicada pela entrada, de 1 a 8.
- [ ] Compartilhar a fila global com exclusão mútua e impedir despacho duplicado do mesmo processo.
- [ ] Dar controle e relógio de quantum independentes a cada CPU.
- [ ] Implementar o desempate por menor índice entre CPUs livres.
- [ ] Reaproveitar as três políticas e adaptar encerramento e limpeza para todos os escalonadores.

**Verificação e conclusão:** reproduzir o exemplo FCFS com 2 CPUs: PID 1 em CPU0, PID 3 em CPU1 e PID 2 depois em CPU0, terminando perto de 3000 ms. Testar mais CPUs do que processos e confirmar que um PID nunca aparece executando simultaneamente em duas CPUs.

### 17/11, terça-feira - coordenação e desempates da fila GLOBAL

**Objetivo:** tornar o comportamento concorrente compatível com as regras do PDF.

- [ ] Fazer reinserção e escolha seguinte da mesma CPU dentro da mesma seção crítica da fila global.
- [ ] Notificar CPUs ociosas depois dessa escolha, impedindo o roubo indevido do processo recém-preemptado.
- [ ] Conferir chegadas simultâneas, chegada versus reinserção e escolha de CPU livre.
- [ ] Verificar RR e Prioridade com despachos em instantes diferentes e quanta sobrepostos.
- [ ] Executar Helgrind em um caso pequeno e repetido; corrigir acessos compartilhados e inversões de mutex detectados.

**Verificação e conclusão:** repetir as mesmas entradas sem mudanças indevidas de PID, CPU ou contagem de eventos. Variação de poucos milissegundos é esperada; decisões de escalonamento precisam respeitar as regras.

### 18/11, quarta-feira - filas POR_CPU e colocação

**Objetivo:** distribuir chegadas entre filas locais corretamente.

- [ ] Criar uma fila e um mutex por CPU e fazer cada escalonador consultar sua fila.
- [ ] Escolher a CPU de chegada pela menor quantidade de processos aguardando mais o processo em execução, com desempate por índice.
- [ ] Proteger a leitura das cargas e a inserção para evitar decisões baseadas em estados inconsistentes.
- [ ] Manter as três políticas funcionando na seleção local e preservar os registros de CPU do processo.
- [ ] Atualizar a ordem global de mutexes para operações com mais de uma fila.

**Verificação e conclusão:** chegadas simultâneas são processadas em ordem de PID e distribuídas segundo a carga atualizada após cada colocação. Variar durações para comprovar que a colocação usa contagem de processos, e não tempo restante. A entrega deste dia ainda não completa POR_CPU: falta a migração do dia seguinte.

### 19/11, quinta-feira - migração entre filas locais

**Objetivo:** completar o modo POR_CPU.

- [ ] Quando a fila local estiver vazia, procurar a fila de espera mais longa, com desempate por menor índice.
- [ ] Retirar o último elemento da doadora, mesmo que haja apenas um processo à espera; não retirar o processo em execução.
- [ ] Proteger a transferência entre filas na ordem de mutexes definida.
- [ ] Registrar origem, destino e PID da migração e atualizar a associação do processo.
- [ ] Acordar CPUs ociosas para nova tentativa quando ocorrer inserção em qualquer fila.
- [ ] Verificar encerramento após migrações e repetir um caso de roubo com várias CPUs ociosas.

**Verificação e conclusão:** criar uma carga desigual que force roubo; conferir último elemento, escolha da doadora, evento único e ausência de duplicação/perda de PID. Testar também todas as filas vazias com chegadas futuras, sem encerramento prematuro.

**Marco 3:** todos os modos de execução e as três políticas estão implementados. O dia 20/11 e o fim de semana permanecem livres.

### 23/11, segunda-feira - relatório completo de métricas

**Objetivo:** transformar os dados já coletados em todas as saídas exigidas.

- [ ] Calcular espera, turnaround e resposta por processo e as três médias.
- [ ] Calcular makespan, throughput, utilização individual de cada CPU e total de trocas de contexto.
- [ ] Manter o último PID por CPU: o primeiro despacho conta, redespacho consecutivo do mesmo PID não conta, despacho de outro PID conta.
- [ ] Excluir retenção ociosa do RR/Prioridade da execução efetiva; preservar utilização separada por CPU mesmo com migrações.
- [ ] Ordenar processos por PID no relatório, preservar unidades e usar duas casas decimais conforme os exemplos.
- [ ] Escrever o bloco de métricas depois de todo o log no mesmo arquivo.

**Verificação e conclusão:** conferir à mão os dois exemplos do PDF e um caso RR com retenção. O makespan é a última finalização, não o instante em que o último escalonador encerra após reter uma CPU. Nenhuma métrica pode depender da ordem dos PIDs no arquivo de entrada.

### 24/11, terça-feira - testes integrados, memória e concorrência

**Objetivo:** encontrar defeitos antes de produzir os resultados da análise.

- [ ] Rodar os testes públicos disponíveis e a matriz direcionada deste plano, cobrindo todas as políticas e as duas arquiteturas de fila.
- [ ] Exercitar 1, 2, 4 e 8 CPUs e 1 a 4 threads por processo.
- [ ] Rodar um caso de estresse com dezenas de processos e muitas chegadas concorrentes, repetindo-o para procurar falhas intermitentes.
- [ ] Usar Valgrind para acessos inválidos e vazamentos e Helgrind para corridas e ordem de mutexes. Priorizar casos curtos nas ferramentas para caber no tempo.
- [ ] Conferir finalização única, todos os joins, limpeza e ausência de mensagens de execução na tela.
- [ ] Corrigir os defeitos encontrados antes de tratar as medições como resultados da análise.

**Verificação e conclusão:** registrar quais entradas passaram, os relatórios das ferramentas e qualquer falha reproduzível. Ferramentas de instrumentação alteram tempos; validar tolerâncias temporais em execução normal, separadamente. Se houver defeito grave pendente, o marco não está concluído.

### 25/11, quarta-feira - experimentos e análise comparativa

**Objetivo:** produzir os dados e as conclusões exigidos no README.

- [ ] Usar a carga_analise.txt oficial, mantendo a mesma carga de processos entre configurações.
- [ ] Executar a matriz de 25 configurações descrita adiante, salvando os resultados com identificação própria para não perdê-los quando o arquivo de saída for sobrescrito.
- [ ] Consolidar a tabela de métricas, a utilização por CPU e a quantidade de migrações nos casos POR_CPU.
- [ ] Escrever a discussão com base nos números observados, respondendo às quatro questões do enunciado.
- [ ] Completar as decisões de projeto, a ordem de mutexes e a explicação de como cada regra da seção 2 foi atendida.

**Verificação e conclusão:** cada linha da tabela pode ser rastreada até uma configuração e um log preservado; nenhuma conclusão depende de resultado inventado ou de uma carga diferente. Se uma repetição variar muito, investigar antes de interpretar a diferença como efeito da política.

### 26/11, quinta-feira - integração final e congelamento do escopo

**Objetivo:** terminar a fase de desenvolvimento com uma versão candidata completa.

- [ ] Verificar todos os critérios de aceite e finalizar qualquer ajuste obrigatório identificado nos dias 24 e 25.
- [ ] Recompilar os dois alvos a partir de uma pasta limpa em Linux e confirmar que alternar os alvos produz a versão correta, sem depender de arquivos intermediários antigos.
- [ ] Executar pelo menos um caso representativo de cada política em cada arquitetura aplicável.
- [ ] Conferir o README completo, as tabelas, os comentários e a ausência de uso de goto.
- [ ] Preparar uma primeira versão do arquivo de entrega com todos os fontes e o Makefile; testar extração e construção em diretório separado.
- [ ] Registrar a versão candidata e uma lista somente de revisão/correção para a semana reservada.

**Verificação e conclusão:** outra pessoa, ou você em uma pasta limpa, consegue extrair, compilar e executar seguindo o padrão do professor. Nenhuma funcionalidade, métrica ou seção obrigatória do README permanece por começar.

**Marco 4:** todo o trabalho obrigatório está pronto. A partir daqui, entram apenas revisão e correções de defeitos.

## Semana reservada: cinco sessões de duas horas

| Data | Foco | Resultado esperado |
| --- | --- | --- |
| 27/11, sexta | Auditar o comportamento contra as seções 2 e 4; conferir eventos, desempates, retenção e fórmulas; corrigir divergências | Lista de requisitos conferida com evidências |
| 30/11, segunda | Repetir estresse, Valgrind e Helgrind; corrigir corridas, deadlocks, acessos inválidos e vazamentos | Diagnósticos revisados e correções verificadas |
| 01/12, terça | Revalidar em Linux/LabGrad, ambos os alvos, troca entre versões e pacote extraído em pasta limpa | Entrega reproduzível no ambiente-alvo |
| 02/12, quarta | Revisar README e análise; repetir medições afetadas por correções; ensaiar explicação oral do funcionamento | Texto e tabelas correspondem à versão final |
| 03/12, quinta | Fazer a última regressão, gerar o pacote definitivo, conferir nome/identificação e enviar pelo Classroom | Submissão confirmada e cópia final preservada |

04/12, sexta: prazo às 07h59. Se necessário, apenas confirmar o envio já realizado; não programar desenvolvimento, correção ou primeira submissão para essa manhã.

Se uma correção da semana reservada alterar escalonamento ou métricas, repetir os testes e experimentos atingidos e atualizar o README. Manter o pacote final sincronizado com a versão corrigida.

## Matriz mínima de verificação

Os cenários abaixo são obrigações de cobertura, não uma exigência de criar uma ferramenta própria de testes. Reaproveite os casos públicos e use entradas pequenas com resultado calculado à mão para cobrir lacunas.

| Área | Cenário | O que observar |
| --- | --- | --- |
| Entrada | Arquivo desordenado e PIDs não consecutivos | Chegada e identificação corretas, sem indexação pelo valor do PID |
| Chegadas | Mesmo instante, PIDs invertidos no arquivo | Inserção em ordem crescente de PID |
| Threads | Mesma duração com 1, 2, 3 e 4 threads | Mesma quantidade de fatias; divisão exata de 100 ms |
| Espera | Primeira chegada tardia e lacuna entre processos | Escalonador espera e retoma sem encerrar cedo |
| FCFS | Processo novo chega enquanto outro executa | Ausência de preempção; liberação imediata no término |
| RR | Duração menor, igual e maior que quantum | Retenção correta, reinserção somente se não terminou |
| RR | Chegada no fim do quantum | Chegada entra antes do processo reinserido |
| Prioridade | Processo mais prioritário chega no meio do quantum | Troca somente ao fim do quantum |
| Prioridade | Prioridades iguais | Alternância FIFO semelhante ao RR |
| Contexto | Mesmo PID despachado em quanta consecutivos | Eventos aparecem; contagem de contexto não aumenta |
| GLOBAL | Várias CPUs livres e chegadas simultâneas | Desempate por CPU de menor índice e nenhum PID duplicado |
| GLOBAL | CPU ociosa durante reinserção de outra CPU | Reinserção e seleção atômicas antes da notificação |
| Quanta | CPUs despacham em instantes diferentes | Prazos independentes por CPU |
| POR_CPU | Empate de carga e durações diferentes | Colocação por quantidade de processos e menor índice |
| Migração | Filas doadoras de mesmo tamanho | Doadora de menor índice e retirada do último elemento |
| Migração | Doadora com um único processo aguardando | Roubo permitido; processo em execução nunca é retirado |
| Notificação | Inserção em outra fila enquanto CPU está ociosa | Nova tentativa de roubo sem espera ocupada |
| Métricas | Finalização antes do quantum | Retenção excluída da utilização e makespan baseado na finalização |
| Encerramento | Últimos processos em CPUs diferentes | Um encerramento global e todas as threads aguardadas |
| Estresse | Dezenas de processos, 4 threads por processo e 8 CPUs | Sem corrida, duplicação, perda de processo ou deadlock |
| Empacotamento | Compilar as duas versões após extração limpa | Mesmo nome de executável e nenhuma dependência local oculta |

### Limites de comparação definidos pelo PDF

- Instantes dos eventos: desvio de até 150 ms.
- Permutação admitida entre eventos cujos instantes diferem menos de 150 ms, preservadas as exigências de sequência da mesma CPU.
- Métricas: desvio de até 10%.
- PIDs, CPUs, contagem dos eventos e ordem dos eventos na mesma CPU precisam corresponder.

Essas tolerâncias acomodam o uso de threads reais e pausas do sistema. Não autorizam ignorar desempates, duplicar eventos ou trocar a CPU escolhida. Em especial, dois casos próximos no tempo continuam precisando de coordenação correta.

## Métricas: definições para conferir durante a implementação

| Medida | Definição |
| --- | --- |
| Turnaround de um processo | Instante de finalização menos chegada |
| Espera de um processo | Turnaround menos duração |
| Resposta de um processo | Primeiro despacho menos chegada |
| Médias | Soma dos valores individuais dividida pela quantidade de processos |
| Makespan | Instante da última finalização, contado desde T0 |
| Trocas de contexto | Primeiro despacho de cada CPU mais despachos de PID diferente do último executado naquela CPU |
| Utilização de uma CPU | Seu tempo efetivamente executando processos dividido pelo makespan, em porcentagem |
| Throughput | Quantidade de processos dividida pelo makespan convertido para segundos |

Não somar retenção ociosa à execução efetiva. Não usar o tempo de encerramento do programa como makespan. Não reiniciar a contagem desde a primeira chegada se ela for tardia. O relatório lista todos os PIDs em ordem crescente e inclui utilização individual de cada CPU, inclusive as ociosas.

Referências numéricas do PDF para validação:

| Exemplo | Espera média | Turnaround médio | Resposta média | Contextos | Utilização | Throughput | Makespan |
| --- | --- | --- | --- | --- | --- | --- | --- |
| FCFS, 1 CPU | 1066,67 ms | 2566,67 ms | 1066,67 ms | 3 | CPU0: 100% | 0,67 processos/s | 4500 ms |
| FCFS, 2 CPUs, GLOBAL | 266,67 ms | 1766,67 ms | 266,67 ms | 3 | CPU0: 100%; CPU1: 50% | 1,00 processo/s | 3000 ms |

O texto acima usa vírgula decimal em português; o arquivo produzido pelo programa deve reproduzir a formatação dos exemplos do enunciado, que usam ponto e duas casas decimais nos valores correspondentes.

## Matriz planejada para a análise comparativa

O PDF pede as três políticas no monoprocessador, comparação de 2 e 4 CPUs com GLOBAL e POR_CPU e RR com quanta de 100, 500 e 1000 ms. Para cobrir os cruzamentos de maneira consistente, este plano adota **25 configurações**. Esse número é uma escolha de cobertura do plano; o PDF não enumera exatamente 25 experimentos.

| Topologia | FCFS | Prioridade | RR | Quantidade |
| --- | --- | --- | --- | --- |
| Monoprocessador | Quantum ignorado | Quantum 500 ms | Quanta 100, 500 e 1000 ms | 5 |
| 2 CPUs, GLOBAL | Quantum ignorado | Quantum 500 ms | Quanta 100, 500 e 1000 ms | 5 |
| 2 CPUs, POR_CPU | Quantum ignorado | Quantum 500 ms | Quanta 100, 500 e 1000 ms | 5 |
| 4 CPUs, GLOBAL | Quantum ignorado | Quantum 500 ms | Quanta 100, 500 e 1000 ms | 5 |
| 4 CPUs, POR_CPU | Quantum ignorado | Quantum 500 ms | Quanta 100, 500 e 1000 ms | 5 |

O quantum de 500 ms na Prioridade é uma escolha metodológica para comparação, não um valor obrigatório imposto à análise pelo PDF. Manter esse valor fixo e declará-lo no README. A execução normal deve sempre obedecer ao quantum da entrada.

Guardar política, CPUs, arquitetura, quantum aplicável, espera média, turnaround médio, resposta média, contextos, utilização por CPU, throughput, makespan e migrações. Preservar os resultados de cada configuração, pois o nome do arquivo de saída do programa é sempre o mesmo. As cópias para análise não alteram esse contrato de saída.

Uma execução normal por configuração forma a base planejada. Repetir casos suspeitos ou instáveis; não apresentar uma amostra única como prova de estabilidade nem atribuir diferenças pequenas automaticamente à política. Não comparar tempos de execução sob Helgrind com resultados normais.

Perguntas que o texto precisa responder usando os dados:

1. Qual política apresentou a menor espera média nessa carga? Que característica da carga e da política explica o resultado?
2. O que aconteceu com os processos de baixa prioridade? Houve grande atraso? Não afirmar starvation infinita apenas porque uma carga finita produziu espera longa.
3. Como mudar o quantum alterou resposta e trocas de contexto? Incluir o efeito da retenção ociosa exigida pelo trabalho.
4. O que mudou entre GLOBAL e POR_CPU em utilização, equilíbrio de carga e migrações? Não presumir que o resultado será sempre favorável a uma das arquiteturas.

## Checklist final de entrega

- [ ] Três políticas nas versões mono e multiprocessador; GLOBAL e POR_CPU com migração.
- [ ] Intervalo de 1 a 8 CPUs e de 1 a 4 threads por processo atendido.
- [ ] Uso obrigatório de C, pthreads, mutexes e variáveis de condição.
- [ ] Compilação com make, suporte a pthreads e avisos exigidos; executável chamado trabSO.
- [ ] Alvos chamados monoprocessador e multiprocessador, funcionando após extração limpa e ao alternar versões.
- [ ] Um único argumento com o nome da entrada; nenhuma pergunta interativa e nenhuma impressão na tela durante a execução.
- [ ] Saída chamada log_execucao_minikernel.txt em qualquer versão/política, com eventos no formato exigido e métricas no final.
- [ ] Chegada, despacho, esgotamento quando aplicável, finalização, migração quando aplicável e encerramento global registrados corretamente.
- [ ] Ordenação de PIDs, casas decimais, unidades e nomes das métricas conferidos.
- [ ] Todos os joins e liberações concluídos; recursos de sincronização destruídos depois do último uso.
- [ ] Valgrind e Helgrind executados; diagnósticos relevantes tratados.
- [ ] Nenhum uso de goto. O PDF atribui nota zero a seu uso e a erro de compilação.
- [ ] Sem segmentation fault, deadlock ou corrida conhecida; comentários explicam decisões de concorrência e trechos que exigem cuidado.
- [ ] README com módulos, estruturas, ordem dos mutexes e atendimento das regras da seção 2.
- [ ] Carga oficial usada; matriz e resultados identificados; tabela e quatro questões de análise respondidas.
- [ ] Todos os fontes e Makefile incluídos; pacote tar.gz testado por extração e compilação em diretório separado.
- [ ] Nome conforme o template TP-SO-2026-2-matricula1-matricula2, acrescentando a extensão tar.gz. Em entrega individual, usar somente o formato expressamente autorizado pelo professor.
- [ ] Questão do trabalho em dupla resolvida. Apenas um integrante envia no formato normal de dupla.
- [ ] Entrega feita pela atividade do Classroom em 03/12 e confirmação guardada. Repositório privado compartilhado com o professor é encorajado pelo PDF; não presumir que ele dispense a submissão na atividade.

Há uma diferença de identificação no próprio documento: o título do PDF menciona 2026_1, mas a seção de entrega exige o template TP-SO-2026-2. Priorizar a regra explícita de entrega e conferir instruções complementares do Classroom. Não renomear o pacote apenas com base no título do PDF.

## Registro para continuar nas próximas sessões

Preencher ao encerrar cada dia:

- Data e etapa do plano:
- Tempo usado, dentro das duas horas:
- Entrega concluída:
- Entrada/caso usado na verificação:
- Resultado esperado e resultado observado:
- Avisos, falhas ou diagnósticos pendentes:
- Decisão de projeto registrada no README:
- Identificação da versão recuperável:
- Primeiro passo da próxima sessão:

Situação inicial: somente o PDF estava presente na pasta. Nenhum requisito foi implementado ou teste do Mini-Kernel executado durante a elaboração deste plano. O próximo passo é a sessão de 05/11: contrato do projeto, ambiente Linux e materiais do Classroom.
