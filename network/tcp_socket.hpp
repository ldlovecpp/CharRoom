#pragma  once
#include<sys/socket.h>
#include<iostream>
#include<string>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
using std::string;
using std::endl;
using std::cout;
class Check{
public:
  bool operator () (bool ret)
  {
    return ret;
  }
};
class Tcp_Socket
{
public:
  Tcp_Socket()
  {
    _fd = -1;
  }
  Tcp_Socket(int fd)
    :_fd(fd)
  {}
  void Close()
  {
    close(_fd);
  }
  bool Socket()
  {
    _fd = socket(AF_INET,SOCK_STREAM,0) ;
    if(_fd==-1)
    {
      cout << "socket" <<endl; 
      return false;
    }
    return true;
  }

  bool Bind(const string& ip,const int& port) const
  {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_aton(ip.c_str(),&addr.sin_addr);
    int r= bind(_fd,(struct sockaddr*)&addr,sizeof(addr));
    if(r==-1)
    {
      cout << "bind" <<endl;
      return false;
    }
    return true;
  }
  bool Listenfd() const
  {
   int r=listen(_fd,SOMAXCONN);
   if(r==-1) 
   {
     cout << "listen" <<endl;
     return false;
   }
   return true;
  }
  int get_fd() const
  {
    return _fd;
  }
  bool Accept(Tcp_Socket* peer) const
  {
   int new_fd = accept(_fd,NULL,NULL);
   if(new_fd==-1)
   {
     cout << "accept" <<endl;
     return false;
   }
   peer->_fd = new_fd;
   return true;
  }
  bool Connect(const string& ip,uint16_t port) const
  {
   struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);
   inet_aton(ip.c_str(),&addr.sin_addr);
   int r= connect(_fd,(struct sockaddr*)&addr,sizeof(addr));
   if(r==-1)
   {
     cout << "connect" <<endl;
     return false;
   }
   return true;
  }
  int Getfd() const
  {
   return _fd;
  }
  bool Recv(string& buf) const
  {
    buf.clear();
    char tmp[1024*10]= {};
    int read_size = recv(_fd,tmp,sizeof(tmp),0);
    if(read_size<0)
    {
      cout << "recv" <<endl;
      return false;
    }
    if(read_size==0)
    {
      return false;
    }
    buf.assign(tmp);
    return true;
  }
  bool Send(const string& buf) const
  {
    int r= send(_fd,buf.data(),buf.size(),0);
    if(r<0)
    {
      printf("send");
      return false;
    }
    return true;
  }
    
private:
  int _fd;
};
