#include <iconv.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define DEFAULT(x) "\x1b[0m"x
#define RED(x) "\x1b[31m"x
#define GREEN(x) "\x1b[32m"x
#define YELLOW(x) "\x1b[33m"x
#define BLUE(x) "\x1b[34m"x
#define Magenta(x) "\x1b[35m"x
#define Cyan(x) "\x1b[36m"x

typedef enum
{ SMALLER, PRINTABLE, LARGER }char_type;

char_type isPrintAble(uint8_t check)
{
    if (check > 0x7e)
        return LARGER;
    else if (check < 0x20)
        return SMALLER;
    else
        return PRINTABLE;
}

void __hexdump(const uint8_t* ptr, const uint64_t size)
{
    uint8_t* format = (uint8_t*)malloc(0x80);
    uint8_t* string = (uint8_t*)malloc(0x80);

    uint8_t* Ptr = (uint8_t*)malloc(size);
    memcpy(Ptr, ptr, size);
    for (int i = 0; i < size; i+=8)
    {
        format[0] = '\0';
        string[0] = '\0';
        printf(Cyan("%p: "), &ptr[i]);
        for (int j = i; j < 8 + i; j++)
        {
            switch (isPrintAble(ptr[j]))
            {
            case PRINTABLE:
                strcat(format, BLUE("%02x"));
                strcat(string, BLUE(" %c"));
                break;
            case SMALLER:
                strcat(format, RED("%02x"));
                strcat(string, RED(" %c"));
                Ptr[j] = '.';
                break;
            case LARGER:
                strcat(format, YELLOW("%02x"));
                strcat(string, YELLOW(" %c"));
                Ptr[j] = '.';
                break;
            }
        }
        printf(format, ptr[i], ptr[i+1], ptr[i+2], ptr[i+3], ptr[i+4], ptr[i+5], ptr[i+6], ptr[i+7]);

        printf(DEFAULT(" | "));

        printf(string, Ptr[i], Ptr[i+1], Ptr[i+2], Ptr[i+3], Ptr[i+4], Ptr[i+5], Ptr[i+6], Ptr[i+7]);

        puts(DEFAULT(" | "));
    }
}

void hexdump(const uint8_t* ptr, const uint64_t size)
{
    uint8_t* Ptr = (uint8_t*)((long)ptr &~ 0xf);
    uint8_t Size = size | 8;
    printf(Magenta("ptr starts at %p\n"), ptr);
    __hexdump(Ptr, Size);
}

void main()
{
    iconv_t cd = iconv_open("ISO-2022-CN-EXT", "UTF-8");

    char input[0x10] = "AAAAA劄";
    char output[0x10] = {0};

    char *pinput = input;
    char *poutput = output;

    size_t input_size = strlen(input);
    size_t output_size = input_size;

    iconv(cd, &pinput, &input_size, &poutput, &output_size);

    printf("Remaining bytes (should be > 0): %zd\n", output_size);

    hexdump(poutput, 0x20);
}