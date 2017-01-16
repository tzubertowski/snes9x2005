#include "../copyright"

#ifdef USE_BLARGG_APU

#ifndef APU_BLARGG_H
#define APU_BLARGG_H

#include "snes9x.h"

typedef void (*dsp_copy_func_t)( unsigned char** io, void* state, size_t );

#define ECHO_HIST_SIZE		8
#define ECHO_HIST_SIZE_X2	16

/* Sound control */

/* Mutes voices corresponding to non-zero bits in mask (issues repeated KOFF events).
   Reduces emulation accuracy. */

#define VOICE_COUNT		8
#define EXTRA_SIZE		16
#define EXTRA_SIZE_DIV_2	8
#define BRR_BUF_SIZE		12
#define BRR_BUF_SIZE_X2		24
#define BRR_BLOCK_SIZE		9

/* DSP register addresses */

/* Global registers */

#define R_MVOLL 0x0C
#define R_MVOLR 0x1C
#define R_EVOLL 0x2C
#define R_EVOLR 0x3C
#define R_KON	0x4C
#define R_KOFF	0x5C
#define R_FLG	0x6C
#define R_ENDX	0x7C
#define R_EFB	0x0D
#define R_EON	0x4D
#define R_PMON	0x2D
#define R_NON	0x3D
#define R_DIR	0x5D
#define R_ESA	0x6D
#define R_EDL	0x7D
#define R_FIR	0x0F	/* 8 coefficients at 0x0F, 0x1F ... 0x7F */

/* Voice registers */
#define V_VOLL		0x00
#define V_VOLR		0x01
#define V_PITCHL	0x02
#define V_PITCHH	0x03
#define V_SRCN		0x04
#define V_ADSR0		0x05
#define V_ADSR1		0x06
#define V_GAIN		0x07
#define V_ENVX		0x08
#define V_OUTX		0x09

/* Status flag handling */

/* Hex value in name to clarify code and bit shifting.
   Flag stored in indicated variable during emulation */

#define N80 0x80	/* nz */
#define V40 0x40	/* psw */
#define P20 0x20	/* dp */
#define B10 0x10	/* psw */
#define H08 0x08	/* psw */
#define I04 0x04	/* psw */
#define Z02 0x02	/* nz */
#define C01 0x01	/* c */

#define NZ_NEG_MASK 0x880	/* either bit set indicates N flag set */

#define REGISTER_COUNT 128

#define ENV_RELEASE	0
#define ENV_ATTACK	1
#define ENV_DECAY	2
#define ENV_SUSTAIN	3

typedef struct
{
   int      buf [BRR_BUF_SIZE_X2]; // decoded samples (twice the size to simplify wrap handling)
   int      buf_pos;               // place in buffer where next samples will be decoded
   int      interp_pos;            // relative fractional position in sample (0x1000 = 1.0)
   int      brr_addr;              // address of current BRR block
   int      brr_offset;            // current decoding offset in BRR block
   uint8_t* regs;                  // pointer to voice's DSP registers
   int      vbit;                  // bitmask for voice: 0x01 for voice 0, 0x02 for voice 1, etc.
   int      kon_delay;             // KON delay/current setup phase
   int      env_mode;
   int      env;                   // current envelope level
   int      hidden_env;            // used by GAIN mode 7, very obscure quirk
   uint8_t  t_envx_out;
} dsp_voice_t;

typedef struct
{
   uint8_t regs [REGISTER_COUNT];

   /* Echo history keeps most recent 8 samples (twice the size to simplify wrap handling) */

   int echo_hist [ECHO_HIST_SIZE_X2] [2];

   int (*echo_hist_pos) [2]; /* &echo_hist [0 to 7] */

   int every_other_sample; /* toggles every sample */
   int kon;                /* KON value when last checked */
   int noise;
   int counter;
   int echo_offset;        /* offset from ESA in echo buffer */
   int echo_length;        /* number of bytes that echo_offset will stop at */
   int phase;              /* next clock cycle to run (0-31) */

   /* Hidden registers also written to when main register is written to */
   int     new_kon;
   uint8_t endx_buf;
   uint8_t envx_buf;
   uint8_t outx_buf;

   /* Temporary state between clocks */

   /* read once per sample */
   int t_pmon;
   int t_non;
   int t_eon;
   int t_dir;
   int t_koff;

   /* read a few clocks ahead then used */
   int t_brr_next_addr;
   int t_adsr0;
   int t_brr_header;
   int t_brr_byte;
   int t_srcn;
   int t_esa;
   int t_echo_enabled;

   /* internal state that is recalculated every sample */
   int t_dir_addr;
   int t_pitch;
   int t_output;
   int t_looped;
   int t_echo_ptr;

   /* left/right sums */
   int t_main_out [2];
   int t_echo_out [2];
   int t_echo_in  [2];

   dsp_voice_t voices [VOICE_COUNT];

   /* non-emulation state */
   uint8_t* ram; /* 64K shared RAM between DSP and SMP */
   int16_t* out;
   int16_t* out_end;
   int16_t* out_begin;
   int16_t  extra [EXTRA_SIZE];

   int     rom_enabled;
   uint8_t *rom;
   uint8_t *hi_ram;
} dsp_state_t;

