#ifndef CAT_STDIO_H
#define CAT_STDIO_H

#include <stdarg.h>
#if 0
    #include <stdio.h>
    #include <stdarg.h>

    typedef unsigned char           uint8_t;
    typedef unsigned short int      uint16_t;
    typedef unsigned int            uint32_t;
    typedef int                     int32_t;
#else //#if 0
    #include "catos_config.h"
    #include "catos_defs.h"
#endif



uint8_t cat_getchar(void);
uint8_t cat_putchar(uint8_t ch);
int32_t cat_scanf(const uint8_t *format, ...);
int32_t cat_printf(const uint8_t *format, ...);


#endif