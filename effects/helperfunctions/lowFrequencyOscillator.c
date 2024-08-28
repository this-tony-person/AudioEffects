// (Function) Simulate a Low-Frequency Oscillator (LFO)
#include "../../pa_effectchain.h"

float LFO(char *fn, int fE, int fS) {
    // INPUT ARGUMENTS:
    // fn — Waveform function to generate ("sine"|"triangle"|"square"|"sawtooth")
    // fE — Frequency of the tremolo effect (>=0, Hz)
    // fS — Audio sampling frequency (>0, Hz)
    
    // Declare local variables:
    static float out = 0; // Normalized waveform (0—1, unitless)
    static int kE = 0; // Iteration counter (>=0, unitless)
    
    // Compute the sign of the input argument x:
    if (strcmp(fn, "sine") == 0) {
        out = sin(2*PI*kE*fE/fS);
    } else if (strcmp(fn, "triangle") == 0) {
        out = 4*abs(kE%(fS/fE) - fS/(2*fE)) - 1;
    } else if (strcmp(fn, "square") == 0) {
        out = sign(sin(2*PI*fE*kE/fS));
    } else if (strcmp(fn, "sawtooth") == 0) {
        out = 1-2*(kE%(fS/fE))/(fS/fE);
    } else {
        printf("%s is not a valid waveform name.\n",fn);
    }
    
    // Increment counter
    kE++;
    
    // Return the computed sign:
    return out;
}
