/*
 * KeyPro.h
 *
 *  Created on: May 21, 2019
 *      Author: zy
 */

#ifndef KEYPRO_H_
#define KEYPRO_H_

#include "Public.h"

typedef enum
{
	KEYINIT,
	READKEY,
	KEYPRESS,
	KEYCLICK,
}_KeyStatus;

typedef struct
{
	_KeyStatus Status;
	u8 KeyVal[2];
	u8 cnt;
}_Key;

extern void KeyPro(void);

#endif /* KEYPRO_H_ */
