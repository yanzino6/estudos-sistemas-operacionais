> **ARQUIVADO em 23/11/2026.** Este arquivo foi absorvido por `04_TRILHA_A.md` e `05_TRILHA_B.md` e não é mais atualizado. Ambiente, oráculo, comparador, valgrind e estresse estão agora nas trilhas (seção 2 e etapas de teste). Este arquivo supunha macOS para o Yan, o que não é o caso. Pode ser apagado.

# 06 · Ambiente, oráculo, comparador, valgrind/helgrind e estresse

## 1. Ambiente Linux (obrigatório para os dois)

O programa é avaliado no Ubuntu do LabGrad. No macOS **não existe `pthread_barrier_t`**
(por isso a barreira é própria) e **o valgrind não roda**. Qualquer uma das três opções
abaixo serve:

**(a) Docker (recomendado no Mac, inclusive Apple Silicon).** Na pasta do repositório:
```bash
docker run --rm -it -v "$PWD":/tp -w /tp ubuntu:24.04 bash
apt update && apt install -y build-essential valgrind python3 git
make multiprocessador && ./trabSO tests/casos/entrada_13.txt && cat log_execucao_minikernel.txt
```
Para não reinstalar tudo a cada vez, salve a imagem uma vez com `docker commit <id> tp-so` e use `docker run ... tp-so bash`.

**(b) VM Ubuntu** (UTM no Mac, VirtualBox no Windows) ou **WSL2** no Windows.

**(c) LabGrad** via SSH. É obrigatório pelo menos no teste final de quarta, 02/12.

O ambiente está pronto quando `gcc --version`, `valgrind --version` e `python3 --version` respondem.

## 2. Oráculo (`ferramentas/oraculo.py`)

Simulador determinístico das regras, em passos de 100 ms, sem threads. Ele reproduz
**exatamente** as saídas dos casos 01, 03, 05, 13, 15, 17 e do exemplo 4.5.

```bash
python3 ferramentas/oraculo.py entrada.txt            > esperado.txt   # multi (usa CPUS e FILA do arquivo)
python3 ferramentas/oraculo.py entrada.txt --mono     > esperado.txt   # como make monoprocessador
python3 ferramentas/oraculo.py entrada.txt --ambiguo  > esperado.txt   # sai com 2 se o caso for ambíguo
python3 ferramentas/oraculo.py entrada.txt --afinidade                 # bônus ligado (sem efeito no mono)
python3 ferramentas/contar_migracoes.py log_execucao_minikernel.txt    # migrações, despachos em outra CPU e métricas
python3 ferramentas/oraculo.py entrada.txt --retencao-nao-conta        # ponto em aberto P1
```
- **Ambíguo** quer dizer que o resultado muda se dois eventos do mesmo instante, em CPUs diferentes, forem tratados em outra ordem. O programa real pode, legitimamente, dar outra saída. **Só use casos não ambíguos para comparar** com `compare.py`. Os casos oficiais não são ambíguos (o LEIA-ME garante isso, e o oráculo confirma).
- O **mono é sempre determinístico**: qualquer entrada mono serve de teste contra o oráculo.
- Com 8 CPUs, quase todo caso aleatório é ambíguo. Para eles, use o verificador de invariantes (seção 5).
- O oráculo **não vai na entrega** (arquivo 07, seção 4).

## 3. Comparador (`tests/compare.py`, de B) e `run_all.sh`

Uso: `python3 tests/compare.py esperado.txt obtido.txt`. Sai com 0 se passar. Se falhar,
sai com 1 e imprime **a primeira diferença** (é o que mais ajuda a depurar).

Regras [E §4.6 e LEIA-ME]:
1. Separar o log das métricas na linha `=== METRICAS ===`.
2. Ler cada evento: `t`, CPU (ou nenhuma), tipo (`Chegada`, `Executando`, `Quantum`, `finalizado`, `Migracao`, `Termino`), PID e, na migração, a origem.
3. **Mesma quantidade de eventos**, no total e por tipo.
4. **Por CPU**: a sequência de (tipo, PID) é idêntica, e cada `|t_esperado − t_obtido| ≤ 150`.
5. **Eventos sem CPU** (`Chegada`, `Termino`): para cada esperado, existe um obtido ainda não usado com o mesmo tipo e PID e diferença de no máximo 150 ms.
6. **Métricas**: as linhas têm o mesmo texto fora dos números. Cada número pode desviar até 10%. **Exceções:** `Trocas de contexto` tem que ser **exata** (LEIA-ME); quando o esperado é 0, aceitar até 150 ms de diferença absoluta (decisão nossa, documentada no README).
7. Autoteste: um arquivo contra ele mesmo passa; com um PID trocado, falha; com um evento a mais, falha; com `Trocas` diferente em 1, falha.

