#ifndef KEYS_H_
#define KEYS_H_

#include "Public.h"

#define KEYSVAL			(~(GPIOA->IDR) & 0x03)

extern void KeysInit(void);

#endif /* KEYS_H_ */
