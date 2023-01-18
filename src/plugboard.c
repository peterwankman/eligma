/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ermac.h"
#include "plugboard.h"
#include "util.h"

static int forward(void *instance, int signal, int *status) {
	el_class_plugboard *self = instance;
	char letter;
	int out;

	if((self == NULL) || (status == NULL)) return -1;

	letter = self->right[signal];
	out = el_find(self->left, sizeof(self->left), letter, status);
	if(*status != EL_OK) return -1;

	return out;
}

static int backward(void *instance, int signal, int *status) {
	el_class_plugboard *self = instance;
	char letter;
	int out;

	if((self == NULL) || (status == NULL)) return -1;

	letter = self->left[signal];
	out = el_find(self->right, sizeof(self->right), letter, status);
	if(*status != EL_OK) return -1;

	return out;
}

/**/

static int init(void *instance, char *wiring) {
	el_class_plugboard *self = instance;
	char *pair, A, B;
	size_t i, n_pairs, posA, posB;
	int status;

	if((self == NULL) || (wiring == NULL))
		return EL_ERR_NULLPTR;

	for(i = 0; i < strlen(wiring); i++) {
		if(wiring[i] < 0) return EL_ERR_INVALID;
		if(!isalpha(wiring[i])) return EL_ERR_INVALID;
		if(islower(wiring[i])) wiring[i] = toupper(wiring[i]);
	}

	el_fill_alphabet(self->left);
	el_fill_alphabet(self->right);
	
	n_pairs = strlen(wiring);
	if(n_pairs % 2 != 0) return EL_ERR_INVALID;
	n_pairs /= 2;

	for(i = 0; i < n_pairs; i++) {
		pair = wiring + 2 * i;
		A = pair[0];
		B = pair[1];

		posA = el_find(self->left, sizeof(self->left), A, &status);
		posB = el_find(self->left, sizeof(self->left), B, &status);
		self->left[posA] = B;
		self->left[posB] = A;
	}

	return EL_OK;
}

static void destroy(void *instance) {
	el_class_plugboard *self = instance;
	if(self == NULL) return;
	/* Destroy secret data. */
	memset(self->left, 0, sizeof(self->left));
	memset(self->right, 0, sizeof(self->right));
	free(self);
}

el_class_plugboard *el_plugboard_new(char *wiring, int *status) {
	el_class_plugboard *out;

	if((wiring == NULL) || (status == NULL)) return NULL;

	if((out = malloc(sizeof(el_class_plugboard))) == NULL) {
		*status = EL_ERR_MALLOC;
		return NULL;
	}

	out->init = init;
	out->destroy = destroy;
	out->forward = forward;
	out->backward = backward;

	if((*status = out->init(out, wiring)) != EL_OK) {
		free(out);
		return NULL;
	}
	return out;
}

void el_plugboard_destroy(void *instance) {
	el_class_plugboard *self = instance;
	self->destroy(self);
}