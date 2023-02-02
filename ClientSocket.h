// Definition of the ClientSocket class

#ifndef ClientSocket_class
#define ClientSocket_class

#include "Socket.h"


class ClientSocket : private Socket
{
 public:

  ClientSocket ( std::string host, int port );
  virtual ~ClientSocket(){};

  const ClientSocket& operator << ( const std::string& ) const;
  const ClientSocket& operator >> ( std::string& ) const;

  //work with files
  void recvFile ( const std::string ) const; //и что предавать тут строку или ссылку на строку
  void sendFile ( const std::string ) const;// аналогично, что передавать


};


#endif