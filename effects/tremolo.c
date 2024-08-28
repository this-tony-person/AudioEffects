// (Function) Pass an input audio signal through a tremolo filter
#include "effects.h"

float tremolo(float in, float depth, float fE, float fS){
    // INPUT ARGUMENTS:
    // in – Audio signal to filter (0-1, unitless)
    // depth — Magnitude of tremolo effect (0-1, unitless)
    // fE — Frequency of the tremolo effect (>=0, Hz)
    // fS — Audio sampling frequency (>0, Hz)
    
    // Declare local variables:
    float tr; // Tremolo multiplier (0-1, unitless)
    float out; // Filtered output signal (0-1, unitless)
    static int kE = 0; // Iteration counter (>=0, unitless)
    
    // Apply tremolo filter to input signal:
    tr = (1-depth) + depth*sin(2*PI*kE*fE/fS);
    out = in*tr;
    
    // Increment effect counter (MAKE INPUT ARGUMENT):
    kE++;
    
    // Return the tremolo signal:
    return out;
}
