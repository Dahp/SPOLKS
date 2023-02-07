// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <vector>


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 25;//65535

class Socket
{
 public:
  Socket(); //конструктор 
  virtual ~Socket(); //деструктор

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const std::string ) const;//(2) а тут передаем строку
  int recv ( std::string& ) const;//(1)почему тут передаем ссылку 

  //File transmission (файлы)
  bool sendF ( const std::string& ) const;//override for send
  bool recvF ( const std::string& ) const;//override for recv


  void set_non_blocking ( const bool );

  bool is_valid() const { return m_sock != -1; }

 private:

  int m_sock;
  sockaddr_in m_addr;

};

/*
вопросы
  нужно ли мне передавать в функцию дескриптор сокета для передачи файлов?
*/

#endif