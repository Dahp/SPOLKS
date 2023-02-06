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
		ServerSocket server_socket ( 30000 );
		std::string data = "", nameFile = "";
		while ( true )
		{
			ServerSocket new_sock;
			server_socket.accept ( new_sock );
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
					} else if (data == "download")//тут скачивают с сервера, те сервер ищет и отправляет
					{
						new_sock >> data;
						std::cout << "downloading\n";
						nameFile = data;
						std::cout << "file name: " << data << std::endl;//для отладки
						server_socket.sendFile(nameFile);
					} else if (data == "upload")
					{
						server_socket.recvFile(nameFile);
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
			catch ( SocketException& e) 
			{
				std::cout << "Exeption was caught[1]:" << e.description() << "\nExiting.\n";
			}

		}
	}
	catch ( SocketException& e )
	{
		std::cout << "Exception was caught[2]:" << e.description() << "\nExiting.\n";
    }

  return 0;
}