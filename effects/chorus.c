// (Function) Pass an input audio signal through a chorus filter
#include "effects.h"

float chorus(float in, float tau, float dt, int fE, int fS){
    // INPUT ARGUMENTS:
    // in — Audio signal to clip (0-1, unitless)
    // tau — Average duration of delay effect (>0, s)
    // dt - Delay oscillation amplitude about the average (>0, s)
    // fE — Delay sweep frequency (>=0, Hz)
    // fS — Sampling frequency (>0, Hz)
    
    // Declare local variables
    static float out = 0; // Final delayed output signal (0-1, unitless)
    static float in1 = 0; // Intermediate delayed input sample 1 (0-1, unitless)
    static float in2 = 0; // Intermediate delayed input sample 2 (0-1, unitless)
    const float wt0 = 0.55; // Weight of current input smaple (0-1, unitless)
    const float wt1 = 0.40; // Weight of delayed input sample 1 (0-1, unitless)
    const float wt2 = 0.05; // Weight of delayed input sample 2 (0-1, unitless)
    const int len = (int)((tau+dt)*fS); // Number of samples in each input buffer (>0, unitless)
    const int depth = (int)(dt*fS); // Number of samples from crest to trough in delay oscillation (>0, unitless)
    static float buf1[4410] = {0}; // Memory buffer for first delayed instance
    static float buf2[4410] = {0}; // Memory buffer for second delayed instance
    static int kW = 0; // Write-index for delay memory buffers
    static int kR = 0; // Read-index for delay memory buffers
    static float lfo = 0; // Low-frequency oscillator multiplier (0-1, unitless)
    
    // Read circular delay buffers
    in1 = buf1[kR];
    in2 = buf2[kR];
    
    // Update circular delay buffers
    buf2[kW] = buf1[kW];
    buf1[kW] = in;
    
    // Sweep circular delay buffer read index
    lfo = ( 1+LFO("sine", fE, fS) )/2;
    if (depth*lfo>1) {
        kR = (kW + (int)(depth*lfo)) % len;
    } else {
        kR = (kW + 1) % len;
    };
    
    // Increment circular delay buffer write index
    kW = (kW+1) % (len);
    
    // Compose final delayed signal
    out = wt0*in+wt1*in1+wt2*in2;
    
    // Return final delayed signal
    return out;
}
