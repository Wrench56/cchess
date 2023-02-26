#include <stdlib.h>

char* xor_cipher(char* data, char* key, int dataLen, int keyLen) {
	char* output = (char*)malloc(sizeof(char) * dataLen + 1);
	output[sizeof(char) * dataLen] = '\0';

	for (int i = 0; i < dataLen; ++i) {
		output[i] = data[i] ^ key[i % keyLen];
	}

	return output;
}
