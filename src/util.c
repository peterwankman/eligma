/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#include <stdint.h>
#include <string.h>

#include "ermac.h"
#include "util.h"

void el_fill_alphabet(char *in) {
	memcpy(in, EL_ALPHABET, sizeof(EL_ALPHABET));
}

int el_rotate_string(char *str, size_t len, int direction) {
	char c;

	if(direction == EL_FORWARD) {
		c = str[0];
		memmove(str, str + 1, len - 1);
		str[len - 1] = c;
	} else if(direction == EL_BACKWARD) {
		c = str[len - 1];
		memmove(str + 1, str, len - 1);
		str[0] = c;
	} else {
		return EL_ERR_INVALID;
	}
	return EL_OK;
}

int el_find(char *haystack, size_t size, char needle, int *status) {
	int pos;

	if(status == NULL) return EL_ERR_NOTFOUND;

	*status = EL_OK;
	for(pos = 0; pos < size; pos++)
		if(haystack[pos] == needle) return pos;

	*status = EL_ERR_NOTFOUND;
	return EL_ERR_NOTFOUND;
}