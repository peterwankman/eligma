/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#ifndef ENIGMA_H_
#define ENIGMA_H_

#include "keyboard.h"
#include "plugboard.h"
#include "reflector.h"
#include "rotors.h"

typedef struct el_class_enigma {
	el_class_keyboard *keyboard;
	el_class_plugboard *plugboard;
	el_class_rotor *rotor[3];
	el_class_reflector *reflector;


	char left[26], right[26];

	int (*init)(void *, el_class_keyboard *, el_class_plugboard *, el_class_reflector *,
		el_class_rotor *, el_class_rotor *, el_class_rotor *);
	void (*destroy)(void *);

	char (*encipher)(void *, char, int *);
	char *(*encipher_string)(void *, char *, int *);
	void (*set_rings)(void *, int, int, int, int *);
	void (*set_key)(void *, char *, int *);

} el_class_enigma;

el_class_enigma *el_enigma_new(el_class_keyboard *kb, el_class_plugboard *pb, el_class_reflector *re,
	el_class_rotor *r1, el_class_rotor *r2, el_class_rotor *r3, int *status);

void el_enigma_destroy(void *instance);

#endif