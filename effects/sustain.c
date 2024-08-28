// (Function) Pass an input audio signal through a smooth distortion clipper
float sustain(float in, float damping){
    // INPUT ARGUMENTS:
    // in — Audio signal to clip (0-1, unitless)
    // damping — Damping coefficient for sustain decay (0-1,unitless)
    
    // Declare local variables:
    float out; // Filtered output signal (0-1, unitless)
    static float in0 = 0; // Previous input sample 1 (0-1, unitless)
    
    // Add previous input sample (with decay) to current input sample
    out = (in + damping*(in0))/2;
    
    // Store current output sample in buffer
    in0 = out;

    // Return the clipped signal:
    return out;
}
