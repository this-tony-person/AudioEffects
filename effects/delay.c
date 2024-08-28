// (Function) Pass an input audio signal through a delay filter
#include "effects.h"

float delay(float in, float tau, int fS){
    // INPUT ARGUMENTS:
    // in — Audio signal to clip (0-1, unitless)
    // tau — Duration of delay effect (>=0, unitless)
    // fS — Sampling frequency (>0)
    
    // Declare local variables
    static float out = 0; // Final delayed output signal (0-1, unitless)
    static float in1 = 0; // Intermediate delayed input sample 1 (0-1, unitless)
    static float in2 = 0; // Intermediate delayed input sample 2 (0-1, unitless)
    const float wt0 = 0.50; // Weight of current input smaple (0-1, unitless)
    const float wt1 = 0.45; // Weight of delayed input sample 1 (0-1, unitless)
    const float wt2 = 0.05; // Weight of delayed input sample 2 (0-1, unitless)
    const int len = round(tau*fS); // Number of samples in each input buffer (>0, unitless)
    static float buf1[44100] = {0}; // Memory buffer for first delayed instance
    static float buf2[44100] = {0}; // Memory buffer for second delayed instance
    static int kD = 0; // Read/write index for delay memory buffers
    
    // Read circular delay buffers
    in1 = buf1[kD];
    in2 = buf2[kD];
    
    // Update circular delay buffers
    buf2[kD] = buf1[kD];
    buf1[kD] = in;
    
    // Increment circular delay buffer index
    kD = (kD+1) % len;
    
    // Compose final delayed signal
    out = wt0*in + wt1*in1 + wt2*in2;
    
    // Return final delayed signal
    return out;
}
