#include"httpServer.hpp"

void Usage(string proc)
{
  cout<< "Usage: \r\n";
  cout << proc << " prot "<<endl;
}

int main(int argc,char *argv[])
{
  if(argc != 2)
  {
    Usage(argv[0]);
    exit(0);
  }

  httpServer *hs = new httpServer(atoi(argv[1]));
  hs->InitServer();
  hs->start();
  delete hs;

  return 0;
}

