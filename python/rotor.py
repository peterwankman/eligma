class Rotor:

    def __init__(self, wiring, notches):
        self.left = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        self.right = wiring
        self.notches = notches

    def forward(self, signal):
        letter = self.right[signal]
        signal = self.left.find(letter)
        return signal

    def backward(self, signal):
        letter = self.left[signal]
        signal = self.right.find(letter)
        return signal

    def show(self):
        print(self.left)
        print(self.right)
        print("")

    def rotate(self, n=1, forward=True):
        for i in range(n):
            if forward:
                self.left = self.left[1:] + self.left[0]
                self.right = self.right[1:] + self.right[0]
            else:
                self.left = self.left[25] + self.left[:25]
                self.right = self.right[25] + self.right[:25]

    def rotate_to_letter(self, letter):
        n = "ABCDEFGHIJKLMNOPQRSTUVWXYZ".find(letter)
        self.rotate(n)

    def set_ring(self, n):
        # rotate the rotor backwards
        self.rotate(n-1, forward=False)

        # adjust the turnover notch in relationship to the wiring
        for idx, notch in enumerate(self.notches):
            n_notch = "ABCDEFGHIJKLMNOPQRSTUVWXYZ".find(notch)
            self.notches[idx] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[(n_notch - n + 1) % 26]
