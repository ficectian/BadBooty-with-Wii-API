// Ver1.0

#include "sound.h"

// Exp Heap
MEMHeapHandle hExpHeap;
// Sample Info
SampleInfo       Samples[NUM_SAMPLES];
// AX Voice info
VoiceInfo Voices[AX_MAX_VOICES];

// Filenames for samples to be played
char SampleFiles [NUM_SAMPLES][256] = 
{
    "kick.dsp"
   /*
    "snare.dsp",
    "tom.dsp",
    "cymbal.dsp",
    "ride.dsp",
    "cowbell.dsp",
    "kick.dsp",
    "bongo1.dsp",
    "bongo2.dsp",
    "bongo3.dsp",
    "bongo4.dsp",
    "mixup.dsp",
    "morning-cafe.dsp",
    "explosion.dsp",
    "sample000L.dsp",
    "sample001L.dsp",*/
    
};

// This demo uses a very simple mixing paradigm. All sounds are played at
// a volume of 1.0 (0x8000). Please see the MIX library for a more
// comprehensive mixing library
AXPBMIX g_mix = {
    0x8000,       // volume left 
    0x0000,       // volume ramp left
    0x8000,       // volume right
    0x0000,       // volume ramp right
    
    0x0000,       // volume AUX A left
    0x0000,       // volume ramp AUX A left
    0x0000,       // volume AUX A right
    0x0000,       // volume ramp AUX A right
    
    0x0000,       // volume AUX B left
    0x0000,       // volume ramp AUX B left
    0x0000,       // volume AUX B right
    0x0000,       // volume ramp AUX B right
    
    0x0000,       // volume AUX C left
    0x0000,       // volume ramp AUX C left
    0x0000,       // volume AUX C right
    0x0000,       // volume ramp AUX C right
    
    0x0000,       // volume surround
    0x0000,       // volume ramp surround
    0x0000,       // volume AUX A surround
    0x0000,       // volume ramp AUX A surround
    0x0000,       // volume AUX B surround
    0x0000,       // volume ramp AUX B surround
    0x0000,       // volume AUX C surround
    0x0000,       // volume ramp AUX C surround
};

AXPBVE g_ve = {
    0x8000,     // volume at start of frame, 0x8000 = 1.0
    0           // signed per sample delta (160 samples per frame)
};

/*---------------------------------------------------------------------------*
    Name:           LoadSamples

    Description:    Loads ADPCM files into Main Memory (Header + Data) and
                    ARAM (Data)

    Arguments:      none

    Returns:        none
 *---------------------------------------------------------------------------*/
void LoadSamples(void)
{
    u32 i;            

    // Load samples
    for (i = 0; i < NUM_SAMPLES; i++)
    {
        // Load ADPCM file into MRAM (96 byte header included)
        Samples[i].mramAddr = LoadFileIntoRam(SampleFiles[i]);

        // Sanity Check
        if (Samples[i].mramAddr == NULL)
        {
            OSReport("WARNING! Sample %d not loaded\n", i);
            continue;
        }
    }
}

/*---------------------------------------------------------------------------*
    Name:           PlaySample

    Description:    dspファイルを再生する

    Arguments:      Samples[再生する要素]を渡す

    Returns:        再生したSampleのインデックス（停止などで利用する）
 *---------------------------------------------------------------------------*/
u32 PlaySample(SampleInfo *sample)
{
    AXVPB    *voice;

    if (sample->mramAddr == NULL)
    {
        OSReport("WARNING! Sample not loaded!\n");
        return -1;
    }

    // Aquire Voice and start
    voice = AquireVoiceADPCM(sample->mramAddr);
    if (voice == NULL)
    {
        OSReport("WARNING: Ran out of voices!\n");
        return -1;
    }
    Voices[voice->index].voice = voice;
    Voices[voice->index].state = VOICE_STATE_START;
    
    return voice->index;
}

/*---------------------------------------------------------------------------*
    Name:           CheckStopeedSample

    Description:    再生中か判断する

    Arguments:      PlaySampleの戻り値を渡す

    Returns:        再生中は0を返す
 *---------------------------------------------------------------------------*/
bool CheckStopeedSample(u32 sampleIndex)
{
	if(Voices[sampleIndex].state == VOICE_STATE_STOPPED)
		return 1;
	else
		return 0;
}

