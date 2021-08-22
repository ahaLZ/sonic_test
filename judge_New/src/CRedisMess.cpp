#include "CRedisMess.h"

#define APPL_DB 0
#define CONFIG_DB  4
// 不需要裁决的数据  直接收主执行体的消息 


CRedisMess::CRedisMess(/* args */)
{

}

CRedisMess::~CRedisMess()
{

}

int CRedisMess::getJudgeDbIndex(){
    m_vec_JudgeIndex.clear();
    m_vec_JudgeIndex.push(APPL_DB);
    m_vec_JudgeIndex.push(CONFIG_DB);

}
int CRedisMess::transfromRedisInfo(){
    while(1){
        if(m_que_credis_recv.empty()){
            usleep(100);
            continue;
        }        
        

    }
    return 1;
}