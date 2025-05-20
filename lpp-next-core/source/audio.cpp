#include <3ds.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "audio.hpp"


bool audioChannels[32];

// Custom playsound function: Prevent desynchronization (luaVideo) and allows
// stereo sounds max volume setting
void My_CSND_playsound(u32 chn, u32 flags, u32 sampleRate, u32 *data0,
                       u32 *data1, u32 size, float vol, float pan) {
  u32 paddr0 = 0, paddr1 = 0;

  int encoding = (flags >> 12) & 3;
  int loopMode = (flags >> 10) & 3;

  if (encoding != CSND_ENCODING_PSG) {
    if (data0) paddr0 = osConvertVirtToPhys((void *)data0);
    if (data1) paddr1 = osConvertVirtToPhys((void *)data1);

    if (encoding == CSND_ENCODING_ADPCM) {
      int adpcmSample = ((s16 *)data0)[-2];
      int adpcmIndex = ((u8 *)data0)[-2];
      CSND_SetAdpcmState(chn, 0, adpcmSample, adpcmIndex);
    }
  }

  u32 timer = CSND_TIMER(sampleRate);
  if (timer < 0x0042)
    timer = 0x0042;
  else if (timer > 0xFFFF)
    timer = 0xFFFF;
  flags &= ~0xFFFF001F;
  flags |= SOUND_ENABLE | SOUND_CHANNEL(chn) | (timer << 16);

  if (pan == 2.0)
    CSND_SetChnRegs(flags, paddr0, paddr1, size, 0xFFFFFFFF, 0);
  else
    CSND_SetChnRegs(flags, paddr0, paddr1, size, CSND_VOL(vol, pan), 0);

  if (loopMode == CSND_LOOPMODE_NORMAL && paddr1 > paddr0) {
    // Now that the first block is playing, configure the size of the subsequent
    // blocks
    size -= paddr1 - paddr0;
    CSND_SetBlock(chn, 1, paddr1, size);
  }
}

// createDspBlock: Create a new block for DSP service
void createDspBlock(ndspWaveBuf *waveBuf, u16 bps, u32 size, bool loop,
                    u32 *data) {
  waveBuf->data_vaddr = (void *)data;
  waveBuf->nsamples = size / bps;
  waveBuf->looping = loop;
  waveBuf->offset = 0;
  DSP_FlushDataCache(data, size);
}

// populatePurgeTable: Create a new block for DSP service
void populatePurgeTable(Music *songFile, ndspWaveBuf *waveBuf) {
  PurgeTable *tmp = songFile->blocks;
  if (tmp == NULL) {
    tmp = (PurgeTable *)(malloc(sizeof(PurgeTable)));
    tmp->pointer = waveBuf;
    tmp->next == NULL;
  } else {
    while (tmp->next != NULL) tmp = tmp->next;
    tmp->next = (PurgeTable *)(malloc(sizeof(PurgeTable)));
    tmp->next->pointer = waveBuf;
    tmp->next->next == NULL;
  }
}

// purgeTable: Free any entry in PurgeTable struct
void purgeTable(PurgeTable *tbl) {
  while (tbl != NULL) {
    PurgeTable *tmp = tbl;
    tbl = tbl->next;
    free(tmp->pointer);
    free(tmp);
  }
}