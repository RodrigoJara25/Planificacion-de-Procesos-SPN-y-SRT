#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct MinimosIndices
{
    int indices[5]; // Supongamos un l mite m ximo de 5 procesos
    int contador;
};

int MenorValor(int nro_procesos, int arreglo[])
{
    int minValor = arreglo[0]; //arreglo[0]
    for(int i=0; i<nro_procesos; i++)
    {
        if(arreglo[i] <= minValor)
        {
            minValor = arreglo[i];
        }
    }
    return minValor;
}

struct MinimosIndices EncontrarIndicesMinimos(int nro_procesos, int arreglo[])
{
    struct MinimosIndices resultados;
    resultados.contador = 0;

    int minValor = MenorValor(nro_procesos, arreglo);

    for(int i = 0; i < nro_procesos; i++)
    {
        if(arreglo[i] == minValor)
        {
            resultados.indices[resultados.contador] = i;
            resultados.contador++;
        }
    }
    return resultados;
}

int DesempateTiempoLlegada(int nro_procesos, int t_CPU[], int t_llegada[])
{
    int minT_CPU = t_CPU[0];
    struct MinimosIndices resultados = EncontrarIndicesMinimos(nro_procesos, t_llegada);

    for (int i = 0; i < resultados.contador; i++)
    {
        if(t_CPU[resultados.indices[i]] < minT_CPU)
        {
            minT_CPU = t_CPU[resultados.indices[i]];
        }
    }
    return minT_CPU;
}

int MinimaPrioridad(struct MinimosIndices minIndices, int prioridad[])
{
    int minPrioridad = 1000;
    for(int i = 0; i < minIndices.contador; i++)
    {
        if(prioridad[minIndices.indices[i]] < minPrioridad)
        {
            minPrioridad = prioridad[minIndices.indices[i]];
        }
    }
    return minPrioridad;
}

int SumaTiemposCPU(int nro_procesos, int t_CPU[])
{
    int suma = 0;

    for(int i=0; i<nro_procesos; i++)
    {
        suma = suma + t_CPU[i];
    }
    return suma;
}

int MenorTiempoLLegada(struct MinimosIndices minIndices, int t_llegada_fijo[])
{
    int minTiempoLlegada = 10000;
    for(int i=0; i<minIndices.contador; i++)
    {
        if(t_llegada_fijo[minIndices.indices[i]]<minTiempoLlegada)
        {
            minTiempoLlegada = t_llegada_fijo[minIndices.indices[i]];
        }
    }
    return minTiempoLlegada;
}

