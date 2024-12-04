#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define SAMPLE_RATE 8000 // 8 kHz
#define MAX_AMP 255
#define MIN_AMP 0

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr,"Usage: %s input.pcm output.pcm\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "rb");
    FILE *output = fopen(argv[2], "wb");
    if (!input || !output) {
        fprintf(stderr,"Error opening files.\n");
        return 1;
    }

    fseek(input, 0, SEEK_END);
    long data_length = ftell(input);
    rewind(input);

    
    fprintf(stderr,"[DEBUG] Attempting to allocate %ld bytes for data\n", data_length);
    uint8_t *data = (uint8_t *)malloc(data_length);
    if (!data) {
        fprintf(stderr, "[ERROR] Failed to allocate memory for data.\n");
        return 1;
    }
    fprintf(stderr,"[DEBUG] Successfully allocated memory for data\n");
    
    
    size_t read_size = fread(data, 1, data_length, input);
    if (read_size != data_length) {
        fprintf(stderr, "[ERROR] Only %ld bytes read, expected %ld\n", read_size, data_length);
        free(data);
        return 1;
    }
    fprintf(stderr,"[DEBUG] Successfully read %ld bytes from input file\n", read_size);
    

    float amp = 128.0;
    
    fprintf(stderr,"[DEBUG] Attempting to allocate %ld bytes for fd\n", data_length * 5 * sizeof(float));
    float *fd = malloc(data_length *5* sizeof(float));
    if (!fd) {
        fprintf(stderr, "[ERROR] Failed to allocate memory for fd.\n");
        return 1;
    }
    fprintf(stderr,"[DEBUG] Successfully allocated memory for fd\n");
    
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
                amp += 3.0;
            } else if (trit == 2) {
                amp -= 3.0;
            }
            fd[fd_index++] = amp;
        }
    }

    uint8_t final_output[data_length*5];
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
