/**
 * @file sql_connection_pool.hpp
 * @author caogh (caoguanghuaplus@163.com)
 * @brief 校验 & 数据库连接池
            ===============
            数据库连接池
            > * 单例模式，保证唯一
            > * list实现连接池
            > * 连接池为静态大小
            > * 互斥锁实现线程安全

            校验
            > * HTTP请求采用POST方式
            > * 登录用户名和密码校验
            > * 用户注册及多线程注册安全
 * @version 0.1
 * @date 2021-12-23
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef _CONNECTION_POOL_HPP_
#define _CONNECTION_POOL_HPP_
#include <stdio.h>          // for stdio 标准C库头文件, 定义输入输出函数。
#include <list>             // list 容器库
#include <error.h>          // for error 标准C库头文件头文件定义了一系列表示不同错误代码的宏
#include <string.h>         // for string 标准C库头文件,定义C语言字符串处理函数
#include <iostream>         // for iostream 标准C++库头文件,数个标准流对象
#include <string>           // for string std::basic_string 类模板
#include <mysql/mysql.h>    //支持mysql
#include "../lock/locker.h" //自定义 线程同步机制包装类
#include "../log/log.h"     //自定义 日志库

using namespace std;

/**
 * @brief mysql连接池类
 *
 */
class connection_pool
{
public:
    MYSQL *GetConnection();              //获取数据库连接
    bool ReleaseConnection(MYSQL *conn); //释放连接
    int GetFreeConn() const;                   //获取连接
    void DestroyPool();                  //销毁所有连接

    //单例模式
    static connection_pool *GetInstance();
    void init(string url, string User, string PassWord, string DataBaseName, int Port, int MaxConn, int close_log);

private:
    connection_pool();
    ~connection_pool();

    int m_MaxConn;          //最大连接数
    int m_CurConn;          //当前已使用连接数
    int m_FreeConn;         //当前空闲连接数
    locker lock;            //互斥锁
    list<MYSQL *> connList; //连接池
    sem reserve;            //信号量
public:
    string m_url;          //主机地址
    string m_Port;         //数据库端口号
    string m_User;         //登陆数据库用户名
    string m_PassWord;     //登陆数据库密码
    string m_DatabaseName; //使用数据库名
    int m_close_log;       //日志开关
};

class connectionRAII
{
public:
    connectionRAII(MYSQL **conn, connection_pool *connPool);
    ~connectionRAII();

private:
    MYSQL *conRAII;
    connection_pool *poolRAII;
};

#endif // _CONNECTION_POOL_HPP_