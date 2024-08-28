/* Pass a digital audio input signal through a serial chain of digital audio effects specified as an array of effect function pointers. */
#include "pa_effectchain.h"

// (Function) PortAudio callback function
static int paCallback(const void *in,
                         void *out,
                         unsigned long len,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData ){
    // INPUT ARGUMENTS:
    // in — Input audio buffer to process
    // out - Processed audio buffer to output
    // len - Number of frames in the audio buffer
    // timeInfo — PortAudio structure with timing information
    // statusFlags — PortAudio structure with status information
    // userData — PortAudio structure with user information
    
    // Suppress unused input argument warnings
    (void) timeInfo;
    (void) statusFlags;
    //(void) userData;
    
    // Declare local variables:
    unsigned int iC, iE; // Iteration index
    SAMPLE *outSample = (SAMPLE*)out; // Pointer to output buffer sample
    EffectParams *params = (EffectParams *)userData; // Pointer to user effect params
    const SAMPLE *inSample = (const SAMPLE*)in; // Pointer to input buffer sample
    SAMPLE inEl; // Sample of audio input buffer

    // Process input buffer:
    if( in == NULL )
    {
        // Write 0s to output buffer
        for( iC=0; iC<len; iC++ )
        {
            *outSample++ = 0;  // Left channel
        }
    }
    else
    {
        // Write filtered input to output buffer
        for(iC=0; iC<len; iC++)
        {
            // Read ith sample from input buffer:
            inEl = *inSample++;
            
            for (iE=0; iE<10; iE++) {
                if (params[iE].type == DELAY) {
                    inEl  = delay(inEl, params[iE].settings.delay.tau, (float)SAMPLE_RATE);
                } else if (params[iE].type == DISTORTION) {
                    inEl  = smoothClipper(inEl,params[iE].settings.distortion.ord);
                } else if (params[iE].type == CHORUS) {
                    inEl = chorus(inEl, params[iE].settings.chorus.tau, params[iE].settings.chorus.dt, params[iE].settings.chorus.fE, (float)SAMPLE_RATE);
                } else if (params[iE].type == TREMOLO) {
                    inEl = tremolo(inEl, params[iE].settings.tremolo.depth, params[iE].settings.tremolo.fE, (float)SAMPLE_RATE);
                } else if (params[iE].type == SUSTAIN) {
                    inEl = sustain(inEl, params[iE].settings.sustain.damping);
                } else if (params[iE].type == ESCAPE) {
                    continue;
                }
            }
            
            *outSample++ = inEl;
        }
    }

    return paContinue;
}

/*******************************************************************/

int main(void)
{
    #define maxN (10) // Max number of effects allowed
    EffectParams effectChain[maxN]; // Array of effect parameter structures
    int NEffect = 0; // Effect counter
    int MAX_EFFECTS = 10; // Max number of effects allowed in effect chain
    
    // Compile effect chain from user input
    while (1) {
        // Obtain effect name from user
        char effectName[32];
        printf("Enter effect name or enter 'run' to start: ");
        scanf("%s",effectName);
        
        // Escape iteration and warn user if max number of effects reached
        if (NEffect >= MAX_EFFECTS) {
            fprintf(stderr, "Error: Maximum number of effects reached. Enter 'run' to start audio processing.\n");
                    continue;
        }
        
        if (strcmp(effectName,"delay") == 0) {
            printf("Delay time (s): ");
            scanf("%f",&effectChain[NEffect].settings.delay.tau);
            effectChain[NEffect].type = DELAY;
        } else if (strcmp(effectName,"distortion") == 0) {
            printf("Distortion order (>1): ");
            scanf("%f",&effectChain[NEffect].settings.distortion.ord);
            effectChain[NEffect].type = DISTORTION;
        } else if (strcmp(effectName,"chorus") == 0) {
            printf("Average delay (s): ");
            scanf("%f",&effectChain[NEffect].settings.chorus.tau);
            printf("Delay oscilation amplitude (s): ");
            scanf("%f",&effectChain[NEffect].settings.chorus.dt);
            printf("Delay oscilation frequency (Hz): ");
            scanf("%f",&effectChain[NEffect].settings.chorus.fE);
            effectChain[NEffect].type = CHORUS;
        } else if (strcmp(effectName,"tremolo") == 0) {
            printf("Tremolo depth (0-1): ");
            scanf("%f",&effectChain[NEffect].settings.tremolo.depth);
            printf("Tremolo frequency (Hz): ");
            scanf("%f",&effectChain[NEffect].settings.tremolo.fE);
            effectChain[NEffect].type = TREMOLO;
        } else if (strcmp(effectName,"sustain") == 0) {
            printf("Sustain damping factor (0-1): ");
            scanf("%f",&effectChain[NEffect].settings.sustain.damping);
            effectChain[NEffect].type = SUSTAIN;
        } else if (strcmp(effectName,"run") == 0) {
            effectChain[NEffect].type = ESCAPE;
        } else {
            fprintf(stderr, "Error: Invalid effect name: %s\nPlease try again.\n", effectName);
        }
        
        // Escape loop and continue program if code word "run" specified
        if (strcmp(effectName,"run") == 0) {
            break;
        }
        
        // Increment effect counter
        NEffect++;
    }
    
    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream;
    PaError err;

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    inputParameters.device = 2; /* Pa_GetDefaultInputDevice();  default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        goto error;
    }
    inputParameters.channelCount = 1;       /* mono input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice(); // default output device
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = 1;       /* stereo output */
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
              paCallback,
              &effectChain );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    printf("Starting PortAudio stream.\n");
    if( err != paNoError ) goto error;

    // Clear input buffer and listen to user input to terminate program
    printf("Hit ENTER to stop program.\n");
    fflush(stdin);
    getchar();
    printf("Closing PortAudio stream.\n");
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    printf("Finished.\n");
    Pa_Terminate();
    return 0;

error:
    Pa_Terminate();
    fprintf( stderr, "An error occurred while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return -1;
}
