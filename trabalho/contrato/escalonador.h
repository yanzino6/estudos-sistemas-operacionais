/* escalonador.h (dono: A) - o unico laco de escalonador (mono, GLOBAL e POR_CPU). */
#ifndef ESCALONADOR_H
#define ESCALONADOR_H
void *thread_escalonador(void *arg_cpu);    /* arg = CPU* (uma thread por CPU) */
#endif
