/**
 * @file block_queue.h
 * @author caogh (caoguanghuaplus@163.com)
 * @brief  循环数组实现的阻塞队列，m_back = (m_back + 1) % m_max_size;
           线程安全，每个操作前都要先加互斥锁，操作完后，再解锁
 * @version 0.1
 * @date 2021-12-23
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __BLOCK_QUEUE_H__
#define __BLOCK_QUEUE_H__
#include <iostream>         // for iostream 标准C++库头文件,数个标准流对象
#include <stdlib.h>         // for stdlib  标准C++库头文件基础工具：内存管理、程序工具、字符串转换、随机数、算法
#include <pthread.h>        //for pthread POSIX线程
#include <sys/time.h>       // Linux系统的日期时间头文件
#include "../lock/locker.h" //自定义 线程同步机制包装类
using namespace std;
/**
 * @brief 循环数组实现的阻塞队列(线程安全)
 *
 * @tparam T
 */
template <class T>
class block_queue
{
public:
    block_queue(int max_size = 1000)
    {
        if (max_size <= 0)
        {
            exit(-1);
        }
        m_max_size = max_size;
        m_array = new T[max_size];
        m_size = 0;
        m_front = -1;
        m_back = -1;
    }

    void clear()
    {
        m_mutex.lock();
        m_size = 0;
        m_front = -1;
        m_back = -1;
        m_mutex.unlock();
    }

    ~block_queue()
    {
        m_mutex.lock();
        if (m_array != nullptr)
        {
            delete[] m_array;
            m_array = nullptr;
        }
        m_mutex.unlock();
    }
    /**
     * @brief 判断队列是否满了
     *
     * @return true
     * @return false
     */
    bool full()
    {
        m_mutex.lock();
        if (m_size >= m_max_size)
        {
            m_mutex.unlock();
            return true;
        }
        m_mutex.unlock();
        return false;
    }
    /**
     * @brief 判断队列是否为空
     *
     * @return true
     * @return false
     */
    bool empty()
    {
        m_mutex.lock();
        if (0 == m_size)
        {
            m_mutex.unlock();
            return true;
        }
        m_mutex.unlock();
        return false;
    }
    /**
     * @brief 返回队首元素
     *
     * @param value
     * @return true
     * @return false
     */
    bool front(T &value)
    {
        m_mutex.lock();
        if (0 == m_size)
        {
            m_mutex.unlock();
            return false;
        }
        value = m_array[m_front];
        m_mutex.unlock();
        return true;
    }
    /**
     * @brief 返回队尾元素
     *
     * @param value
     * @return true
     * @return false
     */
    bool back(T &value)
    {
        m_mutex.lock();
        if (0 == m_size)
        {
            m_mutex.unlock();
            return false;
        }
        value = m_array[m_back];
        m_mutex.unlock();
        return true;
    }
    /**
     * @brief 循环数组size
     *
     * @return int
     */
    int size()
    {
        int tmp = 0;
        m_mutex.lock();
        tmp = m_size;
        m_mutex.unlock();
        return tmp;
    }
    /**
     * @brief 循环数组最大size
     *
     * @return int
     */
    int max_size()
    {
        int tmp = 0;
        m_mutex.lock();
        tmp = m_max_size;
        m_mutex.unlock();
        return tmp;
    }
    /**
     * @brief 往队列添加元素，需要将所有使用队列的线程先唤醒
              当有元素push进队列,相当于生产者生产了一个元素
              若当前没有线程等待条件变量,则唤醒无意义
     *
     * @param item
     * @return true
     * @return false
     */
    bool push(const T &item)
    {
        m_mutex.lock();
        if (m_size >= m_max_size)
        {
            m_cond.broadcast();
            m_mutex.unlock();
            return false;
        }
        m_back = (m_back + 1) % m_max_size;
        m_array[m_back] = item;
        m_size++;
        m_cond.broadcast();
        m_mutex.unlock();
        return true;
    }

    /**
     * @brief pop时,如果当前队列没有元素,将会等待条件变量
     * 
     * @param item 
     * @return true 
     * @return false 
     */
    bool pop(T &item)
    {
        m_mutex.lock();
        while(m_size <= 0){
            if(!m_cond.wait(m_mutex.get())){
                m_mutex.unlock();
                return false;
            }
        }
        m_front = (m_front +1) %m_max_size;
        item = m_array[m_front];
        m_size--;
        m_mutex.unlock();
        return true
    }
    /**
     * @brief pop时,如果当前队列没有元素,将会等待条件变量(增加了超时处理)
     * 
     * @param item 
     * @param ms_timeout 
     * @return true 
     * @return false 
     */
    bool pop(T &item,int ms_timeout) {
        struct timespec t = {0,0};
        struct timeval now = {0,0};
        gettimeofday(&now,nullptr);
        m_mutex.lock();
        if(m_size <= 0){
            t.tv_sec = now.tv_sec + ms_timeout /1000;
            t.tv_nsec = (ms_timeout%1000)*1000;
            if(!m_cond.timewait(m_mutex.get(),t)){
                m_mutex.lock();
                return false;
            }
        }
        if(m_size <= 0){
            m_mutex.unlock();
            return false;
        }
        m_front = (m_front + 1) % m_max_size;
        item = m_array[m_front];
        m_size--;
        m_mutex.unlock();
        return true;
    }


private:
    locker m_mutex; //互斥锁
    cond m_cond;    //条件变量

    T *m_array;     //循环数组
    int m_size;     //循环数组size
    int m_max_size; //循环数组最大size
    int m_front;    //循环数组队首下标
    int m_back;     //循环数组队尾下标
};

#endif /* __BLOCK_QUEUE_H__ */