#include "IR.h"

#define IR_IN_VAL P12

#define NUM_BIT 12

#define MIN_LOW 100
#define MAX_LOW 1600
#define LOW_VAL 800
#define MIN_HI 100
#define MAX_HI 1600

static void tim2_init(void);
static void cap_init(void);

#define STATE_IDLE 0
#define STATE_RECEIVE 1
#define STATE_CHECK_BIT_0 2
#define STATE_CHECK_BIT_1 3
#define STATE_DECODE_SUCC 4

typedef void (*pfunc)(void);

void idle_state(void);
void receive_state(void);
void check_bit_0_state(void);
void check_bit_1_state(void);
void decode_succ_state(void);

static pfunc funcSet[] = {
	idle_state,
	receive_state,
	check_bit_0_state,
	check_bit_1_state,
	decode_succ_state
};

static uint8_t u8State = STATE_IDLE;
static uint8_t u8Count = 0;
static uint32_t u32Result = 0;

void idle_state(void)
{
	if (!IR_IN_VAL) {
		u8Count = 0;
		u32Result = 0;
		u8State = STATE_RECEIVE;
	}
}

void receive_state(void)
{
	uint16_t u16Tmp;
	if (IR_IN_VAL) {
		u16Tmp = C0H * 256 + C0L;
		if (u16Tmp < MIN_LOW) {
			u8State = STATE_IDLE;
			return;
		}
		if (u16Tmp > MAX_LOW) {
			u8State = STATE_IDLE;
			return;
		}
		if (u16Tmp > LOW_VAL) {
			u32Result <<= 1;
			++u8Count;
			if (u8Count == NUM_BIT) {
				u8State = STATE_DECODE_SUCC;
				return;
			}
			u8State = STATE_CHECK_BIT_0;
		} else {
			u32Result <<= 1;
			u32Result |= 1;
			++u8Count;
			if (u8Count == NUM_BIT) {
				u8State = STATE_DECODE_SUCC;
				return;
			}			
			u8State = STATE_CHECK_BIT_1;
		}
	}
}

void check_bit_0_state(void)
{
	uint16_t u16Tmp;
	if (!IR_IN_VAL) {
		u16Tmp = C0H * 256 + C0L;
		if (u16Tmp < MIN_HI) {
			u8State = STATE_IDLE;
			return;
		}
		if (u16Tmp > MAX_HI) {
			u8State = STATE_IDLE;
			return;
		}
		if (u16Tmp < LOW_VAL) {
			u8State = STATE_RECEIVE;
		} else {
			u8State = STATE_IDLE;
		}
	}
}

void check_bit_1_state(void)
{
	uint16_t u16Tmp;
	if (!IR_IN_VAL) {
		u16Tmp = C0H * 256 + C0L;
		if (u16Tmp < MIN_HI) {
			u8State = STATE_IDLE;
			return;
		}
		if (u16Tmp > MAX_HI) {
			u8State = STATE_IDLE;
			return;
		}
		if (u16Tmp > LOW_VAL) {
			u8State = STATE_RECEIVE;
		} else {
			u8State = STATE_IDLE;
		}
	}
}

void decode_succ_state(void)
{
	
}

static void tim2_init(void)
{
    /* pre-scalar = 1 / 16*/
    T2MOD &= ~(0x70);
    T2MOD |= (0x20);
    /* auto reload mode */
    CM_RL2 = 0;
	/* auto clear */
	T2MOD |= (1 << 3);
}

static void cap_init(void)
{
	T2MOD |= ((1<<0));
    TR2 = 1;
    CAPCON3 = 0x00;
    CAPCON1 = 0x02;
    CAPCON2 = (1<<4);
    CAPCON0 |= (1<<4);
}

void IR_Init(void)
{
	u8State = STATE_IDLE;
    u32Result = 0;
    u8Count = 0;
    tim2_init();
	cap_init();
}

uint8_t IR_Check(uint32_t *pu32Cmd)
{
	if (TF2) {
		TF2 = 0;
		u8State = STATE_IDLE;
	}
	if (CAPCON0 & (1<<0)) {
		funcSet[u8State]();
		CAPCON0 &= ~(1<<0);
	}
	if (u8State == STATE_DECODE_SUCC) {
		u8State = STATE_IDLE;
		*pu32Cmd = u32Result;
		return 1;
	}
	return 0;
}