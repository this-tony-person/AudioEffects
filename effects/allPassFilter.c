// (Function) Pass an input audio signal through a first-order all-pass filter
float allPassFilter(float in, float tau, float Ts) {
    // INPUT ARGUMENTS:
    // in — Audio signal to filter (0-1, unitless)
    // tau — Time constant of low-pass filter (>=0, s)
    // Ts — Duration of a single audio sample (>0, s)
    
    // Declare local variables:
    static float in0 = 0; // Input signal at previous time step (0-1, unitless)
    static float out0 = 0; // Filtered output signal at previous time step (0-1, unitless)
    static float out = 0; // Filtered output signal at current time step (0-1, unitless)
    
    // Apply a first-order low-pass filter to the input signal:
    out = in0 + (2*tau-Ts)/(2*tau+Ts)*(out0-in);
    
    // Update input and output memory:
    in0 = in;
    out0 = out;
    
    // Return filtered signal:
    return out;
}
