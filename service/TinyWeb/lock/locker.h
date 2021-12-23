/**
 * @file locker.h
 * @author caogh (caoguanghuaplus@163.com)
 * @brief 线程同步机制包装类
            多线程同步，确保任一时刻只能有一个线程能进入关键代码段.
            > * 信号量
            > * 互斥锁
            > * 条件变量
 * @version 0.1
 * @date 2021-12-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __LOCKER_H__
#define __LOCKER_H__
#include <exception>
#include <pthread.h>   // for pthread POSIX线程
#include <semaphore.h> // for semaphore POSIX 信号量
/**
 * @brief 信号量
 *
 */
class sem
{
public:
    sem()
    {
        if (sem_init(&m_sem, 0, 0) != 0)
        {
            throw std::exception();
        }
    }
    sem(unsigned int num)
    {
        if (sem_init(&m_sem, 0, num) != 0)
        {
            throw std::exception();
        }
    }

    ~sem()
    {
        sem_destroy(&m_sem);
    }

    bool wait()
    {
        return sem_wait(&m_sem) == 0;
    }

    bool post()
    {
        return sem_post(&m_sem) == 0;
    }

private:
    sem_t m_sem;
};

/**
 * @brief 互斥锁
 *
 */
class locker
{
public:
    locker()
    {
        if (pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            throw std::exception();
        }
    }
    ~locker()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    bool lock()
    {
        return pthread_mutex_lock(&m_mutex) == 0;
    }

    bool unlock()
    {
        return pthread_mutex_unlock(&m_mutex) == 0;
    }

    pthread_mutex_t *get() { return &m_mutex; }

private:
    pthread_mutex_t m_mutex;
};

/**
 * @brief 条件变量
 *
 */
class cond
{
public:
    cond()
    {
        if (pthread_cond_init(&m_cond, NULL) != 0)
        {
            throw std::exception();
        }
    }
    ~cond()
    {
        pthread_cond_destroy(&m_cond);
    }

    bool wait(pthread_mutex_t *mutex)
    {
        return pthread_cond_wait(&m_cond, mutex) == 0;
    }

    bool timewait(pthread_mutex_t *mutex, struct timespec t)
    {
        return pthread_cond_timedwait(&m_cond, mutex, &t) == 0;
    }

    bool signal()
    {
        return pthread_cond_signal(&m_cond) == 0;
    }

    bool broadcast()
    {
        return pthread_cond_broadcast(&m_cond) == 0;
    }

private:
    pthread_cond_t m_cond;
};

#endif // __LOCKER_H__