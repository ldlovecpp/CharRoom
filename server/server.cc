#include<iostream>
#include"../network/epoll_server.hpp"
#include<unordered_map>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string>

using namespace std;

struct User_info
{
 string _name;
 string _passwd;
 User_info(string name,string passwd)
   :_name(name)
    ,_passwd(passwd)
  {}
};

struct Chaterrs_Info
{
  Tcp_Socket  _ts;
  User_info*  user_info;
  Chaterrs_Info(Tcp_Socket& ts,string name,string passwd)
     :_ts(ts) 
     ,user_info(new User_info(name,passwd))
  {}
};

class manager_usr
{
public:
  typedef Chaterrs_Info Chat;

  bool add_chartter(Tcp_Socket& fd,string name,string passwd)
  {
     auto e = mp.find(fd.get_fd());
     if(e!=mp.end())
     {
       return false;
     }
     mp[fd.get_fd()] = new Chat(fd,name,passwd);
     return true;
  }

  void Send(string& message)
  {
    auto e  = mp.begin();
    while(e!=mp.end())
    {
       e->second->_ts.Send(message);
       e++;
    }
  }
  
  bool delete_chartter(int name)
  {
     auto e = mp.find(name);
     if(e!=mp.end())
     {
      mp.erase(e);
      return true;
     }
     return false;
  }
  bool in_List(int id)
  {
    return mp.find(id)!=mp.end();
  }
  string get_name(int fd)
  {
    return mp[fd]->user_info->_name + " : ";
  }
private:
  unordered_map<int,Chat*>mp;
};

class Server
{
public:
  void make_server()
  {
    //创建监听套接字
    //监听套接字加入epoll
    //循环等待用户接入
    //当有用户接入的时候,将接入的用户放入epool,监听消息
    //将接入的用户放在用户表中
    //如果用户有发送消息,并将用户发送的消息发送给其他成员
    Tcp_Socket lsocket;
    check(lsocket.Socket());
    check(lsocket.Bind("172.25.107.16",9999));
    check(lsocket.Listenfd());
    epoll.Add(lsocket);
    cout << "listen successful" <<endl;
    while(1)
    {
      vector<Tcp_Socket>prepared;
      if(!check(epoll.Wait(prepared)))
        continue;
      for(auto e : prepared)
      {
         if(e.Getfd()!=lsocket.get_fd()) 
         {
          
          string req;
          bool ret = e.Recv(req);
          if(!ret)
          {
            epoll.Del(e);
            e.Close(); 
            continue;
          }
          string res = usr_list.get_name(e.get_fd()) + req;
          usr_list.Send(res);
         }
         else 
         {
           Tcp_Socket new_socket;
           e.Accept(&new_socket);
           epoll.Add(new_socket);
           string name,passwd;
           new_socket.Send("用户名: ");
           new_socket.Recv(name);
           new_socket.Send("密码: ");
           new_socket.Recv(passwd);
           name.pop_back();
           passwd.pop_back();
           usr_list.add_chartter(new_socket,name,passwd);
         }
      }
    }
  }
private:
  manager_usr usr_list;
  Epoll epoll; 
  Check check;
};
int main()
{
  Server server;
  server.make_server();
}
