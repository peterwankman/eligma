/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#ifndef ROTORS_H_
#define ROTORS_H_

typedef struct el_class_rotor {
	char left[26], right[26];
	size_t n_notches;
	char *notch;

	int (*init)(void *, char *, char *);
	void (*destroy)(void *);
	int (*forward)(void*, int, int*);
	int (*backward)(void*, int, int*);
	void (*show)(void *);
	void (*rotate)(void *, int, int, int *);
	void (*rotate_to_letter)(void *, char, int *);
	void (*set_ring)(void *, int, int *);
} el_class_rotor;

el_class_rotor * el_rotor_new(char *alphabet, char *notches, int *status);

#endif
