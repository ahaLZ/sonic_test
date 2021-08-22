/*************************************************************************
	> File Name : CZeroMQ.cpp
	> Author : FengChao
	> EMail : smile.fengchao@gmail.com 
	> Created Time : Fri 17 Jul 2020 10:00:33 AM CST
 ************************************************************************/

#include <iostream>
#include <zmq.h>
#include "CZeroMQ.h"
#include "CLog.h"
#include "CTypedef.h"

CZMQSubscriber::CZMQSubscriber(const char *pcAddrPort, const char *pcPriKey, const char *pcPubKey)
{
    m_strAddrPort = pcAddrPort;
    m_StrPriKey.clear();
    m_StrPubKey.clear();
    if(NULL != pcPriKey) m_StrPriKey = pcPriKey;
    if(NULL != pcPubKey) m_StrPubKey = pcPubKey;
    m_pvSubCtx = NULL;
    m_pvSubSock = NULL;
    SocketInit();
}

CZMQSubscriber::~CZMQSubscriber()
{
    if(NULL != m_pvSubSock) zmq_close(m_pvSubSock);
    if(NULL != m_pvSubCtx) zmq_ctx_destroy(m_pvSubCtx);
}

int CZMQSubscriber::SocketInit()
{
    int iRet = 0;
    
    m_pvSubCtx = zmq_ctx_new();
    if(NULL == m_pvSubCtx)
    {
        SysLog(SLOG_ERR, "ZMQ ctx new failed!");
        return -1;
    }

    m_pvSubSock = zmq_socket(m_pvSubCtx, ZMQ_SUB);
    if(NULL == m_pvSubSock)
    {
        SysLog(SLOG_ERR, "ZMQ socket failed!");
        zmq_ctx_destroy(m_pvSubCtx);
        m_pvSubCtx = NULL;
        return -1;
    }

    if(!m_StrPubKey.empty())
    {
        int iOptRet = 0;

        SysLog(SLOG_INFO, "ZMQ commit with curve.");
        iOptRet += zmq_setsockopt(m_pvSubSock, ZMQ_CURVE_SERVERKEY, m_StrPubKey.c_str(), m_StrPubKey.length());

        char szPubKey[64] = {0};
        char szPriKey[64] = {0};
        iOptRet += zmq_curve_keypair(szPubKey, szPriKey);
        iOptRet += zmq_setsockopt(m_pvSubSock, ZMQ_CURVE_PUBLICKEY, szPubKey, strlen(szPubKey));
        iOptRet += zmq_setsockopt(m_pvSubSock, ZMQ_CURVE_SECRETKEY, szPriKey, strlen(szPriKey));
        if(iOptRet) SysLog(SLOG_ERR, "ZMQ with curve failed!");
    }

    iRet = zmq_connect(m_pvSubSock, m_strAddrPort.c_str());
    if(0 > iRet)
    {
        SysLog(SLOG_ERR, "ZMQ connect failed!");
        zmq_close(m_pvSubSock);
        zmq_ctx_destroy(m_pvSubCtx);
        m_pvSubCtx = NULL;
        m_pvSubSock = NULL;
        return -1;
    }

    iRet = zmq_setsockopt(m_pvSubSock, ZMQ_SUBSCRIBE, "", 0);
    if(0 != iRet)
    {
        SysLog(SLOG_ERR, "ZMQ set ZMQ_SUBSCRIBE failed");
        zmq_close(m_pvSubSock);
        zmq_ctx_destroy(m_pvSubCtx);
        m_pvSubCtx = NULL;
        m_pvSubSock = NULL;
        return -1;
    }

    SetRecvTimeout();
    
    return 0;
}

int CZMQSubscriber::SetRecvTimeout(unsigned int uiSeconds)
{
    unsigned int uiTimeout = uiSeconds * 1000;

    return zmq_setsockopt(m_pvSubSock, ZMQ_RCVTIMEO, &uiTimeout, sizeof(uiTimeout));
}

