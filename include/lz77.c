void __exit(int status) {
    ExitProcess((UINT)status);
}
double __pow(double base, int exp) {
    double result = 1.0;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}
size_t __strlen(const char* str) {
    return lstrlenA(str);
}
void __str_out(const char* message) {
    /* output string to console */

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteConsoleA(hConsole, message, __strlen(message), &written, __NULL);
    }
}
char* __strcpy(char* dest, const char* src) {

    return lstrcpyA(dest, src);
}
char* __strcat(char* dest, const char* src) {

    return lstrcatA(dest, src);
}
int __strcmp(const char* str1, const char* str2) {
    /* string.h strcmp replacement */

    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}
int __memcmp(const void* s1, const void* s2, size_t n) {

    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}
void __ulong2hex(unsigned long value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    char temp[17];
    int i = 0;

    if (value == 0) {
        temp[i++] = '0';
    }

    while (value > 0) {
        temp[i++] = hex_digits[value & 0xF];
        value >>= 4;
    }

    temp[i] = '\0';

    int j = 0;
    while (i > 0) {
        buffer[j++] = temp[--i];
    }
    buffer[j] = '\0';
}
void __ulong2str(unsigned long value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    int index = 0;

    if (value == 0) {
        buffer[index++] = '0';
    }

    int num_digits = 0;
    char reverse_hex[50];

    while (value != 0) {
        reverse_hex[num_digits++] = hex_digits[value & 0xF];
        value >>= 4;
    }

    for (int i = num_digits - 1; i >= 0; i--) {
        buffer[index++] = reverse_hex[i];
    }

    buffer[index] = '\0';
}
void __size_t2str(size_t value, char* buffer) {
    char temp[32];
    int i = 0, j = 0;

    do {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    temp[i] = '\0';

    for (j = 0, i--; i >= 0; i--, j++) {
        buffer[j] = temp[i];
    }
    buffer[j] = '\0';
}
void __ushort2str(unsigned short value, char* buffer) {
    char temp[6];
    int i = 0;
    do {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    } while (value > 0);
    temp[i] = '\0';
    // Reverse the string
    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[i] = '\0';
}
void __uchar2str(unsigned char value, char* buffer) {
    char temp[4];
    int i = 0;
    do {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    } while (value > 0);
    temp[i] = '\0';
    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[i] = '\0';
}
void __int2str(int value, char* buffer) {
    char temp[32];
    int i = 0, j = 0;
    int is_negative = (value < 0);
    if (is_negative) {
        value = -value;
    }
    do {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);
    if (is_negative) {
        temp[i++] = '-';
    }
    temp[i] = '\0';

    for (j = 0, i--; i >= 0; i--, j++) {
        buffer[j] = temp[i];
    }
    buffer[j] = '\0';
}
void __float2str(float value, char* buffer, int precision) {
    if (precision < 0) precision = 6;

    if (value == 0.0f) {
        __strcpy(buffer, "0.0");
        return;
    }

    int negative = (value < 0);
    if (negative) value = -value;

    int int_part = (int)value;
    float frac_part = value - int_part;

    char int_buffer[32];
    __int2str(int_part, int_buffer);

    char frac_buffer[32];
    frac_part *= __pow(10, precision);
    __int2str((int)frac_part, frac_buffer);

    if (negative) {
        __strcpy(buffer, "-");
        __strcat(buffer, int_buffer);
    } else {
        __strcpy(buffer, int_buffer);
    }
    __strcat(buffer, ".");
    __strcat(buffer, frac_buffer);
}
void __ptr2str(void* ptr, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    uintptr_t value = (uintptr_t)ptr;
    char* p = buffer;
    *p++ = '0';
    *p++ = 'x';
    for (int i = (sizeof(void*) * 2) - 1; i >= 0; i--) {
        *p++ = hex_digits[(value >> (i * 4)) & 0xF];
    }
    *p = '\0';
}
void __uint2str(size_t value, char* buffer) {
    char temp[32];
    int i = 0, j = 0;

    do {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    temp[i] = '\0';

    for (j = 0, i--; i >= 0; i--, j++) {
        buffer[j] = temp[i];
    }
    buffer[j] = '\0';
}
void __uint2hex(unsigned int value, char* buffer) {
    const char* hex_digits = "0123456789ABCDEF";
    int index = 0;

    if (value == 0) {
        buffer[index++] = '0';
    }

    int num_digits = 0;
    char reverse_hex[50];

    while (value != 0) {
        reverse_hex[num_digits++] = hex_digits[value & 0xF];
        value >>= 4;
    }

    for (int i = num_digits - 1; i >= 0; i--) {
        buffer[index++] = reverse_hex[i];
    }

    buffer[index] = '\0';
}
long __strtol(const char* str, char** endptr, int base) {
    if (base < 0 || base == 1 || base > 36) {
        if (endptr) {
            *endptr = (char*)str;
        }
        return 0;
    }

    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r' || *str == '\f' || *str == '\v') {
        str++;
    }

    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    long result = 0;
    while (*str) {
        int digit;
        if (*str >= '0' && *str <= '9') {
            digit = *str - '0';
        } else if (*str >= 'A' && *str <= 'Z') {
            digit = *str - 'A' + 10;
        } else if (*str >= 'a' && *str <= 'z') {
            digit = *str - 'a' + 10;
        } else {
            break;
        }

        if (digit >= base) {
            break;
        }

        result = result * base + digit;
        str++;
    }

    if (endptr) {
        *endptr = (char*)str;
    }

    return sign * result;
}
int __sprintf(char* buffer, const char* format, ...) {
    char num_buffer[64];
    const char* p = format;
    char* b = buffer;
    __va_list args;
    __va_start(args, format);

    while (*p) {
        if (*p == '%') {
            ++p;
            int width = 0;
            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                ++p;
            }
            if (*p == 'l') {
                ++p;
                if (*p == 'X' || *p == 'x') {
                    unsigned long value = __va_arg(args, unsigned long);
                    __ulong2str(value, num_buffer);
                    int num_len = __strlen(num_buffer);
                    __strcpy(b, num_buffer);
                    b += num_len;
                }
            } else {
                *b++ = '%';
                *b++ = *p;
            }
        } else {
            *b++ = *p;
        }
        ++p;
    }

    *b = '\0';
    __va_end(args);
    return __strlen(buffer);
}
int __printf(const char* format, ...) {
    char buffer[1024];
    char num_buffer[64];
    const char* p = format;
    char* b = buffer;
    __va_list args;
    __va_start(args, format);

    while (*p) {
        if (*p == '%') {
            ++p;
            int width = 0;
            int precision = 0;
            int zero_pad = 0;

            if (*p == '0') {
                zero_pad = 1;
                ++p;
            }

            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                ++p;
            }

            if (*p == '.') {
                ++p;
                while (*p >= '0' && *p <= '9') {
                    precision = precision * 10 + (*p - '0');
                    ++p;
                }
            }

            if (*p == 'l') {
                ++p;
                if (*p == 'X') {
                    unsigned long value = __va_arg(args, unsigned long);
                    __ulong2hex(value, num_buffer);
                    int len = __strlen(num_buffer);
                    int pad = (width > len) ? width - len : 0;
                    if (zero_pad) {
                        while (pad-- > 0) *b++ = '0';
                    }
                    __strcpy(b, num_buffer);
                    b += len;
                } else if (*p == 'u') {
                    unsigned long value = __va_arg(args, unsigned long);
                    __ulong2str(value, num_buffer);
                    int len = __strlen(num_buffer);
                    int pad = (width > len) ? width - len : 0;
                    if (zero_pad) {
                        while (pad-- > 0) *b++ = '0';
                    }
                    __strcpy(b, num_buffer);
                    b += len;
                }
            } else if (*p == 'z') {
                ++p;
                if (*p == 'u') {
                    size_t value = __va_arg(args, size_t);
                    __size_t2str(value, num_buffer);
                    int len = __strlen(num_buffer);
                    int pad = (width > len) ? width - len : 0;
                    if (zero_pad) {
                        while (pad-- > 0) *b++ = '0';
                    }
                    __strcpy(b, num_buffer);
                    b += len;
                }
            } else if (*p == 'h') {
                ++p;
                if (*p == 'u') {
                    unsigned short value = (unsigned short)__va_arg(args, unsigned int);
                    __ushort2str(value, num_buffer);
                    int len = __strlen(num_buffer);
                    int pad = (width > len) ? width - len : 0;
                    if (zero_pad) {
                        while (pad-- > 0) *b++ = '0';
                    }
                    __strcpy(b, num_buffer);
                    b += len;
                } else if (*p == 'h') {
                    ++p;
                    if (*p == 'u') {
                        unsigned char value = (unsigned char)__va_arg(args, unsigned int);
                        __uchar2str(value, num_buffer);
                        int len = __strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        __strcpy(b, num_buffer);
                        b += len;
                    }
                }
            } else {
                switch (*p) {
                    case 'i':
                    case 'd': {
                        int value = __va_arg(args, int);
                        __int2str(value, num_buffer);
                        int len = __strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        __strcpy(b, num_buffer);
                        b += len;
                        break;
                    }
                    case 'f': {
                        double value = __va_arg(args, double);
                        __float2str((float)value, num_buffer, precision);
                        int len = __strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        __strcpy(b, num_buffer);
                        b += len;
                        break;
                    }
                    case 's': {
                        const char* str = __va_arg(args, const char*);
                        int len = __strlen(str);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        __strcpy(b, str);
                        b += len;
                        break;
                    }
                    case 'c': {
                        int char_value = __va_arg(args, int);
                        *b++ = (char)char_value;
                        break;
                    }
                    case 'p': {
                        void* ptr = __va_arg(args, void*);
                        __ptr2str(ptr, num_buffer);
                        int len = __strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        __strcpy(b, num_buffer);
                        b += len;
                        break;
                    }
                    case 'u': {
                        unsigned int value = __va_arg(args, unsigned int);
                        __uint2str(value, num_buffer);
                        int len = __strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        __strcpy(b, num_buffer);
                        b += len;
                        break;
                    }
                    case 'X': {
                        unsigned int value = __va_arg(args, unsigned int);
                        __uint2hex(value, num_buffer);
                        int len = __strlen(num_buffer);
                        int pad = (width > len) ? width - len : 0;
                        if (zero_pad) {
                            while (pad-- > 0) *b++ = '0';
                        }
                        __strcpy(b, num_buffer);
                        b += len;
                        break;
                    }
                    default:
                        *b++ = '%'; *b++ = *p;
                        break;
                }
            }
        } else {
            *b++ = *p;
        }
        p++;
    }

    *b = '\0';
    __va_end(args);

    __str_out(buffer);
    return __strlen(buffer);
}
void* __malloc(size_t size) {
    __MallocBlock *block, *last;
    size_t totalSize = size + __MALLOC_BLOCK_SIZE;

    if (freeList == __NULL) {
        block = (__MallocBlock*)VirtualAlloc(__NULL, totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (!block) {
            __printf("VirtualAlloc failed\n");
            return __NULL;
        }
        block->size = size;
        block->next = __NULL;
        block->free = 0;
        freeList = block;
        return block->data;
    }

    last = freeList;
    block = freeList;
    while (block) {
        if (block->free && block->size >= size) {
            block->free = 0;
            return block->data;
        }
        last = block;
        block = block->next;
    }

    block = (__MallocBlock*)VirtualAlloc(__NULL, totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!block) {
        __printf("VirtualAlloc failed\n");
        return __NULL;
    }
    block->size = size;
    block->next = __NULL;
    block->free = 0;
    last->next = block;
    return block->data;
}
void __free(void* ptr) {
    if (!ptr) return;

    __MallocBlock* block = (__MallocBlock*)((char*)ptr - __offsetof(__MallocBlock, data));
    block->free = 1;

    __MallocBlock* current = freeList;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += __MALLOC_BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}
size_t __read(void* ptr, size_t count, FILE* stream) {

    DWORD bytesRead;
    if (!ReadFile((HANDLE)stream->hFile, ptr, (DWORD)count, &bytesRead, __NULL)) {
        return -1;
    }

    return bytesRead;
}
int __fgetc(FILE* stream) {
    char c;
    if (__read(&c, 1, stream->hFile) == 1) {
        return (unsigned char)c;
    } else {
        return __EOF;
    }
}
FILE* __fopen(const char* filename, const char* mode) {
    DWORD dwDesiredAccess = 0;
    DWORD dwCreationDisposition = OPEN_EXISTING;

    if (__strcmp(mode, "rb") == 0) {
        dwDesiredAccess = GENERIC_READ;
    } else if (__strcmp(mode, "wb") == 0) {
        dwDesiredAccess = GENERIC_WRITE;
        dwCreationDisposition = CREATE_ALWAYS;
    } else if (__strcmp(mode, "r+b") == 0) {
        dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
        dwCreationDisposition = OPEN_ALWAYS;
    }

    HANDLE hFile = CreateFile(filename, dwDesiredAccess, FILE_SHARE_READ, __NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, __NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return __NULL;
    }

    FILE* file = (FILE*)__malloc(sizeof(FILE));
    if (!file) {
        CloseHandle((HANDLE)hFile);
        return __NULL;
    }

    file->hFile = hFile;
    return file;
}
void __fclose(FILE* stream) {
    if (stream) {
        FlushFileBuffers((HANDLE)stream->hFile);
        CloseHandle((HANDLE)stream->hFile);
        __free(stream);
    }
}
size_t __fread(void* ptr, size_t size, size_t count, FILE* stream) {
    if (size == 0 || count == 0) return 0;

    size_t total_size = size * count;

    DWORD bytesRead;
    if (!ReadFile((HANDLE)stream->hFile, ptr, (DWORD)total_size, &bytesRead, __NULL)) {
        return -1;
    }

    size_t elements_read = bytesRead / size;
    return elements_read;
}
size_t __fwrite(const void* ptr, size_t size, size_t count, FILE* stream) {
    size_t total_size = size * count;
    DWORD bytesWritten;
    BOOL result = WriteFile((HANDLE)stream->hFile, ptr, (DWORD)total_size, &bytesWritten, __NULL);
    if (!result || bytesWritten < total_size) {
        __printf("Failed to write expected bytes: expected %zu, wrote %lu\n", total_size, bytesWritten);
        return bytesWritten / size;
    }
    return count;
}
int __fseek(FILE* stream, long offset, int whence) {
    DWORD dwMoveMethod;
    switch (whence) {
        case __SEEK_SET: dwMoveMethod = FILE_BEGIN; break;
        case __SEEK_CUR: dwMoveMethod = FILE_CURRENT; break;
        case __SEEK_END: dwMoveMethod = FILE_END; break;
        default: return -1;
    }
    if (SetFilePointer((HANDLE)stream->hFile, offset, __NULL, dwMoveMethod) == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR) {
        return -1;
    }
    return 0;
}
long __ftell(FILE* stream) {
    if (!stream || !stream->hFile) {
        __printf("Error: Invalid file stream or handle.\n");
        return -1;
    }

    DWORD dwPos = SetFilePointer((HANDLE)stream->hFile, 0, __NULL, FILE_CURRENT);
    if (dwPos == INVALID_SET_FILE_POINTER) {
        DWORD dwError = GetLastError();
        __printf("SetFilePointer failed with error %lu\n", dwError);
        return -1;
    }
    return (long)dwPos;
}
void __rewind(FILE* stream) {
    SetFilePointer((HANDLE)stream->hFile, 0, __NULL, FILE_BEGIN);
}
size_t __wcslen(const wchar_t* s) {
    const wchar_t* p = s;
    while (*p) {
        p++;
    }
    return p - s;
}
size_t __wcstombs(char* dest, const wchar_t* src, size_t max) {
    size_t count = 0;
    while (count < max && *src) {
        if (*src < 0x80) {
            *dest++ = (char)*src++;
        } else if (*src < 0x800) {
            if (count + 1 >= max) break;
            *dest++ = (char)(0xC0 | (*src >> 6));
            *dest++ = (char)(0x80 | (*src & 0x3F));
            src++;
            count++;
        } else {
            if (count + 2 >= max) break;
            *dest++ = (char)(0xE0 | (*src >> 12));
            *dest++ = (char)(0x80 | ((*src >> 6) & 0x3F));
            *dest++ = (char)(0x80 | (*src & 0x3F));
            src++;
            count += 2;
        }
        count++;
    }
    *dest = '\0';
    return count;
}
