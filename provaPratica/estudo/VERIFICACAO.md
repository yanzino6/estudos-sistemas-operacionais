# Verificação da versão voltada à prova prática

Revisão de 9 de novembro de 2026, executada em macOS com Python 3 e `cc`.

- **49 exercícios, todos de programação C**, com arquivo em `respostas/`, script e testes.
- **137 casos de execução**, com repetição nos exercícios concorrentes indicados no manifesto.
- Todos os **49 modelos iniciais compilaram**. São esqueletos para o aluno completar; não são soluções prontas.
- Todos os **49 corretores aceitaram a referência do contrato de saída e rejeitaram um programa incompleto**.
- **21 testes de infraestrutura passaram**: oito do executor, doze dos validadores de eventos e um do adaptador de semáforos.
- Validação de eventos cobre grafos, lotes completos, saída após fim, reposição de comida, limite de recursos, vizinhos dos filósofos, gerações de barreira, leitores/escritores, capacidade/prioridade do porto e fila da barbearia.
- Testes negativos incluem ordem inválida, duplicação, lote incompleto, entrada excedente, filósofos vizinhos ativos, passagem prematura pela barreira, conflito leitura/escrita, violação de prioridade, programa travado e FIFO não removida.
- Adaptador de semáforos exercitado com **quatro threads reais e 40 mil incrementos**, além de bloqueio/liberação inicial.
- Links locais e referências a entradas/saídas conferidos.

## Reproduzir

Da raiz `provaPratica`:

```bash
python3 -m unittest discover -s estudo/_infra -p 'testar_*.py'
python3 estudo/_infra/validar_material.py
```

`validar_material.py` compila seus arquivos de resposta, mas não os sobrescreve nem exige que continuem vazios. Se você estiver editando um arquivo e ele ainda não compilar, essa etapa informará a falha correspondente.

## Alcance da verificação

Os arquivos de `_infra/fixtures` são **oráculos sequenciais de entrada/saída**, não gabaritos dos requisitos concorrentes. Eles calculam resultados ou produzem um escalonamento válido para testar os corretores. Nos grafos e multiconjuntos, também usam ordens diferentes dos exemplos `.out`. O pipeline ping/grep do oráculo executa processos reais com ping controlado e grep real.

A verificação não implementa e certifica 49 soluções concorrentes completas. Os corretores verificam resultados, rastros, término, retorno e, no exercício FIFO, remoção do nome. A conferência do uso de primitivas e da distribuição de trabalho está em `rubrica.md`. Isso não cria uma entrega teórica: o aluno entrega apenas código, que também deve cumprir os requisitos estruturais.

O ramo Linux do adaptador de semáforos usa `sem_init`; ele não foi executado neste host macOS. A execução real de ping contra google.com não faz parte dos testes: os casos são offline por definição.
