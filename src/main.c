/*****************************************
 * SPDX-License-Identifier: GPL-2.0-only *
 *  Copyright (C) 2023  Martin Wolters   *
 *****************************************/

/*************************************************************
 * This was meant as an exercise in writing a class based    *
 * object oriented program in C. It was interesting, but     *
 * somewhat painful in places, as OOP tends to be.           *
 *                                                           *
 * Based on a video series on implementing an Enigma machine *
 * in Python. https://www.youtube.com/watch?v=sbm2dmkmqgQ    *
 *                                                           *
 * Special thanks to the creator, "Coding Cassowary".        *
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enigma.h"
#include "ermac.h"
#include "getopt.h"
#include "keyboard.h"
#include "plugboard.h"
#include "reflector.h"
#include "rotors.h"
#include "util.h"

/* Historical rotor wirings... */

el_class_rotor *R1, *R2, *R3, *R4, *R5, *R6, *R7, *R8;
el_class_reflector *BETA, *GAMMA, *RA, *RB, *RC, *RBT, *RCT;

static void init_objects(void) {
	int status; 

	R1 = el_rotor_new("EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q", &status);
	R2 = el_rotor_new("AJDKSIRUXBLHWTMCQGZNPYFVOE", "E", &status);
	R3 = el_rotor_new("BDFHJLCPRTXVZNYEIWGAKMUSQO", "V", &status);
	R4 = el_rotor_new("ESOVPZJAYQUIRHXLNFTGKDCMWB", "J", &status);
	R5 = el_rotor_new("VZBRGITYUPSDNHLXAWMJQOFECK", "Z", &status);
	R6 = el_rotor_new("JPGVOUMFYQBENHZRDKASXLICTW", "MZ", &status);
	R7 = el_rotor_new("NZJHGRCXMYSWBOUFAIVLPEKQDT", "MZ", &status);
	R8 = el_rotor_new("FKQHTLXOCBJSPDZRAMEWNIUYGV", "MZ", &status);

	
	BETA = el_reflector_new("LEYJVCNIXWPBQMDRTAKZGFUHOS", &status);
	GAMMA = el_reflector_new("FSOKANUERHMBTIYCWLQPZXVGJD", &status);
	RA = el_reflector_new("EJMZALYXVBWFCRQUONTSPIKHGD", &status);
	RB = el_reflector_new("YRUHQSLDPXNGOKMIEBFZCWVJAT", &status);
	RC = el_reflector_new("FVPJIAOYEDRZXWGCTKUQSBNMHL", &status);
	RBT = el_reflector_new("ENKQAUYWJICOPBLMDXZVFTHRGS", &status);
	RCT = el_reflector_new("RDOBJNTKVEHMLFCWZAXGYIPSUQ", &status);
}

static void destroy_objects(void) {
	R1->destroy(R1);
	R2->destroy(R2);
	R3->destroy(R3);
	R4->destroy(R4);
	R5->destroy(R5);
	R6->destroy(R6);
	R7->destroy(R7);
	R8->destroy(R8);

	BETA->destroy(BETA);
	GAMMA->destroy(GAMMA);
	RA->destroy(RA);
	RB->destroy(RB);
	RC->destroy(RC);
	RBT->destroy(RBT);
	RCT->destroy(RCT);
}

static int usage(char *argv) {
	printf("USAGE: ");
	printf("%s [-k KEY] [-R ROTORS] [-r RINGS] [-e REFLECTOR] [-p PLUGBOARD] [-m MESSAGE]\n", argv);
	printf("Example: ");
	printf("%s -k DMV -R 746 -r AGJ -e B -p BMDXEWGPJOKVNZRT -m BJUWJBFTIMXLRINPCOPSNAU\n", argv);
	printf("Without a message, input will be read from standard input.\n");
	printf("Available rotors: 1, 2, 3, 4, 5, 6, 7, 8\n");
	printf("Available reflectors: BETA, GAMMA, A, B, C, BT, CT\n");

	return EXIT_FAILURE;
 }

