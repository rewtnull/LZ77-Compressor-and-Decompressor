#include "include\lz77.h"

LZ77Token* lz77_compress(unsigned char* input_data, size_t input_size, size_t* token_count) {
    size_t max_tokens = input_size * 2;
    LZ77Token* tokens = (LZ77Token*)__malloc(max_tokens * sizeof(LZ77Token));
    if (!tokens) {
        __printf("Error allocating memory for tokens\n");
        __exit(-1);
    }

    size_t window_size = 16384;
    size_t buffer_size = 128;
    *token_count = 0;

    size_t input_index = 0;
    while (input_index < input_size) {
        size_t max_match_length = 0;
        size_t max_match_offset = 0;
        unsigned char next_char = 0;

        for (size_t j = (input_index > window_size) ? input_index - window_size : 0; j < input_index; ++j) {
            size_t match_length = 0;
            while (match_length < buffer_size && j + match_length < input_index && input_data[j + match_length] == input_data[input_index + match_length]) {
                match_length++;
            }

            if (match_length > max_match_length) {
                max_match_length = match_length;
                max_match_offset = input_index - j;
                if (input_index + match_length < input_size) {
                    next_char = input_data[input_index + match_length];
                } else {
                    next_char = 0;
                }
            }
        }

        if (max_match_length >= 3) {
            tokens[*token_count].offset = max_match_offset;
            tokens[*token_count].length = max_match_length;
            tokens[*token_count].literal = next_char;
            input_index += max_match_length + 1;
        } else {
            tokens[*token_count].offset = 0;
            tokens[*token_count].length = 0;
            tokens[*token_count].literal = input_data[input_index++];
        }

        (*token_count)++;
    }

    return tokens;
}

unsigned char* lz77_decompress(LZ77Token* tokens, size_t token_count, size_t original_size) {
    unsigned char* output_data = (unsigned char*)__malloc(original_size);
    if (!output_data) {
        __printf("Error allocating memory for output data\n");
        __exit(-1);
    }

    size_t output_index = 0;

    for (size_t i = 0; i < token_count; i++) {
        if (tokens[i].length == 0) {
            output_data[output_index++] = tokens[i].literal;
        } else {
            for (size_t j = 0; j < tokens[i].length; j++) {
                if (output_index - tokens[i].offset >= original_size) {
                    __printf("Error: Invalid offset during decompression\n");
                    __free(output_data);
                    return __NULL;
                }
                output_data[output_index] = output_data[output_index - tokens[i].offset];
                output_index++;
            }
            if (output_index < original_size) {
                output_data[output_index++] = tokens[i].literal;
            }
        }
    }

    return output_data;
}

int validate_compression(unsigned char* input_data, size_t input_size, LZ77Token* tokens, size_t token_count) {
    unsigned char* decompressed_data = lz77_decompress(tokens, token_count, input_size);
    if (!decompressed_data) {
        __printf("Error decompressing data");
        __exit(-1);
    }

    int result = (__memcmp(input_data, decompressed_data, input_size) == 0);
    __free(decompressed_data);

    return result;
}

void write_lz77(LZ77Token* tokens, size_t token_count, const char* output_name, size_t input_size) {
    FILE* output_file = __fopen(output_name, "wb");
    if (!output_file) {
        __printf("Error opening file for writing: %s\n", output_name);
        __exit(-1);
    }

    char header[21];
    int header_length = __sprintf(header, "LZ%lX!", input_size);
    __fwrite(header, 1, header_length, output_file);

    __fwrite(&token_count, sizeof(size_t), 1, output_file);
    __fwrite(tokens, sizeof(LZ77Token), token_count, output_file);
    __fclose(output_file);
}

void compress_main(const char* input_name, const char* output_name) {
    FILE* input_file = __fopen(input_name, "rb");
    if (!input_file) {
        __printf("Error opening file for reading: %s\n", input_name);
        __exit(-1);
    }

    __fseek(input_file, 0, __SEEK_END);
    long input_size = __ftell(input_file);
    __rewind(input_file);

    unsigned char* input_data = (unsigned char*)__malloc(input_size);
    if (!input_data) {
        __printf("Error allocating memory for input data\n");
        __fclose(input_file);
        __exit(-1);
    }

    size_t elements_read = __fread(input_data, 1, input_size, input_file);
    if (elements_read != input_size) {
        __printf("Error reading input file\n");
        __free(input_data);
        __fclose(input_file);
        __exit(-1);
    }

    size_t token_count;
    LZ77Token* tokens = lz77_compress(input_data, input_size, &token_count);
    if (!tokens) {
        __printf("Error allocating memory for tokens\n");
        __free(input_data);
        __fclose(input_file);
        __exit(-1);
    }

    if (!validate_compression(input_data, input_size, tokens, token_count)) {
        __printf("Validating compression failed\n");
        __free(tokens);
        __free(input_data);
        __fclose(input_file);
        __exit(-1);
    }

    write_lz77(tokens, token_count, output_name, input_size);

    __free(tokens);
    __free(input_data);
    __fclose(input_file);
}

int main_entry(int argc, char* argv[]) {
    if (argc != 3) {
        __printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        __exit(-1);
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    compress_main(input_file, output_file);
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
