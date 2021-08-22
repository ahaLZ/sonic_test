#ifndef CJUDGE_REDIS_H
#define CJUDGE_REDIS_H


#include <stdio.h>
#include <vector>
#include "CTypedef.h"
#include "CUdpServer.h"

#define MAX_NOS_NUM 5
#define MAX_INLINE_NUM 3
#define STOP_JUDGE 0
#define START_JUDGE 1


using namespace std


typedef struct nosinfo{
    int m_nState;
    int m_nPort;
 //   int m_nIndex;
    char m_strIp[32];

    nosinfo::nosinfo () : m_nState(0)
    ,m_nPort(0)
 //   ,m_nIndex(0)
    ,m_strIp('')
    {}

    nosinfo::reset()
    {
        memset(this, 0, sizeof(nosinfo));
    }

} Nos_Info;

class CJudgeRedis
{
private:
    UINT8 m_nJudgeState;
    Nos_Info  m_NosArr[MAX_NOS_NUM];
 
    char* m_pstrNosInfo;


    CUdpServer m_RecvShdData_Sock;
    SOCKADDR_IN_T m_ToShd_Sock;
public:
    CJudgeRedis(/* args */);
    ~CJudgeRedis();
    int Init();
    // 初始化调度发送数据的socket 
    int InitShdSockInfo();

    int sendToShd();
    int sendToRedis();
    int ManageRedisInfo();
    int equalMessage(char* pstr1, char* pstr2);

    int setJudgeState(int nState);
    int getJudgeState(int nState){return m_nJudgeState;}

    int getNosInfo();
    int setNosInfo(char* pstr);



public:     
    int RecvShdData();
    int SendDataToShd(char* pstr, int nleng );
    static void* BegRecvShdDaraThread(void* arg);
};
