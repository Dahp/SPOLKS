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

bool Socket::recvF( const std::string& s ) const
{ 
  /*
  сервер\клиент принимает файл
  тут мы получаем имя файла который хочет получить тот кто спрашивает
  мы передаем сюда имя файла, 
  во ттут вопрос: мне надо вызывать функцию recv и тут написать свою функцию
  аналогично recv 
  получаем имя файлы и проверяем есть ли такой файл уже, если есть то мы создаем с 
  таким же именем + (n) и расширением или как вариант кидаем исключение что такой файл уже есть
  и начнаем принимать и писать в файл 
  в конце закрываем файл
  */
 std::cout << "[+] into recvF()\n";//log
 std::cout << "[?] string s: " << s << std::endl;//log

 std::ofstream writeF;//ofstream - allows to WRITE contents to a file
 writeF.open(s, std::ios_base::out);
 std::cout << "[+] into recvF(): file created\n";//log
 writeF.close();

 std::cout << "[+] into recvF(): file closed\n";//log
 if(!checkF(s)) return false;
 char buf [MAXRECV + 1];
 int len = 0;
 memset ( buf, 0, MAXRECV + 1 );

 writeF.open(s, std::ios_base::in |std::ios_base::app);
 std::cout << "[+] into recvF(): file open\n";//log
 
 int status = ::recv ( m_sock, buf, MAXRECV, 0 );
 std::cout << "[+] into recvF(): buf(1) \n-----------------\n" << buf << "-----------\n";
 do
 {
  len = ::recv(m_sock, (char*)buf, sizeof(buf), MSG_DONTWAIT);
   std::cout << "[+] into recvF(): buf(other) \n-----------------\n" << buf << "-----------\n";
  if (len < 0)
  {
    std::cout << "[-] recvF(): error recv (do...while)\n";//log
    return false;
  }

  if (strcmp(buf, MEOF) == 0)
  {
    std::cout << "[+] recvF(): find EOF\n";//log
    break;
  }
    
  std::cout << "[+] recvF(): write in file, num of sym: " << len << std::endl;//log
  writeF.write(buf, len);  
 } while ( len!=0 );

 std::cout << "[+] recvF(): end write in file\n";
 return true;
 
  // std::cout << "wait file for rcv\n";//для отладки
  // char buf [ MAXRECV ];
  // int len = 
  // +0;
  // memset(buf, '!', MAXRECV);
  // //if( !checkF(s)) return false;
  // std::ofstream file;
  // file.open(s, std::ios_base::out);
  // if (!file.is_open())  return false;
  // do
  // {
  //   len = ::recv ( m_sock, buf, MAXRECV, 0 );
  //   if (len < 0)
  //   {
  //     std::cout << "recv file err\n";
  //     return false;
  //   }else if (strcmp(buf, MEOF) == 0)
  //   {
  //     break;
  //   }
  //   std::cout << "write into file\n";
  //   file.write(buf, len);
  // } while (len != 0);
  // std::cout << "complete recv file\n";
  // return true;

  // file.open(s);
  // std::cout << "file open\n";
  // if(file.is_open())
  // {
  //   int rcv = 0;
  //   do 
  //   {
  //       rcv = ::recv ( m_sock, buf, MAXRECV, 0 );
  //       std::cout << "recive rcv (int(-1 or 0)): " << rcv << std::endl;//для отладки
  //       file << buf;
  //       std::cout << "buf: " << buf << std::endl;//для отладки
  //   } while (rcv != -1 /*&& rcv != 0*/);
  //   file.close();
  //   return true;      
  // }
  // std::cout << "recvF() error\n";
  // return false;
}



bool Socket::sendF( const std::string s ) const
{
  /*сначала получаем имя файла и проверяем файл на наличие, если нет, то кидаем исключение
    потом когда нашли файл, открываем его и начинаем читать его понемногу в буфер
    и начинем отправлять !(размер буфера должен быть равен размеру буфера сокета для отправки и чтения)  
  */
 char buf[MAXRECV];
 int sended = 0, readed = 0;



 std::ifstream readF;
 readF.open(s, std::ios_base::out | std::ios_base::in);
 std::cout << "[+] into sendF(): file created\n";//log

 if(!readF.is_open())//retrun 1 if file found and opened
 {
  std::cout << "[-] sendF(): is_open() true\n";//log
  return false;
 }

 readF.read(buf, sizeof(buf));

 std::cout << "[+] readF(): read from file:\n----------------\n" << buf << "\n-------------\n"; //log
 while ((readed = readF.gcount()) != 0) 
 {
  std::cout << "[+] readF(): into while..., gcount() = " << readF.gcount() << "\n";
  //sended = ::send(m_sock, (char*)buf, readed, 0);
  sended = send(buf); //v2
  std::cout << "[+] readF(): send file, sended: " << sended << "\n";//log
  if (sended < 0)
  {
    std::cout << "[-] sendF(): invalid read\n";//log
    return false;
  }
  readF.read(buf, sizeof(buf));  
 }
 std::cout << "[+] sendF(): before sleep(1)\n";
 sleep(1);
 std::cout << "[+] sendF(): sleep(1)\n";//log
 ::send(m_sock, (char*)MEOF, sizeof(MEOF), 0);
std::cout << "[+] sendF(): sended EOF and return true\n";//log
 return true;
 

  // std::cout << "func sendF\n";//для отладки
  // char buf [ MAXRECV ];
  // int sended = 0, readed = 0;
  // memset(buf, '@', MAXRECV);
  // //if( !readFfile(s, buf)) return false;//спросить как оно работало бы 
  // if( checkF(s) == false) return false;
  // std::cout << "after checkF()\n";
  // std::ifstream file (s, std::ios_base::in | std::ios_base::app/*| std::ios_base::binary*/);
  // std::cout << "file create\n";
  // file.read(buf, sizeof(buf));
  // std::cout << "file readed\n";
  // while ((readed = file.gcount()) != 0)
  // {
  //   int sended = ::send( m_sock, buf, readed, 0 );
  //   std::cout << "sending\n";
  //   if (sended < 0)
  //   {
  //     std::cout << "sending err\n";
  //     return false;
  //   }
  //   file.read(buf, sizeof(buf));    
  // }
  // sleep(1);
  // ::send(m_sock, (char*)MEOF, sizeof(MEOF), 0);
  // return true;  
  
  // if(file.is_open())
  // {
  //   std::cout << "file open\n";//потом в логи писать
  //   int i = 0;//временная переменная для отладки
  //   while (!file.eof())
  //   {
  //     std::cout << "readinf from a file: " << i << std::endl;//для отладки
  //     file.getline(buf, MAXRECV);
  //     if(send(buf)) continue;
  //     else
  //     {
  //       file.close();
  //       std::cout << "sending, problem send() == false\n";
  //       return false;
  //     }
  //   }
  //   file.close();
  //   return true;      
  // }
  //std::cout << "senf() error\n";
  //return false;
}

bool Socket::checkF(const std::string& s) const//потом передавать сюда не только имя файла, но и ссылку на поток чтения или типо того
{
  std::ifstream fileForCheck;//ifstream - allows to READ from the contents of a file
  fileForCheck.open(s, std::ios_base::in);
  std::cout << "checkF(), recved string name - s: " << s << std::endl;//log
  if (fileForCheck.is_open())
  {
    fileForCheck.close();
    std::cout << "[+] chechFile(): file found!\n";//log
    return true;
  }
  std::cout << "[-] checkFile(): Not found!\n";//log
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