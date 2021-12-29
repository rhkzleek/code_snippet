/**
 * @file http_conn.h
 * @author caogh (caoguanghuaplus@163.com)
 * @brief http连接处理类
        根据状态转移,通过主从状态机封装了http连接类。其中,主状态机在内部调用从状态机,从状态机将处理状态和数据传给主状态机
        > * 客户端发出http连接请求
        > * 从状态机读取数据,更新自身状态和接收数据,传给主状态机
        > * 主状态机根据从状态机状态,更新自身状态,决定响应请求还是继续读取
 * @version 0.1
 * @date 2021-12-24
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __HTTP_CONN_H__
#define __HTTP_CONN_H__
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
#include <map>          //stl map容器

#include "../lock/locker.h"                  //自定义 线程同步机制包装类
#include "../CGImysql/sql_connection_pool.h" //自定义 数据库连接池
#include "../timer/lst_timer.h"              //自定义 定时器处理非活动连接
#include "../log/log.h"                      //自定义 日志模块

class http_conn
{
public:
    static const int FILENAME_LEN = 200;
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;

    enum METHOD
    {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATH
    };

    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };
    enum HTTP_CODE
    {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };

    enum LINE_STATUS
    {
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN
    };

public:
    http_conn() {}
    ~http_conn() {}

public:
    void init(int sockfd, const sockaddr_in &addr, char *, int, int, string user, string passwd, string sqlname);
    void close_conn(bool real_close = true);
    void process();
    bool read_once();
    bool write();
    sockaddr_in *get_address()
    {
        return &m_address;
    }

    void initmysql_result(connection_pool *connPool);
    int timer_flag;
    int improv;

private:
    void init();
    HTTP_CODE process_read();
    bool process_write(HTTP_CODE ret);
    HTTP_CODE parse_request_line(char *text);
    HTTP_CODE parse_headers(char *text);
    HTTP_CODE parse_content(char *text);
    HTTP_CODE do_request();
    char *get_line() { return m_read_buf + m_start_line; }
    LINE_STATUS parse_line();
    void unmap();
    bool add_response(const char *format, ...);
    bool add_content(const char *content);
    bool add_status_line(int status, const char *status_line);
    bool add_headers(int content_length);
    bool add_content_type();
    bool add_content_length(int content_length);
    bool add_linger();
    bool add_blank_line();
public:
    static int m_epollfd;
    static int m_user_count;
    MYSQL *mysql;
    int m_state;  //读为0,写为1
private:
    int m_socket;
    sockaddr_in m_address;
    char m_read_buf[READ_BUFFER_SIZE];
    int m_read_idx;
    int m_checked_idx;
    int m_start_line;
    char m_write_buf[WRITE_BUFFER_SIZE];
    int m_write_idx;
    CHECK_STATE m_check_state;
    METHOD m_method;
    char m_real_file[FILENAME_LEN];
    char *m_url;
    char *m_version;
    char *m_host;
    int m_content_length;
    bool m_linger;
    char *m_file_address;
    struct stat m_file_stat;
    struct iovec m_iv[2];
    int m_iv_count;
    int cgi;        //是否启用post
    char *m_string; //存储请求头数据
    int bytes_to_send;
    int bytes_have_send;
    char *doc_root;

    map<string, string> m_users;
    int m_TRIGMode;
    int m_close_log;

    char sql_user[100];
    char sql_passwd[100];
    char sql_name[100];
};

#endif /* __HTTP_CONN_H__ */