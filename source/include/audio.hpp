#include <3ds.h>

// Additional encoding
typedef enum {
  CSND_ENCODING_VORBIS = 4,
  CSND_ENCODING_MPEG
} CSND_EXTRA_ENCODING;

// Audio structs
struct PurgeTable {
  ndspWaveBuf *pointer;
  PurgeTable *next;
};

struct Music {
  u32 magic;
  u32 samplerate;
  u16 bytepersample;
  u8 *audiobuf;
  u8 *audiobuf2;
  ndspWaveBuf *wavebuf;
  ndspWaveBuf *wavebuf2;
  u32 lastCheck;
  u32 size;
  u32 mem_size;
  Handle sourceFile;
  u32 startRead;
  char author[256];
  char title[256];
  u64 tick;
  bool isPlaying;
  u32 ch;
  u8 audiotype;
  bool streamLoop;
  bool big_endian;
  u8 encoding;
  PurgeTable *blocks;
  u32 audio_pointer;
};

struct wav {
  u32 magic;
  u32 samplerate;
  u16 bytepersample;
  u8 *audiobuf;
  u8 *audiobuf2;
  u32 size;
  u32 mem_size;
  Handle sourceFile;
  u32 startRead;
  char author[256];
  char title[256];
  u32 moltiplier;
  u64 tick;
  bool isPlaying;
  u32 ch;
  u32 ch2;
  bool streamLoop;
  bool big_endian;
  u8 encoding;
  u32 *misc;
  u32 audio_pointer;
  u32 package_size;
  u32 total_packages_size;
  u32 loop_index;
};
