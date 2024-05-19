#include "include\lz77.h"

void lz77_decompress(const char* input_filename, const char* output_filename) {
    FILE* input_file = __fopen(input_filename, "rb");
    if (!input_file) {
        __printf("Error opening file for reading: %s\n", input_filename);
        return;
    }

    char header[3];
    __fread(header, 1, 2, input_file);
    header[2] = '\0';

    if (__strcmp(header, "LZ") != 0) {
        __printf("Error: Invalid header\n");
        __fclose(input_file);
        return;
    }

    char hex_size[17];
    size_t hex_index = 0;
    int ch;
    while ((ch = __fgetc(input_file)) != '!') {
        if (hex_index >= 16) {
            __printf("Error: Mismatch in header size\n");
            __fclose(input_file);
            return;
        }
        hex_size[hex_index++] = ch;
    }
    hex_size[hex_index] = '\0';

    size_t decompressed_size = (size_t)__strtol(hex_size, __NULL, 16);

    FILE* output_file = __fopen(output_filename, "wb");
    if (!output_file) {
        __printf("Error opening file for writing: %s\n", output_filename);
        __fclose(input_file);
        return;
    }

    size_t token_count;
    __fread(&token_count, sizeof(size_t), 1, input_file);

    LZ77Token* tokens = (LZ77Token*)__malloc(token_count * sizeof(LZ77Token));
    if (!tokens) {
        __printf("Error allocating memory for tokens\n");
        __fclose(input_file);
        __fclose(output_file);
        return;
    }

    size_t elements_read = __fread(tokens, sizeof(LZ77Token), token_count, input_file);
    if (elements_read != token_count) {
        __printf("Error reading tokens from input file\n");
        __free(tokens);
        __fclose(input_file);
        __fclose(output_file);
        return;
    }

    unsigned char* decompressed_data = (unsigned char*)__malloc(decompressed_size);
    if (!decompressed_data) {
        __printf("Error allocating memory for decompressed data\n");
        __free(tokens);
        __fclose(input_file);
        __fclose(output_file);
        return;
    }

    size_t output_index = 0;

    for (size_t i = 0; i < token_count; i++) {
        if (tokens[i].length == 0) {
            if (output_index >= decompressed_size) {
                __printf("Error: Output index out of bounds\n");
                __free(tokens);
                __free(decompressed_data);
                __fclose(input_file);
                __fclose(output_file);
                return;
            }
            decompressed_data[output_index++] = tokens[i].literal;
        } else {
            if (tokens[i].offset == 0 || tokens[i].offset > output_index) {
                __printf("Error: Invalid offset at token %zu, offset %hu, output_index %zu\n", i, tokens[i].offset, output_index);
                __free(tokens);
                __free(decompressed_data);
                __fclose(input_file);
                __fclose(output_file);
                return;
            }
            for (size_t j = 0; j < tokens[i].length; j++) {
                if (output_index >= decompressed_size) {
                    __printf("Error: Output index out of bounds\n");
                    __free(tokens);
                    __free(decompressed_data);
                    __fclose(input_file);
                    __fclose(output_file);
                    return;
                }
                decompressed_data[output_index] = decompressed_data[output_index - tokens[i].offset];
                output_index++;
            }
            if (output_index < decompressed_size) {
                decompressed_data[output_index++] = tokens[i].literal;
            }
        }
    }

    __fwrite(decompressed_data, 1, decompressed_size, output_file);

    __free(tokens);
    __free(decompressed_data);
    __fclose(input_file);
    __fclose(output_file);
}

int main_entry(int argc, char* argv[]) {
    if (argc != 3) {
        __printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        __exit(-1);
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    lz77_decompress(input_file, output_file);

    return 0;
}

void _start() {
    LPWSTR cmdLine = GetCommandLineW();
    int argc;
    LPWSTR* argvW = CommandLineToArgvW(cmdLine, &argc);

    if (argvW == __NULL) {
        __exit(-1);
    }

    char** argv = (char**) __malloc(argc * sizeof(char*));
    for (int i = 0; i < argc; i++) {
        size_t len = __wcslen(argvW[i]) + 1;
        argv[i] = (char*) __malloc(len);
        __wcstombs(argv[i], argvW[i], len);
    }

    main_entry(argc, argv);

    for (int i = 0; i < argc; i++) {
        __free(argv[i]);
    }
    __free(argv);
    LocalFree(argvW);

    __exit(0);
}
