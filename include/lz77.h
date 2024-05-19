#define WIN32_LEAN_AND_MEAN

#include <windef.h>
#include <winbase.h>
#include <wingdi.h> // wincon.h LF_FACESIZE macro
#include <wincon.h> // WriteConsoleA
#include <shellapi.h> // CommandLineToArgvW()

#define __NULL ((void *)0)

#define __MALLOC_BLOCK_SIZE (sizeof(__MallocBlock) - sizeof(char))
#define __offsetof(type, member) ((size_t) &((type *)0)->member)

typedef char* __va_list;
#define __va_start(ap, param) (ap = (char*)&param + sizeof(param))
#define __va_arg(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type)))
#define __va_end(ap) (ap = __NULL)

#define __SEEK_SET 0
#define __SEEK_CUR 1
#define __SEEK_END 2
#define __EOF (-1)

typedef unsigned int size_t;
typedef unsigned int uintptr_t;
typedef struct FILE FILE;
struct FILE {
    HANDLE hFile;
    int fd;
};
typedef struct __MallocBlock {
    size_t size;
    struct __MallocBlock* next;
    int free;
    char data[1];
} __MallocBlock;
__MallocBlock* freeList = __NULL;
typedef struct {
    unsigned short offset;
    unsigned char length;
    unsigned char literal;
} LZ77Token;

#include "lz77.c"
