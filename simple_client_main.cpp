#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <ctime>

int main ()
{
  try
    {
      ClientSocket client_socket ( "ip to", 30000 );
      std::string reply = "", request = "", nameFile = "";

      try
      {
        while(true)
        {
          std::cin >> request;
          if (request == "download")//++++++++++
          {
            client_socket << request;
            std::cout << "Input name of file: ";
            std::cin >> nameFile;
            client_socket << nameFile;
            client_socket.recvFile(nameFile);//+++++++++++++
            std::cout << "--------------------@";
          } else if (request == "upload")
          {
            std::cout << "Input name of file: ";
            std::cin >> nameFile;
            client_socket.sendFile(nameFile);
          } else{
            client_socket << request;//отправляем
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