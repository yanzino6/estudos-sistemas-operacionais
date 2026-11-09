# Parte 5 — Corridas, mutexes e divisão do trabalho

Fontes: Aula 17, páginas 21–30; roteiro 4.2, exercícios 4, 5 e 7. Esta parte transforma a criação de threads em um programa concorrente que produz resultados consistentes.

## 1. Compartilhar endereço não basta

Duas threads podem somar regiões distintas do mesmo vetor sem mutex quando o vetor é somente leitura e cada uma acumula em variável local. Compartilhar um vetor não implica automaticamente corrida. O conflito aparece quando acessos à mesma posição/variável podem ocorrer concorrentemente, pelo menos um escreve, e não há sincronização adequada.

Um índice global usado como “próximo trabalho” é um estado composto: ler índice, reservar posição e avançar índice devem formar uma operação indivisível. Se A e B leem 3 antes de qualquer incremento, ambas processam a posição 3. Se ambas escrevem 4, uma reserva desaparece. O resultado pode duplicar elementos e omitir outros.

O mesmo vale para `total += parcial`: cada parcial estar correta não torna a acumulação compartilhada atômica. `volatile` não transforma essa expressão em sincronização. Em C, data race significa comportamento indefinido; não limite a discussão a “às vezes imprime um número menor”.

## 2. Ciclo de vida de um mutex

```c
pthread_mutex_t m;
pthread_mutex_init(&m, NULL);
/* criar trabalhadores */
pthread_mutex_lock(&m);
/* operação inteira sobre o estado compartilhado */
pthread_mutex_unlock(&m);
/* depois dos joins */
pthread_mutex_destroy(&m);
```

Como alternativa de inicialização estática, use `PTHREAD_MUTEX_INITIALIZER`. Não reinicialize um mutex que já está em uso. Verifique retornos, libere o lock em todos os caminhos e destrua somente quando não houver usuários/esperas.

`pthread_mutex_trylock` tenta adquirir sem bloquear; falhar não significa adquirir. Tipos normal, recursivo e error-checking têm semânticas diferentes; não dependa de detectar todos os erros com o tipo normal. Atributos robustos tratam a morte de um proprietário com protocolo de recuperação; process-shared exige objeto em memória compartilhada e suporte apropriado. Apenas declarar uma variável antes de fork não cria um mutex compartilhado entre processos.

Um mutex oferece exclusão e ordenação de memória para quem respeita seu protocolo. Não garante FIFO ou justiça por si. Implementações podem usar caminhos rápidos e espera adaptativa; “sempre uma chamada de núcleo” seria uma simplificação.

## 3. Reserva dinâmica de índices

Uma organização próxima à Aula 17:

```text
trabalhador:
  parcial = 0
  repetir:
    lock(m)
    se proximo == n:
      unlock(m)
      interromper
    k = proximo
    proximo++
    unlock(m)
    parcial += vetor[k]
  retornar parcial

main:
  criar todos os trabalhadores
  total = 0
  para cada trabalhador: join e somar seu resultado
```

A região crítica protege a decisão e a reserva. O cálculo independente acontece fora dela. Como apenas a main combina resultados depois dos joins, total não precisa de mutex. Se os próprios trabalhadores acumularem num total global, proteja essa acumulação separadamente.

No exercício 4 do roteiro, resultados podem estar num vetor global sem corrida se cada thread escreve exclusivamente em sua própria posição e a main lê somente após os joins. Se duas threads escrevem no mesmo slot ou usam índice global desprotegido para escolher o slot, a análise muda. Declare a política em sua solução.

O exercício 5 menciona “programa do Exercício 5”, uma autorreferência; a sequência deixa claro que se trata de corrigir o exercício 4. Os exercícios desta pasta registram essa correção editorial.

## 4. Divisão estática equilibrada

Quando o trabalho por elemento é semelhante, dispense o índice global: cada thread recebe um intervalo fixo `[inicio,fim)`. Para n elementos e t threads, use:

```text
q = n / t; r = n % t
inicio(i) = i*q + min(i,r)
tamanho(i) = q + (i < r ? 1 : 0)
fim(i) = inicio(i) + tamanho(i)
```

