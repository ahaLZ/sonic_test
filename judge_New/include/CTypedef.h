/*************************************************************************
	> File Name : CTypedef.h
	> Author : FengChao
	> EMail : smile.fengchao@gmail.com 
	> Created Time : Fri 17 Jul 2020 09:32:54 AM CST
 ************************************************************************/
#ifndef CTYPEDEF_H
#define CTYPEDEF_H

#include <map>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include "CLog.h"
#include "CSwitchCommon.h"

#ifndef MAX_NOS_NUM
#define NOS_MAX_NUM     8
#else
#define NOS_MAX_NUM     MAX_NOS_NUM
#endif

#define PORT_EV_BROAD_IFACE     (const char *)"nos"

#define CONF_FILE_PATH  "/etc/judge_conf.ini"

#define JUDGE_PUBLIC_KEY  (const char *)"p=lDOa9WKUKz!I9{G)uPX4@&CrV-(>tDg:kaSGzE"
#define JUDGE_PRIVATE_KEY (const char *)"q0]#)<fZP>iuwR*H5hz.}<AR^RSWunZ8H+oc8l1k"
#define BUF_CHAR_16 16

typedef socklen_t SOCKLEN_T;
typedef struct timeval TIMEVAL_T;
typedef struct sockaddr SOCKADDR_T;
typedef struct sockaddr_in SOCKADDR_IN_T;

typedef unsigned int uint;

typedef u_int8_t	UINT8;  /* 1 byte   */
typedef u_int16_t	UINT16; /* 2 byte   */
typedef u_int32_t	UINT32; /* 4 byte   */
typedef u_int64_t	UINT64; /* 8 bytes  */

typedef struct
{
    unsigned int uiNosIdx;
    TIMEVAL_T stRcvTime;
    unsigned int uiDataLen;
    unsigned int uiPort;
    int iCmd;
    unsigned char ucData[1024];
}NosConfMsg_ST;

typedef struct
{
    int iCmd;
    unsigned char ucData[1024];
}ConfJudgeMsg_ST;

typedef struct
{
    unsigned int uiNosIdx;
    unsigned char ucHost[16];
    int iState;  // 2 start
    bool bTransMaster;
    int iSHDState;  // 0 调度 1 N-1模攻击
}NosState_T;

typedef struct
{
    uint uiType;          // 1
    char szIp[32];
    TIMEVAL_T stRcvTime;
    uint uiCpuRate;
    uint uiMemRate;
    uint uiTcpEstablishNum;
    uint uiRunningProcessNum;
    uint uiCronNum;
    char szPasswdMd5[64];
    char szGroupMd5[64];
    uint uiRst;          // 1-Judge 0-Trans
}SysState_T;

typedef struct
{
    uint uiType;          // 2
    char szIp[32];
    TIMEVAL_T stRcvTime;
    char szProcessName[32];
    uint uiProcessState;
}ProcessState_T;

typedef struct
{
    uint uiType;          // 3
    char szIp[32];
    TIMEVAL_T stRcvTime;
    char szFileName[32];  // 被改动的文件名称
    char eventType[16];   // 说明
    char szFileMd5[64];   // 文件的md5值
}FileState_T;

typedef struct
{
    uint uiType;          // 4
    char szIp[32];
    TIMEVAL_T stRcvTime;
    char szDirPath[32];      // 监控的的文件路径
    char szFileName[32]; //增加或删除的文件名称
    uint uiFileNum;       // 当前文件夹下文件数量
}FileNum_T;

typedef struct
{
    uint uiType;          //5
    char szIp[32];
    TIMEVAL_T stRcvTime;
    char szLogFile[32];
    char szMsg[5][128];
}SysLog_T;

typedef enum MESSAGE_QUQUQ_TYPE_E{
        MQT_CONFIG_E = 1,
        MQT_SYS_STATE_E,
        MQT_PROCESS_STATE_E,
        MQT_FILE_NUM_E,
        MQT_FILE_STATE_E,
        MQT_SYS_LOG_E
}Message_QueueType_E;

typedef struct
{   
    int enState;
    int uiState;
    int uiRet;
}InitSucc_T;

typedef struct
{   
    int enState;
    int uiState;
    int uiRet;
}InitCFGChoice_T;

typedef struct __packed{
    uint uiCMD;
    uint uiOp; // 0 关闭 1 打开
    uint uiNos;
    uint uiRet; // 0 配置成功 1 NOS不在线
}BackdoorCFG_T;


typedef struct __packed_switch{
        uint enState;    
        uint ip;
        uint nos_id;
        uint port_id;
        char  name[16];
        uint uiRet;/*成功写数据库返回0，失败返回1*/
}Switch_Port_T; 

/*
 * 获取设备温度结构体
 * 裁决(client) --UDP-- A9 device monitor(server-10021)
*/ 
typedef struct
{   
    double fPower1Temp;
    double fPower2Temp;
    float fBLCTemp; // U3 后拼板左中 左CPU板子背面
    float fALCTemp; // U39 前拼版左中
    float fARCTemp; // U40 前拼版右中
    float fACCTemp; // U41 前拼版中中
}SwitchTemp_T;

/*
 * 发布主执行体结构体
 * add by tianlun song in June 8,2021.
*/ 
typedef struct
{   
    char sIP[BUF_CHAR_16];
}PubMasterNos_T;

/*
 * 执行体配置拟态状态，是否启动拟态
 * add by tianlun song in June 24,2021.
*/ 
typedef struct {
	UINT32 enState;
	UINT32 uiOp;   /*1-使能非拟态（透传） 0-关闭非拟态*/
    UINT32 uiNos;  /*指定切换为非拟态后的NOS 取值1-5*/
 	UINT32 uiRet;  /*指定的NOS如果在线返回0，调度裁决A7继续执行切换。  不在线返回1并结束切换任务，不再进行后续操作*/
} uMimicTransMsg_T;

typedef enum MIMIC_TRANS_TYPE_E{
        MTT_CLOSE_E  = 0, // 关闭非拟态
        MTT_OPEN_E = 1  // 使能非拟态（透传）
}Mimic_TransType_E;

typedef enum MIMIC_TRANS_RETURN_TYPE_E{
        MTRT_SUCC_E  = 0, // 继续
        MTRT_FAIL_E = 1  // 失败
}Mimic_TransReturnType_E;


#define INIT_CFG_CHOICE 8
#define INIT_CFG_CHOICE_RUNNING 1

#endif
