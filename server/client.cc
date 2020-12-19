#include<iostream>
#include"../network/epoll_server.hpp"
int main()
{
  Tcp_Socket ts;
  Tcp_Socket Key(1);
  ts.Socket();
  ts.Connect("8.131.79.37",9999);
  Epoll ep;
  ep.Add(ts);
  ep.Add(Key);
  while(1)
  {
    vector<Tcp_Socket>pre;
    ep.Wait(pre);
    for(auto e : pre)
    {
      if(e.get_fd() == Key.get_fd())
      {
        char buf[1024];
        fgets(buf,1024,stdin);
        string sd(buf);
        ts.Send(sd);
      }
      else 
      {
        string buf;
        ts.Recv(buf);
        cout << buf <<std::flush;
      }
    }
  }
}
