/* config.h (dono: A) - configuracao lida do arquivo de entrada. */
#ifndef CONFIG_H
#define CONFIG_H
typedef enum { POL_FCFS, POL_RR, POL_PRIORIDADE } TipoPolitica;
typedef enum { FILA_GLOBAL, FILA_POR_CPU } ModoFila;
typedef struct {
    int num_cpus;               /* 1..8 (com -DMONO, sempre 1) */
    int quantum;                /* ms */
    TipoPolitica politica;
    ModoFila modo_fila;         /* com -DMONO, sempre FILA_GLOBAL */
    int n_processos;
} Config;
extern Config g_cfg;            /* definido em main.c; so leitura depois de entrada_ler */
#endif
