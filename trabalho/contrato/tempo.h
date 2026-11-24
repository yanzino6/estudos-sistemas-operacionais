/* tempo.h (dono: A) - relogio da simulacao. */
#ifndef TEMPO_H
#define TEMPO_H
#define FATIA      100          /* ms: menor unidade de execucao simulada */
#define T_INFINITO 1000000000   /* "nenhuma chegada pendente" */
void tempo_iniciar(void);       /* marca T0 com gettimeofday; chamar uma vez, logo apos ler a entrada */
int  agora_ms(void);            /* ms reais desde T0 */
void dormir_ate(int t_ms);      /* usleep em pedacos de no maximo 500 ms ate agora_ms() >= t_ms */
#endif
