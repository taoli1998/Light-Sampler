#include "joystick.h"
#include "LEDmatrix.h"
#include "function.h"
#include "button.h"
#include "sampler.h"
#include "analysis.h"
#include "control.h"

int main(){
    start();
    check_button();
    terminate();
    return 0;
}