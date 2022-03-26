/**
 * @file cat_sys_print.h
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-06-09
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-06-09 1.0    amoigus             内容
 */

#ifndef CAT_SYS_PRINT_H
#define CAT_SYS_PRINT_H

#include "catos_config.h"
#include "catos_defs.h"

#if (CATOS_ENABLE_SYS_PRINT == 1)

#if defined(__CC_ARM)
    #define CAT_SYS_PRINTF(_fmt, ...) \
        do{printf(_fmt, ##__VA_ARGS__);}while(0)

    #define CAT_SYS_SCANF(_fmt, ...) \
        do{scanf(_fmt, ##__VA_ARGS__);}while(0)

    #define CAT_SYS_PUTCHAR(_ch) \
        do{putchar(_ch);}while(0)

    #define CAT_SYS_GETCHAR() \
        getchar()

#elif defined(__GNUC__) //#if defined(__CC_ARM)

    #if 0
        int32_t cat_sys_scanf(uint8_t fmt, ...);
        int32_t cat_sys_printf(uint8_t fmt, ...);
        uint8_t cat_sys_getchar(void);
        int32_t cat_sys_putchar(uint8_t ch);
    #endif //#if 0


    #if 1
        #define CAT_SYS_PRINTF(_fmt, ...) \
            cat_printf(_fmt, ##__VA_ARGS__)
    #else
        #define CAT_SYS_PRINTF(_fmt, ...) \
            do{printf(_fmt, ##__VA_ARGS__);fflush(stdout);}while(0)
    #endif

    #if 1
        #define DEBUG_PRINTF(_fmt, ...) \
            cat_printf(_fmt, ##__VA_ARGS__)
    #endif

    #define CAT_SYS_SCANF(_fmt, ...) \
        cat_scanf(_fmt, ##__VA_ARGS__)

    #define CAT_SYS_PUTCHAR(_ch) \
        cat_putchar(_ch)

    #define CAT_SYS_GETCHAR() \
        cat_getchar()



#endif //#if defined(__CC_ARM)



/***************************************************/


/***************************************************/

#if( DEBUG_PRINT_ENABLE == 1)
    #define DEBUG_PRINT(_fmt, ...)
#else //#if( DEBUG_PRINT_ENABLE == 1)
    #define DEBUG_PRINT(_fmt, ...)
#endif //#if( DEBUG_PRINT_ENABLE == 1)



#endif // (CAT_SYS_PRINT_ENABLE == 1)

#endif
