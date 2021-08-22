/*************************************************************************
	> File Name : CUdpServer.h
	> Author : FengChao
	> EMail : smile.fengchao@gmail.com 
	> Created Time : Fri 17 Jul 2020 09:29:21 AM CST
 ************************************************************************/
#ifndef CUDPSERVER_H
#define CUDPSERVER_H

#include "CTypedef.h"


class CUdpServer
{
public:
    CUdpServer();
    CUdpServer(unsigned int uiPort);
    ~CUdpServer();

    int CUdpSocket();
    int CUdpSocket(unsigned int uiPort);
    int CUdpRecvData(void *pvBuff, unsigned int uiBuffLen, SOCKADDR_IN_T *pstClientInfo);
    int CUdpSendData(const void *pvData, unsigned int uiDataLen, SOCKADDR_IN_T stClientInfo);


private:
    int m_iSerSock;
    unsigned int m_uiPort;
};

#endif /*CUDPSERVER_H*/

