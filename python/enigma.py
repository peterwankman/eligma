class Enigma:
    def __init__(self, re, r1, r2, r3, pb, kb):
        self.re = re
        self.r1 = r1
        self.r2 = r2
        self.r3 = r3
        self.pb = pb
        self.kb = kb

    def set_rings(self, rings):
        self.r1.set_ring(rings[0]);
        self.r2.set_ring(rings[1]);
        self.r3.set_ring(rings[2]);

    def set_key(self, key):
        self.r1.rotate_to_letter(key[0])
        self.r2.rotate_to_letter(key[1])
        self.r3.rotate_to_letter(key[2])


    def encipher(self, letter):
        # rotate the rotors

        self.carry2 = False
        for notch in self.r2.notches:
            if self.r2.left[0] == notch:
                self.carry2 = True

        self.carry3 = False
        for notch in self.r3.notches:
            if self.r3.left[0] == notch:
                self.carry3 = True

        if self.carry2 and self.carry3:
            self.r1.rotate()
            self.r2.rotate()
            self.r3.rotate()
        elif self.carry2:
            self.r1.rotate()
            self.r2.rotate()
            self.r3.rotate()
        elif self.carry3:
            self.r2.rotate()
            self.r3.rotate()
        else:
            self.r3.rotate()

        # pass signal through the machine
        signal = self.kb.forward(letter)
        signal = self.pb.forward(signal)
        signal = self.r3.forward(signal)
        signal = self.r2.forward(signal)
        signal = self.r1.forward(signal)
        signal = self.re.reflect(signal)
        signal = self.r1.backward(signal)
        signal = self.r2.backward(signal)
        signal = self.r3.backward(signal)
        signal = self.pb.backward(signal)
        letter = self.kb.backward(signal)
        return letter