/*---------------------------------------------------------------------------*
    Name:           StopSample

    Description:    BGMを停止する。
    　　　　　　　　実行前にCheckStopeedSampleで再生中か確認すること。

    Arguments:      PlaySampleの戻り値を渡す

    Returns:        なし
 *---------------------------------------------------------------------------*/
void StopSample(u32 sampleIndex)
{
	if(Voices[sampleIndex].state == VOICE_STATE_PLAYING)
	    Voices[sampleIndex].state = VOICE_STATE_STOP;
}

/*---------------------------------------------------------------------------*
    Name:            AudioFrameCallback

    Description:    Callback that process audio data per 5ms audio frame.
                    In this case, it stops, resets, and starts a voice.

    Arguments:      none

    Returns:        none
 *---------------------------------------------------------------------------*/
void AudioFrameCallback(void)
{
    u32 i;
    BOOL bStopFlag = FALSE;

    // Monitor each voice and process states. This must be done in the
    // callback
    for (i = 0; i < AX_MAX_VOICES; i++)
    {

        // Skip NULL entries
        if (Voices[i].voice == NULL) continue;

        switch (Voices[i].state)
        {
            case VOICE_STATE_STOPPED:
                break;
            case VOICE_STATE_START:
                // Start the voice
                AXSetVoiceState(Voices[i].voice, AX_PB_STATE_RUN);
                Voices[i].state = VOICE_STATE_STARTED;
                break;
            case VOICE_STATE_STARTED:
                // Skip a frame
                Voices[i].state = VOICE_STATE_PLAYING;
                break;
            case VOICE_STATE_PLAYING:
                // Check to see if the voice is finished, if so, stop it
                if (Voices[i].voice->pb.state == AX_PB_STATE_STOP)
                    bStopFlag = TRUE;
                break;
            case VOICE_STATE_STOP:
                // Force a voice to stop
                bStopFlag = TRUE;
                break;
        }

        // A voice must be stopped
        if (bStopFlag)
        {
            AXSetVoiceState(Voices[i].voice, AX_PB_STATE_STOP);
            AXFreeVoice(Voices[i].voice);
            Voices[i].voice = NULL;
            Voices[i].state = VOICE_STATE_STOPPED;
            bStopFlag = FALSE;
        }
    }
}

/*---------------------------------------------------------------------------*
    Name:           VoiceCallback

    Description:    Callback for when a voice is dropped.

    Arguments:      unused

    Returns:        none
 *---------------------------------------------------------------------------*/
void VoiceCallback(void * voiceIn)
{
    AXVPB *voice = (AXVPB*)voiceIn;

    // Note: Voice is auto-magically stopped by AX layer when dropped

    // Application clean-up
    Voices[voice->index].voice = NULL;
    Voices[voice->index].state = VOICE_STATE_STOPPED;
}

/*---------------------------------------------------------------------------*
    Name:           AquireVoiceADPCM

    Description:    Parses the ADPCM header, sets voice parameters

    Arguments:      pDSPADPCMData    Pointer to the ADPCM data in MRAM
                    pARAMStart        ARAM Start address

    Returns:        pointer to the allocated voice or NULL
 *---------------------------------------------------------------------------*/
