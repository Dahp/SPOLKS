#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <ctime>

int main ( int argc, int argv[] )
{
  try
    {

          ClientSocket client_socket ( "ip to", 30000 );
          std::string reply = "", nameFile = "";

      try
      {
        std::string tmp;
        while(true)
        {
          std::cin >> tmp;
          if(tmp == "time"){  
            char* dt;
            client_socket << tmp;  
            client_socket >> reply;
            std::cout << reply << std::endl;
          } else if (tmp == "download")
          {
            std::cout << "Input name of file: ";
            std::cin >> nameFile;
            client_socket.recvFile(nameFile);
          } else if (tmp == "upload")
          {
            std::cout << "Input name of file: ";
            std::cin >> nameFile;
            client_socket.sendFile(nameFile);
          } else{
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