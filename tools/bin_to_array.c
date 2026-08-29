#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *program)
{
    fprintf(stderr, "usage: %s input.bin output.c [array_name]\n", program);
}

int main(int argc, char **argv)
{
    const char *array_name = argc == 4 ? argv[3] : "program";
    FILE *input;
    FILE *output;
    uint8_t bytes[4];
    size_t word_count = 0;

    if (argc != 3 && argc != 4) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    input = fopen(argv[1], "rb");
    if (input == NULL) {
        fprintf(stderr, "cannot open %s: %s\n", argv[1], strerror(errno));
        return EXIT_FAILURE;
    }

    output = fopen(argv[2], "w");
    if (output == NULL) {
        fprintf(stderr, "cannot open %s: %s\n", argv[2], strerror(errno));
        fclose(input);
        return EXIT_FAILURE;
    }

    if (fprintf(output,
                "#include <stddef.h>\n"
                "#include <stdint.h>\n\n"
                "const uint32_t %s[] = {\n",
                array_name) < 0) {
        fprintf(stderr, "failed to write %s\n", argv[2]);
        fclose(input);
        fclose(output);
        return EXIT_FAILURE;
    }

    while (1) {
        size_t bytes_read = fread(bytes, 1, sizeof(bytes), input);

        if (bytes_read == 0) {
            if (ferror(input)) {
                fprintf(stderr, "failed to read %s\n", argv[1]);
                fclose(input);
                fclose(output);
                return EXIT_FAILURE;
            }
            break;
        }

        for (size_t i = bytes_read; i < sizeof(bytes); ++i) {
            bytes[i] = 0;
        }

        {
            uint32_t word = ((uint32_t)bytes[0])
                          | ((uint32_t)bytes[1] << 8)
                          | ((uint32_t)bytes[2] << 16)
                          | ((uint32_t)bytes[3] << 24);

            if (fprintf(output, "    0x%08" PRIx32 ",\n", word) < 0) {
                fprintf(stderr, "failed to write %s\n", argv[2]);
                fclose(input);
                fclose(output);
                return EXIT_FAILURE;
            }
        }

        ++word_count;
    }

    if (fprintf(output,
                "};\n\n"
                "const size_t %s_words =\n"
                "    sizeof(%s) / sizeof(%s[0]);\n",
                array_name, array_name, array_name) < 0) {
        fprintf(stderr, "failed to write %s\n", argv[2]);
        fclose(input);
        fclose(output);
        return EXIT_FAILURE;
    }

    if (fclose(input) != 0 || fclose(output) != 0) {
        fprintf(stderr, "failed to close a file\n");
        return EXIT_FAILURE;
    }

    fprintf(stderr, "wrote %zu words to %s\n", word_count, argv[2]);
    return EXIT_SUCCESS;
}