float AlgoritmoSPN(int nro_procesos, int prioridad[], int t_llegada[], int t_CPU[], char letra_proceso[])
{
    int t_actual = 0;
    int t_total = SumaTiemposCPU(nro_procesos, t_CPU);

    int proceso_actual;

    int prioridad_fijo[nro_procesos];
    int t_llegada_fijo[nro_procesos];
    int t_CPU_fijo[nro_procesos];
    for(int i=0; i<nro_procesos; i++)
    {
        prioridad_fijo[i]=prioridad[i];
        t_llegada_fijo[i]=t_llegada[i];
        t_CPU_fijo[i]=t_CPU[i];
    }

    //llenar matriz con valores default
    int tabla[nro_procesos][t_total];
    for (int i = 0; i < nro_procesos; i++)
    {
        for (int j = 0; j < t_total; j++)
        {
            tabla[i][j] = -1;
        }
    }

    t_actual = MenorValor(nro_procesos, t_llegada);
    for(int i=0; i<nro_procesos; i++)
    {
        for(int z=0; z<nro_procesos;z++)
        {
            for(int t=0; t<t_total; t++)
            {
                if(t_llegada[z]<=t)
                {
                    tabla[z][t]=0;
                }
            }
            if(t_llegada[z]<t_actual)
            {
                t_llegada[z]=t_actual;
            }
        }
        struct MinimosIndices minIndices = EncontrarIndicesMinimos(nro_procesos, t_llegada);
        int minTiempoCPU = DesempateTiempoLlegada(nro_procesos, t_CPU, t_llegada);

        int contador_minimos_CPU = 0;
        for (int j = 0; j < minIndices.contador; j++)
        {
            if(t_CPU[minIndices.indices[j]] == minTiempoCPU)
            {
                contador_minimos_CPU++;
            }
        }

        if(contador_minimos_CPU>=2)
        {
            int contador_minimos_prioridad=0;
            for(int j = 0; j < minIndices.contador; j++)
            {
                if(prioridad[minIndices.indices[j]] == MinimaPrioridad(minIndices, prioridad) && t_CPU[minIndices.indices[j]] == minTiempoCPU)
                {
                    contador_minimos_prioridad++;
                }
            }
            if(contador_minimos_prioridad==1)
            {
                for(int j = 0; j < minIndices.contador; j++)
                {
                    if(prioridad[minIndices.indices[j]] == MinimaPrioridad(minIndices, prioridad) && t_CPU[minIndices.indices[j]] == minTiempoCPU)
                    {
                        proceso_actual = minIndices.indices[j];
                        tabla[minIndices.indices[j]][t_actual]=1;
                        for(int t=t_actual; t<t_actual+minTiempoCPU; t++)
                        {
                            tabla[minIndices.indices[j]][t] = 1;
                        }
                        for(int r=t_actual+minTiempoCPU; r<t_total; r++)
                        {
                            tabla[minIndices.indices[j]][r] = -1;
                        }
                    }
                }
            }
            else if(contador_minimos_prioridad>=2)
            {
                for(int j = 0; j < minIndices.contador; j++)
                {
                    if(prioridad[minIndices.indices[j]] == MinimaPrioridad(minIndices, prioridad) && t_CPU[minIndices.indices[j]] == minTiempoCPU && t_llegada[minIndices.indices[j]]== MenorTiempoLLegada(minIndices, t_llegada_fijo))
                    {
                        proceso_actual = minIndices.indices[j];
                        tabla[minIndices.indices[j]][t_actual]=1;
                        for(int t=t_actual; t<t_actual+minTiempoCPU; t++)
                        {
                            tabla[minIndices.indices[j]][t] = 1;
                        }
                        for(int r=t_actual+minTiempoCPU; r<t_total; r++)
                        {
                            tabla[minIndices.indices[j]][r] = -1;
                        }
                    }
                }
            }
        }
        else
        {
            for (int j = 0; j < minIndices.contador; j++)
            {
                if(t_CPU[minIndices.indices[j]] == minTiempoCPU)
                {
                    proceso_actual = minIndices.indices[j];
                    tabla[minIndices.indices[j]][t_actual]=1;
                    for(int t=t_actual; t<t_actual+minTiempoCPU; t++)
                    {
                        tabla[minIndices.indices[j]][t] = 1;
                    }
                    for(int r=t_actual+minTiempoCPU; r<t_total; r++)
                    {
                        tabla[minIndices.indices[j]][r] = -1;

                    }
                }
            }
        }
        //printf("%c",letra_proceso[proceso_actual]);

        t_CPU[proceso_actual] = INT_MAX;
        prioridad[proceso_actual] = INT_MAX;
        t_llegada[proceso_actual] = INT_MAX;

        t_actual = t_actual + minTiempoCPU;

    }

    //printf("\nTiempo actual: %i",t_actual);
    //printf("\nTiempo total: %i",t_total);

    //mostrar tabla
    printf("\n");
    for (int i = 0; i < nro_procesos; i++)
    {
        for (int j = 0; j < t_total; j++)
        {
            if(tabla[i][j]==-1)
            {
                printf("\033[0;36m");
                printf(" - |");
                printf("\033[0;30m");
            }
            else if(tabla[i][j]==0)
            {
                printf("\033[0;33m");
                printf(" W |");
                printf("\033[0;30m");
            }
            else if(tabla[i][j]==1)
            {
                printf("\033[0;31m");
                printf(" X |");
                printf("\033[0;30m");
            }
        }
        printf("\n");
    }

    //tabla de par metros
    int tiempo_espera[nro_procesos];
    int tiempo_retorno[nro_procesos];
    for (int i = 0; i < nro_procesos; i++)
    {
        int t_espera=0;
        int t_retorno=0;
        for (int j = 0; j < t_total; j++)
        {
            if(tabla[i][j]==-1)
            {
                continue;
            }
            else if(tabla[i][j]==0)
            {
                t_espera++;
                t_retorno++;
            }
            else if(tabla[i][j]==1)
            {
                t_retorno++;
            }
        }
        tiempo_espera[i]=t_espera;
        tiempo_retorno[i]=t_retorno;
    }
    //mostrar tabla
    printf("\033[0;32m");
    printf("\nProceso\t Prioridad\t t_llegada\t t_CPU\t t_retorno\t t_espera\n");
    printf("\033[0;30m");
    for(int i=0; i<nro_procesos; i++)
    {
        printf("\033[0;33m");
        printf("%c\t %i\t\t %i\t\t %i\t %i\t\t %i\n",letra_proceso[i],prioridad_fijo[i],t_llegada_fijo[i],t_CPU_fijo[i],tiempo_retorno[i],tiempo_espera[i]);
        printf("\033[0;0m");
        //printf("\n");
    }

    //Resultados
    float tiempo_retorno_global = (float) SumaTiemposCPU(nro_procesos, tiempo_retorno)/nro_procesos;
    float tiempo_espera_global = (float) SumaTiemposCPU(nro_procesos, tiempo_espera)/nro_procesos;
    float tiempo_servicio = tiempo_retorno_global - tiempo_espera_global;
    float tiempo_retorno_normalizado = tiempo_retorno_global / tiempo_servicio;

    printf("\n");
    printf("Tiempo de retorno             | %.3f",tiempo_retorno_global);
    printf("\n");
    printf("Tiempo de espera              | %.3f",tiempo_espera_global);
    printf("\n");
    printf("Tiempo de servicio            | %.3f",tiempo_servicio);
    printf("\n");
    printf("Tiempo de retorno normalizado | %.3f",tiempo_retorno_normalizado);

        // ARCHIVO TXT: tabla de planificacion
    FILE *archivo1 = fopen("datos2.txt", "w");
    if(archivo1 !=  NULL)
    {
        fprintf(archivo1, "%s %10s %15s %10s %15s %15s\n", "Proceso", "Prioridad", "Tiempo_llegada", "Tiempo_CPU", "Tiempo_Espera", "Tiempo_retorno");

        for (int i = 0; i < nro_procesos; i++)
        {
            fprintf(archivo1, "%c %10d %15d %10d %15d %15d\n",
                    letra_proceso[i], prioridad_fijo[i], t_llegada_fijo[i], t_CPU_fijo[i], tiempo_espera[i], tiempo_retorno[i]);
        }
    }

    // RESULTADOS
    fprintf(archivo1, "\n");
    fprintf(archivo1, "Tiempo de retorno             | %.3f",tiempo_retorno_global);
    fprintf(archivo1, "\n");
    fprintf(archivo1, "Tiempo de espera              | %.3f",tiempo_espera_global);
    fprintf(archivo1, "\n");
    fprintf(archivo1, "Tiempo de servicio            | %.3f",tiempo_servicio);
    fprintf(archivo1, "\n");
    fprintf(archivo1, "Tiempo de retorno normalizado | %.3f",tiempo_retorno_normalizado);

    //RESULTADOS: TABLA X W
    fprintf(archivo1, "\n\n");
    for (int i = 0; i < nro_procesos; i++)
    {
        for (int j = 0; j < t_total; j++)
        {
            if(tabla[i][j]==-1)
            {
                fprintf(archivo1, " - |");
            }
            else if(tabla[i][j]==0)
            {
                fprintf(archivo1, " W |");
            }
            else if(tabla[i][j]==1)
            {
                fprintf(archivo1, " X |");
            }
        }
        fprintf(archivo1, "\n");
    }
    return tiempo_retorno_normalizado;


    fclose(archivo1);
}
#define MAX 5

