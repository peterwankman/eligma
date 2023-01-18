/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#ifndef FUTIL_H_
#define FUTIL_H_

#define EL_ALPHABET		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"

void el_fill_alphabet(char *in);
int el_rotate_string(char *str, size_t len, int direction);
int el_find(char *haystack, size_t size, char needle, int *status);

#endif
