#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


/**
 * @file ProyectoGF.c
 * @brief Sistema de procesamiento paralelo de transacciones usando hilos POSIX.
 *
 * Este programa simula 3 tipos de transacciones: consultas a BD,
 * operaciones con archivos y cálculos complejos.  
 * Las transacciones se reparten entre varios hilos y se mide
 * el tiempo total de ejecución en paralelo.
 */

#define NUM_TRANSACCIONES 20
#define NUM_HILOS 4

// Variables globales para simular transacciones
int transaccion_id[NUM_TRANSACCIONES];
int transaccion_tipo[NUM_TRANSACCIONES]; // 0=BD, 1=Archivo, 2=Cálculo
int transaccion_duracion_ms[NUM_TRANSACCIONES]; // EN MILISEGUNDOS (ms)
double transaccion_resultado[NUM_TRANSACCIONES];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Simula una transacción de tipo consulta a base de datos.
 *
 * Esta función representa una operación de acceso a BD usando un tiempo de
 * espera configurable. Calcula un resultado simple basado en el ID real
 * de la transacción.
 *
 * @param indice Índice de la transacción dentro de los arreglos globales.
 */
void simular_consulta_bd(int indice) {
    int id_real = transaccion_id[indice];  // ← Obtener el ID real
    printf("Consultando BD para transacción %d\n", id_real);
    usleep(transaccion_duracion_ms[indice] * 1000);
    transaccion_resultado[indice] = id_real * 3.14;  // ← Usar ID real
}

/**
 * @brief Simula una transacción de tipo procesamiento de archivo.
 *
 * Genera un retardo equivalente a su duración en milisegundos y
 * calcula un resultado final basado en el ID de la transacción.
 *
 * @param indice Índice de la transacción dentro de los arreglos globales.
 */
void simular_procesar_archivo(int indice) {
    int id_real = transaccion_id[indice];
    printf("Procesando archivo para transacción %d\n", id_real);
    usleep(transaccion_duracion_ms[indice] * 1000);
    transaccion_resultado[indice] = id_real * 100;  // ← Usar ID real
}

/**
 * @brief Simula una transacción de tipo cálculo complejo.
 *
 * Realiza una espera equivalente a la duración configurada y ejecuta
 * un bucle intensivo para generar carga real de CPU.
 *
 * @param indice Índice de la transacción dentro de los arreglos globales.
 */
void simular_calculo(int indice) {
    int id_real = transaccion_id[indice];
    printf("Hilo: Ejecutando cálculo para transacción %d (%d ms)\n", 
           id_real, transaccion_duracion_ms[indice]);
    usleep(transaccion_duracion_ms[indice] * 1000);
    
    double resultado = 0;
    for (int i = 0; i < 100000; i++) {
        resultado += (id_real * i) * 0.00001;  // ← CORREGIDO
    }
    transaccion_resultado[indice] = resultado;
}

/**
 * @brief Función principal ejecutada por cada hilo.
 *
 * Cada hilo procesa un subconjunto de transacciones determinado según su ID.
 * La función selecciona el tipo de operación correspondiente (BD, archivo
 * o cálculo) y actualiza los resultados globales.
 *
 * Se usan mutex para evitar interferencia entre hilos al imprimir mensajes.
 *
 * @param arg Puntero genérico que contiene el ID del hilo (int*).
 * @return Siempre retorna NULL.
 */
void *procesar_transacciones(void *arg) {
    int id_hilo = *((int*)arg);
    int inicio = (id_hilo - 1) * (NUM_TRANSACCIONES / NUM_HILOS);
    int fin = inicio + (NUM_TRANSACCIONES / NUM_HILOS);
    
    // Ajustar para el último hilo si hay división no exacta
    if (id_hilo == NUM_HILOS) {
        fin = NUM_TRANSACCIONES;
    }
    
    printf("Hilo %d iniciado - procesando transacciones %d a %d\n", id_hilo, inicio, fin-1);
    
    for (int i = inicio; i < fin; i++) {
        pthread_mutex_lock(&mutex);
        printf("Hilo %d procesando transacción %d (tipo: %d, duración: %d ms)\n", 
               id_hilo, transaccion_id[i], transaccion_tipo[i], transaccion_duracion_ms[i]);
        pthread_mutex_unlock(&mutex);
        
        switch (transaccion_tipo[i]) {
            case 0:
                simular_consulta_bd(i);
                break;
            case 1:
                simular_procesar_archivo(i);
                break;
            case 2:
                simular_calculo(i);
                break;
        }
        
        pthread_mutex_lock(&mutex);
        printf("Hilo %d completó transacción %d - Resultado: %.2f\n", 
               id_hilo, transaccion_id[i], transaccion_resultado[i]);
        pthread_mutex_unlock(&mutex);
    }
    
    printf("Hilo %d finalizado\n", id_hilo);
    return NULL;
}

