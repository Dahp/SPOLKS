// Implementation of the Socket class.


#include "Socket.h"
#include "string.h"
#include <iostream>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <bits/stdc++.h>

#define MEOF "EOF"


Socket::Socket() :
  m_sock ( -1 )
{

  memset ( &m_addr, 0, sizeof ( m_addr ) ); 

}

Socket::~Socket()
{
  if ( is_valid() )
    ::close ( m_sock );
}

bool Socket::create()
{
  m_sock = socket ( AF_INET,SOCK_STREAM, 0 );

  if ( ! is_valid() ) return false;
  // TIME_WAIT - argh
  int on = 1;

  //установка опций сокета
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 ){
    //m_sock - дескриптор сокета
    //манипулирует флагами, установленными на сокете.
    return false;
  }
  return true;
}



bool Socket::bind ( const int port )
{
  if ( ! is_valid() )
  {
    return false;
  }

  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock,
			     ( struct sockaddr * ) &m_addr,
			     sizeof ( m_addr ) );
  
  if ( bind_return == -1 )
  {
    return false;
  }

  return true;
}

bool Socket::listen() const
{
  if ( ! is_valid() )
  {
    return false;
  }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );


  if ( listen_return == -1 )
  {
    return false;
  }

  return true;
}

bool Socket::accept ( Socket& new_socket ) const
{
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, 
                                ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
  else{
    std::cout << "[+] Connected!\n";
    return true;
  }
}

bool Socket::send ( const std::string s ) const
{
  std::cout << "into send(): " << s << "\n";
  std::cout << "into send(): s.c_str(): " << s.c_str() << "\n";
  int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
  if ( status == -1 )
  {
    return false;
  }
  else
  {
    return true;
  }
}

int Socket::recv ( std::string& s ) const
{
  char buf [ MAXRECV + 1 ];
  s = "";
  memset ( buf, 0, MAXRECV + 1 );
  int status = ::recv ( m_sock, buf, MAXRECV, 0 );
  if ( status == -1 )
  {
      std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
      return 0;
  }else if ( status == 0 )
  {
    return 0;
  }
  else
  {
    s = buf;
    return status;
  }
}

bool Socket::recvF( const std::string& s ) const
{
  std::ofstream writeF;//ofstream - allows to WRITE contents to a file
  writeF.open(s, std::ios_base::binary |std::ios_base::app);
  if(!writeF.is_open())//retrun 1 if file found and opened
    return false;

  char buf [MAXRECV + 1];
  int len = 0;
  memset ( buf, 0, MAXRECV + 1 );
  std::string str = "";
  int status = recv (str);
  do
  {
    len = recv(str);
    if (len < 0)
    {
      return false;
    }

    if (strcmp(buf, MEOF) == 0)
    {
      break;
    }
    writeF.write(buf, len);  
  } while ( len!=0 );
  return true;
}



bool Socket::sendF( const std::string& s ) const
{
  char buf[MAXRECV - 1 ];
  memset ( buf, 0, MAXRECV );

  int sended = 0, readed = 0;
  std::ifstream readF;
  readF.open(s, std::ios_base::binary | std::ios_base::out | std::ios_base::in);

  if(!readF.is_open())//retrun 1 if file found and opened
    return false;


  readF.read(buf, sizeof(buf));
  while ((readed = readF.gcount()) != 0) 
  {
    //sended = ::send(m_sock, (char*)buf, readed, 0);
    sended = send(buf); //v2
    if (sended < 0)
    {
      return false;
    }
    readF.read(buf, sizeof(buf));
  }
  sleep(1);
  //::send(m_sock, (char*)MEOF, sizeof(MEOF), 0);
  send(MEOF);
  return true;
}



bool Socket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) 
    return false;

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}

void Socket::set_non_blocking ( const bool b )
{

  int opts;

  opts = fcntl ( m_sock, F_GETFL );

  if ( opts < 0 )
    return;

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock, F_SETFL,opts );

}