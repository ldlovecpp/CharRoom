#pragma once
#include"tcp_socket.hpp"
#include<unistd.h>
#include<sys/epoll.h>
#include<string>
#include<vector>
using std::vector;
class Epoll
{
public:
  Epoll()
  {
    epoll_fd = epoll_create(1000);
  }
  ~Epoll()
  {
    close(epoll_fd);
  }
  bool Add(Tcp_Socket& skt,bool et=false)
  {
    int fd = skt.get_fd();
    epoll_event evt;
    evt.data.fd = fd;
    if(et==true)
    {
      evt.events = EPOLLIN | EPOLLET;
    }
    else 
      evt.events = EPOLLIN;
    int ret = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&evt);
    if(ret<0)
    {
      cout << "epoll_ctl ADD" <<endl;
      return false;
    }
    return true;
  }
  bool Del(const Tcp_Socket& skt)
  {
   int fd = skt.get_fd();
   int ret = epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,NULL);
   if(ret<0)
   {
     cout << "del" <<endl;
     return false;
   }
   return true;
  }
  bool Wait(vector<Tcp_Socket>& output) const
  {
    output.clear();
    epoll_event events[1024];
    int nfds = epoll_wait(epoll_fd,events,1024,-1);
    if(nfds<0)
    {
      cout << "epoll_wait" <<endl;
      return false;
    }
    for(int i=0;i<nfds;i++)
    {
      Tcp_Socket sock(events[i].data.fd);
      output.push_back(sock);
    }
    return true;
  }
private:
  int epoll_fd; 
};
