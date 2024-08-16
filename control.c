#include "control.h"
#include "joystick.h"
#include "LEDmatrix.h"
#include "analysis.h"
#include "function.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define Joy_thres 0.5

static pthread_t threadId;
static int stop = 0;
static void* Controlling(void* args);

void Control_start(void){
    pthread_create(&threadId, NULL, Controlling, NULL);
}

void Control_stop(void){
    stop = 1;
    pthread_join(threadId, NULL);
}

static void* Controlling(void* args){
    while(stop==0){
        double minV, maxV;
        getMaxV(&maxV);
        getMinV(&minV);
        long long minNs, maxNs;
        getMinNs(&minNs);
        getMaxNs(&maxNs);
        double x,y;
        joystick_readXY(&x,&y);

        if(x > Joy_thres){
            dou_display(maxNs/1000000.0);
        }
        else if(x < -Joy_thres){
            dou_display(minNs/1000000.0);
        }
        else if(y > Joy_thres){
            dou_display(maxV);
        }
        else if(y < -Joy_thres){
            dou_display(minV);
        }
        else{
            int_display(getdip());
        }
        sleepForMs(100);
    }
    return NULL;
}

