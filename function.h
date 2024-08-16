#ifndef Sampler_function
#define Sampler_function

void start(void);
void terminate(void);
long long getTimeInMs(void);
void sleepForMs(long long delayInMs);
void runCommand(char* command);
int readFromFileToScreen(char *fileName);
long long getTimeInNs(void);
int getVoltage0Reading(char* file);

int initI2cBus(char* bus, int address);
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);
void print_all(long long AveI, long long minI, long long maxI, double AveV, double minV, double maxV, int dip, long long cur, long long last);

#endif