int CZMQSubscriber::RecvData(void *pvBuff, unsigned int uiBuffLen)
{
    if(NULL == m_pvSubSock) SocketInit();
    
    return zmq_recv(m_pvSubSock, pvBuff, uiBuffLen, 0);
}

CZMQPublish::CZMQPublish(const char *pcAddrPort, const char *pcPriKey, const char *pcPubKey)
{
    m_strAddrPort = pcAddrPort;
    m_StrPriKey.clear();
    m_StrPubKey.clear();
    if(NULL != pcPriKey) m_StrPriKey = pcPriKey;
    if(NULL != pcPubKey) m_StrPubKey = pcPubKey;
    m_pvSubCtx = NULL;
    m_pvSubSock = NULL;
    SocketInit();
}

CZMQPublish::~CZMQPublish()
{
    if(NULL != m_pvSubSock) zmq_close(m_pvSubSock);
    if(NULL != m_pvSubCtx) zmq_ctx_destroy(m_pvSubCtx);
}

int CZMQPublish::SocketInit()
{
    int iRet = 0;
    
    m_pvSubCtx = zmq_ctx_new();
    if(NULL == m_pvSubCtx)
    {
        SysLog(SLOG_ERR, "ZMQ ctx new failed!");
        return -1;
    }

    m_pvSubSock = zmq_socket(m_pvSubCtx, ZMQ_PUB);
    if(NULL == m_pvSubSock)
    {
        SysLog(SLOG_ERR, "ZMQ socket failed!");
        zmq_ctx_destroy(m_pvSubCtx);
        m_pvSubCtx = NULL;
        return -1;
    }

    if(!m_StrPubKey.empty())
    {
        int iOptRet = 0;

        SysLog(SLOG_INFO, "ZMQ commit with curve.");
        iOptRet += zmq_setsockopt(m_pvSubSock, ZMQ_CURVE_SERVERKEY, m_StrPubKey.c_str(), m_StrPubKey.length());

        char szPubKey[64] = {0};
        char szPriKey[64] = {0};
        iOptRet += zmq_curve_keypair(szPubKey, szPriKey);
        iOptRet += zmq_setsockopt(m_pvSubSock, ZMQ_CURVE_PUBLICKEY, szPubKey, strlen(szPubKey));
        iOptRet += zmq_setsockopt(m_pvSubSock, ZMQ_CURVE_SECRETKEY, szPriKey, strlen(szPriKey));
        if(iOptRet) SysLog(SLOG_ERR, "ZMQ with curve failed!");
    }

    iRet = zmq_bind(m_pvSubSock, m_strAddrPort.c_str());
    if(0 > iRet)
    {
        SysLog(SLOG_ERR, "ZMQ bind failed!");
        zmq_close(m_pvSubSock);
        zmq_ctx_destroy(m_pvSubCtx);
        m_pvSubCtx = NULL;
        m_pvSubSock = NULL;
        return -1;
    }

    SetSendTimeout();
    
    return 0;
}

int CZMQPublish::SetSendTimeout(unsigned int uiSeconds)
{
    unsigned int uiTimeout = uiSeconds * 1000;

    return zmq_setsockopt(m_pvSubSock, ZMQ_SNDTIMEO, &uiTimeout, sizeof(uiTimeout));
}

int CZMQPublish::SendData(const void *pvBuff, unsigned int uiBuffLen)
{
    if(NULL == m_pvSubSock) SocketInit();
    
    return zmq_send(m_pvSubSock, pvBuff, uiBuffLen, 0);
}

CZMQReqRep::CZMQReqRep(int iType, const char *pcAddrPort, const char *pcPriKey, const char *pcPubKey)
{
    m_iType = iType;
    m_strAddrPort = pcAddrPort;
    m_StrPriKey.clear();
    m_StrPubKey.clear();
    if(NULL != pcPriKey) m_StrPriKey = pcPriKey;
    if(NULL != pcPubKey) m_StrPubKey = pcPubKey;
    m_pvCtx = NULL;
    m_pvSock = NULL;
    SocketInit();
}

CZMQReqRep::~CZMQReqRep()
{
    if(NULL != m_pvSock) zmq_close(m_pvSock);
    if(NULL != m_pvCtx) zmq_ctx_destroy(m_pvCtx);
}

