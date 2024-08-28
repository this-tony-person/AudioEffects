// Header file for main effect chain program

// Load C libraries:
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "portaudio.h"
#include "effects/effects.h"

// Define global constants:
#define SAMPLE_RATE (44100)
#define PA_SAMPLE_TYPE (paFloat32)
#define FRAMES_PER_BUFFER (64)

// Create descriptive data type aliases:
typedef float SAMPLE;

// Create enumerated data type for effects
typedef enum {
    PASSTHROUGH, // Default unfiltered node
    DELAY,
    DISTORTION,
    HIGHPASSFILTER,
    LOWPASSFILTER,
    CHORUS,
    FLANGER,
    WAH,
    TREMOLO,
    SUSTAIN,
    ESCAPE // Effect chain termination node
} EffectType;

// Create effect settings structures
typedef struct {
    float tau; // Delay time constant (>0, s)
} DelaySettings;

typedef struct {
    float fE; // Frequency of the tremolo effect (>=0, Hz)
    float depth; // Magnitude of tremolo effect (0-1, unitless)
} TremoloSettings;

typedef struct {
    float ord; // Order of the distortion clipping function (>=0, unitless)
} DistortionSettings;

typedef struct {
    float tau; // Duration of delay effect (>=0, unitless)
    float fE; // Delay sweep frequency (>=0, Hz)
} FlangerSettings;

typedef struct {
    float tau; // Average duration of delay effect (>0, s)
    float fE; // Delay sweep frequency (>=0, Hz)
    float dt; // Delay oscillation amplitude about the average (>0, s)
} ChorusSettings;

typedef struct {
    float damping; // Damping coefficient for sustain decay (0-1,unitless)
} SustainSettings;

typedef struct {
    DelaySettings delay; // Delay settings
    TremoloSettings tremolo; // Tremolo settings
    DistortionSettings distortion; // Distortion settings
    FlangerSettings flanger; // Flanger settings
    ChorusSettings chorus; // Chorus settings
    SustainSettings sustain; // Sustain settings
} EffectSettings;

// Create structure data type for effect parameters
typedef struct {
    EffectType type; // Enumeration member corresponding to effect name
    EffectSettings settings; // Effect settings
} EffectParams;