/**
 * @brief Inicializa todas las transacciones del sistema.
 *
 * Asigna:
 * - ID incremental (1..N)
 * - Tipo aleatorio (0=BD, 1=Archivo, 2=Cálculo)
 * - Duración aleatoria entre 100 y 500 ms
 * - Resultado inicial en 0
 *
 * No recibe parámetros ni retorna valores, ya que opera sobre variables globales.
 */
void inicializar_transacciones() {
    srand(time(NULL));
    
    for (int i = 0; i < NUM_TRANSACCIONES; i++) {
        transaccion_id[i] = i + 1;
        transaccion_tipo[i] = rand() % 3;
        // Duración en MILISEGUNDOS (ms) - valores entre 100 y 500 ms
        transaccion_duracion_ms[i] = 100 + (rand() % 400); 
        transaccion_resultado[i] = 0.0;
    }
}

/**
 * @brief Muestra en consola la lista completa de transacciones generadas.
 *
 * Imprime el ID, tipo textual y duración en milisegundos de cada transacción.
 * Útil para verificar la correcta inicialización antes del procesamiento en paralelo.
 */
void mostrar_transacciones() {
    printf("=== TRANSACCIONES GENERADAS ===\n");
    printf("Todas las duraciones están en MILISEGUNDOS (ms)\n\n");
    
    for (int i = 0; i < NUM_TRANSACCIONES; i++) {
        const char *tipo_str;
        switch (transaccion_tipo[i]) {
            case 0: tipo_str = "Consulta BD"; break;
            case 1: tipo_str = "Procesar archivo"; break;
            case 2: tipo_str = "Cálculo"; break;
            default: tipo_str = "Desconocido";
        }
        printf("  Transacción %d: %s (%d ms)\n", 
               transaccion_id[i], tipo_str, transaccion_duracion_ms[i]);
    }
    printf("\n");
}

/**
 * @brief Función principal del programa.
 *
 * Orquesta todo el sistema:
 * - Muestra información inicial sobre las unidades de tiempo utilizadas.
 * - Inicializa y muestra las transacciones.
 * - Crea los hilos de procesamiento.
 * - Mide el tiempo total usando clock() y CLOCKS_PER_SEC.
 * - Espera a que todos los hilos terminen.
 * - Muestra los resultados finales.
 *
 * @return 0 si el programa finaliza correctamente.
 */

int main() {
    printf("=== SISTEMA DE PROCESAMIENTO PARALELO ===\n");
    printf(" UNIDADES DE TIEMPO:\n");
    printf("   - transaccion_duracion_ms: MILISEGUNDOS (ms)\n");
    printf("   - usleep(): MICROSEGUNDOS (μs)\n");
    printf("   - clock(): TICKS del procesador\n");
    printf("   - CLOCKS_PER_SEC: TICKS por segundo\n");
    printf("   - Tiempo total: SEGUNDOS (s)\n\n");
    
    printf("Inicializando %d transacciones con %d hilos...\n\n", 
           NUM_TRANSACCIONES, NUM_HILOS);
    
    // Inicializar transacciones
    inicializar_transacciones();
    mostrar_transacciones();
    
    // Crear hilos
    pthread_t hilos[NUM_HILOS];
    int id_hilos[NUM_HILOS];
    
    // clock() mide en TICKS del procesador
    // CLOCKS_PER_SEC convierte TICKS a SEGUNDOS
    clock_t inicio = clock();
    
    for (int i = 0; i < NUM_HILOS; i++) {
        id_hilos[i] = i + 1;
        if (pthread_create(&hilos[i], NULL, procesar_transacciones, &id_hilos[i]) != 0) {
            perror("Error al crear hilo");
            return 1;
        }
    }
    
    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }
    
    clock_t fin = clock();
    
    // CONVERSIÓN FINAL: TICKS → SEGUNDOS
    double tiempo_total_segundos = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    double tiempo_total_milisegundos = tiempo_total_segundos * 1000;
    
    // Mostrar resultados finales
    printf("\n=== RESULTADOS FINALES ===\n");
    printf(" MEDICIÓN DE TIEMPO:\n");
    //printf("   - Ticks del procesador: %ld\n", (fin - inicio));
    //printf("   - CLOCKS_PER_SEC: %ld\n", CLOCKS_PER_SEC);
    printf("   - Tiempo total: %.3f segundos\n", tiempo_total_segundos);
    printf("   - Tiempo total: %.0f milisegundos\n", tiempo_total_milisegundos);
    
    printf("\nResumen de transacciones:\n");
    for (int i = 0; i < NUM_TRANSACCIONES; i++) {
        printf("  Transacción %d: %.2f\n", transaccion_id[i], transaccion_resultado[i]);
    }
    
    printf("\nProcesamiento paralelo completado exitosamente!\n");
    return 0;
}