#if !SPC_NO_COPY_STATE_FUNCS

typedef struct {
   dsp_copy_func_t func;
   unsigned char** buf;
} spc_state_copy_t;

#define SPC_COPY( type, state ) state = (type) spc_copier_copy_int(&copier, state, sizeof (type) );

#endif

#define REG_COUNT	0x10
#define PORT_COUNT	4
#define TEMPO_UNIT	0x100
#define STATE_SIZE	68 * 1024L /* maximum space needed when saving */
#define TIMER_COUNT	3
#define ROM_SIZE	0x40
#define ROM_ADDR	0xFFC0

/* 1024000 SPC clocks per second, sample pair every 32 clocks */
#define CLOCKS_PER_SAMPLE 32

#define R_TEST		0x0
#define R_CONTROL	0x1
#define R_DSPADDR	0x2
#define R_DSPDATA	0x3
#define R_CPUIO0	0x4
#define R_CPUIO1	0x5
#define R_CPUIO2	0x6
#define R_CPUIO3	0x7
#define R_F8		0x8
#define R_F9		0x9
#define R_T0TARGET	0xA
#define R_T1TARGET	0xB
#define R_T2TARGET	0xC
#define R_T0OUT		0xD
#define R_T1OUT		0xE
#define R_T2OUT		0xF

/* Value that padding should be filled with */
#define CPU_PAD_FILL 0xFF

#if !SPC_NO_COPY_STATE_FUNCS
   /* Saves/loads state */
   void spc_copy_state( unsigned char** io, dsp_copy_func_t );
#endif

/* rel_time_t - Time relative to m_spc_time. Speeds up code a bit by eliminating
   need to constantly add m_spc_time to time from CPU. CPU uses time that ends
   at 0 to eliminate reloading end time every instruction. It pays off. */

typedef struct
{
   int next_time; /* time of next event */
   int prescaler;
   int period;
   int divider;
   int enabled;
   int counter;
} Timer;

/* Support SNES_MEMORY_APURAM */
uint8_t *spc_apuram (void);

typedef struct
{
   Timer timers [TIMER_COUNT];

   uint8_t smp_regs [2] [REG_COUNT];

   struct
   {
      int pc;
      int a;
      int x;
      int y;
      int psw;
      int sp;
   } cpu_regs;

   int dsp_time;
   int spc_time;

   int tempo;

   int      extra_clocks;
   int16_t* buf_begin;
   int16_t*	buf_end;
   int16_t* extra_pos;
   int16_t  extra_buf [EXTRA_SIZE];

   int     rom_enabled;
   uint8_t rom    [ROM_SIZE];
   uint8_t hi_ram [ROM_SIZE];

   unsigned char cycle_table [256];

   struct
   {
      /* padding to neutralize address overflow */
      union {
         uint8_t  padding1 [0x100];
         uint16_t align; /* makes compiler align data for 16-bit access */
      } padding1 [1];
      uint8_t ram      [0x10000];
      uint8_t padding2 [0x100];
   } ram;
} spc_state_t;

/* Number of samples written to output since last set */
#define SPC_SAMPLE_COUNT() ((m.extra_clocks >> 5) * 2)

typedef void (*apu_callback)(void);

#define SPC_SAVE_STATE_BLOCK_SIZE	(STATE_SIZE + 8)

bool S9xInitAPU (void);
void S9xDeinitAPU (void);
void S9xResetAPU (void);
void S9xSoftResetAPU (void);
uint8_t S9xAPUReadPort (int port);
void S9xAPUWritePort (int port, uint8_t byte);
void S9xAPUExecute (void);
void S9xAPUSetReferenceTime (int32_t cpucycles);
void S9xAPUTimingSetSpeedup (int ticks);
void S9xAPUAllowTimeOverflow (bool allow);
void S9xAPULoadState (uint8_t * block);
void S9xAPUSaveState (uint8_t * block);

bool S9xInitSound (int buffer_ms, int lag_ms);

bool S9xSyncSound (void);
int S9xGetSampleCount (void);
void S9xFinalizeSamples (void);
void S9xClearSamples (void);
bool S9xMixSamples (int16_t * buffer, unsigned sample_count);
void S9xSetSamplesAvailableCallback (apu_callback);

#endif // APU_BLARGG_H

#endif
