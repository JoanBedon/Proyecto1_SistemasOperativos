# Proyecto1_SistemasOperativos
## Problema.

Se buscar el simular el procesamiento de 20 transacciones de diferentes tipos de forma paralela y medir el tiempo total de la ejecucion en paralelo, buscando solucionar la concurrencia y el paralelismo.

## Procedimiento para la Ejecución.
### Compilar.

Para compilar es necesario utilizar el compilador GCC e incluir el enlace a la libreria de hilos POSIX (-pthread o -lpthread).

### Ejecución.
Se ejecuta es decir que se inicia la simulación del proceso en paralelo.
### Fases del programa.
1. Inicializacion: El programa inicia 20 transacciones y una duración alaeatoria

2. Medición y Creación de Hilos:
   
* Tiempo de inicio con (clock).
* Crea 4 Hilos(pthread_create).
* Cada Hilo recibe un ID y determina el subconjunto de transacciones que le toca procesar.
  
3. Procesamiento Paralelo:
   
* Simular_consulta_bd().
* Simular_procesar_archivo().
* Simular_calculo().
* Las salidas de la consola son envueltas en un phread_mutex sea lock() o unlock() segun corresponda para tener la integridad en la concurrencia.

4. Espera y Finalizacion.
- El hilo principal espera a que los 4 hilos finalicen la ejecucion y luego registra el tiempo final.

## Resultados.

1. Medición del Rendimiento.
   
El resultado más importante es la diferencia entre el tiempo que el programa tardara de forma secuencia y el tiempo que tarde al realizarlo en paralelo

*Tiempo Teórico Secuencial: Es el valor que representa la suma de la duración en las 20 transacciones.
* Tiempo de Ejecución Paralelo: Es el valor que el programa muestra como Tiempo total, pero dado que se usan 4 hilos para la distribucion el tiempo debe ser significativamente menor.
* Se busca como el tiempo de respuesta se reduce de forma drastica gracias a realizar los hilos (Paralelismo).

2. Resumen de Transacciones.
Se espera que cuando se finalice el programa se muestra las 20 transacciones en forma de listado, cada uno con su respectivo ID.

| Tipo  | Función | 
|----------|----------|
| Consulta a BD (0)    | simular_consulta_bd()   | 
| Procesamiento     | simular_procesar_archivo()   | 
| Cálculo    | simular_calculo()   | 


<img width="1057" height="401" alt="image" src="https://github.com/user-attachments/assets/8b619b08-0a9d-47ad-918a-e6e9d01fad8b" />


