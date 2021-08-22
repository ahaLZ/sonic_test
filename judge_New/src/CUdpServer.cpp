/*************************************************************************
	> File Name : CUdpServer.cpp
	> Author : FengChao
	> EMail : smile.fengchao@gmail.com 
	> Created Time : Tue 21 Jul 2020 10:31:10 AM CST
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h>
#include <assert.h>
#include "CUdpServer.h"
#include "CLog.h"

CUdpServer::CUdpServer()
{
    m_uiPort = 0;
    m_iSerSock = -1;
}

CUdpServer::CUdpServer(unsigned int uiPort)
{
    m_uiPort = uiPort;
    CUdpSocket();
}

CUdpServer::~CUdpServer()
{
    m_uiPort = 0;
    if(0 < m_iSerSock) close(m_iSerSock);
}

int CUdpServer::CUdpSocket()
{
    if(0 == m_uiPort)
    {
        SysLog(SLOG_ERR, "Para is error!");
        return -1;
    }

    return CUdpSocket(m_uiPort);
}

int CUdpServer::CUdpSocket(unsigned int uiPort)
{
    int iRet = 0;
    
    m_uiPort = uiPort;
    if(0 == m_uiPort)
    {
        assert(true);
        SysLog(SLOG_ERR, "Para is error!");
        return -1;
    }

    m_iSerSock = socket(AF_INET, SOCK_DGRAM, 0);
    if(0 > m_iSerSock)
    {
        SysLog(SLOG_ERR, "Udp server socket failed!");
        return -1;
    }

    // int flag = fcntl(m_iSerSock, F_GETFL, 0); 
    // if (flag < 0) {
    //     SysLog(SLOG_ERR, "fcntl F_GETFL fail!");
    //     return -1;
    // }   
    // if (fcntl(m_iSerSock, F_SETFL, flag | O_NONBLOCK) < 0) {
    //     SysLog(SLOG_ERR, "fcntl F_SETFL fail");
    //     return -1;
    // }

    TIMEVAL_T stTimeout;
    stTimeout.tv_sec = 3;
    stTimeout.tv_usec = 0;
    setsockopt(m_iSerSock, SOL_SOCKET, SO_RCVTIMEO, &stTimeout, sizeof(stTimeout));

    stTimeout.tv_sec = 0;
    stTimeout.tv_usec = 0;
    setsockopt(m_iSerSock, SOL_SOCKET, SO_SNDTIMEO, &stTimeout, sizeof(stTimeout));

    int iOptval = 1;
    setsockopt(m_iSerSock, SOL_SOCKET, SO_REUSEADDR, &iOptval, sizeof(int));

    struct sockaddr_in stAddr;
    stAddr.sin_family = AF_INET;
    stAddr.sin_port = htons(m_uiPort);
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    iRet = bind(m_iSerSock, (struct sockaddr *)&stAddr, (socklen_t)sizeof(stAddr));
    if(0 > iRet)
    {
        SysLog(SLOG_ERR, "Udp server bind failed!");
        return -1;
    }

    return m_iSerSock;
}

int CUdpServer::CUdpRecvData(void *pvBuff, unsigned int uiBuffLen, SOCKADDR_IN_T *pstClientInfo)
{
    int inRead = 0;
    SOCKLEN_T stAddrLen;
    
    if((NULL == pvBuff) || (NULL == pstClientInfo))
    {
        SysLog(SLOG_ERR, "Para is error!");
        return -1;
    }

    stAddrLen = sizeof(SOCKADDR_IN_T);
    inRead = recvfrom(m_iSerSock, pvBuff, uiBuffLen, 0, (SOCKADDR_T *)pstClientInfo,(SOCKLEN_T *)&stAddrLen);
	//SysLog(SLOG_INFO, "recv:%d, len1:%d, len2:%d", *(int*)pvBuff, inRead, uiBuffLen);
	
    return inRead;
}

int CUdpServer::CUdpSendData(const void *pvData, unsigned int uiDataLen, SOCKADDR_IN_T stClientInfo)
{
    int inSend = 0;

    if(NULL == pvData)
    {
        SysLog(SLOG_ERR, "Para is error!");
        return -1;
    }

    inSend = sendto(m_iSerSock, pvData, uiDataLen, 0, (SOCKADDR_T *)&stClientInfo, sizeof(SOCKADDR_T));
	//SysLog(SLOG_INFO, "send:%d len1:%d, len2:%d", *(int*)pvData, inSend, uiDataLen);

    return inSend;
}



