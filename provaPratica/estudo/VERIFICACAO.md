# Verificação da entrega

Verificado em 9 de novembro de 2026, neste ambiente macOS com Python 3 e compilador `cc`.

- 11 PDFs inventariados, com 248 páginas no total.
- Cinco partes com conteúdo didático e catálogo de exercícios.
- 49 atividades com script próprio, casos e rubrica: 35 de análise e 14 de programas.
- 155 cenários de teste ao todo; algumas execuções de programas são repetidas para exercitar o contrato.
- Seis testes do motor passaram: programa válido, saída errada, timeout, análise correta com aviso de revisão manual, distinção de tipos JSON e linhas concorrentes fora de ordem.
- Os 49 scripts aceitaram a referência de seu contrato e rejeitaram uma solução incompleta. As referências teóricas usam os valores publicados no gabarito; isso verifica a ligação entre casos e corretores, não constitui revisão independente da teoria.
- Os contratos de programas foram conferidos com um oráculo C que calcula os resultados a partir da entrada. Ele é propositalmente sequencial e **não é uma solução válida dos requisitos concorrentes dos exercícios**. O caso do pipeline executa fork/dup2/exec com ping controlado e grep real.
- Links locais dos documentos conferidos.

## Reproduzir

Da raiz do projeto:

```bash
python3 estudo/_infra/testar_corretor.py
python3 estudo/_infra/validar_material.py
```

Os testes não certificam ausência de corrida, justiça de escalonamento ou fechamento correto dos descritores de uma futura solução do aluno. Essas propriedades exigem a revisão estrutural indicada em cada rubrica. Também não foi testada uma execução real de ping contra google.com: os testes desse exercício são intencionalmente offline.

O programa de verificação cria arquivos temporários e os apaga ao final. As soluções que você desenvolver permanecem sob sua organização; nenhuma resolução sua é sobrescrita pelo corretor.