int main(int argc, char **argv) {
	el_class_enigma *ENIGMA;
	el_class_keyboard *KB;
	el_class_plugboard *PB;
	el_class_rotor *ROT[3] = { NULL, NULL, NULL };
	el_class_reflector *RE = NULL;

	char *key = NULL, *rotors = NULL, *rings = NULL, *reflector = NULL, *plugs = NULL, *message = NULL;
	char *ciphertext, cipherchar, input;
	int status, i, count = 0;

	while((i = getopt(argc, argv, "k:R:r:e:p:m:h")) > -1) {
		switch(i) {
			case 'k':	key = optarg;			break;
			case 'R':	rotors = optarg;		break;
			case 'r':	rings = optarg;			break;
			case 'e':	reflector = optarg;		break;
			case 'p':	plugs = optarg;			break;
			case 'm':	message = optarg;		break;
			case 'h':
			default:
				return usage(argv[0]);

		}
	}

	init_objects();

	KB = el_keyboard_new(&status);
	if(status != EL_OK) return usage(argv[0]);

	if(plugs != NULL) {
		fprintf(stderr, "Setting plugs...\n");
		PB = el_plugboard_new(plugs, &status);
		if(status != EL_OK) return usage(argv[0]);
	} else {
		fprintf(stderr, "Leaving plugs unplugged...\n");
		PB = el_plugboard_new("AABBCCDD", &status);
	}

	if(rotors != NULL) {
		fprintf(stderr, "Setting rotors...\n");
		if(strlen(rotors) != 3) return usage(argv[0]);
		for(i = 0; i < 3; i++) {
			if(rotors[i] == '1') ROT[i] = R1;
			else if(rotors[i] == '2') ROT[i] = R2;
			else if(rotors[i] == '3') ROT[i] = R3;
			else if(rotors[i] == '4') ROT[i] = R4;
			else if(rotors[i] == '5') ROT[i] = R5;
			else if(rotors[i] == '6') ROT[i] = R6;
			else if(rotors[i] == '7') ROT[i] = R7;
			else if(rotors[i] == '8') ROT[i] = R8;
			else return usage(argv[0]);
		}
	} else {
		fprintf(stderr, "Setting rotors to '123'...\n");
		ROT[0] = R1;
		ROT[1] = R2;
		ROT[2] = R3;
	}

	if(reflector != NULL) {
		fprintf(stderr, "Setting reflector...\n");
		if(!strcmp(reflector, "A")) RE = RA;
		else if(!strcmp(reflector, "B")) RE = RB;
		else if(!strcmp(reflector, "C")) RE = RC;
		else if(!strcmp(reflector, "TB")) RE = RBT;
		else if(!strcmp(reflector, "TC")) RE = RCT;
		else if(!strcmp(reflector, "BETA")) RE = BETA;
		else if(!strcmp(reflector, "GAMMA")) RE = GAMMA;
		else return usage(argv[0]);
	} else {
		fprintf(stderr, "Setting reflector to 'B'...\n");
		RE = RB;
	}

	ENIGMA = el_enigma_new(KB, PB, RE, ROT[0], ROT[1], ROT[2], &status);

	if(key != NULL) {
		fprintf(stderr, "Setting key...\n");
		ENIGMA->set_key(ENIGMA, key, &status);
	} else {
		fprintf(stderr, "Setting key to 'AAA'...\n");
		ENIGMA->set_key(ENIGMA, "AAA", &status);
	}
	if(status != EL_OK) return usage(argv[0]);

	if(rings != NULL) {
		fprintf(stderr, "Setting rings...\n");
		ENIGMA->set_rings(ENIGMA, rings[0], rings[1], rings[2], &status);
	} else {
		fprintf(stderr, "Setting rings to 'AAA'...\n");
		ENIGMA->set_rings(ENIGMA, 'A', 'A', 'A', &status);
	}
	if(status != EL_OK) return usage(argv[0]);

	if(message != NULL) {
		ciphertext = ENIGMA->encipher_string(ENIGMA, message, &status);
		if(status != EL_OK) return usage(argv[0]);
		printf("%s\n", ciphertext);
		free(ciphertext);
	} else {
		printf("Type now. Press ESC to exit.\n\n");
		while((input = KB->get_key(KB, &status)) != 27) {
			if(status != EL_OK) goto end;
			cipherchar = ENIGMA->encipher(ENIGMA, input, &status);

			if(count && !(count % 5)) printf(" ");
			if(count && !(count % 35)) printf("\n");
			count++;

			printf("%c", cipherchar);
			fflush(stdout);
		}
	}

end:
	destroy_objects();
	return EXIT_SUCCESS;
}
