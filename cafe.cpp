/*---------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* Laboratorio 7 
* Ejercicio 01
* Autores: Vianka Castro 23201
*           Ricardo Godinez 23247
*
* Fecha: 2024/09/25
*
*   Reporte de ventas del mes de julio y agosto
*   Procesos paralelos
*
*----------------------------------------
*/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <string>

// Declaración de mutex y variable condicional
pthread_mutex_t lock;
pthread_cond_t cond;

pthread_t tid[16];

//Inializacion de arrays de contenido en el excel
const char *productos[8] = {
    "Porción pastel de chocolate",
    "White mocha",
    "Cafe americano 8oz",
    "Latte 8oz",
    "Toffee coffee",
    "Cappuccino 8oz",
    "S'mores Latte",
    "Cafe tostado molido"};  
double precioUnitario[8] = {60.00,32.00,22.00,24.00,28.00,24.00,32.00,60.00};
double costoFijo_Unidad [8] = {20.00,19.20,13.20,17.20,20.10,17.20,23.00,20.00};
//Como ambos meses vendieron cantidades distintas
int unidadesVendidas[16] = {300,400,1590,200,390,1455,800,60,250,380,800,250,600,1200,1540,15};
double ventas_Producto[16];
double utilidades_Producto[16];

int ready = 0;  // Variable para condición

//Funcion para calcular monto en ventas por producto
// Unidades vendidas x Precio unitario
void* montoVentas(void *arg){

     int index = *((int *)arg);
    
    sleep(1);  // Simular procesamiento

    double monto = unidadesVendidas[index] * precioUnitario[index % 8];
    
    // Protección con mutex para acceso a la variable compartida
    pthread_mutex_lock(&lock);
    ventas_Producto[index] = monto;
    ready++;  // Incrementar la condición
    pthread_cond_signal(&cond);  // Notificar que se completó el cálculo
    pthread_mutex_unlock(&lock);

    printf("Calculo hilo %d iniciado\n", index);
    sleep(1);
    printf("Calculo hilo %d terminado\n", index);

    pthread_exit(NULL);
}

// Calculo de utilidades por producto
// Ventas por producto - (Unidades vendidas * Costo fijo por unidad)
void* utilidades(void *arg) {
     int index = *((int *)arg);

    // Esperar hasta que los cálculos de ventas estén listos
    pthread_mutex_lock(&lock);
    while (ready <= index) {  // Esperar si no está listo el cálculo de ventas
        pthread_cond_wait(&cond, &lock);
    }
    double utilidad = ventas_Producto[index] - (unidadesVendidas[index] * costoFijo_Unidad[index % 8]);
    utilidades_Producto[index] = utilidad;
    pthread_mutex_unlock(&lock);

    printf("Calculo hilo %d iniciado\n", index);
    sleep(1);
    printf("Calculo hilo %d terminado\n", index);

    pthread_exit(NULL);
}


int main(void)
{
   // Inicialización de mutex y variable condicional
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    int indices[16], err;
    
    // Creación de hilos para el cálculo de ventas en JULIO    
    for (int i = 0; i < 8 ; i++) {
        indices[i] = i;
        err = pthread_create(&(tid[i]), NULL, &montoVentas, &indices[i]);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));  
    }

    for (int i = 0; i < 8; i++) {
        pthread_join(tid[i], NULL);
    }

    // Creación de hilos para el cálculo de ventas en AGOSTO
    for (int i = 8; i < 16; i++) {
        indices[i] = i;
        err = pthread_create(&(tid[i]), NULL, &montoVentas, &indices[i]);
        if (err != 0)
            printf("No se pudo crear el hilo %d: %s\n", i, strerror(err));
    }

    for (int i = 8; i < 16; i++) {
        pthread_join(tid[i], NULL);
    }

    // Creación de hilos para el cálculo de utilidades en JULIO
    for (int i = 0; i < 8; i++) {
        err = pthread_create(&(tid[i]), NULL, &utilidades, &indices[i]);
        if (err != 0)
            printf("No se pudo crear el hilo %d: %s\n", i, strerror(err));
    }

    for (int i = 0; i < 8; i++) {
        pthread_join(tid[i], NULL);
    }

    // Creación de hilos para el cálculo de utilidades en AGOSTO
    for (int i = 8; i < 16; i++) {
        err = pthread_create(&(tid[i]), NULL, &utilidades, &indices[i]);
        if (err != 0)
            printf("No se pudo crear el hilo %d: %s\n", i, strerror(err));
    }

    for (int i = 8; i < 16; i++) {
        pthread_join(tid[i], NULL);
    }

    // Cálculo total de ventas y utilidades
    double totalVentasJulio = 0, totalUtilidadJulio = 0;
    double totalVentasAgosto = 0, totalUtilidadAgosto = 0;

    for (int i = 0; i < 8; i++) {
        totalVentasJulio += ventas_Producto[i];
        totalUtilidadJulio += utilidades_Producto[i];
    }

    for (int i = 8; i < 16; i++) {
        totalVentasAgosto += ventas_Producto[i];
        totalUtilidadAgosto += utilidades_Producto[i];
    }

    printf("--------------------------------------\n");
    printf("REPORTE DEL MES DE JULIO\n");
    printf("--- Monto de Ventas por Producto ---\n");
    for (int i = 0; i < 8; i++) {
        printf("- %s: Q%.2f\n", productos[i], ventas_Producto[i]);
    }
    printf("--- Utilidad por Producto --- \n");
    for (int i = 0; i < 8; i++) {
        printf("%s: Q%.2f\n", productos[i], utilidades_Producto[i]);
    }
    printf("--- Monto Total Ventas de JULIO: Q%.2f\n", totalVentasJulio);
    printf("--- Utilidad del mes de JULIO: Q%.2f\n", totalUtilidadJulio);

    printf("--------------------------------------\n");
    printf("REPORTE DEL MES DE AGOSTO\n");
    printf("--- Monto de Ventas por Producto ---\n");
    for (int i = 8; i < 16; i++) {
        printf("- %s: Q%.2f\n", productos[i % 8], ventas_Producto[i]);
    }
    printf("--- Utilidad por Producto --- \n");
    for (int i = 8; i < 16; i++) {
        printf("%s: Q%.2f\n", productos[i % 8], utilidades_Producto[i]);
    }
    printf("--- Monto Total Ventas de AGOSTO: Q%.2f\n", totalVentasAgosto);
    printf("--- Utilidad del mes de AGOSTO: Q%.2f\n", totalUtilidadAgosto);

    // Destruir mutex y variable condicional
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}