// Implementation of the Socket class.


#include "Socket.h"
#include "string.h"
#include <iostream>
#include <string.h>
#include <errno.h>
#include <fcntl.h>



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

  if ( ! is_valid() )
    return false;


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
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      s = buf;
      return status;
    }
}

bool Socket::recvF( const std::string s ) const
{//сервер\клиент принимает файл
  //тут мы получаем имя файла который хочет получить тот кто спрашивает
  //мы передаем сюда имя файла, 
  // во ттут вопрос: мне надо вызывать функцию recv и тут написать свою функцию
  //аналогично recv 
  /*
  получаем имя файлы и проверяем есть ли такой файл уже, если есть то мы создаем с 
  таким же именем + (n) и расширением или как вариант кидаем исключение что такой файл уже есть
  и начнаем принимать и писать в файл 
  в конце закрываем файл
  
  */
    return false;
}

bool Socket::sendF( const std::string s ) const
{//сервер\клиент отправляет файл
  //алгорит
  /*сначала получаем имя файла и проверяем файл на наличие, если нет, то кидаем исключение
    потом когда нашли файл, открываем егоои начинаем читать его по немногу в буфер
    и начинем отправлять !(размер буфера должен быть равен размеру буфера сокета для отправки и чтения)  
  */

    return false;
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