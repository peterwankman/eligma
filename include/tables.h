/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

#ifndef TABLES_H_
#define TABLES_H_

const char rotor_i[26] =		"EKMFLGDQVZNTOWYHXUSPAIBRCJ";
const char rotor_ii[26] =		"AJDKSIRUXBLHWTMCQGZNPYFVOE";
const char rotor_iii[26] =		"BDFHJLCPRTXVZNYEIWGAKMUSQO";
const char rotor_iv[26] =		"ESOVPZJAYQUIRHXLNFTGKDCMWB";
const char rotor_v[26] =		"VZBRGITYUPSDNHLXAWMJQOFECK";
const char rotor_vi[26] =		"JPGVOUMFYQBENHZRDKASXLICTW";
const char rotor_vii[26] =		"NZJHGRCXMYSWBOUFAIVLPEKQDT";
const char rotor_viii[26] =		"FKQHTLXOCBJSPDZRAMEWNIUYGV";

const char rotor_beta[26] =		"LEYJVCNIXWPBQMDRTAKZGFUHOS";
const char rotor_gamma[26] =	"FSOKANUERHMBTIYCWLQPZXVGJD";

const char reflector_a[26] =	"EJMZALYXVBWFCRQUONTSPIKHGD";
const char reflector_b[26] =	"YRUHQSLDPXNGOKMIEBFZCWVJAT";
const char reflector_c[26] =	"FVPJIAOYEDRZXWGCTKUQSBNMHL";
const char reflector_bt[26] =	"ENKQAUYWJICOPBLMDXZVFTHRGS";
const char reflector_ct[26] =	"RDOBJNTKVEHMLFCWZAXGYIPSUQ";

const char notch_i1 =		'Q', notch_i2 =		'Q';
const char notch_ii1 =		'E', notch_ii2 =	'E';
const char notch_iii1 =		'V', notch_iii2 =	'V';
const char notch_iv1 =		'J', notch_iv2 =	'J';
const char notch_v1 =		'Z', notch_v2 =		'Z';
const char notch_vi1 =		'Z', notch_vi2 =	'M';
const char notch_vii1 =		'Z', notch_vii2 =	'M';
const char notch_viii1 =	'Z', notch_viii2 =	'M';

#endif
