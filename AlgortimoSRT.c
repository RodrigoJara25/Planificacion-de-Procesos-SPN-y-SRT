#include <stdio.h>
#include <stdlib.h>

#define MAX 5

typedef struct Proceso {
    char nombre;
    int prioridad, t_llegada, t_cpu, t_ejecucion, t_servicio, t_espera, t_retorno;
    float t_normalizado;
} Proceso;

void ResultadoFinal(Proceso procesos[MAX]) {
    FILE *archivo1 = fopen("datos1.txt", "w");
    if (archivo1 != NULL) {
        fprintf(archivo1, "%s %10s %15s %10s %15s %15s\n", "Proceso", "Prioridad", "Tiempo_llegada", "Tiempo_CPU", "Tiempo_Espera", "Tiempo_retorno");

        // Ordenar procesos alfabéticamente
        for (int i = 0; i < MAX - 1; i++) {
            for (int j = 0; j < MAX - i - 1; j++) {
                if (procesos[j].nombre > procesos[j + 1].nombre) {
                    Proceso temp = procesos[j];
                    procesos[j] = procesos[j + 1];
                    procesos[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < MAX; i++) {
            fprintf(archivo1, "%c %10d %15d %10d %15d %15d\n", procesos[i].nombre, procesos[i].prioridad, procesos[i].t_llegada, procesos[i].t_cpu, procesos[i].t_espera, procesos[i].t_retorno);
        }

        fclose(archivo1);
    }
}

void ProcesoTabla(Proceso procesos[MAX]) {
    char x = 'x';
    char w = 'w';
    char null = ' ';

    // Ordenar procesos alfabéticamente
    for (int i = 0; i < MAX - 1; i++) {
        for (int j = 0; j < MAX - i - 1; j++) {
            if (procesos[j].nombre > procesos[j + 1].nombre) {
                Proceso temp = procesos[j];
                procesos[j] = procesos[j + 1];
                procesos[j + 1] = temp;
            }
        }
    }

    // Imprimir la tabla
    printf("  |");
    for (int i = 0; i < 20; i++) {
        printf("%6d|", i);
    }
    printf("\n");

    for (int i = 0; i < MAX; i++) {
        printf("%c |", procesos[i].nombre);
        for (int j = 0; j < 20; j++) {
            if (j < procesos[i].t_espera) {
                printf("%6c|", w);
            } else if (j < procesos[i].t_espera + procesos[i].t_cpu) {
                printf("%6c|", x);
            } else {
                printf("%6c|", null);
            }
        }
        printf("\n");
    }
}

int main() {
    Proceso procesos[MAX];

    // Ingreso de datos para los procesos
    for (int i = 0; i < MAX; i++) {
        system("clear");

        procesos[i].nombre = 'A' + i;
        procesos[i].t_ejecucion = 0;
        procesos[i].t_servicio = 0;
        procesos[i].t_espera = 0;
        procesos[i].t_retorno = 0;
        procesos[i].t_normalizado = 0.0;

        printf("Detalles del proceso %c:\n", procesos[i].nombre);

        do {
            printf("Prioridad: ");
            scanf("%d", &procesos[i].prioridad);
            getchar();
        } while (procesos[i].prioridad < 0);

        do {
            printf("Tiempo de llegada: ");
            scanf("%d", &procesos[i].t_llegada);
            getchar();
        } while (procesos[i].t_llegada < 0);

        do {
            printf("Tiempo de CPU: ");
            scanf("%d", &procesos[i].t_cpu);
            getchar();
        } while (procesos[i].t_cpu < 0);
    }
    system("clear");

    // Ordenar los procesos por tiempo de llegada y desempatar según la prioridad y el tiempo de CPU
    for (int i = 0; i < MAX - 1; i++) {
        for (int j = 0; j < MAX - i - 1; j++) {
            if (procesos[j].t_llegada > procesos[j + 1].t_llegada ||
                (procesos[j].t_llegada == procesos[j + 1].t_llegada && procesos[j].prioridad > procesos[j + 1].prioridad) ||
                (procesos[j].t_llegada == procesos[j + 1].t_llegada && procesos[j].prioridad == procesos[j + 1].prioridad && procesos[j].t_cpu > procesos[j + 1].t_cpu)) {
                Proceso temp = procesos[j];
                procesos[j] = procesos[j + 1];
                procesos[j + 1] = temp;
            }
        }
    }

    // Implementación del algoritmo SRT
    int tiempo_actual = 0;
    int procesos_terminados = 0;

    while (procesos_terminados < MAX) {
        int proceso_actual = -1;
        int menor_tiempo_restante = 99999999;

        for (int i = 0; i < MAX; i++) {
            if (procesos[i].t_llegada <= tiempo_actual && procesos[i].t_ejecucion < procesos[i].t_cpu) {
                if (procesos[i].t_cpu - procesos[i].t_ejecucion < menor_tiempo_restante) {
                    proceso_actual = i;
                    menor_tiempo_restante = procesos[i].t_cpu - procesos[i].t_ejecucion;
                } else if (procesos[i].t_cpu - procesos[i].t_ejecucion == menor_tiempo_restante &&
                           procesos[i].prioridad < procesos[proceso_actual].prioridad) {
                    proceso_actual = i;
                }
            }
        }

        if (proceso_actual != -1) {
            // Ejecutar el proceso actual durante un ciclo de CPU
            printf("Tiempo %d: Ejecutando proceso %c\n", tiempo_actual, procesos[proceso_actual].nombre);
            procesos[proceso_actual].t_ejecucion++;
            procesos[proceso_actual].t_servicio++;

            for (int i = 0; i < MAX; i++) {
                if (i != proceso_actual && procesos[i].t_llegada <= tiempo_actual && procesos[i].t_ejecucion < procesos[i].t_cpu) {
                    procesos[i].t_espera++;
                }
            }

            tiempo_actual++;

            if (procesos[proceso_actual].t_ejecucion == procesos[proceso_actual].t_cpu) {
                procesos_terminados++;
                procesos[proceso_actual].t_retorno = tiempo_actual - procesos[proceso_actual].t_llegada;
                procesos[proceso_actual].t_normalizado = (float)procesos[proceso_actual].t_retorno / procesos[proceso_actual].t_cpu;
            }
        } else {
            // No hay procesos listos, esperar
            printf("Tiempo %d: Esperando proceso\n", tiempo_actual);
            tiempo_actual++;
        }
    }

    // Mostrar tiempos al final
    printf("\033[0;31m");
    printf("\nTIEMPOS:\n\n");
    printf("\033[0;32m");
    printf("| Proceso | Tiempo de Retorno | Tiempo de Servicio   |  Tiempo de Espera |\n");
    printf("\033[0;32m");

    // Ordenar procesos alfabéticamente
    for (int i = 0; i < MAX - 1; i++) {
        for (int j = 0; j < MAX - i - 1; j++) {
            if (procesos[j].nombre > procesos[j + 1].nombre) {
                Proceso temp = procesos[j];
                procesos[j] = procesos[j + 1];
                procesos[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < MAX; i++) {
        printf("|    %c    |         %2d        |          %2d          |         %2d        |\n",
               procesos[i].nombre, procesos[i].t_retorno, procesos[i].t_servicio, procesos[i].t_espera);
    }
    printf("\033[0m");

    // Tiempo normalizado es TR/TS
    float prom_retorno = 0;
    float prom_servicio = 0;
    float t_normalizado = 0;

    float suma_retorno = 0;
    float suma_servicio = 0;
    float suma_espera = 0;

    for (int i = 0; i < MAX; i++) {
        suma_retorno += procesos[i].t_retorno;
        suma_servicio += procesos[i].t_servicio;
        suma_espera += procesos[i].t_espera;
    }

    prom_retorno = suma_retorno / MAX;
    prom_servicio = suma_servicio / MAX;
    t_normalizado = prom_retorno / prom_servicio;

    printf("\n");
    printf("\033[0;35m");
    printf("------ EL TIEMPO NORMALIZADO ES: %.2f ------\n", t_normalizado);

    ResultadoFinal(procesos);

    ProcesoTabla(procesos);

    return 0;
}