`tests/run_all.sh [mono|multi|porcpu|todos]`:
```
para cada tests/casos/entrada_X.txt do grupo pedido:
    compilar o alvo certo (01..06 → mono; 07..18, ex45, g*, p* → multi), uma vez por grupo
    ./trabSO entrada_X.txt
    compare.py saida_X.txt log_execucao_minikernel.txt  →  "ok X" ou "FALHOU X: <primeira diferença>"
no fim: "N/M ok"; código de saída 0 só se todos passarem
```

## 4. valgrind e helgrind

```bash
valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./trabSO tests/casos/entrada_05.txt
valgrind --tool=helgrind --error-exitcode=1 ./trabSO caso_pequeno.txt
```
- **Objetivo: 0 erros nos dois.** Rode o memcheck a partir do M2 e o helgrind a partir do M3.
- Sob o helgrind, o programa fica dezenas de vezes mais lento, e os tempos do log saem errados. Isso é normal. Use casos pequenos (de 8 a 12 processos) e olhe só o relatório de erros.
- Se o helgrind acusar "lock order violated", procure quem trava fila e processo juntos (proibido) ou trava as filas fora da ordem crescente.
- "Possible data race" em `ultimo_pid`, `ocupado_ms` ou `trocas` lidos pelo `main`: confira que a leitura só acontece depois do `pthread_join`.

## 5. Estresse (de A)

**`ferramentas/gerar_estresse.py <semente> [--cpus N] [--politica P] [--fila F] [--sem-ambiguidade]`**
```
n ← 30..60 (ou 8..12 para o helgrind);  PIDs distintos e embaralhados (não sequenciais)
chegada ← múltiplo de 100 em [0, 5000], com vários processos de propósito no mesmo instante (inclusive t=0)
duração ← múltiplo de 100 em [100, 2000];  prioridade ← 1..5;  threads ← 1..4
cabeçalho: CPUS (1..8), QUANTUM (100..1000, múltiplo de 100), POLITICA, FILA
linhas em ordem aleatória
--sem-ambiguidade: troca de semente até `oraculo.py --ambiguo` sair com 0 (funciona bem com 2 e 4 CPUs)
```

**`ferramentas/verificar_invariantes.py entrada.txt log_execucao_minikernel.txt`**: confere o que
vale em qualquer execução correta, mesmo nos casos ambíguos:
1. O programa terminou dentro de um tempo limite (rodar com `timeout 120`). Se não terminou, é deadlock.
2. Cada PID tem exatamente uma `Chegada` e um `finalizado`. Existe exatamente um `Escalonador terminou`, e ele é a última linha do log.
3. Nenhum `Executando` de um PID antes da sua `Chegada`. Um PID nunca está em execução em duas CPUs ao mesmo tempo.
4. Em cada CPU, depois de um `Executando`, a próxima linha da mesma CPU é `Quantum esgotado` ou `finalizado` do mesmo PID (ou, depois de `finalizado` com retenção, um novo `Executando` só a partir do fim do quantum).
5. Para cada PID: o tempo executado (soma dos intervalos `Executando` até `Quantum`/`finalizado`) é igual à duração, com tolerância de 150 ms por intervalo.
6. FCFS: nenhuma linha `Quantum esgotado`. Só POR_CPU tem `Migracao`.
7. Métricas: as linhas PID batem com o próprio log (turnaround = finalizado − chegada, com tolerância), e `Trocas de contexto` é igual à contagem de `Executando` com PID diferente do anterior na mesma CPU.

**Rotina recomendada (a partir do M3):**
```bash
for s in $(seq 1 20); do
  python3 ferramentas/gerar_estresse.py $s --cpus 8 > /tmp/e.txt
  timeout 120 ./trabSO /tmp/e.txt || echo "TRAVOU ou falhou: semente $s"
  python3 ferramentas/verificar_invariantes.py /tmp/e.txt log_execucao_minikernel.txt || echo "INVARIANTE: semente $s"
done
```
Quando uma semente falhar, guarde a entrada em `tests/estresse/falha_<s>.txt` e abra uma issue com o rótulo `bug`.
