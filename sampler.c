#include "sampler.h"
#include "function.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <sched.h>
#include <math.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/syscall.h>

#define Voltage_read "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_MAX_READING 4095
#define A2D_VOLTAGE_REF_V 1.8
#define max_all 50000

static long long total = 0;
static int count = 0;
static int stop = 0;
static pthread_t thread_id;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static samplerDatapoint_t* Sampler_data_pointer = NULL;
static void *sampling(void *args);
static void def_pth();
static void count_up(double sample);

void Sampler_startSampling(void){
    pthread_mutex_lock(&mutex);
    {
        Sampler_data_pointer = malloc(sizeof(*Sampler_data_pointer)*max_all);
        memset(Sampler_data_pointer,0,max_all*sizeof(*Sampler_data_pointer));
    }
    pthread_mutex_unlock(&mutex);
    def_pth();
    
}

void Sampler_stopSampling(void){
    stop = 1;
    pthread_join(thread_id,NULL);
    pthread_mutex_lock(&mutex);
    {
        count = 0;
        free(Sampler_data_pointer);
        Sampler_data_pointer = NULL;
    }
    pthread_mutex_unlock(&mutex);
}

samplerDatapoint_t* Sampler_extractAllValues(int *length){
    samplerDatapoint_t* copy = NULL;
    pthread_mutex_lock(&mutex);
    {
        copy = malloc(sizeof(*Sampler_data_pointer)*count);
        for(int i = 0; i < count; i++){
            memcpy(&copy[i], &Sampler_data_pointer[i], sizeof(samplerDatapoint_t));
        }
        int size = count;
        count = 0;
        *length = size;      
    }
    pthread_mutex_unlock(&mutex);
    return copy;
}

int Sampler_getNumSamplesInHistory(){
    int size = 0;
    pthread_mutex_lock(&mutex);
    {
        size = count;
    }
    pthread_mutex_unlock(&mutex);
    return size;
}

long long Sampler_getNumSamplesTaken(void){
    return total;
}

static void def_pth(){
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&thread_id, &attr, &sampling, NULL);
}

static void count_up(double sample){
    pthread_mutex_lock(&mutex);
    {
        if(count>max_all){
            printf("Maxmium number reach!\n");
            exit(-1);
        }
        Sampler_data_pointer[count].timestampInNanoS = getTimeInNs();
        Sampler_data_pointer[count].sampleInV = sample;
        count++;
    }
    pthread_mutex_unlock(&mutex);
    
}

static void *sampling(void *args){
    while(!stop){
        double voltage = ((double)getVoltage0Reading(Voltage_read)/ A2D_MAX_READING) * A2D_VOLTAGE_REF_V;
        count_up(voltage);
        total++;
        sleepForMs(1);
    }
    return NULL;
}
