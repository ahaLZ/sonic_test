#include <iostream>
#include "CJudgeRedis.h"
#include "CLog.h"


#define PORT_JUDGE_LISTEN 10016
#define PORT_RRECV_FROM_SHD  10018

CJudgeRedis::CJudgeRedis():m_pstrNosInfo(NULL)
{
    m_pstrNosInfo  = new char(sizeof(Nos_Info)* MAX_NOS_NUM);
    if(m_pstrNosInfo == NULL)
    {
        printf("new mem eerror ;\n");
    }
}

CJudgeRedis::~CJudgeRedis()
{
    if(m_pstrNosInfo != NULL)
        delete m_pstrNosInfo[];
    m_pstrNosInfo = NULL;
}

int CJudgeRedis::Init()
{
    setJudgeState(STOP_JUDGE);
    m_RecvShdData_Sock.CUdpSocket(PORT_JUDGE_LISTEN);
    InitShdSockInfo();

    printf("CJudgeRedis init success ;\n");
    return 1;
}

int CJudgeRedis::InitShdSockInfo(){

    char str[20] = "192.168.68.177";
    memset(m_ToShd_Sock, 0, sizeof(SOCKADDR_IN_T));
    m_ToShd_Sock.sin_family = AF_INET;
    m_ToShd_Sock.sin_port = htons(PORT_RRECV_FROM_SHD);
    m_ToShd_Sock.sin_addr.s_addr = inet_addr(str);
    if(m_ToShd_Sock.sin_addr.s_addr == INADDR_ANY)
    {
        SysLog(SLOG_ERR, "init shd socket error !\n");
        return 0;
    }
    return 1;
}

int CJudgeRedis::equalMessage(char* pstr1, char* pstr2){
 
    if(pstr1 == NULL || pstr2 == NULL)
        printf("message  is NULL ;");
    if(0 != strcmp(pstr1, pstr2))
    {
        printf("message  not equal ;");
        return 0;
    }
    return 1;
}

int CJudgeRedis::equalMessage(char* pstr1, char* pstr2, char* pstr3){

    int iRet = 0;
    if(if(pstr1 == NULL || pstr2 == NULL || pstr3 == NULL)
        printf("once of message is NULL ;"); 
    if(0 == strcmp(pstr1, pstr2)
       iRet |=  1 ;
    if(0 == strcmp(pstr2, pstr3)
        iRet |= 1 <<1; 
    if(iRet == 3)
        return iRet;
    if(0 == strcmp(pstr1, pstr3))
        iRet |= 1 <<2;
    
}


int CJudgeRedis::setJudgeState(int nState){
    if(nState == STOP_JUDGE)
        m_nJudgeState = STOP_JUDGE;
    else if(nState == START_JUDGE)
        m_nJudgeState = START_JUDGE;
    else
    {
        printf("error input");
        return 0;
    }
     return 1;
}

int CJudgeRedis::getNosInfo(){

    return 1;
}

int CJudgeRedis::setNosInfo(char* pstr){

    if(pstr ==NULL || strlen(pstr) != sizeof(Nos_Info)*5)
    {
        printf("error input ;\n");
        return 0;
    }
    int i=0;
    char str1 = pstr;
    for(i; i< MAX_NOS_NUM; i++)
    {
        m_NosArr[i].reset();
        memcpy(&(m_NosArr[i]), sizeof(Nos_Info), str1);
        str1 += sizeof(Nos_Info);
    }
    printf("set Nos info success ;\n");
    return 1;
}
// 
int CJudgeRedis::RecvShdData(){

    SOCKADDR_IN_T stSockAddrIn;
    Nos_Info recNosInfo; 
    int inRead=0;
    char* pTmp = m_pstrNosInfo;
    int iLeng = sizeof(Nos_Info)* MAX_NOS_NUM; 

    m_RecvShdData_Sock.CUdpSocket(PORT_JUDGE_LISTEN);
    while(1)
    {   
        memset(&stSockAddrIn, 0x00, sizeof(stSockAddrIn));
        memset(&pTmp, 0x00, iLeng);
        inRead = m_RecvShdData_Sock.CUdpRecvData(pTmp, iLeng, &stSockAddrIn);
        if (inRead <= 0)
        {   
            int err = errno;
            if (err != EAGAIN && err != EWOULDBLOCK)
            {   
                SysLog(LOG_ERR, "[uiPort:%d] error or switch_port error", err);
            }
            usleep(100);
            continue;
        }
        // check info source right 
        if(strlen(pTmp) == iLeng)
            setNosInfo(pTmp);
    }

    return 1;
}

void* CJudgeRedis::BegRecvShdDaraThread(void* arg){
    CJudgeRedis* obj = (CJudgeRedis*)arg;
    if(obj == NULL)
    {
        printf("err input ;\n");
        return 0;
    }
    obj->RecvShdData();
    return ;
}



int CJudgeRedis::SendDataToShd(char* pstr, int nleng ){
    if(pstr == NULL || nleng <1)
    {
        printf("send info error ;\n");
        return 0;
    }
    
    return sendto(m_RecvShdData_Sock, pstr, nleng, 0, (SOCKADDR_T *)&m_ToShd_Sock, (SOCKLEN_T)sizeof(m_ToShd_Sock));;
}

/*
    循环处理redis数据， 
*/
int CJudgeRedis::ManageRedisInfo()
{

    char strarry[20];
    memset(strarry, 0, 20;);
    sprintf(strarry, "hello word ");
    while(1)
    {
        if(m_nJudgeState == STOP_JUDGE)
        {
            sleep(1);
            continue;
        } 

        SendDataToShd(strarry, strlen(strarry));
        sleep(6);
    }
    return 1;
}