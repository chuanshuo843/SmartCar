#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "MysqlHelper.h"
#include "Protocol.h"
#include "TcpEvent.h"
#include "json/json.h"
#include "Config.h"
#include <sys/fcntl.h>
#include <stdlib.h>
#include <set>
#include <map>
#include <string>
#include <string.h>

using namespace std;
using namespace mysqlhelper;

class Device;
#ifndef _DEVICE_STRUCT_
#define _DEVICE_STRUCT_
typedef void (Device::*pfunc)(Conn*&, Json::Value&);
#endif

class Device : public TcpEventServer{
    public:
        Device();
        ~Device();
        //设置配置文件
        void setConfig(const char* path);
        //退出事件，响应Ctrl+C
        static void QuitCb(int sig, short events, void *data);
        //定时器事件，每10秒向所有客户端发一句hello, world
        static void TimeOutCb(int id, int short events, void *data);
    protected:
        //重载各个处理业务的虚函数
        void ReadEvent(Conn *conn);
        void WriteEvent(Conn *conn);
        void ConnectionEvent(Conn *conn);
        void CloseEvent(Conn *conn, short events);
        void ReadApiEvent(const char *str);
    private:
        map<string,pfunc> device_api_list;
        map<int,Conn*> sock_list;
        MysqlHelper* mysql;
        void SetDeviceOffline(int fd);
        void handlerDeverInfo(Conn* &conn, Json::Value &request_data);
        void handlerKeyDown(Conn* &conn, Json::Value &request_data);
        void handlerGetDeviceBaseInfo(Conn* &conn, Json::Value &request_data);
        void setCameraPower(Conn* &conn, Json::Value &request_data);
        void initApiList();
        void sendData(Conn* &conn,const string resp_data);
        void call(Conn* &conn, Json::Value &request_data,const string func);
        Conn* getConnBaySocketFd(int sock_fd);
};
#endif
