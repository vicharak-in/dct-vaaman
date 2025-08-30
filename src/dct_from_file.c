#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include <sys/stat.h>

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

	if (argc < 3) {
		printf("%s <in_file> <out_file>\n", argv[0]);
		return 1;
	}

	const char *in_file = argv[1];
	const char *out_file = argv[2];

	struct stat st;
	stat(in_file, &st);
	const size_t len = 16384;
	// const size_t len = st.st_size;
	const int num_count = len / 4;

	int sending_len = (len * 3) / 2;
	printf("File length: %ld\n", len);
	printf("Sending bytes length: %d\n", sending_len);

	unsigned char *buf = malloc(len);

	int fd = open(in_file, O_RDONLY);
	int n_bytes = read(fd, buf, len);
	close(fd);

	unsigned char *values = malloc(sending_len);
	memset(values, 0, sending_len);

	float b;
	for (int i = 0; i < len; i += 4) {
		memcpy((void *) &b, buf + i, 4);
		reverse_string(buf + i, values + (i * 3 / 2) + 2, 4);
	}

	rah_clear_buffer(APP_ID);

	rah_write(APP_ID, values, sending_len);
	printf("Sent Input frame\n");

	fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);

	unsigned char *output = malloc(sending_len);
	rah_read(APP_ID, output, sending_len);

	for (int i = 0; i < sending_len; i += 6) {
		unsigned char data[4] = {0};
		reverse_string(output + i + 2, data, 4);
		write(fd, data, 4);
		memcpy(&b, data, 4);
		printf("%f\n", b);
	}

	close(fd);

	free(values);
	free(buf);

	return 0;
}
