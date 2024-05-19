# LZ77-Compressor-and-Decompressor

This is a proof of concept project using a subset of my custom replacement for the C standard library and the following Win32 API includes:


    windef.h
    winbase.h
    wingdi.h
    wincon.h
    shellapi.h


Also including pre compiled binaries if you don't want to compile this yourself.


Usage is very simple:


    lz77_compressor <in_file> <out_file>
    lz77_decompressor <in_file> <out_file>


If you don't get any output the operation succeeded! 


You can build this with gcc in Msys2/mingw32 using some - or all - of these flags :D


    -nostdlib -Oz -s -fno-builtin -fno-math-errno -ffast-math -fno-exceptions -fsingle-precision-constant -mfpmath=387 -mfancy-math-387 -fmerge-all-constants -fno-unroll-loops -fno-ident -fno-unwind-tables -fno-asynchronous-unwind-tables -Wall -DNDEBUG -m32 -fno-stack-protector -fomit-frame-pointer -fdata-sections -ffunction-sections -fvisibility=hidden -falign-functions=1 -falign-jumps=1 -falign-loops=1 -mpreferred-stack-boundary=2 -Bsymbolic -Wl,--build-id=none -Wl,--gc-sections -Wl,-nmagic -Wl,--no-insert-timestamp -lkernel32 -lshell32 "-Wl,-e,__start"


The compressed data has a header in this format:


    LZ<size>!<compressed data>


where size represents the size of the uncompressed data in hex.


This code is a part of a larger project and I aim to use it to store a compressed array with 16 bit byte data directly in my executable and decompress it when needed. Compression rate is pretty good for this kind of data. I compressed a file slightly shy of 40kb down to around 4kb.


I may or may not optimize this in various ways in the future.


Cheerios!
