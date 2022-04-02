#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "speex/speex_preprocess.h"
#include "speex/speex_echo.h"

#define PERIOD_PER_FRAME 	(20) 	/* 20ms */
#define TAIL 				(1024)


int main(int argc, char *argv[])
{
	FILE *fpInMicPcm = NULL;
	FILE *fpInSpkPcm = NULL;
	FILE *fpOutEchoPcm = NULL;
	char *inMicFileName = NULL;
	char *inSpkFileName = NULL;
	char *outEchoFileName = NULL;
	short *inMicPcmBuf = NULL; 	 /* mic in */
	short *inSpkPcmBuf = NULL; 	 /* spk out */
	short *outEchoPcmBuf = NULL; /* process output echo cannel */
	int readSamples = -1;
	int sampleRate = -1;
	int framesize = -1; /* samples per frame !! */
	SpeexPreprocessState *preprocess_state = NULL;
	SpeexEchoState *echo_state = NULL;

	if (argc != 5)
	{
		printf("Usage: (The PCM file must be mono.)\n"
			   "    %s <in mic PCM file> <in spk PCM file> <samplerate> <out echo PCM file>\n"
			   "Examples: \n"
			   "    %s audio/out_8000_16bit_mono_agc.pcm audio/out_8000_16bit_mono_denoise.pcm 8000 out_8000_16bit_mono_aec.pcm\n",
			   argv[0], argv[0]);

		return -1;
	}
	else
	{
		inMicFileName = argv[1];
		inSpkFileName = argv[2];
		sampleRate = atoi(argv[3]);
		outEchoFileName = argv[4];

		framesize = sampleRate/(1000/PERIOD_PER_FRAME);
		inMicPcmBuf = (short *)malloc(framesize * sizeof(short));
		inSpkPcmBuf = (short *)malloc(framesize * sizeof(short));
		outEchoPcmBuf = (short *)malloc(framesize * sizeof(short));
	}

	/* Open input file and output file. */
	fpInMicPcm = fopen(inMicFileName, "rb");
	fpInSpkPcm = fopen(inSpkFileName, "rb");
	fpOutEchoPcm = fopen(outEchoFileName, "wb");
	if(!fpInMicPcm || !fpInSpkPcm || !fpOutEchoPcm)
	{
		fprintf(stderr, "Open file failed!\n");
		goto exit;
	}

	/* speex aec 1/4: init */
	echo_state = speex_echo_state_init(framesize, TAIL);
	preprocess_state = speex_preprocess_state_init(framesize, sampleRate);

	/* speex aec 2/4: configure */
	speex_echo_ctl(echo_state, SPEEX_ECHO_SET_SAMPLING_RATE, &sampleRate);
	speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_ECHO_STATE, echo_state);

	while(1)
	{
		readSamples = fread(inMicPcmBuf, sizeof(short), framesize, fpInMicPcm);
		if(readSamples <= 0)
			break;
		readSamples = fread(inSpkPcmBuf, sizeof(short), framesize, fpInSpkPcm);
		if(readSamples <= 0)
			break;

		/* speex aec 3/4: process */
		speex_echo_cancellation(echo_state, inMicPcmBuf, inSpkPcmBuf, outEchoPcmBuf);
		speex_preprocess_run(preprocess_state, outEchoPcmBuf);

		fwrite(outEchoPcmBuf, sizeof(short), framesize, fpOutEchoPcm);
	}

exit:

	/* speex aec 4/4: exit */
	if(preprocess_state) speex_preprocess_state_destroy(preprocess_state); 

	if(fpInMicPcm) fclose(fpInMicPcm);
	if(fpInSpkPcm) fclose(fpInSpkPcm);
	if(fpOutEchoPcm) {fflush(fpOutEchoPcm); fclose(fpOutEchoPcm);};
	if(inMicPcmBuf) free(inMicPcmBuf);
	if(inSpkPcmBuf) free(inSpkPcmBuf);
	if(outEchoPcmBuf) free(outEchoPcmBuf);

	return 0;
}

