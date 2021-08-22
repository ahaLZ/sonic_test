/**
 * 注意
 *  很多函数的注释在头文件
*/

#include "CRedis.h"

#include <event2/event.h>

#include "CJudgeUtils.h"
#include "CMessageQueue.h"

#include <string>
#include <string.h>

CRedis::CRedis(void)
{
    pthread_mutex_init(&redis_host_mutex, NULL);
    pthread_cond_init(&redis_host_cond, NULL);
};

CRedis::~CRedis(void)
{
    pthread_mutex_destroy(&redis_host_mutex);
    pthread_cond_destroy(&redis_host_cond);
};

void CRedis::redis_Psub(void)
{
    for (int i = 0; i < REDIS_MAIN_COUNT; i++)
    {
        pthread_create(&thread_redis_t[i], NULL, thread_redis_client_task, (void* )this);
    }
};

/***/
int CRedis::Redis_connect_reset(int index, const char* host, int port)
{
    if (index >= REDIS_MAIN_COUNT)
        return EOVERFLOW;

    if (pthread_mutex_lock(&redis_host_mutex) == 0)
    {
        redis_host[index].host = host;
        redis_host[index].port = port;
        redis_host_reset_queue.push(index);

        if (redis_host[index].is_work == true)
        {

        }

        pthread_mutex_unlock(&redis_host_mutex);
        pthread_cond_signal(&redis_host_cond);

        return 0;
    }
    else
        return EV_SIGNAL;
};

void CRedis::thread_recv_handle(redisAsyncContext *c, void *reply, void *privdata)
{
    s_credis_recv credis_recv;
    redisReply *r = (redisReply* )reply;

    if (reply == NULL) return;

    if (r->type == REDIS_REPLY_ARRAY) 
    {
        printf("**************\n");
        for (size_t j = 0; j < r->elements; j++) 
        {
            printf(" %s\n", r->element[j]->str);
        }
        
        CMessageQueue<s_credis_recv>::GetInstance().push(credis_recv);
    }
}

void* CRedis::thread_redis_client(void* args)
{
    redisAsyncContext *c;
    struct event_base *base;

    if (pthread_mutex_lock(&redis_host_mutex) == 0)
    {
        if (pthread_cond_wait(&redis_host_cond, &redis_host_mutex) == 0)                    // 收到可以连接的redis数据库
        {
            unsigned int redis_index;
            redis_index = redis_host_reset_queue.front();

            base = event_base_new();
            c = redisAsyncConnect(redis_host[redis_index].host, redis_host[redis_index].port);
            if (c->err) 
            {
                printf("error: %s\n", c->errstr);
                redis_host[redis_index].is_work = false;                                   // 连接失败
            }
            else
            {
                redisLibeventAttach(c, base);
                redis_host[redis_index].is_work = true;                                     // 连接成功
            }
        }
        pthread_mutex_unlock(&redis_host_mutex);
    }

    redisAsyncCommand(c, thread_recv_handle, NULL, "psubscribe __keyevent@0__:*");
    event_base_dispatch(base);

    event_base_free(base);
    redisAsyncFree(c); 

    return NULL;
}

void* CRedis::thread_redis_client_task(void* args)
{
    CRedis *ptr = (CRedis *)args;
    return ptr->thread_redis_client(NULL);
}
