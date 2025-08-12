#ifndef __BIN_UTILS_HPP__
#define __BIN_UTILS_HPP__

#define INCBIN(NAME, FILENAME) \
    extern const unsigned char NAME[]; \
    extern const unsigned char NAME##_end[];   \
    extern const unsigned int NAME##_size;     \
    __asm__(".section .rodata\n" \
            ".global " #NAME "\n" \
            ".balign 16\n" \
            #NAME ":\n" \
            ".incbin \"" FILENAME "\"\n" \
            ".global " #NAME "_end\n" \
            #NAME "_end:\n" \
            ".global " #NAME "_size\n" \
            #NAME "_size:\n" \
            ".int " #NAME "_end - " #NAME "\n");

#define INCSTR(NAME, FILENAME) \
    extern const unsigned char NAME[]; \
    extern const unsigned char NAME##_end[];   \
    extern const unsigned int NAME##_size;     \
    __asm__(".section .rodata\n" \
            ".global " #NAME "\n" \
            ".balign 16\n" \
            #NAME ":\n" \
            ".incbin \"" FILENAME "\"\n" \
            ".global " #NAME "_end\n" \
            #NAME "_end:\n" \
            ".byte 0\n" \
            ".global " #NAME "_size\n" \
            #NAME "_size:\n" \
            ".int " #NAME "_end - " #NAME "\n");

#endif