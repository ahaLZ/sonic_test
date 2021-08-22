/*************************************************************************
	> File Name : CLog.h
	> Author : FengChao
	> EMail : smile.fengchao@gmail.com 
	> Created Time : Fri 17 Jul 2020 09:33:22 AM CST
 ************************************************************************/
 #ifndef CLOG_H
 #define CLOG_H
 
#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <time.h>


//颜色宏定义
#define NONE         "\033[m"
#define RED          "\033[0;32;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"

#define PRO_NAME "JUDGE"
#define _FUN_  __FUNCTION__

typedef enum tagLogLevel
{
    FATAL = 0,
    ALERT,
    ERROR,
    WARN,
    NOTICE,
    INFO,
    DEBUG
} CYBERER_LOG_LEVEL_E;


/*
 * priorities/facilities are encoded into a single 32-bit quantity, where the
 * bottom 3 bits are the priority (0-7) and the top 28 bits are the facility
 * (0-big number).  Both the priorities and the facilities map roughly
 * one-to-one to strings in the syslogd(8) source code.  This mapping is
 * included in this file.
 *
 * priorities (these are ordered)
 *
 * #define  LOG_EMERG   0   //  system is unusable  /
 * #define  LOG_ALERT   1   //  action must be taken immediately  /
 * #define  LOG_CRIT    2   //  critical conditions  /
 * #define  LOG_ERR     3   //  error conditions  /
 * #define  LOG_WARNING 4   //  warning conditions  /
 * #define  LOG_NOTICE  5   //  normal but significant condition  /
 * #define  LOG_INFO    6   //  informational  /
 * #define  LOG_DEBUG   7   //  debug-level messages  /
*/
#define SLOG_EMERG   LOG_EMERG      /* system is unusable */
#define SLOG_ALERT   LOG_ALERT      /* action must be taken immediately */
#define SLOG_CRIT    LOG_CRIT       /* critical conditions */
#define SLOG_ERR     LOG_ERR        /* error conditions */
#define SLOG_WARNING LOG_WARNING    /* warning conditions */
#define SLOG_NOTICE  LOG_NOTICE     /* normal but significant condition */
#define SLOG_INFO    LOG_INFO       /* informational */
#define SLOG_DEBUG   LOG_DEBUG      /* debug-level messages */
#define SysLog(_level, fmt, ...) \
        do{openlog("[JUDGE]", LOG_NDELAY | LOG_CONS | LOG_PID | LOG_PERROR , LOG_LOCAL5); \
        syslog(_level, "[%s:%d] " fmt "", __FILE__, __LINE__, ##__VA_ARGS__); \
        closelog();}while(0)

extern void LogPrint(const int level,const char *proName,const char *func,const int line,const char *format, ...);
extern void BufPrint(const char *func,const int line, const char *pData, int iDataLen);

#define cPrint(fmt, ...) //do{fprintf(stderr, PRO_NAME "[%s:%d] " fmt "\r\n", __FILE__, __LINE__, ##__VA_ARGS__);}while(0)
#define dPrint(fmt, ...) //do{fprintf(stderr, YELLOW PRO_NAME "[%s:%d] " fmt "\r\n" NONE, __FILE__, __LINE__, ##__VA_ARGS__);}while(0)
#define lPrint(DLevel, format, ...) //LogPrint(DLevel, PRO_NAME, __FILE__, __LINE__,format, ##__VA_ARGS__)
#define bufPrint(pData, iDataLen) BufPrint(__FILE__, __LINE__, (const char *)pData, (int)iDataLen)

#endif

