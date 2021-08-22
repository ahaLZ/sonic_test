#ifndef C_NOS_CTRL_H
#define C_NOS_CTRL_H

#include <iostream>
#include <map>
#include "CTypedef.h"

using namespace::std


class CNosCtrl
{
private:
   map<int, SOCKADDR_IN_T> m_map_Nos_Socket;
public:
    CNosCtrl(/* args */);
    ~CNosCtrl();
};



#endif