typedef struct Proceso {
    char nombre;
    int prioridad, t_llegada, t_cpu, t_ejecucion, t_servicio, t_espera, t_retorno;
    float t_normalizado;
} Proceso;


float AlgoritmoSRT(Proceso procesos[MAX])
{
    int t_llegada_fijo[MAX];
    for(int i=0; i<MAX; i++)
    {
        t_llegada_fijo[i] = procesos[i].t_llegada;
    }

    // Ordenar los procesos por tiempo de llegada y desempatar seg n la prioridad y el tiempo de CPU
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



    // Implementaci n del algoritmo SRT
    int tiempo_actual = 0;
    int procesos_terminados = 0;
    int suma = 0;
    for(int i=0; i<MAX; i++)
    {
        suma = suma + procesos[i].t_cpu;
    }
    int tabla[MAX][suma];
    for(int i=0; i<MAX; i++)
    {
        for(int j=0; j<suma; j++)
        {
            tabla[i][j] = -1;
        }
    }

    while (procesos_terminados < MAX) {
        int proceso_actual = -1;
        int menor_tiempo_restante = 99999999;

        for (int i = 0; i < MAX; i++)
        {
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

        //char x = 'x';
        //char w = 'w';
        //char null = ' ';

        if (proceso_actual != -1)
        {
            // Ejecutar el proceso actual durante un ciclo de CPU
            printf("Tiempo %d: Ejecutando proceso %c\n", tiempo_actual, procesos[proceso_actual].nombre);
            tabla[proceso_actual][tiempo_actual]=1;
            procesos[proceso_actual].t_ejecucion++;
            procesos[proceso_actual].t_servicio++;

            for (int i = 0; i < MAX; i++)
            {
                if (i != proceso_actual && procesos[i].t_llegada <= tiempo_actual && procesos[i].t_ejecucion < procesos[i].t_cpu)
                {
                    procesos[i].t_espera++;
                }
            }

            tiempo_actual++;

            if (procesos[proceso_actual].t_ejecucion == procesos[proceso_actual].t_cpu)
            {
                procesos_terminados++;
                procesos[proceso_actual].t_retorno = tiempo_actual - procesos[proceso_actual].t_llegada;
                procesos[proceso_actual].t_normalizado = (float)procesos[proceso_actual].t_retorno / procesos[proceso_actual].t_cpu;
            }
        }
        else
        {
            // No hay procesos listos, esperar
            printf("Tiempo %d: Esperando proceso\n", tiempo_actual);
            tiempo_actual++;
        }
    }

    // Mostrar tiempos al final
    printf("\033[0;31m");
    printf("\nTIEMPOS:\n\n");
    printf("\033[0;32m");
    printf("| Proceso | Tiempo de Retorno |  Tiempo de Espera |\n");
    printf("\033[0;32m");

    // Ordenar procesos alfab ticamente
    for (int i = 0; i < MAX - 1; i++) {
        for (int j = 0; j < MAX - i - 1; j++) {
            if (procesos[j].nombre > procesos[j + 1].nombre) {
                Proceso temp = procesos[j];
                procesos[j] = procesos[j + 1];
                procesos[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < MAX; i++)
    {
        printf("|    %c    |         %2d        |         %2d        |\n",
               procesos[i].nombre, procesos[i].t_retorno, procesos[i].t_espera);
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
    printf("\n\n\n");
    printf("\033[0;35m");
    printf("Tiempo de Retorno                 |%f     ",prom_retorno);
    float resta = prom_retorno-prom_servicio;
    printf("\nTiempo de Espera                |%f    ",resta);
    printf("\nTiempo de Servicio              |%f",prom_servicio);
    printf("\nTiempo de Retorno Normalizado   |%f",t_normalizado);
    printf("\n\n\n");

    for(int i=0; i<MAX; i++)
    {
        for(int j=t_llegada_fijo[i]; j<procesos[i].t_retorno + t_llegada_fijo[i]; j++) //tiempo actual
        {
            if(tabla[i][j]== -1)
            {
                tabla[i][j] = 0;
            }
        }
    }

    //printear tabla
    for(int i=0; i<MAX; i++)
    {
        for(int j=0; j<suma; j++)
        {
            if(tabla[i][j]==-1)
            {
                printf("\033[0;37m");
                printf(" - |");
                printf("\033[0;30m");
            }
            else if(tabla[i][j]==0)
            {
                printf("\033[0;31m");
                printf(" W |");
                printf("\033[0;30m");
            }
            else if(tabla[i][j]==1)
            {
                printf("\033[0;34m");
                printf(" X |");
                printf("\033[0;30m");
            }
        }
        printf("\n");
    }

        // ARCHIVO TXT: tabla de planificacion
        FILE *archivo1 = fopen("datos1.txt", "w");
    if (archivo1 != NULL) {
        fprintf(archivo1, "%s %10s %15s %10s %15s %15s\n", "Proceso", "Prioridad", "Tiempo_llegada", "Tiempo_CPU", "Tiempo_Espera", "Tiempo_retorno");

        // Ordenar procesos alfab ticamente
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
    }

        fprintf(archivo1, "\n\n\n");
        fprintf(archivo1, "Tiempo de Retorno                 |%f     ",prom_retorno);
        fprintf(archivo1, "\nTiempo de Espera                |%f    ",resta);
        fprintf(archivo1, "\nTiempo de Servicio              |%f",prom_servicio);
        fprintf(archivo1, "\nTiempo de Retorno Normalizado   |%f",t_normalizado);
        fprintf(archivo1, "\n\n\n");

    for(int i=0; i<MAX; i++)
    {
        for(int j=0; j<suma; j++)
        {
            if(tabla[i][j]==-1)
            {

                fprintf(archivo1, " - |");

            }
            else if(tabla[i][j]==0)
            {

                fprintf(archivo1, " W |");

            }
            else if(tabla[i][j]==1)
            {

                fprintf(archivo1, " X |");

            }
        }
        fprintf(archivo1, "\n");
    }
    return t_normalizado;

    fclose(archivo1);
}

int main()
{
    Proceso procesos[MAX];
    //printf("\033[0;30m");

    printf("\033[0;35m");
    printf("******* Ingresar Datos *******\n");
    printf("\033[33m");

    //n mero de procesos
    int nro_procesos;
    printf("Ingresar numero de procesos: ");
    scanf("%i",&nro_procesos);

    //llenar datos de los procesos
    int prioridad[nro_procesos];
    int t_llegada[nro_procesos];
    int t_CPU[nro_procesos];
    char letra_proceso[nro_procesos];

    for(int i=0; i<nro_procesos; i++)
    {
        letra_proceso[i] = 'A' + i; //van aumentando las letras
        printf("\nDatos del Proceso %c \n",letra_proceso[i]);

        printf("Prioridad: ");
        scanf("%i",&prioridad[i]);

        printf("Tiempo de llegada: ");
        scanf("%i",&t_llegada[i]);

        printf("Tiempo de CPU: ");
        scanf("%i",&t_CPU[i]);


        procesos[i].nombre = 'A' + i;
        procesos[i].t_ejecucion = 0;
        procesos[i].t_servicio = 0;
        procesos[i].t_espera = 0;
        procesos[i].t_retorno = 0;
        procesos[i].t_normalizado = 0.0;

        procesos[i].prioridad = prioridad[i];
        procesos[i].t_llegada = t_llegada[i];
        procesos[i].t_cpu = t_CPU[i];
    }

    printf("\033[0;31m");
    printf("------- Algoritmo SPN -------\n\n");

    float retorno_normalizado_SPN = AlgoritmoSPN(nro_procesos, prioridad, t_llegada, t_CPU, letra_proceso);

    printf("\n\n");
    printf("------- Algoritmo SPN -------\n\n");


    //Algoritmo SRT
    printf("\033[0;31m");
    printf("------- Algoritmo SRT -------\n\n");

    float retorno_normalizado_SRT = AlgoritmoSRT(procesos);

    printf("\n\n");
    printf("------- Algoritmo SRT -------\n\n");


    printf("\033[0;37m");
    if(retorno_normalizado_SPN<retorno_normalizado_SRT)
    {
        printf("EL MJOR ALGORITMO ES: SPN");
    }
    else if(retorno_normalizado_SRT<retorno_normalizado_SPN)
    {
        printf("EL MJOR ALGORITMO ES: SRT");
    }
    else
    {
        printf("EL MEJOR ALGORTIMO ES: SRT O SPN");
    }

    return 0;
}
