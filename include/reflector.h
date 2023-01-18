/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#ifndef REFLECTOR_H_
#define REFLECTOR_H_

typedef struct el_class_reflector {
	char left[26], right[26];

	int (*init)(void *, char *);
	void (*destroy)(void *);
	int (*reflect)(void *, int, int *);
} el_class_reflector;

el_class_reflector *el_reflector_new(char *wiring, int *status);
void el_reflector_destroy(void *instance);

#endif