AXVPB* AquireVoiceADPCM(void *pDSPADPCMData)
{
    DSPADPCM            *ps = (DSPADPCM*)pDSPADPCMData;
    AXPBADDR            addr;
    AXPBADPCM           adpcm;
    AXPBSRC             src;
    AXPBADPCMLOOP       adpcmLoop;
    AXVPB*              voice;
    u32                 srcBits;
    u32                 mramAddress;
    u32                 pMRAMStart;

    // Allocate a voice for use
    voice = AXAcquireVoice(VOICE_PRIO_MED, VoiceCallback, 0);

    if (voice == NULL)
    {
        OSReport("WARNING! Voice Acquisition failed!\n");
        return NULL;
    }

    // Fill AXPBADDR structure
    // All the folowing addresses are in nibbles

    addr.loopFlag           = ps->loop_flag;
    addr.format             = ps->format;

    pMRAMStart = OSCachedToPhysical(GetDSPADPCMDataAddress(pDSPADPCMData));

    // Support for looping
    if (addr.loopFlag)
    {
        adpcmLoop.loop_pred_scale = ps->lps;
        adpcmLoop.loop_yn1        = ps->lyn1;
        adpcmLoop.loop_yn2        = ps->lyn2;
    }

    mramAddress             = (ps->sa + Bytes2Nibbles(pMRAMStart));
    addr.loopAddressHi      = (u16)(mramAddress >> 16);              
    addr.loopAddressLo      = (u16)(mramAddress & 0xFFFF);       

    mramAddress             = (ps->ea + Bytes2Nibbles(pMRAMStart));
    addr.endAddressHi       = (u16)(mramAddress >> 16);       
    addr.endAddressLo       = (u16)(mramAddress & 0xFFFF);       

    mramAddress             = (ps->ca + Bytes2Nibbles(pMRAMStart));
    addr.currentAddressHi   = (u16)(mramAddress >> 16);       
    addr.currentAddressLo   = (u16)(mramAddress & 0xFFFF);
    
    // Fill AXPBADPCM structure
    adpcm.a[0][0]           = ps->coef[0];
    adpcm.a[0][1]           = ps->coef[1];
    adpcm.a[1][0]           = ps->coef[2];
    adpcm.a[1][1]           = ps->coef[3];
    adpcm.a[2][0]           = ps->coef[4];
    adpcm.a[2][1]           = ps->coef[5];
    adpcm.a[3][0]           = ps->coef[6];
    adpcm.a[3][1]           = ps->coef[7];
    adpcm.a[4][0]           = ps->coef[8];
    adpcm.a[4][1]           = ps->coef[9];
    adpcm.a[5][0]           = ps->coef[10];
    adpcm.a[5][1]           = ps->coef[11];
    adpcm.a[6][0]           = ps->coef[12];
    adpcm.a[6][1]           = ps->coef[13];
    adpcm.a[7][0]           = ps->coef[14];
    adpcm.a[7][1]           = ps->coef[15];
    adpcm.gain              = ps->gain; 
    adpcm.pred_scale        = ps->ps;
    adpcm.yn1               = ps->yn1;
    adpcm.yn2               = ps->yn2;

    // Fill AXPBSRC structure for proper sample rates
    srcBits = (u32)(0x00010000 * ((f32)ps->sample_rate / AX_IN_SAMPLES_PER_SEC));
    src.ratioHi = (u16)(srcBits >> 16);
    src.ratioLo = (u16)(srcBits & 0xFFFF);
    src.currentAddressFrac = 0;
    src.last_samples[0] = 0;
    src.last_samples[1] = 0;
    src.last_samples[2] = 0;
    src.last_samples[3] = 0;

    // Set voice type
    AXSetVoiceType(voice, AX_PB_TYPE_NORMAL);

    // Set Address and ADPCM information from header
    AXSetVoiceAddr(voice, &addr);
    AXSetVoiceAdpcm(voice, &adpcm);
    AXSetVoiceAdpcmLoop(voice, &adpcmLoop);

    // Set simple volumes
    AXSetVoiceMix(voice, &g_mix);
    AXSetVoiceVe(voice, &g_ve);

    // Set sample rate
    AXSetVoiceSrcType(voice, AX_SRC_TYPE_LINEAR);
    AXSetVoiceSrc(voice, &src);

    return voice;
}

/*---------------------------------------------------------------------------*
    Name:            LoadFileIntoRam

    Description:    Loads a file into memory. Memory is allocated.

    Arguments:      path    File to load into main memory

    Returns:        pointer to file in main memory or NULL if not opened
 *---------------------------------------------------------------------------*/
void * LoadFileIntoRam(char *path)
{
    DVDFileInfo handle;
    u32         round_length;
    s32         read_length;
    void        *buffer;

    // Open File
    if (!DVDOpen(path, &handle))
    {
        OSReport("WARNING! Failed to open %s\n", path);
        return NULL;
    }

    // Make sure file length is not 0
    if (DVDGetLength(&handle) == 0)
    {
        OSReport("WARNING! File length is 0\n");
        return NULL;
    }

    round_length = OSRoundUp32B(DVDGetLength(&handle));
    buffer       = MEMAllocFromExpHeapEx(hExpHeap, round_length,  32);

    // Make sure we got a buffer
    if (buffer == NULL)
    {
        OSReport("WARNING! Unable to allocate buffer\n");
        return NULL;
    }

    // Read Files
    read_length  = DVDRead(&handle, buffer, (s32)(round_length), 0);

    // Make sure we read the file correctly
    if (read_length <= 0)
    {
        OSReport("WARNING! File lenght is wrong\n");
        return NULL;
    }

    return buffer;
}

