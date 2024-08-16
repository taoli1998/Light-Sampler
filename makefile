all:
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread light_sampler.c joystick.c LEDmatrix.c button.c function.c sampler.c analysis.c control.c -o light_sampler
	cp light_sampler $(HOME)/cmpt433/public/myApps/
