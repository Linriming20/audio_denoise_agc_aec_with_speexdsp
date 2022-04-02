#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "speex/speex_preprocess.h"

#define PERIOD_PER_FRAME 	(20) 	/* 20ms */


int main(int argc, char *argv[])
{
	FILE *fpInPcm = NULL;
	FILE *fpOutPcm = NULL;
	char *inSrcFileName = NULL;
	char *outAgcFileName = NULL;
	short *pcmBuf = NULL;
	int readSamples = -1;
	int sampleRate = -1;
	int framesize = -1; /* samples per frame !! */
	int agcLevel = -1;
	SpeexPreprocessState *preprocess_state = NULL;

	if (argc != 5)
	{
		printf("Usage: (The PCM file must be mono.)\n"
			   "    %s <in src PCM file> <samplerate> <agc level> <out agc PCM file>\n"
			   "Examples: \n"
			   "    %s audio/test_8000_16bit_mono.pcm 8000 100000 out_8000_16bit_mono_agc.pcm\n",
			   argv[0], argv[0]);

		return -1;
	}
	else
	{
		inSrcFileName = argv[1];
		sampleRate = atoi(argv[2]);
		agcLevel = atoi(argv[3]);
		outAgcFileName = argv[4];

		framesize = sampleRate/(1000/PERIOD_PER_FRAME);
		pcmBuf = (short *)malloc(framesize * sizeof(short));
	}

	/* Open input file and output file. */
	fpInPcm = fopen(inSrcFileName, "rb");
	fpOutPcm = fopen(outAgcFileName, "wb");
	if(!fpInPcm || !fpOutPcm)
	{
		fprintf(stderr, "Open file failed!\n");
		goto exit;
	}

	/* speex agc 1/4: init */
	preprocess_state = speex_preprocess_state_init(framesize, sampleRate);

	/* speex agc 2/4: configure */
	int iSetVal;
	iSetVal = 0;
	speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_AGC, &iSetVal);
	iSetVal = agcLevel;
	speex_preprocess_ctl(preprocess_state, SPEEX_PREPROCESS_SET_AGC_LEVEL, &iSetVal);

	while(1)
	{
		readSamples = fread(pcmBuf, sizeof(short), framesize, fpInPcm);
		if(readSamples <= 0)
			break;

		/* speex agc 3/4: process */
		speex_preprocess_run(preprocess_state, (spx_int16_t *)pcmBuf);

		fwrite(pcmBuf, sizeof(short), framesize, fpOutPcm);
	}

exit:

	/* speex agc 4/4: exit */
	if(preprocess_state) speex_preprocess_state_destroy(preprocess_state); 

	if(fpInPcm) fclose(fpInPcm);
	if(fpOutPcm) {fflush(fpOutPcm); fclose(fpOutPcm);};
	if(pcmBuf) free(pcmBuf);

	return 0;
}

