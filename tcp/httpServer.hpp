#pragma once
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<signal.h>

#define BACKLOG 5
using namespace std;

class httpServer{
    private:
        int port;
        int lsock;
    public:
        httpServer(int _p):port(_p),lsock(-1)
        {}
        void InitServer()
        {
            signal(SIGCHLD,SIG_IGN);
            lsock = socket(AF_INET, SOCK_STREAM, 0);
            if( lsock < 0 )
            {
                cerr << "socket error " << endl;
                exit(2);
            }
            struct sockaddr_in local;
            local.sin_family = AF_INET;
            local.sin_port = htons(port);
            local.sin_addr.s_addr = INADDR_ANY;

            if(bind(lsock, (struct sockaddr*)&local, sizeof(local)) < 0){
                cerr << "bind error!" << endl;
                exit(3);
            }

            if(listen(lsock, BACKLOG) < 0){
              cerr << "listen error"<<endl;
              exit(4);
            }
        }
        
        void Echo_http(int sock)
        {
          char request[2048];
          ssize_t s = recv(sock,request,sizeof(request),0);//bug
          if(s > 0){
            //表示接收成功
            request[s] = 0;
            cout << request << endl;


            string response = "HTTP/1.0 200 OK\r\n";//响应行
            response += "Content-type: text/html\r\n";//响应报头
            response += "\r\n";//空行
            response += "\
              <!DOCTYPE html>\
              <html>\
              <head>\
              <title>sweet</title>\
              </head>\
              <body>\
              <h1>Welcome</h1>\
              <p>helloworld</p>\
              </body>\
              </html>\
              ";  
              //响应正文
              send(sock,response.c_str(),response.size(),0);
          }
          close(sock);
        }

        void start()
        {
            struct sockaddr_in peer;
            for(;;){
                socklen_t len = sizeof(peer);
                int sock = accept(lsock, (struct sockaddr*)&peer,&len);
                if( sock < 0 ){
                    cerr << "accept error!" << endl;
                    continue;
                }
                if(fork() == 0){
                  close(lsock);
                  Echo_http(sock);
                  exit(0);
                }
                close(sock);
            }
        }
        ~httpServer()
        {
            close(lsock);
        }
};




