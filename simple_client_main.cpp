#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <ctime>

int main ( int argc, int argv[] )
{
  try
    {

      ClientSocket client_socket ( "localhost", 30000 );

      std::string reply;

      try
	{
    std::string tmp;
    while(true){
      std::cin >> tmp;
      if(tmp == "time"){  
        char* dt;
        client_socket << tmp;  
        client_socket >> reply;
        std::cout << reply << std::endl;
      }else{
        client_socket << tmp;//отправляем
	      client_socket >> reply;//получаем
        std::cout << reply << std::endl;
      }
	    
    }
	}
      catch ( SocketException& ) {}

      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;

    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return 0;
}