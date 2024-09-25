/*---------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* Sistemas Operativos
* Autores: Vianka Castro 23201
*           Ricardo Godinez 23247
*
* Fecha: 2024/09/25
*
* References:
* ------
* "Sincronización: Mutex, Variables condición y Semáforos."  
* Universidad Nacional Autonoma de Mexico
* Facultad de Ingenieria
* "Laboratorio Intel para la academia"
* ---------------------------------------
* Variables de condición p/acceder a 
* region crítica 
* variables de condición.
*----------------------------------------
*/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <string>

pthread_t tid[16];

void* montoVentas(void *arg){

}

void* utilidades(void *arg){

}

int main(void)
{
    //Contador de unidades vendidas
    int i = 0;
    //Contador de hilos
    int j = 0;
    int err;

    char *productos[8] = {"Porción pastel de chocolate","White mocha","Cafe americano 8oz","Latte 8oz","Toffee coffee","Cappuccino 8oz","S´mores Latte","Cafe tostado molido"};  
    double precioUnitario[8] = {60.00,32.00,22.00,24.00,28.00,24.00,32.00,60.00};
    double costoFijo_Unidad [8] = {20.00,19.20,13.20,17.20,20.10,17.20,23.00,20.00};
    //Cambios de los 2 meses
    int unidadesVendidas[16] = {300,400,1590,200,390,1455,800,60,250,380,800,250,600,1200,1540,15};
  
    //Inicializacion de listas para almacenar resultados en orden
    double ventas_Producto[16];
    double utilidades_Producto[16];

    //Creacion de hilos de JULIO
    while(j < 8){

        err = pthread_create(&(tid[j]),NULL,&montoVentas,NULL);
         if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));	//impresion de mensaje si el hilo no se crea correctamente
        j++;
    }

    //Creacion de hilos de AGOSTO
      while(j < 8){

        err = pthread_create(&(tid[j]),NULL,&utilidades,NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));	//impresion de mensaje si el hilo no se crea correctamente
        j++;
    }



    return 0;
}