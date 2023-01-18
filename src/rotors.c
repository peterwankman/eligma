/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ermac.h"
#include "rotors.h"
#include "util.h"

static int forward(void *instance, int signal, int *status) {
	el_class_rotor *self = instance;
	char letter;
	int out;

	if((self == NULL) || (status == NULL)) return -1;

	letter = self->right[signal];
	out = el_find(self->left, sizeof(self->left), letter, status);
	if(*status != EL_OK) return -1;

	return out;
}

static int backward(void *instance, int signal, int *status) {
	el_class_rotor *self = instance;
	char letter;
	int out;

	if((self == NULL) || (status == NULL)) return -1;

	letter = self->left[signal];
	out = el_find(self->right, sizeof(self->right), letter, status);
	if(*status != EL_OK) return -1;

	return out;
}

static void show(void *instance) {
	el_class_rotor *self = instance;
	int i;

	if(self == NULL) return;

	for(i = 0; i < sizeof(self->left); i++)
		printf("%c", self->left[i]);
	printf("\n");
	for(i = 0; i < sizeof(self->right); i++)
		printf("%c", self->right[i]);
	printf("\n\n");
}

static void rotate(void *instance, int n, int direction, int *status) {
	el_class_rotor *self = instance;
	int i;
	n = n % 26;

	if((self == NULL) || (status == NULL)) return;

	for(i = 0; i < n; i++) {
		el_rotate_string(self->left, sizeof(self->left), direction);
		el_rotate_string(self->right, sizeof(self->right), direction);
	}

	*status = EL_OK;
}

static void rotate_to_letter(void *instance, char letter, int *status) {
	el_class_rotor *self = instance;
	int pos;

	if((self == NULL) || (status == NULL)) return;

	pos = el_find(EL_ALPHABET, sizeof(EL_ALPHABET), letter, status);
	if(*status != EL_OK) return;
	rotate(self, pos, EL_FORWARD, status);
}

static void set_ring(void *instance, int n, int *status) {
	el_class_rotor *self = instance;
	int i;

	if((self == NULL) || (status == NULL)) return;

	self->rotate(self, n - 1, EL_BACKWARD, status);
	if(*status != EL_OK) return;

	for(i = 0; i < self->n_notches; i++) {
		el_find(EL_ALPHABET, sizeof(EL_ALPHABET), self->notch[i], status);
		self->notch[i] -= (n - 1);
		if(self->notch[i] < 'A') self->notch[i] += 26;
	}
}

/**/

static int init(void *instance, char *wiring, char *notches) {
	el_class_rotor *self = instance;
	size_t n_notches;
	int i;	

	if((self == NULL) || (wiring == NULL) || (notches == NULL))
		return EL_ERR_NULLPTR;

	for(i = 0; i < sizeof(self->right); i++) {
		if(wiring[i] < 0) return EL_ERR_INVALID;
		if(!isalpha(wiring[i])) return EL_ERR_INVALID;
		if(islower(wiring[i])) wiring[i] = toupper(wiring[i]);
	}

	el_fill_alphabet(self->left);
	memcpy(self->right, wiring, 26);

	n_notches = strlen(notches);
	for(i = 0; i < n_notches; i++) {
		if(notches[i] < 0) return EL_ERR_INVALID;
		if(!isalpha(notches[i])) return EL_ERR_INVALID;
		if(islower(notches[i])) notches[i] = toupper(notches[i]);
	}

	if((self->notch = malloc(n_notches)) == NULL)
		return EL_ERR_MALLOC;
	memcpy(self->notch, notches, n_notches);
	self->n_notches = n_notches;

	return EL_OK;
}

static void destroy(void *instance) {
	el_class_rotor *self = instance;

	if(self == NULL) return;
	/* Rotors contain secret data. Overwrite. */
	memset(self->left, 0, sizeof(self->left));
	memset(self->right, 0, sizeof(self->left));
	memset(self->notch, 0, self->n_notches);

	free(self->notch);
	free(self);
}

el_class_rotor *el_rotor_new(char *wiring, char *notches, int *status) {
	el_class_rotor *out;

	if((wiring == NULL) || (notches == NULL) || (status == NULL)) return NULL;
	
	if((out = malloc(sizeof(el_class_rotor))) == NULL) {
		*status = EL_ERR_MALLOC;
		return NULL;
	} 

	out->init = init;
	out->destroy = destroy;

	out->forward = forward;
	out->backward = backward;
	out->show = show;
	out->rotate = rotate;
	out->rotate_to_letter = rotate_to_letter;
	out->set_ring = set_ring;

	if((*status = out->init(out, wiring, notches)) != EL_OK) {
		free(out);
		return NULL;
	}
	return out;
}

void el_rotor_destroy(void *instance) {
	el_class_rotor *self = instance;
	self->destroy(self);
}