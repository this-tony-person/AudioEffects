// (Function) Pass an input audio signal through a smooth distortion clipper
#include "effects.h"

float smoothClipper(float in, int ord){
    // INPUT ARGUMENTS:
    // in — Audio signal to clip (0-1, unitless)
    // ord — Order of the distortion clipping function (>=0, unitless)
    
    // Declare local variables:
    float out; // Filtered output signal (0-1, unitless)
    
    // Apply clipping filter to input signal:
    if(in<0){
        out = sign(in+1)*pow(fabs(in+1),ord) - 1;
    }
    else
    {
        out = sign(in-1)*pow(fabs(in-1),ord) + 1;
    }

    // Return the clipped signal:
    return out;
}
