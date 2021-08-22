/*************************************************************************
	> File Name : CLog.cpp
	> Author : FengChao
	> EMail : smile.fengchao@gmail.com 
	> Created Time : Fri 17 Jul 2020 05:36:24 PM CST
 ************************************************************************/

#include "CLog.h"

#define DEBUG_LOG   //lPrint()打印开关

// 打印信息级别
char *PutOutLevel[] =
{
    (char *)"[FATEL ]",
    (char *)"[ALERT ]",
    (char *)"[ERROR ]",
    (char *)"[ WARN ]",
    (char *)"[NOTICE]",
    (char *)"[ INFO ]",
    (char *)"[DEBUG ]"
};

// 打印信息颜色
char *PutOutColor[] =
{
    (char *)LIGHT_RED,  //FATEL
    (char *)PURPLE,     // ALERT
    (char *)RED,        // ERROR
    (char *)YELLOW,     // WARM
    (char *)BLUE,       // NOTICE
    (char *)CYAN,       // INFO
    (char *)GREEN       // DEBUG
};
/*****************************************************************************
 Prototype    : GetTimeString
 Description  : 获取格式化收后的时间字符串
 Input        : char  *pTimeBuf
                int s32Len
 Output       : None
 Return Value : char

  History        : 
  1.Date         : 20181011
    Author       : FengChao
    Modification : Created function

*****************************************************************************/
char *GetTimeString(char  *pTimeBuf, int s32Len)
{
    char timeBuf[32] = {0};
    time_t nowTime = 0;
    struct tm *pTm = NULL;

    if (s32Len < (int)sizeof(timeBuf))
    {
        return NULL;
    }

    nowTime = time((time_t *)NULL);
    pTm = localtime(&nowTime);
    if (strftime(timeBuf, sizeof(timeBuf), "%D %H:%M:%S", pTm) == 0)
    {
        return NULL;
    }

    if(pTimeBuf != NULL)
    {
        memcpy(pTimeBuf, timeBuf, sizeof(timeBuf));
    }
    else
    {
        return NULL;
    }

    return pTimeBuf;
}


/*****************************************************************************
 Prototype    : 
 Description  : 
 Input        : None
 Output       : None
 Return Value : int

  History        :
  1.Date         : 20181011
    Author       : FengChao
    Modification : Created function

*****************************************************************************/
void LogPrint(const int level,const char *proName,const char *func,const int line,const char *format, ...)
{
    #ifdef DEBUG_LOG
    va_list args;

    if ((level > DEBUG) || (level < FATAL))
    {
        printf("Gole Printf Input Err Level %d\n", level);
        return;
    }

    char timeBuf[32] = {0};

    va_start(args, format);
    printf("%s%s:%-8s: %s %s [%d]: ",PutOutColor[level],\
            GetTimeString(timeBuf,sizeof(timeBuf)),\
            PutOutLevel[level],\
            proName,func,line);

    vprintf(format, args);
    printf("\r\n" NONE);

    va_end(args);
    #endif
}

void BufPrint(const char *func,const int line, const char *pData, int iDataLen)
{
    int i = 0;

    printf("[%s:%d] :\n", func, line);
    printf("*****************************\n" GREEN);
    for(i = 0; i < iDataLen; i++)
    {
        printf("%02x ", pData[i] & 0xFF);
        if(!((i+1) % 10)) printf("\n");
    }
    
    printf("\nsize = %d\n", iDataLen);
    printf(NONE "*****************************\r\n");
}


