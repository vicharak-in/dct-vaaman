#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rah.h>

#define APP_ID 1

int reverse_string(unsigned char *in_data, unsigned char *out_data, int len) {
	for (int i = 0; i < len; i++) {
		out_data[len - i - 1] = in_data[i];
	}

	return 0;
}

int main(int argc, char *argv[]) {
	float a;

	if (argc <= 2) {
		printf("More number of arguments are required\n");
		return 1;
	}

	printf("Numbers: %d\n", argc);

	rah_clear_buffer(APP_ID);

#if 1
	const int len = (argc - 1) * 6;
#else
	const int len = argc - 1;
#endif

	unsigned char *values = malloc(len);
	memset(values, 0, len);

#if 1
	for (int i = 0; i < argc - 1; i++) {
		unsigned char val[6] = {0};
		// int l = i < 8 ? 8 : i < 16 ? 16 : i < 24 ? 24 : 32;
		// float fval = strtof(argv[l - i + l - 8], NULL);
		float fval = strtof(argv[i + 1], NULL);
		reverse_string((void *) &fval, values + (i * 6) + 2, 4);
		
		for (int j = 0; j < 6; j++)
			printf("%02x ", values[(i * 6) + j]);
	}

	printf("\n");
#else

	for (int i = 0; i < len; i++) {
		values[i] = (unsigned char) strtoul(argv[i + 1], NULL, 16);
	}
#endif

	rah_write(APP_ID, values, len);
	printf("Data Sent\n");

	unsigned char *output = malloc(len);
	rah_read(APP_ID, output, len);

	for (int i = 0; i < len; i += 6) {
		unsigned char data[4] = {0};
		reverse_string(output + i + 2, data, 4);
		memcpy(&a, data, 4);
		printf("%f\n", a);
	}

	for (int j = 0; j < len; j++)
		printf("%02x ", output[j]);
	printf("\n");

	free(output);
	free(values);

	return 0;
}
