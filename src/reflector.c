/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "ermac.h"
#include "reflector.h"
#include "util.h"

static int reflect(void *instance, int signal, int *status) {
	el_class_reflector *self = instance;
	char letter;
	int out;

	if(status == NULL) return -1;
	*status = EL_OK;

	letter = EL_ALPHABET[signal];
	out = el_find(self->right, sizeof(self->right), letter, status);
	if(*status != EL_OK) return -1;

	return out;
}

/**/

static int init(void *instance, char *wiring) {
	el_class_reflector *self = instance;
	size_t i;

	if((self == NULL) || (wiring == NULL))
		return EL_ERR_NULLPTR;

	for(i = 0; i < sizeof(self->right); i++) {
		if(wiring[i] < 0) return EL_ERR_INVALID;
		if(!isalpha(wiring[i])) return EL_ERR_INVALID;
		if(islower(wiring[i])) wiring[i] = toupper(wiring[i]);
	}

	el_fill_alphabet(self->left);
	memcpy(self->right, wiring, 26);

	return EL_OK;
}

static void destroy(void *instance) {
	el_class_reflector *self = instance;

	if(self == NULL) return;
	/* Overwrite secret data. */
	memset(self->right, 0, sizeof(self->right));
	free(self);
}

el_class_reflector *el_reflector_new(char *wiring, int *status) {
	el_class_reflector *out;

	if((wiring == NULL) || (status == NULL)) return NULL;

	if((out = malloc(sizeof(el_class_reflector))) == NULL) {
		*status = EL_ERR_MALLOC;
		return NULL;
	}

	out->init = init;
	out->destroy = destroy;
	out->reflect = reflect;

	if((*status = out->init(out, wiring)) != EL_OK) {
		free(out);
		return NULL;
	}
	return out;
}

void el_reflector_destroy(void *instance) {
	el_class_reflector *self = instance;
	self->destroy(self);
}