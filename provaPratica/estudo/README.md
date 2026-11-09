# Estudo direcionado — Sistemas Operacionais

Material organizado a partir dos **11 PDFs de `../materiais` (248 páginas)**, em cinco partes. Inclui **49 atividades**, sendo **14 com testes de programas C** e **35 com cenários objetivos de análise mais revisão manual**. Exercícios criados para complementar a preparação estão identificados; os exercícios do professor mantêm sua origem e requisitos.

## Por onde começar

| Ordem | Conteúdo e exercícios | Ao terminar, você deve conseguir |
|---|---|---|
| 1 | [Semáforos](01_semaforos/conteudo.md) · [exercícios](01_semaforos/exercicios.md) | Transformar dependências e invariantes em P/V |
| 2 | [Monitores](02_monitores/conteudo.md) · [exercícios](02_monitores/exercicios.md) | Escolher predicados e explicar wait/signal |
| 3 | [Pipes](03_pipes/conteudo.md) · [exercícios](03_pipes/exercicios.md) | Criar processos, fechar descritores e montar pipelines |
| 4 | [Threads](04_threads/conteudo.md) · [exercícios](04_threads/exercicios.md) | Criar, passar argumentos e recolher retornos |
| 5 | [Mutex e paralelismo](05_mutex_e_paralelismo/conteudo.md) · [exercícios](05_mutex_e_paralelismo/exercicios.md) | Corrigir corridas e dividir trabalho sem perder elementos |

Se a prova prática estiver próxima, faça **3 → 4 → 5** primeiro; depois 1 → 2 para consolidar sincronização. Esse foco foi inferido dos roteiros de laboratório, não é uma previsão do que o professor cobrará.

## Rotina sugerida de estudo

Em cada parte, leia o conteúdo, feche o texto e explique o mecanismo com suas palavras. Resolva os exercícios sem abrir gabaritos. Execute os testes e use a mensagem de falha para localizar o problema. Por último, faça a revisão estrutural da rubrica: acertar saída não demonstra que você usou os processos, threads ou primitivas solicitados.

Sugestão de seis sessões, ajustáveis ao seu ritmo:

1. Pipes: desenhar descritores e fazer p01/p03; depois p02 e análise de EOF.
2. Threads: argumentos, join e retorno; fazer t04, t06, t07 e t09.
3. Mutex: reproduzir um intercalamento ruim no papel, corrigir x02 e fazer x03.
4. Semáforos: precedência, buffer e recursos; depois problemas de lotes.
5. Monitores: if/while, porto, barbeiro e implementação das disciplinas.
6. Simulado: p03, t09, x03, s11 e m01 sem consultar conteúdo; corrija tanto os casos quanto as rubricas. Sugestão: 90–120 minutos, sem valor oficial.

Avance quando conseguir explicar por que uma solução errada falha, além de obter aprovação nos casos.

## Estrutura de cada parte

```text
parte/
  conteudo.md
  exercicios.md
  testes/
    identificador/
      corrigir.py
      casos.json
      rubrica.md
      # exercícios C: entradas .in e saídas .out
      # análise: cenarios.md, respostas.modelo.json, gabarito.md
```

Os pequenos scripts por exercício compartilham o motor `_infra/corretor.py`. Mantenha a pasta `estudo` inteira para que os imports funcionem. Nenhum pacote Python adicional é necessário para estudar/corrigir. Os PDFs originais não foram alterados.

## Como executar

Requisitos: terminal POSIX (Linux ou macOS), Python 3 e compilador C com Pthreads (`cc` ou variável `CC`). Linux é o ambiente-alvo dos roteiros. No Windows, use ambiente Linux/WSL. O exercício s16 explica a diferença de suporte a semáforos no macOS.

Execute da raiz `provaPratica`:

```bash
# Exemplo de programa C
python3 estudo/03_pipes/testes/p01/corrigir.py /caminho/solucao.c

# Exemplo teórico: copie e preencha o modelo, sem editar os casos
cp estudo/02_monitores/testes/m01/respostas.modelo.json /tmp/m01.json
# Preencha /tmp/m01.json e escreva sua justificativa/pseudocódigo em resposta.md
python3 estudo/02_monitores/testes/m01/corrigir.py /tmp/m01.json

# Verificar o motor de correção
python3 estudo/_infra/testar_corretor.py
```

Códigos de saída: **0** = todos os casos objetivos passaram; **1** = caso, compilação ou tempo falhou; **2** = arquivo/uso/configuração incorreta. A revisão manual continua necessária mesmo com código 0.

Nos programas, envie resultados a stdout e diagnósticos a stderr. Os contratos dos exercícios dizem quando usar frases ou somente números. A comparação aceita diferenças de espaços e quebras; nos exercícios de impressão concorrente aceita qualquer ordem de linhas, mas exige todas as linhas, sem duplicação. O timeout encerra também processos descendentes deixados pelo programa. Cada teste usa diretório temporário; caminhos relativos da solução serão relativos a esse diretório.

## O que a correção demonstra

Nos programas, o corretor compila/roda o arquivo fornecido, compara resultados, verifica retorno e impõe tempo máximo. Repetições ajudam a revelar bugs intermitentes. Não provam ausência de data races, starvation ou deadlock em todos os escalonamentos. As rubricas verificam o uso de primitivas, a distribuição de trabalho, a vida útil da memória e o encerramento.

Nas questões teóricas, os test cases são cenários de escalonamento/estado com respostas objetivas em JSON. O script **não corrige automaticamente uma dissertação ou pseudocódigo**. Depois de responder, confira a explicação do gabarito e a rubrica. Os gabaritos são abertos por finalidade didática; copiá-los só testa o formato, não o aprendizado.

O roteiro 4.1 faz referência a arquivos em um Drive compartilhado; eles não estão nesta pasta. As perguntas foram preservadas e há variantes descritas para você reconstruir o experimento. Não apresentamos essas variantes como os fontes originais.

Confira [mapa de cobertura](MAPA_DE_COBERTURA.md), [ressalvas dos PDFs](RESSALVAS_DOS_PDFS.md) e [verificação dos materiais](VERIFICACAO.md).
