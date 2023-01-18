/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>
#endif

#include <stdlib.h>

#include "ermac.h"
#include "keyboard.h"
#include "util.h"

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>

static char get_key(void *instance, int *status) {
	el_class_keyboard *self = instance;
	struct termios old, new = { 0 };
	char out;

	if((self == NULL) || (status == NULL)) return 0;
	*status = EL_ERR_NULLPTR;

	tcgetattr(0, &old);

	new.c_lflag &= ~ICANON;
	new.c_lflag &= ~ECHO;
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 0;

	if(tcsetattr(0, TCSANOW, &new) < 0) return '\0';
	if(read(0, &out, 1) < 0) return '\0';
	if(tcsetattr(0, TCSANOW, &old) < 0) return '\0';

	*status = EL_OK;
	return out;
}
#else
static char get_key(void *instance, int *status) {
	el_class_keyboard *self = instance;
	DWORD cc;
	INPUT_RECORD irec;
	HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
	KEY_EVENT_RECORD *event;

	if((self == NULL) || (status == NULL)) return 0;

	if(h == NULL) return FALSE;

	for(;;) {
		ReadConsoleInput(h, &irec, 1, &cc);
		event = (KEY_EVENT_RECORD *)&irec.Event;

		if((irec.EventType == KEY_EVENT) &&
			(event->bKeyDown)) {
			*status = EL_OK;
			return irec.Event.KeyEvent.uChar.AsciiChar;
		}
	}
	return 0;
}
#endif

static int forward(void *instance, char letter, int *status) {
	int signal;
	if(status == NULL) return EL_ERR_NULLPTR;
	signal = el_find(EL_ALPHABET, sizeof(EL_ALPHABET), letter, status);
	return signal;
}

static char backward(void *instance, int signal, int *status) {
	if(status == NULL) return 0;
	*status = EL_OK;
	return EL_ALPHABET[signal];
}

/**/

static int init(void *instance) {
	el_class_keyboard *self = instance;
	if(self == NULL) return EL_ERR_NULLPTR;
	return EL_OK;
}

static void destroy(void *instance) {
	el_class_keyboard *self = instance;
	if(self == NULL) return;
	free(self);
}

el_class_keyboard *el_keyboard_new(int *status) {
	el_class_keyboard *out;

	if(status == NULL) return NULL;

	if((out = malloc(sizeof(el_class_keyboard))) == NULL) {
		*status = EL_ERR_MALLOC;
		return NULL;
	}

	out->init = init;
	out->destroy = destroy;
	out->forward = forward;
	out->backward = backward;
	out->get_key = get_key;

	if((*status = out->init(out)) != EL_OK) {
		free(out);
		return NULL;
	}
	return out;
}

void el_keyboard_destroy(void *instance) {
	el_class_keyboard *self = instance;
	self->destroy(self);
}