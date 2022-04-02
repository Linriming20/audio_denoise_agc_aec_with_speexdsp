
CC := gcc

CFLAGS := -I./include
#LDFLAGS := -lm -lspeexdsp -L./lib  # "-lm" should be in the back.
LDFLAGS := -lspeexdsp -L./lib -lm

ifeq ($(DEBUG), 1)
CFLAGS += -DENABLE_DEBUG
endif

all: speexdsp_aec speexdsp_agc speexdsp_denoise

speexdsp_aec: main_aec.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

speexdsp_agc: main_agc.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

speexdsp_denoise: main_denoise.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f speexdsp_aec speexdsp_agc speexdsp_denoise out*
.PHONY := clean
