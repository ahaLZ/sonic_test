#ifndef __CREDIS_H__
#define __CREDIS_H__

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>

/******************* define ********************************************/
#define     REDIS_MAIN_COUNT        1

/********************* typedef *********************************************/
typedef enum
{
    CREDISS_RECV_STRING,
}e_credis_recv_type;

typedef struct
{
    bool    is_work;                            // 是否正在连接

    std::string  host;
    int     port;
}s_credis_host;

typedef struct
{
    unsigned int    uiNosIdx;
    struct timeval  stRcvTime;

    /**** key-value **********/
    std::string          ke;
    e_credis_recv_type   type;
    union           u_redis_value
    {   
        std::string      str;
    }redis_value;
}s_credis_recv;

/******************************************************************/
class CRedis
{
public:
    CRedis(void);
    virtual ~CRedis(void);

    /**
     * 
    */
    void redis_Psub(void);
    
    /**
     * param:
     *  index: 执行体的index，不超过REDIS_MAIN_COUNT
     *  host : 
     *  port :
     * return
     *  0   : succeed
     *  else: false, errno
    */
    int Redis_connect_reset(int index, const char* host, int port);

private:

    /**
     * 对 redis 链接进行控制
     * 
     * 参考 
     *  https://blog.csdn.net/dddddz/article/details/8619141
     */
    pthread_mutex_t redis_host_mutex;                   //
    pthread_cond_t redis_host_cond;
    s_credis_host redis_host[REDIS_MAIN_COUNT];         // 
    std::queue<unsigned int> redis_host_reset_queue;    // 通知host被重置

    pthread_t thread_redis_t[REDIS_MAIN_COUNT];
    void* thread_redis_client(void* args);              // 与redis链接的线程
    // void thread_recv(redisAsyncContext *c, void *reply, void *privdata);
    static void* thread_redis_client_task(void* args);
    static void thread_recv_handle(redisAsyncContext *c, void *reply, void *privdata);
};

#endif
