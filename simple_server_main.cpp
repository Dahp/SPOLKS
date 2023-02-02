#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <cstring>
#include <iostream>
#include <ctime>
#include<algorithm>

int main ( int argc, int argv[] )
{
  std::cout << "running....\n";

  try
    {
      // Create the socket
      ServerSocket server ( 30000 );
	  std::string data = "";
      while ( true )
	{
	  ServerSocket new_sock;
	  server.accept ( new_sock );

	  try
	    {
			while ( true )
			{
				new_sock >> data;
				if(data == "ping"){
					new_sock << "pong";
				} else if(data == "time")
				{
					time_t now = time(0);
					char* dt = ctime(&now);
					new_sock << dt;
					
				} else if (data == "file")
				{
					//отправляем по факту строки и пишем в файл
					//для этого написать надо функцию
					
				} else if( data == "echo")
				{	
					//доделать этот блок
					//глянуть в сторону регулярных выражений
					new_sock << data;
				}
				else{
					new_sock << "No such command";
				}
			}
	    }
	  catch ( SocketException& ) {}

	}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}