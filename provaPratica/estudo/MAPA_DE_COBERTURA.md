# Mapa de cobertura dos PDFs

As páginas são as posições no PDF, contando a primeira como 1. Slides repetidos/animações foram consolidados na explicação do conceito. As explicações são reescritas didaticamente; não são transcrição literal. Todos os exercícios foram adaptados para escrita de código C, com contratos finitos de entrada/saída. O mapa indica a origem temática, não que o novo enunciado seja uma transcrição do professor.

| PDF em materiais | Páginas | Destino e atividades |
|---|---|---|
| Aula 11 - Sincronizacao - Semaforos | 1–13 | Parte 1: soluções de hardware/software, bloqueio, P/V, mutex, contadores e precedência; s01–s03, s15 |
| mesmo | 14–23 | Buffer circular, três semáforos, ordem de aquisição, SPSC, otimização e limitações; s14, s16 |
| Aula 11.2 - Sincronizacao - Semaforos - Exercícios Resolvidos | 1–9 | Encadeamento e dois grafos; s01–s03 |
| mesmo | 10–20 | Pombo, montanha-russa, canibais, filme; s04–s07 |
| mesmo | 21–33 | Três políticas de leitores/escritores; s08 |
| mesmo | 34–38 | Barbeiro, deadlock e filósofos; s09–s10 |
| Lista 6 - Semáforos | 1–3 | Questões 1/2/3/4 em s11/s08/s12/s13; grafo e código visual preservados em referências |
| Aula 12 - Sincronizacao - Monitores | 1–12 | Encapsulamento, entradas, exclusão, condições; Parte 2, m01/m06 |
| mesmo | 13–19 | Buffer, Hoare, sinaliza-e-continua e sinaliza-e-sai; m01/m03 |
| mesmo | 20–28 | Filósofos, implementação por semáforos e Java; m05/m03, conteúdo §10 |
| Lista 7 - Monitores | 1–2 | Questões 1/2/3/4 em m01/m02/m03/m04 |
| Aula 14 - Pipes | 1–23 | IPC, memória compartilhada/núcleo, pipes, descritores, fork, read/write; Parte 3 |
| mesmo | 24–27 | pipe1 e alteração solicitada; p04 |
| mesmo | 28–41 | Exec, herança, double/son, dup/dup2 e ls/wc; p05/p06 |
| mesmo | 42–43 | Alarm, capacidade e comunicação bidirecional; p07/p08 |
| mesmo | 44–55 | FIFO, permissões, open/unlink, reader/writer; p08 |
| roteiro_lab03 | 1–4 | Exercícios 1/2/3 em p01/p02/p03 |
| Aula 16 - Threads (Parte 1) | 1–29 | Fluxos, aplicações, recursos versus escalonamento, PCB/TCB, estados, economia e responsividade; Parte 4 §§1–2, t11 |
| mesmo | 30–52 | ULT, KLT, M:1, 1:1, M:N e comparação; Parte 4 §3, t11 |
| Aula 17 - Threads (Parte 2) | 1–13 | NPTL, biblioteca, create, atributos, exit, join, self; Parte 4 §§3–6 |
| mesmo | 14–20 | Seis exemplos: global, x/o, Hello, atributos, nomes e retorno; t01/t05 e Parte 4 §10 |
| mesmo | 21–28 | Soma, índice global, mutex e atributos; Parte 5, x01–x03/x05 |
| mesmo | 29–33 | Linux/clone, Java e comparação de desempenho; Parte 4 §§3/11 |
| roteiro_lab04_1 | 1–5 | NPTL e APIs no conteúdo; A.1=t01; A.2=t02; B1=t03; B2(a–g)=t04; B3(a–d)=t05; exercício final=t06 |
| roteiro_lab_04_2 (1) | 1–5 | Ex1(a–c)=t07; Ex2(d–f)=t08; Ex3(g–i)=t09; Ex4(j–l)=x01; Ex5(m–o)=x02; Ex6 parte1=t10/parte2=t04; Ex7=x03 |

Complementos criados: s15/s16, m06/m07, p09, t11/t12, x04/x05. Atividades como s08 e t04 agrupam questões repetidas entre PDFs convertendo as perguntas de cada fonte em requisitos de implementação. Diagramas essenciais foram conferidos visualmente, não inferidos apenas da extração textual.

Bibliografias e links de referência dos slides permanecem nos PDFs originais. O conteúdo indicado apenas por link externo (por exemplo vídeo de filósofos ou fontes do Drive) não foi tratado como se estivesse anexado; a solução conceitual dos filósofos é explicada aqui a partir dos slides disponíveis.

## Adaptações práticas desta revisão

As perguntas discursivas permanecem explicadas em `conteudo.md`, mas as entregas são exclusivamente C. p06 troca ls/wc por grep/wc com entrada controlada; p07 troca a medição de capacidade por transferência não bloqueante; s09/m04 usam comandos de chegada/término para tornar o atendimento reproduzível; t11 exercita compartilhamento de heap. As demais adaptações e limites aparecem em cada enunciado.
