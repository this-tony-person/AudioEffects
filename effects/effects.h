// Header file for individual audio effect functions

// Load C libraries:
#include <math.h>

// Define global constants:
#define PI (3.1415)

// Declare effect functions
extern float lowPassFilter(float in, float tau, float Ts);
extern float highPassFilter(float in, float tau, float Ts);
extern float allPassFilter(float in, float tau, float Ts);
extern float tremolo(float in, float depth, float fE, float fS);
extern float sustain(float in, float damping);
extern float smoothClipper(float in, int ord);
extern float delay(float in, float tau, int fS);
extern float flanger(float in, float tau, int fE, int fS);
extern float chorus(float in, float tau, float dt, int fE, int fS);

// Declare effect helper functions
extern float LFO(char *fn, int fE, int fS);
extern int sign(float in);
