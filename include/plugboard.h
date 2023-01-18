/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#ifndef PLUGBOARD_H_
#define PLUGBOARD_H_

typedef struct el_class_plugboard {
	char left[26], right[26];

	int (*init)(void *, char *);
	void (*destroy)(void *);
	int (*forward)(void *, int, int *);
	int (*backward)(void *, int, int *);
} el_class_plugboard;

el_class_plugboard *el_plugboard_new(char *wiring, int *status);
void el_plugboard_destroy(void *instance);

#endif