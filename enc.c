#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SAMPLE_RATE 8000 // 8 kHz

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s input.pcm output.cat\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "rb");
    FILE *output = fopen(argv[2], "wb");
    if (!input || !output) {
        printf("Error opening files.\n");
        return 1;
    }

    fseek(input, 0, SEEK_END);
    long data_length = ftell(input);
    rewind(input);

    uint8_t *data = (uint8_t *)malloc(data_length);
    fread(data, 1, data_length, input);

    uint8_t header[10] = {'K', 'a', 't', 'z', 'e', '-', '8', '0', '0', '0'};
    fwrite(header, 1, sizeof(header), output);

    uint8_t diffs[data_length];
    int diff_index = 0;

    for (int i = 1; i < data_length; ++i) {
        int8_t diff = (int8_t)data[i] - (int8_t)data[i - 1];
        diffs[diff_index++] = diff;
    }

    uint8_t encoded[data_length / 5];
    int encoded_index = 0;

    int trit_accum = 0;
    int trit_bits = 0;

    for (int i = 0; i < diff_index; ++i) {
        int8_t diff = diffs[i];
        int trit = (diff == 0) ? 0 : ((diff > 0) ? 1 : 2);

        trit_accum = (trit_accum * 3) + trit;
        trit_bits += 1;

        if (trit_bits == 5) {
            encoded[encoded_index++] = (trit_accum & 0xFF);
            trit_accum = 0;
            trit_bits = 0;
        }
    }

    if (trit_bits > 0) { // Handle remaining trits
        encoded[encoded_index++] = (trit_accum & 0xFF);
    }

    fwrite(encoded, 1, encoded_index, output);

    free(data);
    fclose(input);
    fclose(output);

    return 0;
}
