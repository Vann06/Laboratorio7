/*---------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* Laboratorio 7
* Ejercicio 02
* Autores: Vianka Castro 23201
*          Ricardo Godinez 23247
*
* Fecha: 2023/10/01
*
* Simulación de producción de café tostado molido
* utilizando hilos, mutex y variables condicionales
*
*----------------------------------------
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond_silo;

int silo = 0;
int bodega = 0;
const int MAX_BODEGA = 400;

void *tostadora(void *arg) {
    char *id = (char *)arg;
    while (1) {
        sleep(1); 

        pthread_mutex_lock(&mutex);
        silo += 1;
        printf("Tostadora %s produjo: 1 lb de café tostado\n", id);

        pthread_cond_signal(&cond_silo);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *empacadora(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (silo < 5) {
            pthread_cond_wait(&cond_silo, &mutex); 
        }

        
        int i;
        for (i = 0; i < 5 && bodega < MAX_BODEGA; ++i) {
            sleep(1);
            silo -= 1;
            bodega += 1;
            printf("Empacadora produjo: 1 bolsa de 1 lb de café\n");
            printf("Lbs de café en silo: %d\n", silo);
            printf("Lbs de café en bodega: %d\n", bodega);
        }


        if (bodega >= MAX_BODEGA) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t hilo_tostadora1, hilo_tostadora2, hilo_empacadora;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_silo, NULL);

    pthread_create(&hilo_tostadora1, NULL, tostadora, (void *)"1");
    pthread_create(&hilo_tostadora2, NULL, tostadora, (void *)"2");
    pthread_create(&hilo_empacadora, NULL, empacadora, NULL);

    pthread_join(hilo_empacadora, NULL);

    pthread_cancel(hilo_tostadora1);
    pthread_cancel(hilo_tostadora2);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_silo);

    printf("Lbs de café en silo: %d\n", silo);
    printf("Lbs de café en bodega: %d\n", bodega);

    return 0;
}
