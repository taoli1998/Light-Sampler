#include "analysis.h"
#include "sampler.h"
#include "function.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#define V_diff 0.1
#define hysteresis 0.03
#define weight 0.999

static double AveV = 0;
static double MinV = 0;
static double MaxV = 0;
static long long Ave_Interval = 0;
static long long Min_Interval = 0;
static long long Max_Interval = 0;
static int Num_Dip = 0;
static long long Latest_Ns = 0;
static int stop = 0;

static pthread_t thread_id;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void* analysising(void* args);
static void anaysis_data(samplerDatapoint_t data[], int size);
static void setData(samplerDatapoint_t data[], int size);
static int getDip(samplerDatapoint_t data[], int size);

void Analysis_start(void){
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&thread_id, &attr, &analysising, NULL);
}

void Analysis_stop(void){
    stop = 1;
    pthread_join(thread_id, NULL);
}

void getMinNs(long long *min_ns){
    pthread_mutex_lock(&mutex);
    {
        *min_ns = Min_Interval;
    }
    pthread_mutex_unlock(&mutex);
}
void getMaxNs(long long* max_ns){
    pthread_mutex_lock(&mutex);
    {
        *max_ns = Max_Interval;
    }
    pthread_mutex_unlock(&mutex);
}


void getMinV(double* Vmin){
    pthread_mutex_lock(&mutex);
    {
        *Vmin = MinV;
    }
    pthread_mutex_unlock(&mutex);
}
void getMaxV(double* Vmax){
    pthread_mutex_lock(&mutex);
    {
        *Vmax = MaxV;
    }
    pthread_mutex_unlock(&mutex);
}

double getAveV(void){
    double V = 0;
    pthread_mutex_lock(&mutex);
    {
        V = AveV;
    }
    pthread_mutex_unlock(&mutex);
    return V;
}

int getdip(void){
    int dip = 0;
    pthread_mutex_lock(&mutex);
    {
        dip = Num_Dip;
    }
    pthread_mutex_unlock(&mutex);
    return dip;
}

static void* analysising(void* args)
{
    long long lastsample = 0;
    while(stop==0){
        sleepForMs(1000);
        int numdata = 0;
        samplerDatapoint_t* pdatapoint = Sampler_extractAllValues(&numdata);
        pthread_mutex_lock(&mutex);
        {
            anaysis_data(pdatapoint, numdata);
            long long curSample = Sampler_getNumSamplesTaken();
            print_all(Ave_Interval,Min_Interval,Max_Interval,AveV,MinV,MaxV,Num_Dip,curSample,lastsample);
            lastsample = curSample;
        }
        pthread_mutex_unlock(&mutex);
        free(pdatapoint);
    }
    
    return NULL;
}

static void anaysis_data(samplerDatapoint_t data[], int size){
    Num_Dip = getDip(data, size);
    setData(data, size);
}

static void setData(samplerDatapoint_t data[], int size){
    if(Latest_Ns == 0){
        Latest_Ns = data[0].timestampInNanoS;
    }
    long long total_Ns = 0;
    for (unsigned i =0; i<size; i++){
        long long Current_Ns = data[i].timestampInNanoS;        
        long long Diff_Ns = Current_Ns - Latest_Ns;
        double curV = data[i].sampleInV;
        if(i == 0){
            Min_Interval = Diff_Ns;
            Max_Interval = Diff_Ns;
            MinV = curV;
            MaxV = curV;
        }
        if(Diff_Ns < Min_Interval){
            Min_Interval = Diff_Ns;
        }
        if(Diff_Ns > Max_Interval){
            Max_Interval = Diff_Ns;
        }
        if(curV < MinV){
            MinV = curV;
        }
        if(curV > MaxV){
            MaxV = curV;
        }
        total_Ns += Diff_Ns;
        Latest_Ns = Current_Ns;
    }
    if (size<=0){
        Ave_Interval = 0;
    }
    else{
        Ave_Interval = total_Ns/size;
    }
}

static int getDip(samplerDatapoint_t data[], int size){
    int check_thres = 0;
    int count_dip = 0;
    for (unsigned i = 0; i< size; i++){
        samplerDatapoint_t* point = &data[i];
        if(AveV >= 0){
            AveV = weight*AveV + (1-weight)*point->sampleInV;            
        }
        else{
            AveV = point->sampleInV;
        }
    }
    for (int i = 0; i< size; i++){
        double curSam = data[i].sampleInV;
        if(check_thres==1){
            if(fabs(curSam - AveV)<V_diff - hysteresis){
                check_thres = 0;
            }
        }
        else {
            if(fabs(curSam - AveV)>V_diff){
                check_thres = 1;
                count_dip++;
            }
        }
    }
    return count_dip;
}

