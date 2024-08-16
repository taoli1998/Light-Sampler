#include "button.h"
#include "function.h"
#include <stdio.h>
#include <stdlib.h>

#define button "/sys/class/gpio/gpio72/value"

void button_start(void){
    runCommand("config-pin p8.43 gpio");
    runCommand("config-pin -q p8.43");
    runCommand("echo 72 > export");
}

static int button_press(){
    return readFromFileToScreen(button);
}

// void check_button()
// {
//     while(is_shutdown()==0){
//         sleepForMs(100);
//         if(button_press()==0){
//             shutdown();
//         }
//     }
// }
void check_button(void)
{
    int shutdown = 0;
    while(shutdown==0){
        sleepForMs(100);
        if(button_press()==0){
            shutdown=1;
        }
    }
}
