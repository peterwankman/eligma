/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

typedef struct el_class_keyboard {
	int (*init)(void *);
	void (*destroy)(void *);
	int (*forward)(void *, char, int *);
	char (*backward)(void *, int, int *);
	char (*get_key)(void *,int *);
} el_class_keyboard;

el_class_keyboard *el_keyboard_new(int *status);
void el_keyboard_destroy(void *instance);

#endif
