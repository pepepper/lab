#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

const int width = 800;
const int height = 480;

// 1 loop = 2 draws
// 30 loop = 60 draws = 1 sec (60 fps)
const int nloop = 30 * 3;

struct timespec diff(struct timespec start, struct timespec end);

int main(void) {
	FILE *f;
	uint16_t buf1[width * height];  //RGB565
	uint16_t buf2[width * height];
	struct timespec ts1, ts2, d;
	int i, j;
	float elapsed;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			buf1[i * width + j] = (i << 8) + j;
			buf2[i * width + j] = ~((i << 8) + j);
		}
	}

	f = fopen("/dev/fb0", "w");
	if (f == NULL) {
		printf("Could not open /dev/fb0\n");
		return 1;
	}

	clock_gettime(CLOCK_MONOTONIC, &ts1);
	for (j = 0; j < nloop; j++) {
		fwrite(buf1, sizeof(buf1), 1, f);
		fseek(f, 0, SEEK_SET);
		fwrite(buf2, sizeof(buf2), 1, f);
		fseek(f, 0, SEEK_SET);
	}
	clock_gettime(CLOCK_MONOTONIC, &ts2);

	d = diff(ts1, ts2);
	elapsed = (float)d.tv_sec + ((float)d.tv_nsec * 1e-9);
	printf("Frames: %d\n", nloop * 2);
	printf("Elapsed: %f\n", elapsed);
	printf("Frames per second: %f\n", (float)(nloop * 2) / elapsed);
}

// https://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
struct timespec diff(struct timespec start, struct timespec end) {
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