Exemplo n=10, t=3: `[0,4)`, `[4,7)`, `[7,10)`. A união cobre todas as posições; intervalos não se sobrepõem; tamanhos diferem em no máximo um. Dividir apenas por q e esquecer r perde o final do vetor. Se t>n, alguns intervalos ficam vazios: pode-se permitir ou rejeitar, mas declare o contrato.

O desafio do professor fixa n=1000, recebe t e pede parâmetros dinâmicos, parciais e liberação. Neste material o contrato aceita `1 <= t <= 1000`. Rejeite zero/negativo antes de dividir. Para treino adicional com n variável, veja o exercício criado desta parte.

Use `long long` para somas dos limites de teste. Um int de 32 bits pode estourar com vários valores grandes; overflow de inteiros assinados também é comportamento indefinido. Em laboratório, limites explícitos ajudam a distinguir bug de sincronização de bug aritmético.

## 5. Determinismo e custo

Com sincronização correta, uma soma inteira sem overflow deve dar o mesmo total a cada execução. A ordem das threads e dos prints ainda pode variar. Isso responde à pergunta sobre “resultado determinístico”: valor final determinístico não exige escalonamento determinístico.

Mutex introduz custo de aquisição/liberação, contenção, possíveis bloqueios e mudanças de contexto. Uma região crítica longa serializa trabalho; uma aquisição por elemento pode custar mais que a soma. Acumular localmente e combinar depois costuma reduzir contenção. Divisão estática pode sofrer desequilíbrio quando alguns elementos custam mais; distribuição dinâmica melhora equilíbrio em troca de coordenação.

O limite clássico de aceleração, para fração serial s e p trabalhadores ideais, é `1/(s+(1-s)/p)`. Com s=0,25 e p=4, o limite é aproximadamente 2,286. É um modelo: overhead e largura de banda podem reduzir o ganho real.

False sharing ocorre quando threads escrevem em variáveis distintas localizadas na mesma linha de cache, gerando tráfego de coerência. Não é uma data race entre essas variáveis, mas pode degradar desempenho. A ausência de corrida não garante escalabilidade.

## 6. Como comparar 1, 2, 4 e 8 threads

Use o mesmo vetor e trabalho equivalente. Meça com relógio monotônico, repita várias vezes e compare medianas. Separe claramente se o tempo inclui criação/join, alocação e leitura de entrada. Não inclua prints dentro da região medida se o objetivo é medir soma. Mil inteiros são poucos: criação e sincronização podem dominar. Não espere que oito threads sejam oito vezes mais rápidas.

Registre uma tabela com t, total, mediana e dispersão. Compare o total primeiro; um programa rápido que perde elementos está errado. Para estudar tendência, crie uma variante com mais trabalho, identificada como extensão ao enunciado.

## 7. Diagnóstico e testes

Testes importantes: zeros, negativos, valores repetidos, grandes valores, divisão com resto, uma thread e muitos trabalhadores. Repita execuções para aumentar a chance de manifestar problemas; não transforme a ausência de falha em prova de ausência de data race.

Uma ferramenta como ThreadSanitizer pode ajudar em ambiente compatível (`-fsanitize=thread -g`), mas disponibilidade depende do compilador/plataforma. AddressSanitizer ajuda a encontrar acessos inválidos, mas não substitui o detector de corridas. Não combine tudo sem verificar suporte e não use tempos instrumentados para comparar performance.

Para a versão deliberadamente incorreta do exercício 4, não há saída errada obrigatória: ela pode parecer correta muitas vezes. Na versão prática deste estudo, x01 exige escrever a reserva corrigida e emitir cada índice exatamente uma vez; x02 exige somar com essa reserva. Os scripts executam ambos e os requisitos exigem inspecionar o mutex. Um programa sequencial pode passar pelos testes de valores e ainda reprovar no requisito de concorrência.

## 8. Roteiro mental antes de entregar

Escreva o dono de cada alocação. Confira se os argumentos vivem até seu último uso. Crie todos os trabalhadores antes de join se desejar sobreposição. Proteja estado composto inteiro. Combine resultados depois da sincronização. Libere cada objeto uma vez. Trate criação parcial. Explique separadamente correção do resultado, progresso e desempenho: são três propriedades diferentes.
