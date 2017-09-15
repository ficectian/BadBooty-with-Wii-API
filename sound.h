// Ver1.0

#ifndef _SOUND_H_
#define _SOUND_H_

#include <demo.h>

/*	for sound */
#define NUM_SAMPLES     7//15
// User Structures to keep track of data
typedef struct
{
    void        *mramAddr;
    
} SampleInfo;

typedef struct
{
    AXVPB           *voice;
    u32             state;
} VoiceInfo;

extern char SampleFiles [NUM_SAMPLES][256];
extern MEMHeapHandle hExpHeap;
extern SampleInfo Samples[NUM_SAMPLES];
extern VoiceInfo Voices[AX_MAX_VOICES];

// Voice  Defines
#define VOICE_PRIO_HIGH 31
#define VOICE_PRIO_MED  15
#define VOICE_PRIO_LOW  1

#define VOICE_STATE_STOPPED        0
#define VOICE_STATE_START          1
#define VOICE_STATE_STARTED        2
#define VOICE_STATE_PLAYING        3
#define VOICE_STATE_STOP           4

// Utility Macro Functions
#define RoundUp64(x) (((u32)(x) + 64 - 1) & ~(64 - 1))
#define Bytes2Nibbles(n) (n << 1)
#define Nibbles2Bytes(n) (n >> 1)
#define GetDSPADPCMDataAddress(a) ((void*)((u32)a + sizeof(DSPADPCM)))
#define GetDSPADPCMDataSize32B(a) (RoundUp64(((DSPADPCM*)a)->num_adpcm_nibbles) >> 1)
#define GetVoiceCurrentAddr32(v) (*(u32 *)(&((v)->pb.addr.currentAddressHi))) 
#define GetVoiceLoopAddr32(v) (*(u32 *)(&((v)->pb.addr.loopAddressHi))) 
#define GetVoiceEndAddr32(v) (*(u32 *)(&((v)->pb.addr.endAddressHi)))

// function Prototypes
void * LoadFileIntoRam(char *path);
void AudioFrameCallback(void);
AXVPB* AquireVoiceADPCM(void *pDSPADPCMData);
void LoadSamples(void);
u32 PlaySample(SampleInfo *sample);
void StopSample(u32 sampleIndex);
bool CheckStopeedSample(u32 sampleIndex);	// BGMëΩèdçƒê∂ñhé~ä÷êî
void VoiceCallback(void * voiceIn);
void PrintIntro(void);
/* for sound */

#endif // M_SOUND_H_