#ifndef Sampler_Analysis
#define Sampler_Analysis

void Analysis_start(void);
void Analysis_stop(void);
double getAveV(void);
int getdip(void);
void getMinNs(long long *min_ns);
void getMaxNs(long long* max_ns);
void getMinV(double* Vmin);
void getMaxV(double* Vmax);

#endif