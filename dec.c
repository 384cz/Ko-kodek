#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define SAMPLE_RATE 8000 // 8 kHz
#define MAX_AMP 255
#define MIN_AMP 0

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s input.pcm output.pcm\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "rb");
    FILE *output = fopen(argv[2], "wb");
    if (!input || !output) {
        printf("Error opening files.\n");
        return 1;
    }

    fseek(input, 0, SEEK_END);
    long data_length = ftell(input)*5;
    rewind(input);

    uint8_t *data = (uint8_t *)malloc(data_length);
    fread(data, 1, data_length, input);

    float amp = 128.0;
    float *fd = malloc(data_length * 5 * sizeof(float));
	if (!fd) {
		fprintf(stderr, "Memory allocation failed\n");
		return 1;
	}

    int fd_index = 0;

    for (int i = 1; i < data_length; ++i) {
        uint8_t d = data[i];
        for (int j = 0; j < 5; ++j) {
            float weight = pow(3, 4 - j);
            uint8_t trit = ((uint8_t)(d / weight)) % 3;

            amp *= 0.9;
            if (trit == 1) {
                amp += 1.0;
            } else if (trit == 2) {
                amp -= 1.0;
            }
            fd[fd_index++] = amp;
        }
    }

    uint8_t final_output[data_length];
    int output_index = 0;

    amp = 128.0;
    for (int i = 4; i < fd_index; ++i) {
        amp += fd[i];
        amp = (128.0 * 0.1) + (amp * 0.9);
        amp = amp < MIN_AMP ? MIN_AMP : (amp > MAX_AMP ? MAX_AMP : amp);
        final_output[output_index++] = (uint8_t)amp;
    }

    fwrite(final_output, 1, output_index, output);

    free(data);
    fclose(input);
    fclose(output);

    return 0;
}
