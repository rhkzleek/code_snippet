/**
 * @file log.h
 * @author caogh (caoguanghuaplus@163.com)
 * @brief 同步/异步日志系统
        同步/异步日志系统主要涉及了两个模块，一个是日志模块，一个是阻塞队列模块,其中加入阻塞队列模块主要是解决异步写入日志做准备.
        > * 自定义阻塞队列
        > * 单例模式创建日志
        > * 同步日志
        > * 异步日志
        > * 实现按天、超行分类
 * @version 0.1
 * @date 2021-12-23
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __LOG_H__
#define __LOG_H__
#include <stdio.h>       // for stdio 标准C库头文件, 定义输入输出函数。
#include <iostream>      // for iostream 标准C++库头文件,数个标准流对象
#include <string.h>      // for string 标准C库头文件,定义C语言字符串处理函数
#include <stdarg.h>      // for stdarg 标准C库头文件, 用于访问传递给函数的不同数量的参数。
#include <pthread.h>     //for pthread POSIX线程
#include "block_queue.h" //自定义 阻塞队列模块
// #include <memory>

using namespace std;

class Log
{
public:
    // C++11以后,使用局部变量懒汉不用加锁
    static Log *get_instance()
    {
        static Log instance;
        return &instance;
    }

    static void *flush_log_thread(void *args)
    {
        Log::get_instance()->async_write_log();
    }

    bool init(const char *file_name, int close_log, int log_buf_size = 8192, int split_lines = 5000000, int max_queue_size = 0);

    /**
     * @brief 写日志
     *
     * @param level
     * @param format
     * @param ...
     */
    void write_log(int level, const char *format, ...);

    /**
     * @brief 清空缓冲区数据
     *
     */
    void flush(void);

private:
    Log();
    virtual ~Log();
    /**
     * @brief 同步写日志,线程安全
     *
     * @return void*
     */
    void *async_write_log()
    {
        string signal_log;
        //从阻塞队列取出一个日志string,写入日志
        while (m_log_queue->pop(signal_log))
        {
            m_mutex.lock();
            fputs(signal_log.c_str(), m_fp);
            m_mutex.unlock();
        }
    }

    char dir_name[128]; //路径名
    char log_name[128]; // log文件名
    int m_split_lines;  //日志最大行数
    int m_log_buf_size; //日志缓冲区大小
    long long m_count;  //日志行数记录
    int m_today;        //需求按天分类,记录当前时间是哪一天
    FILE *m_fp;         //打开log的文件指针
    char *m_buf;

    block_queue<string> *m_log_queue; //阻塞队列
    // shared_ptr< block_queue<string> > m_log_queue;
    bool m_is_async; //是否同步标志位
    locker m_mutex;  //互斥锁
    int m_close_log; //关闭日志
};

#define LOG_BEBUG(format, ...)                                    \
    if (0 == m_close_log)                                         \
    {                                                             \
        Log::get_instance()->write_log(0, format, ##__VA_ARGS__); \
        Log::get_instance()->flush();                             \
    }
#define LOG_INFO(format, ...)                                     \
    if (0 == m_close_log)                                         \
    {                                                             \
        Log::get_instance()->write_log(1, format, ##__VA_ARGS__); \
        Log::get_instance()->flush();                             \
    }
#define LOG_WARN(format, ...)                                     \
    if (0 == m_close_log)                                         \
    {                                                             \
        Log::get_instance()->write_log(2, format, ##__VA_ARGS__); \
        Log::get_instance()->flush();                             \
    }
#define LOG_ERROR(format, ...)                                    \
    if (0 == m_close_log)                                         \
    {                                                             \
        Log::get_instance()->write_log(3, format, ##__VA_ARGS__); \
        Log::get_instance()->flush();                             \
    }

#endif