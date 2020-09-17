/*
 ============================================================================
 Name        : final_merge.c
 Author      : Srikanth Pendem
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "fps_test.h"
#include "keys.h"

void* threadFunction(void *a)
    {
    biometrics();
    return 0;
    }

void* threadFunction2(void *b)
    {
    KEYACCESS();
    return 0;
    }

int main(void)
    {

    pthread_t id;
    pthread_t id2;
    printf("Place your finger or give the key\n");
    pthread_create(&id, NULL, &threadFunction, NULL);
    pthread_create(&id2, NULL, &threadFunction2, NULL);
    pthread_join(id, NULL);
    pthread_join(id2, NULL);

    return 0;
    }

