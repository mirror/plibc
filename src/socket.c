/*
     This file is part of PlibC.
     (C) 2005 Nils Durner (and other contributing authors)

	   This library is free software; you can redistribute it and/or
	   modify it under the terms of the GNU Lesser General Public
	   License as published by the Free Software Foundation; either
	   version 2.1 of the License, or (at your option) any later version.
	
	   This library is distributed in the hope that it will be useful,
	   but WITHOUT ANY WARRANTY; without even the implied warranty of
	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	   Lesser General Public License for more details.
	
	   You should have received a copy of the GNU Lesser General Public
	   License along with this library; if not, write to the Free Software
	   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/**
 * @file src/socket.c
 * @brief socket()
 */

#include "plibc_private.h"

/**
 * @brief Accepts an incoming connection attempt on a socket
 */
int _win_accept(int s, struct sockaddr *addr, int *addrlen)
{
  SOCKET r;

  r = accept(s, addr, addrlen);

  SetErrnoFromWinsockError(WSAGetLastError());

  if (r == INVALID_SOCKET)
    return -1;

  __win_SetHandleType(r, SOCKET_HANDLE);

  return r;
}

/**
 * @brief Accept a new connection on a socket
 */
int _win_bind(int s, const struct sockaddr *name, int namelen)
{
  SOCKET r;

  r = bind(s, name, namelen);
  if (r == INVALID_SOCKET)
    return -1;

  SetErrnoFromWinsockError(WSAGetLastError());

  return r;
}

/**
 * @brief Initiate a connection on a socket
 */
int _win_connect(int s, const struct sockaddr *name, int namelen)
{
  int iRet, iWSErr;

  iRet = WSAConnect(s, name, namelen, NULL, NULL, NULL, NULL);
  iWSErr = WSAGetLastError();

  SetErrnoFromWinsockError(iWSErr);

  return iRet;
}

/**
 * @brief Get the name of the peer socket
 */
int _win_getpeername(int s, struct sockaddr *name,
                int *namelen)
{
  int iRet = getpeername(s, name, namelen);

  SetErrnoFromWinsockError(WSAGetLastError());

  return iRet;
}

/**
 * @brief Get the socket name
 */
int _win_getsockname(int s, struct sockaddr *name,
                int *namelen)
{
  int iRet = getsockname(s, name, namelen);

  SetErrnoFromWinsockError(WSAGetLastError());

  return iRet;
}

/**
 * @brief Get the socket options
 */
int _win_getsockopt(int s, int level, int optname, char *optval, int *optlen)
{
  int iRet = getsockopt(s, level, optname, optval, optlen);
  if (iRet == 0 && level == SOL_SOCKET && optname == SO_ERROR)
    *((int *) optval) = GetErrnoFromWinsockError(*((int *) optval));

  SetErrnoFromWinsockError(WSAGetLastError());

  return iRet;
}

/**
 * @brief Listen for socket connections and limit the queue of incoming connections
 */
int _win_listen(int s, int backlog)
{
  int iRet = listen(s, backlog);

  SetErrnoFromWinsockError(WSAGetLastError());

  return iRet;
}

/**
 * @brief Receive a message from a connected socket
 */
int _win_recv(int s, char *buf, int len, int flags)
{
  int iRet = recv(s, buf, len, flags);

  SetErrnoFromWinsockError(WSAGetLastError());

  return iRet;
}

/**
 * @brief Receive a message from a socket
 */
int _win_recvfrom(int s, void *buf, int len, int flags,
             struct sockaddr *from, int *fromlen)
{
  int iRet = recvfrom(s, buf, len, flags, from, fromlen);

  SetErrnoFromWinsockError(WSAGetLastError());

  return iRet;
}

/**
 * @brief Send a message on a socket
 */
int _win_send(int s, const char *buf, int len, int flags)
{
  int iRet = send(s, buf, len, flags);

  SetErrnoFromWinsockError(WSAGetLastError());

  return iRet;
}

/**
 * @brief Send a message on a socket
 */
int _win_sendto(int s, const char *buf, int len, int flags,
                const struct sockaddr *to, int tolen)
{
  int iRet = sendto(s, buf, len, flags, to, tolen);

  SetErrnoFromWinsockError(WSAGetLastError());

  return iRet;
}

/**
 * @brief Set the socket options
 */
int _win_setsockopt(int s, int level, int optname, const void *optval,
                    int optlen)
{
  int iRet = setsockopt(s, level, optname, (const char *) optval, optlen);

  SetErrnoFromWinsockError(WSAGetLastError());

  return iRet;
}

/**
 * @brief Shut down socket send and receive operations
 */
int _win_shutdown(int s, int how)
{
  int iRet = shutdown(s, how);

  SetErrnoFromWinsockError(WSAGetLastError());

  return iRet;
}

/**
 * @brief Create an endpoint for communication
 */
int _win_socket(int af, int type, int protocol)
{
  int iRet;

  errno = 0;

  iRet = socket(af, type, protocol);
  if (iRet == SOCKET_ERROR)
  {
    SetErrnoFromWinsockError(WSAGetLastError());

    return -1;
  }
  else
  {
    /* Sockets are not blocking by default under Windows 9x */
    u_long l;
    l = 0;
    ioctlsocket(iRet, FIONBIO, &l);

    __win_SetHandleType((DWORD) iRet, SOCKET_HANDLE);

    return iRet;
  }
}

/**
 * @brief Retrieve the host information corresponding to a network address
 */
struct hostent *_win_gethostbyaddr(const char *addr, int len, int type)
{
  struct hostent *pHost = gethostbyaddr(addr, len, type);

  SetHErrnoFromWinError(WSAGetLastError());
  SetErrnoFromWinsockError(WSAGetLastError());

  return pHost;
}

/**
 * @brief Retrieves host information corresponding to a host name from a host database
 */
struct hostent *_win_gethostbyname(const char *name)
{
  struct hostent *pHost = gethostbyname(name);

  SetHErrnoFromWinError(WSAGetLastError());
  SetErrnoFromWinsockError(WSAGetLastError());

  return pHost;
}

/**
 * @brief get network host entry
 * @warning supports AF_INET only
 */
struct hostent *gethostbyname2(const char *name, int af)
{
#if USE_IPV6
  if (af != AF_INET && af != AF_UNSPEC && af != AF_INET6)
#else
  if (af != AF_INET && af != AF_UNSPEC)
#endif
  {
    SetHErrnoFromWinError(WSANO_RECOVERY);
    return NULL;
  }
  else
    return _win_gethostbyname(name);
}

/* end of socket.c */
