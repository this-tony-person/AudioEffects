// (Function) Compute the sign of an input argument x
int sign(float in) {
    // INPUT ARGUMENTS:
    // in — Variable to compute the sign of
    
    // Declare local variables:
    int out; // Sign of the input argument in (-1|0|+1, unitless)
    
    // Compute the sign of the input argument x:
    out = (in > 0) - (in < 0);
    
    // Return the computed sign:
    return out;
}
