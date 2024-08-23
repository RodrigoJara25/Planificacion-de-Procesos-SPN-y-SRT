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

void AlgoritmoSPN(int nro_procesos, int prioridad[], int t_llegada[], int t_CPU[], char letra_proceso[])
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
        /*else if(contador_minimos>2)
        {
            for(int j = 0; j < minIndices.contador; j++)
            {
                if(prioridad[minIndices.indices[j]] == MinimaPrioridad(minIndices, prioridad) && t_CPU[minIndices.indices[j]] == minTiempoCPU && t_llegada[minIndices.indices[j]]==MenorValor(minIndices.contador,minIndices.indices))
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
        }*/
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


    fclose(archivo1);
}

int main()
{
    printf("\033[0;31m");
    printf("------- Algoritmo SPN -------\n\n");
    printf("\033[0;30m");

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
    }

    AlgoritmoSPN(nro_procesos, prioridad, t_llegada, t_CPU, letra_proceso);

    printf("\n\n");
    printf("------- Algoritmo SPN -------\n\n");

    return 0;
}
