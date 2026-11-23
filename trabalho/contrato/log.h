/* log.h (dono: B) - buffer de eventos em memoria, ordenado so na gravacao. */
#ifndef LOG_H
#define LOG_H
#include <stdio.h>
enum { EV_CHEGADA = 0, EV_FINALIZADO = 1, EV_OUTRO = 2, EV_TERMINO = 3 };
void log_iniciar(const char *nome_politica);
/* cpu = -1: linha sem "[CPUk]". fmt e so a mensagem, ex.: "Executando PID %d".
 * t_logico = instante teorico do evento (multiplo de 100), usado para ORDENAR;
 * a linha impressa usa o tempo medido agora_ms(). */
void log_registrar(int t_logico, int cpu, int categoria, const char *fmt, ...);
void log_gravar(FILE *saida);   /* ordena por (t_logico, categoria, ordem de registro) e escreve */
void log_destruir(void);
#endif
