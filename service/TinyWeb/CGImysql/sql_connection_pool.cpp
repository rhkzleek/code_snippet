#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>
#include "sql_connection_pool.h"

using namespace std;

connection_pool::connection_pool()
{
    m_CurConn = m_FreeConn = 0;
}

connection_pool *connection_pool::GetInstance()
{
    static connection_pool connPool;
    return &connPool;
}

//构造初始化
void connection_pool::init(string url, string User, string PassWord, string DBName, int Port, int MaxConn, int close_log)
{
    m_url = url;
    m_Port = Port;
    m_User = User;
    m_PassWord = PassWord;
    m_DatabaseName = DBName;
    m_close_log = close_log;

    for (int i = 0; i < MaxConn; i++)
    {
        MYSQL *conn = nullptr;
        conn = mysql_init();

        if (conn == nullptr)
        {
            LOG_ERROR("Mysql Error");
            exit(1);
        }

        conn = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DBName.c_str(), Port, nullptr, 0);

        if (con == nullptr)
        {
            LOG_ERROR("Mysql Error");
            exit(1);
        }

        connList.push_back(con);
        ++m_FreeConn;
    }

    reserve = sem(m_FreeConn);
    m_MaxConn = m_FreeConn;
}
/**
 * @brief 当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
 *
 * @return MYSQL*
 */
MYSQL *connection_pool::GetConnection()
{
    MYSQL *con = nullptr;
    if (0 == connList.size())
    {
        return nullptr;
    }
    reserve.wait();
    lock.lock();
    con = connList.front();
    conList.pop_front();
    --m_FreeConn;
    ++m_CurConn;
    lock.unlock();
    return con;
}
/**
 * @brief 释放当前使用的连接
 *
 * @param conn
 * @return true
 * @return false
 */
bool connection_pool::ReleaseConnection(MYSQL *conn)
{
    if (nullptr == conn)
    {
        return false;
    }
    lock.lock();
    connList.push_back(conn);
    ++m_FreeConn;
    --m_CurConn;
    lock.unlock();

    reserve.post();
    return true;
}
/**
 * @brief 销毁数据库连接池
 *
 */
void connection_pool::DestroyPool()
{
    lock.lock();
    if (connList.size() > 0)
    {
        for (auto &con : connList)
        {
            mysql_close(con);
        }
        m_CurConn = m_FreeConn = 0;
        connList.clear();
    }
    lock.unlock();
}
/**
 * @brief 当前空闲连接数
 *
 * @return int
 */
inline int connection_pool::GetFreeConn() const
{
    return m_FreeConn;
}

connection_pool::~connection_pool()
{
    DestroyPool();
}

connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool)
{
    *SQL = connPool->GetConnection();
    conRAII = *SQL;
    poolRAII = connPool;
}

connectionRAII::~connectionRAII()
{
    poolRAII->ReleaseConnection(conRAII);
}