# monocbcdecoder
Tries to crack a mono-alphabetic cipher in CBC mode.

The roots of this project lie in an exercise in cryptography:
Decipher a cryptogram encrypted with a mono-alphabetic cipher in CBC mode.

This program solves this exercise by trying several substitutions and comparing the outcome with a provided dictionary of frequent words. Suitable dictionaries are the files provided at https://github.com/first20hours/google-10000-english.
You might try to decipher the files in the examples dictionary.

Obviously this program contains room for improvements and experiments. For example one could try to add weights to the words in the dictionary or add some randomness to the program.