int CZMQReqRep::RecvData(void *pvBuff, unsigned int uiBuffLen)
{
    return zmq_recv(m_pvSock, pvBuff, uiBuffLen, 0);
}

int CZMQReqRep::SendData(const void *pvData, unsigned int uiDataLen)
{
    return zmq_send(m_pvSock, pvData, uiDataLen, 0);
}

int CZMQReqRep::SetRecvTimeout(unsigned int uiSeconds)
{
    unsigned int uiTimeout = uiSeconds * 1000;

    return zmq_setsockopt(m_pvSock, ZMQ_RCVTIMEO, &uiTimeout, sizeof(uiTimeout));
}

int CZMQReqRep::SetSendTimeout(unsigned int uiSeconds)
{
    unsigned int uiTimeout = uiSeconds * 1000;

    return zmq_setsockopt(m_pvSock, ZMQ_SNDTIMEO, &uiTimeout, sizeof(uiTimeout));
}

int CZMQReqRep::SocketInit()
{
    int iOptRet = 0;
    unsigned int uiTimeout = 0;
    
    m_pvCtx = zmq_ctx_new();
    if(NULL == m_pvCtx)
    {
        SysLog(SLOG_ERR, "ZMQ ctx new failed!");
        return -1;
    }

    m_pvSock = zmq_socket(m_pvCtx, m_iType);
    if(NULL == m_pvSock)
    {
        SysLog(SLOG_ERR, "ZMQ socket failed!");
        zmq_ctx_destroy(m_pvCtx);
        m_pvCtx = NULL;
        return -1;
    }

    uiTimeout = 3*1000;
    zmq_setsockopt(m_pvSock, ZMQ_LINGER, &uiTimeout, sizeof(uiTimeout));

    SetRecvTimeout();
    SetSendTimeout();

    if(ZMQ_REQ == m_iType)
    {
        if(!m_StrPubKey.empty())
        {
            iOptRet = 0;
            SysLog(SLOG_INFO, "ZMQ commit with curve.");
            iOptRet += zmq_setsockopt(m_pvSock, ZMQ_CURVE_SERVERKEY, m_StrPubKey.c_str(), m_StrPubKey.length());

            char szPubKey[64] = {0};
            char szPriKey[64] = {0};
            iOptRet += zmq_curve_keypair(szPubKey, szPriKey);
            iOptRet += zmq_setsockopt(m_pvSock, ZMQ_CURVE_PUBLICKEY, szPubKey, strlen(szPubKey));
            iOptRet += zmq_setsockopt(m_pvSock, ZMQ_CURVE_SECRETKEY, szPriKey, strlen(szPriKey));
            if(iOptRet) SysLog(SLOG_ERR, "ZMQ with curve failed!");
        }
        if(0 > zmq_connect(m_pvSock, m_strAddrPort.c_str()))
        {
            zmq_close(m_pvSock);
            zmq_ctx_destroy(m_pvCtx);
            m_pvCtx = NULL;
            m_pvSock = NULL;
            return -1;
        }
    }
    else if(ZMQ_REP == m_iType)
    {
        if(!m_StrPriKey.empty())
        {
            iOptRet = 0;
            SysLog(SLOG_INFO, "ZMQ commit with curve.");
            int option = 1;
            iOptRet += zmq_setsockopt(m_pvSock, ZMQ_CURVE_SERVER, &option, sizeof(option));
            iOptRet += zmq_setsockopt(m_pvSock, ZMQ_CURVE_SECRETKEY, m_StrPriKey.c_str(), m_StrPriKey.length());
            if(iOptRet) SysLog(SLOG_ERR, "ZMQ with curve failed!");
        }
        if(0 > zmq_bind(m_pvSock, m_strAddrPort.c_str()))
        {
            zmq_close(m_pvSock);
            zmq_ctx_destroy(m_pvCtx);
            m_pvCtx = NULL;
            m_pvSock = NULL;
            return -1;
        }
    }

    return 0;
}



