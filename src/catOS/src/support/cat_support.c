/**
 * @file cat_support.c
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

#include "cat_support.h"
#include "cat_sys_print.h"

#include "cat_error.h"

/****************** macros *******************/
#define CAT_SUPPORT_INT_LONG 10


/*************************************/

/****************** funcs *******************/
int32_t cat_strcmp(const uint8_t *str1, const uint8_t *str2)
{
    int32_t ret = -2;

    if((str1 == NULL) || (str2 == NULL))
    {
        CAT_SYS_PRINTF("[cat_strcmp] at least one arg is NULL!\r\n");
    }
    else
    {
        while((*str1 == *str2) && (*str1 != '\0'))
        {
            str1++;
            str2++;
        }
        
        if((*str1 == '\0') && (*str2 == '\0'))
        {
            ret = 0;
        }
        else
        {
            ret = -1;
        }
    }

    return ret;
}

int32_t *cat_strcpy(uint8_t *dest, const uint8_t *src, uint32_t dest_len)
{
    //uint8_t *p = src;
    int32_t ret = 0;
    CAT_ASSERT(NULL != dest);
    CAT_ASSERT(NULL != src);
    uint32_t i = 0;
    while(
        (src[i] != '\0') &&
        (i < dest_len)
    )
    {
        dest[i] = src[i];
    }

    return 0;
}

int32_t cat_atoi(int32_t *dest, const uint8_t *src)
{
    CAT_ASSERT(dest);
    CAT_ASSERT(src);

    int32_t ret = 0;
    int32_t temp = 0;
    uint8_t sign = src[0];

    if(('-' == *src) || ('+' == *src))
    {
        src++;
    }
    while('\0' != *src)
    {
        if((*src < '0') || (*src > '9'))
        {
            ret = -1;
            break;
        }
        temp = temp * 10 + (*src - '0');
        src++;
    }
    if('-' == sign)
    {
        temp = -temp;
    }
		
		*dest = temp;

    return ret;
}

int32_t cat_itoa(uint8_t *dest, int32_t src)
{
	int32_t ret = 0;

#if 0
    uint32_t tmp = 0;
    /* uint32_t max val = 4294967295 */
    uint8_t buf[CAT_SUPPORT_INT_LONG] = {0};
    uint8_t i = 0;

    if(num < 0)
    {
        *(dest++) = '-';
        num = -num;
    }

    /* 计算每一位的数字大小 */
    do
    {
        buf[i++] = num % 10;
        num = num / 10;

    }while((0 != num) && (i < CAT_SUPPORT_INT_LONG));

    /* 记录长度 */
    ret = i;

    while(i > 0)
    {
        *(dest++) = buf[--i] + '0';
    }
    *(dest) = '\0';
#endif //#if 0

	return ret;
}

