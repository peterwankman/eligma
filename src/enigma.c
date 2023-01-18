/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "ermac.h"

#include "enigma.h"
#include "keyboard.h"
#include "plugboard.h"
#include "reflector.h"
#include "rotors.h"

static void set_key(void *instance, char *key, int *status) {
	el_class_enigma *self = instance;

	if((self == NULL) || (status == NULL)) return;
	*status = EL_ERR_INVALID;

	if((key[0] < 0) || (!isalnum(key[0]))) return;
	self->rotor[0]->rotate_to_letter(self->rotor[0], toupper(key[0]), status);

	if((key[0] < 0) || (!isalnum(key[0]))) return;
	self->rotor[1]->rotate_to_letter(self->rotor[1], toupper(key[1]), status);

	if((key[0] < 0) || (!isalnum(key[0]))) return;
	self->rotor[2]->rotate_to_letter(self->rotor[2], toupper(key[2]), status);

	*status = EL_OK;
}

static void set_rings(void *instance, int ring1, int ring2, int ring3, int *status) {
	el_class_enigma *self = instance;

	if((self == NULL) || (status == NULL)) return;
	*status = EL_ERR_INVALID;

	if(ring1 >= 'A' && ring1 <= 'Z') ring1 -= 'A' - 1;
	if((ring1 < 1) || (ring1 > 26)) return;
	self->rotor[0]->set_ring(self->rotor[0], ring1, status);

	if(ring2 >= 'A' && ring2 <= 'Z') ring2 -= 'A' - 1;
	if((ring2 < 1) || (ring2 > 26)) return;
	self->rotor[1]->set_ring(self->rotor[1], ring2, status);

	if(ring3 >= 'A' && ring3 <= 'Z') ring3 -= 'A' - 1;
	if((ring3 < 1) || (ring3 > 26)) return;
	self->rotor[2]->set_ring(self->rotor[2], ring3, status);	

	*status = EL_OK;
}

static char encipher(void *instance, char letter, int *status) {
	el_class_enigma *self = instance;
	int carry[3] = { 0, 0, 0 };
	int n_rotor, n_notch, signal;

	if((self == NULL) || (status == NULL)) return 0;

	for(n_rotor = 0; n_rotor < 3; n_rotor++) {
		for(n_notch = 0; n_notch < self->rotor[n_rotor]->n_notches; n_notch++) {
			if(self->rotor[n_rotor]->notch[n_notch] == self->rotor[n_rotor]->left[0]) {
				carry[n_rotor] = 1;
			}
		}
	}

	/* Rotate one step */
	if(carry[1] && carry[2]) {
		self->rotor[0]->rotate(self->rotor[0], 1, EL_FORWARD, status);
		self->rotor[1]->rotate(self->rotor[1], 1, EL_FORWARD, status);
		self->rotor[2]->rotate(self->rotor[2], 1, EL_FORWARD, status);
	} else if(carry[1]) {
		self->rotor[0]->rotate(self->rotor[0], 1, EL_FORWARD, status);
		self->rotor[1]->rotate(self->rotor[1], 1, EL_FORWARD, status);
		self->rotor[2]->rotate(self->rotor[2], 1, EL_FORWARD, status);
	} else if(carry[2]) {
		self->rotor[1]->rotate(self->rotor[1], 1, EL_FORWARD, status);
		self->rotor[2]->rotate(self->rotor[2], 1, EL_FORWARD, status);
	} else {
		self->rotor[2]->rotate(self->rotor[2], 1, EL_FORWARD, status);
	}
	
	/* Send the signal through the wire. */
	signal = self->keyboard->forward(self->keyboard, toupper(letter), status);	//rintf("KB %d\n", signal);
	signal = self->plugboard->forward(self->plugboard, signal, status); //rintf("PB %d\n", signal);
	signal = self->rotor[2]->forward(self->rotor[2], signal, status);   //printf("R3 %d\n", signal);
	signal = self->rotor[1]->forward(self->rotor[1], signal, status);	//printf("R2 %d\n", signal);
	signal = self->rotor[0]->forward(self->rotor[0], signal, status);	//printf("R1 %d\n", signal);
	signal = self->reflector->reflect(self->reflector, signal, status); //printf("RE %d\n", signal);
	signal = self->rotor[0]->backward(self->rotor[0], signal, status);	//printf("R1 %d\n", signal);
	signal = self->rotor[1]->backward(self->rotor[1], signal, status);	//printf("R2 %d\n", signal);
	signal = self->rotor[2]->backward(self->rotor[2], signal, status);	//	printf("R3 %d\n", signal);
	signal = self->plugboard->backward(self->plugboard, signal, status);//	printf("PB %d\n", signal);
	letter = self->keyboard->backward(self->keyboard, signal, status);	//	printf("KB %c\n", letter);

	return letter;
}

static char *encipher_string(void *instance, char *message, int *status) {
	el_class_enigma *self = instance;
	size_t len, i;
	char plaintext, ciphertext, *out;

	if((self == NULL) || (message == NULL) || (status == NULL)) return NULL;

	len = strlen(message);
	if((out = malloc(len + 1)) == NULL) return NULL;

	for(i = 0; i < len; i++) {
		plaintext = message[i];
		if((plaintext < 0) || (!isalpha(plaintext))) {
			out[i] = '*';
			continue;
		}
		if(islower(plaintext))
			plaintext = toupper(plaintext);
	
		ciphertext = encipher(self, plaintext, status);
		out[i] = ciphertext;
	}
	out[len] = '\0';

	*status = EL_OK;
	return out;
}

/**/

static int init(void *instance, el_class_keyboard *kb, el_class_plugboard *pb, el_class_reflector *re,
	el_class_rotor *r1, el_class_rotor *r2, el_class_rotor *r3) {

	el_class_enigma *self = instance;

	if((self == NULL) || (kb == NULL) || (pb == NULL) || (re == NULL) || (r1 == NULL) || (r2 == NULL) || (r3 == NULL))
		return EL_ERR_NULLPTR;

	self->keyboard = kb;
	self->plugboard = pb;
	self->reflector = re;
	self->rotor[0] = r1;
	self->rotor[1] = r2;
	self->rotor[2] = r3;

	return EL_OK;
}

static void destroy(void *instance) {
	el_class_enigma *self = instance;

	if(self == NULL) return;
	/* Components (keyboard, plugboard, ...) are allocated by themselves
	 * before creating the enigma object. They should then also be destroyed
	 * on their own and not by us here. */
	free(self);
}

el_class_enigma *el_enigma_new(el_class_keyboard *kb, el_class_plugboard *pb, el_class_reflector *re,
	el_class_rotor *r1, el_class_rotor *r2, el_class_rotor *r3, int *status) {
	el_class_enigma *out;

	if(status == NULL) return NULL;

	if((out = malloc(sizeof(el_class_enigma))) == NULL) {
		*status = EL_ERR_MALLOC;
		return NULL;
	}

	out->init = init;
	out->destroy = destroy;
	out->set_key = set_key;
	out->set_rings = set_rings;
	out->encipher = encipher;
	out->encipher_string = encipher_string;
	

	if((*status = out->init(out, kb, pb, re, r1, r2, r3)) != EL_OK) {
		free(out);
		return NULL;
	}
	return out;
}

void el_enigma_destroy(void *instance) {
	el_class_enigma *self = instance;
	self->destroy(self);
}