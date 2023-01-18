from enigma import Enigma
from keyboard import Keyboard
from plugboard import Plugboard
from rotor import Rotor
from reflector import Reflector

I = Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", ["Q"])
II = Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", ["E"])
III = Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", ["V"])
IV = Rotor("ESOVPZJAYQUIRHXLNFTGKDCMWB", ["J"])
V = Rotor("VZBRGITYUPSDNHLXAWMJQOFECK", ["Z"])
VI = Rotor("JPGVOUMFYQBENHZRDKASXLICTW", ["M", "Z"])
VII = Rotor("NZJHGRCXMYSWBOUFAIVLPEKQDT", ["M", "Z"])
VIII = Rotor("FKQHTLXOCBJSPDZRAMEWNIUYGV", ["M", "Z"])

A = Reflector("EJMZALYXVBWFCRQUONTSPIKHGD")
B = Reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT")
C = Reflector("FVPJIAOYEDRZXWGCTKUQSBNMHL")

KB = Keyboard()
PB = Plugboard(["AB", "CD", "EF"])

# define enigma machine
#ENIGMA = Enigma(B, IV, II, I, PB, KB)
ENIGMA = Enigma(B, III, II, I, PB, KB)

# set the rings
ENIGMA.set_rings((1,1,1))

# set message key
ENIGMA.set_key("AAA")

# encipher a message
message = "TESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTINGTESTING"
cipher_text = ""
letter_count = 0
for letter in message:
    cipher_text = cipher_text + ENIGMA.encipher(letter)
    letter_count = letter_count + 1;
    if letter_count % 5 == 0:
        cipher_text = cipher_text + " "
print(cipher_text)

