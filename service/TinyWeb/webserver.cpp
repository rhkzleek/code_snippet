#include "webserver.h"

WebServer::WebServer(){
    users = new http_conn[MAX_FD];

    //root文件夹路径
    char server_path[200];
    getcwd(server_path,200);
    char root[6] = "/root";
    m_root =(char *)malloc(strlen(server_path) + strlen(root) + 1);
    strcpy(m_root,server_path);
    strcat(m_root,root);

    users_timer = new client_data[MAX_FD];

}

WebServer::~WebServer(){
    close(m_epollfd);
    close(m_listenfd);
    close(m_pipefd[1]);
    close(m_pipefd[0]);
    if(users != nullptr){
        delete[]users;
        users = nullptr;
    }
    if(users_timer != nullptr){
        delete[]users_timer;
        users_timer = nullptr;
    }
    if(m_pool != nullptr){
        delete[]m_pool;
        m_pool = nullptr;
    }
}

void WebServer::init(int port, string user, string passWord, string databaseName, int log_write, 
                     int opt_linger, int trigmode, int sql_num, int thread_num, int close_log, int actor_model)
{
    m_port = port;
    m_user = user;
    m_passWord = passWord;
    m_databaseName = databaseName;
    m_sql_num = sql_num;
    m_thread_num = thread_num;
    m_log_write = log_write;
    m_OPT_LINGER = opt_linger;
    m_TRIGMode = trigmode;
    m_close_log = close_log;
    m_actormodel = actor_model;
}