/**
 * @file lst_timer.h
 * @author caogh (caoguanghuaplus@163.com)
 * @brief 定时器处理非活动连接
            ===============
            由于非活跃连接占用了连接资源，严重影响服务器的性能，通过实现一个服务器定时器，处理这种非活跃连接，释放连接资源。利用alarm函数周期性地触发SIGALRM信号,该信号的信号处理函数利用管道通知主循环执行定时器链表上的定时任务.
            > * 统一事件源
            > * 基于升序链表的定时器
            > * 处理非活动连接
 * @version 0.1
 * @date 2021-12-24
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __LST_TIMER_H__
#define __LST_TIMER_H__
#include <unistd.h>     // for unistd POSIX 符号常量
#include <signal.h>     // for signal C signal.h 头文件定义了一个变量类型 sig_atomic_t、两个函数调用和一些宏来处理程序执行期间报告的不同信号
#include <sys/types.h>  // POSIX 基本系统数据类型
#include <sys/epoll.h>  // IO多路复用
#include <fcntl.h>      // for fcntl POSIX 文件控制
#include <sys/socket.h> // POSIX 套接字接口
#include <netinet/in.h> //POSIX Internet地址族
#include <arpa/inet.h>  //POSIX Internet定义
#include <assert.h>     // for assert C 提供了一个名为 assert 的宏，它可用于验证程序做出的假设，并在假设为假时输出诊断消息
#include <sys/stat.h>   //POSIX 文件状态
#include <string.h>     // for string 标准C库头文件,定义C语言字符串处理函数
#include <pthread.h>    //for pthread POSIX线程
#include <stdio.h>      // for stdio 标准C库头文件, 定义输入输出函数。
#include <stdlib.h>     // for stdlib  标准C++库头文件基础工具：内存管理、程序工具、字符串转换、随机数、算法
#include <sys/mman.h>   //POSIX 内存管理声明
#include <stdarg.h>     // for stdarg 标准C库头文件, 用于访问传递给函数的不同数量的参数。
#include <error.h>      // for error 标准C库头文件头文件定义了一系列表示不同错误代码的宏
#include <sys/wait.h>   //POSIX 进程控制
#include <sys/uio.h>    // POSIX 矢量I/O操作

class util_timer;

struct client_data
{
    scokaddr_in address;
    int sockfd;
    util_timer *timer;
};

class util_timer
{
public:
    util_timer() : prev(nullptr), next(nullptr)
    {
    }

public:
    time_t expire;
    void (*cb_func)(client_data *);
    client_data *user_data;
    util_timer *prev;
    util_timer *next;
};

class sort_timer_lst
{
public:
    sort_timer_lst();
    ~sort_timer_lst();

    void add_timer(util_timer *timer);
    void adjust_timer(util_timer *timer);
    void del_timer(util_timer *timer);
    void tick();

private:
    void add_timer(util_timer *timer, util_timer *lst_head);
    util_timer *head;
    util_timer *tail;
};

class Utils
{
public:
    Utils();
    ~Utils();

    void init();
    /**
     * @brief 对文件描述符设置非阻塞
     *
     * @param fd
     * @return int
     */
    int setnonblocking(int fd);
    /**
     * @brief 将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
     *
     * @param epollfd
     * @param fd
     * @param one_shot
     * @param TRIGMode
     */
    void addfd(int epollfd, int fd, bool one_shot, int TRIGMode);
    /**
     * @brief 信号处理函数
     *
     * @param sig
     */
    static void sig_handler(int sig);
    /**
     * @brief 设置信号函数
     *
     */
    void addsig(int sig, void(handler)(int), bool restart = true);
    /**
     * @brief 定时处理任务，重新定时以不断触发SIGALRM信号
     *
     */
    void timer_handler();

    void show_error(int connfd, const char *info);

public:
    static int *u_pipefd;
    sort_timer_lst m_timeout;
    static int u_epollfd;
    int m_TIMESLOT;
};

#endif /* __LST_TIMER_H__ */