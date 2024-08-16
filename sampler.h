#ifndef Sampler__Sampler
#define Sampler__Sampler

typedef struct {
 double sampleInV;
 long long timestampInNanoS;
} samplerDatapoint_t;
void Sampler_startSampling(void);
void Sampler_stopSampling(void);
samplerDatapoint_t* Sampler_extractAllValues(int *length);
int Sampler_getNumSamplesInHistory();
long long Sampler_getNumSamplesTaken(void);

#endif