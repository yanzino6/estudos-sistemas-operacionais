# Estudo direcionado para prova prática de Sistemas Operacionais

**Todas as 49 questões agora são de escrita de código C.** Cada uma tem arquivo inicial para preencher, entradas de teste, saída esperada ou validação de eventos, e script de correção que compila e executa o programa. O conteúdo didático dos 11 PDFs permanece nas cinco partes.

## Onde escrever as respostas

Os modelos ficam em **`estudo/<parte>/respostas/<id>.c`**. Eles contêm o enunciado resumido, interfaces sugeridas e TODOs. São esqueletos compiláveis, não gabaritos prontos. Inicialmente reprovam porque ainda não implementam a atividade.

**[Abrir o índice dos 49 modelos e comandos de correção](MODELOS_DE_RESPOSTA.md).**

| Parte | Conteúdo | Exercícios | Modelos C |
|---|---|---|---|
| Semáforos | [conteudo.md](01_semaforos/conteudo.md) | [s01–s16](01_semaforos/exercicios.md) | [respostas](01_semaforos/respostas/README.md) |
| Monitores | [conteudo.md](02_monitores/conteudo.md) | [m01–m07](02_monitores/exercicios.md) | [respostas](02_monitores/respostas/README.md) |
| Pipes | [conteudo.md](03_pipes/conteudo.md) | [p01–p09](03_pipes/exercicios.md) | [respostas](03_pipes/respostas/README.md) |
| Threads | [conteudo.md](04_threads/conteudo.md) | [t01–t12](04_threads/exercicios.md) | [respostas](04_threads/respostas/README.md) |
| Mutex e paralelismo | [conteudo.md](05_mutex_e_paralelismo/conteudo.md) | [x01–x05](05_mutex_e_paralelismo/exercicios.md) | [respostas](05_mutex_e_paralelismo/respostas/README.md) |

## Primeiro exercício: abrir, implementar, corrigir

Da raiz `provaPratica`:

```bash
# 1. Abra estudo/03_pipes/respostas/p01.c no editor e implemente os TODOs.
# 2. Rode o script; ele encontra esse arquivo automaticamente:
python3 estudo/03_pipes/testes/p01/corrigir.py

# Para corrigir um arquivo em outro lugar:
python3 estudo/03_pipes/testes/p01/corrigir.py /caminho/minha_solucao.c
```

O script compila com `cc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -pthread`. Use `CC` para selecionar outro compilador. Requisitos: Python 3, compilador C/Pthreads e terminal Linux/macOS; no Windows, use ambiente Linux/WSL. Os modelos incluem um adaptador de semáforos para threads no macOS/Linux, explicado no guia de modelos.

Código de saída do corretor: **0** = passou nos casos; **1** = compilação, execução, resultado ou tempo reprovou; **2** = problema de uso/configuração. Cada execução tem timeout e limpeza dos processos descendentes. Resultados vão para stdout; diagnósticos para stderr. Não imprima prompts que o enunciado não pediu.

O corretor cria automaticamente os diretórios e salva a saída do seu programa em `testes/<id>/saidas/01.out`, `02.out`, etc., com o stderr correspondente em `.err`, inclusive quando o teste reprova. Casos com repetições usam `01_01.out`, `01_02.out`, etc. Esses arquivos são atualizados a cada execução do caso. Se um gabarito como `testes/<id>/01.out` estiver ausente, o corretor o gera compilando e executando o programa C de referência de `_infra/fixtures`, com a entrada do caso. Gabaritos existentes são preservados; a saída da sua solução não é usada como gabarito.

## Ordem remanejada para a prova prática

Comece por **Threads → Pipes → Mutex**, depois progrida para **Semáforos → Monitores**. Veja o [roteiro prático com sequências e simulados](ROTEIRO_PROVA_PRATICA.md). Os diretórios mantêm seus nomes para preservar links e referências, mas a ordem de resolução foi reorganizada por dificuldade de implementação.

As antigas tarefas de explicação, previsão em JSON ou pseudocódigo foram reescritas como programas. Exemplos: a análise de `&i` virou implementação com argumentos independentes; o grafo exige threads reais e semáforos; o porto exige monitor em C; o problema de EOF exige leitura real de pipe até encerramento.

## Como os testes corrigem concorrência

Há três formas de comparação, indicadas em cada exercício:

- **Resultado em ordem:** números/textos comparados ignorando espaços e quebras extras.
- **Linhas em qualquer ordem:** permite escalonamentos diferentes, mas exige todos os registros, sem omissões ou duplicações.
- **Eventos e invariantes:** aceita diferentes sequências válidas e verifica precedência, capacidade, exclusão, lotes, fases, prioridade ou barreiras. Nesses exercícios, `.out` é um exemplo válido, não uma saída única obrigatória.

Registre eventos no momento da alteração protegida do estado. Não junte um log inventado após o término. O corretor observa resultados e rastros; a lista `rubrica.md` também exige conferir no código o uso das primitivas e a distribuição do trabalho. Passar testes não demonstra ausência de corridas em todos os escalonamentos nem substitui o requisito de usar threads/processos reais. Não há mais resposta teórica para entregar.

## Organização

```text
parte/
  conteudo.md
  exercicios.md
  respostas/
    identificador.c       # você escreve aqui
    README.md
  testes/
    identificador/
      corrigir.py         # sem argumento corrige o modelo acima
      casos.json          # configuração do corretor; não é uma resposta
      01.in / 01.out
      saidas/             # saídas .out e diagnósticos .err da sua solução
      README.md
      rubrica.md
```

Mantenha a pasta `estudo` inteira: scripts e modelos usam `_infra`. Os fontes citados no Drive do roteiro 4.1 não vieram nos PDFs; as implementações propostas aqui são adaptações de treino identificadas, não cópias desses fontes ausentes.

Veja também [cobertura dos PDFs](MAPA_DE_COBERTURA.md), [ressalvas técnicas](RESSALVAS_DOS_PDFS.md) e [verificação](VERIFICACAO.md).
