#ifndef CREDIS_MESS_H
#define CREDIS_MESS_H

#include <stdio.h>
#include "CTypedef.h"
#include "CRedis.h"
using namespace std;

// redis data manage class 
class CRedisMess
{
private:
    queue<s_credis_recv> m_que_credis_recv;
    map<string, vector<s_credis_recv>> m_map_info;

    //the index of need judge DB 
    vector<int>  m_vec_JudgeIndex;
public:l
    CRedisMess(/* args */);
    ~CRedisMess();

    int getJudgeDbIndex();
    int transfromRedisInfo();

};






#endif