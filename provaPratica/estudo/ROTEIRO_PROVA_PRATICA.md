# Sequência de treino para escrever código sob tempo

Todas as entregas são arquivos C. Leia o enunciado, implemente no modelo indicado e execute o corretor sem argumentos. Use o conteúdo para estudar antes; nos simulados, tente primeiro sem consulta.

## Etapa 1 — Criar, esperar e passar dados

Resolva nesta ordem: **t01 → t02 → t03 → t04 → t05 → t06 → t09 → t12**.

Treine criação, argumentos estáveis, retorno por ponteiro e liberação. Complete t07/t10 para identificação e impressão concorrente; t11 para compartilhar heap. Ao final, implemente t08 para comparar os dois padrões de create/join com uma barreira verificável.

## Etapa 2 — Comunicar processos

Resolva **p04 → p01 → p09 → p05 → p03 → p06 → p02 → p08 → p07**.

Comece com uma direção e EOF, avance para ida/volta e hierarquia, então redirecionamento/exec, FIFO e escrita não bloqueante. Antes de codificar, desenhe as pontas do pipe numa folha; esse desenho é ferramenta de trabalho, não uma resposta exigida.

## Etapa 3 — Proteger e repartir trabalho

Resolva **x01 → x02 → x04 → x03 → x05**.

Primeiro implemente reserva de índice indivisível, depois soma e divisão equilibrada. x03 é o desafio de mil elementos; x05 verifica handshake e trylock sem depender de sleep.

## Etapa 4 — Sincronizar com semáforos

Resolva **s16 → s15 → s01 → s03 → s11 → s02 → s14 → s13 → s12**.

Depois faça os protocolos com fases: **s04 → s06 → s05 → s07 → s09 → s08 → s10**. Todos precisam terminar após o número de rodadas pedido, inclusive as threads que estavam esperando. Os testes de eventos verificam a sequência observada sem impor uma ordem global desnecessária.

## Etapa 5 — Encapsular em monitores

Resolva **m07 → m01 → m06 → m04 → m05 → m02 → m03**.

m01 amplia o buffer para vários produtores/consumidores. m06 exige separar gerações da barreira. m02 testa prioridade do porto. m03 é avançado: construa a disciplina de Hoare usando semáforos, incluindo a fila urgente.

## Simulados sugeridos

| Simulado | Questões | Tempo de treino |
|---|---|---:|
| Base de laboratório | t09, p01, x02 | 90 min |
| Comunicação e divisão | p03, p06, x03 | 120 min |
| Sincronização | s11, s13, m07 | 120 min |
| Desafio | s05, m02, m03 | sem limite na primeira tentativa |

Esses tempos são sugestões de prática, não regras do professor. Execute os casos e confira os requisitos de implementação. Se falhar, localize o primeiro evento/resultado inválido, corrija e rode novamente. Para exercícios longos, comece pelo menor caso e depois aumente participantes e rodadas.
