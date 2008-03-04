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
 * @file src/strerror.c
 * @brief strerror()
 */

#include "plibc_private.h"

extern const char *errlist;

const char *hstrerror(int err)
{
	char *errmsg;
	
  switch(err)
  {
  	case HOST_NOT_FOUND:
  		errmsg = "Unknown host";
  		break;
  	case TRY_AGAIN:
  		errmsg = "Host name lookup failure";
  		break;
  	case NO_RECOVERY:
  		errmsg = "Unknown server error";
  		break;
  	case NO_ADDRESS:
  		errmsg = "No address associated with name";
  		break;
  	case 0:
  		errmsg = "No error";
  		break;  		
  	default:
  		if (err < 0)
  			errmsg = "Internal resolver error";
  		else
  			errmsg = "Unknown resolver error";
  }
  
  return errmsg;
}

/**
 * address and name information error description
 */
const char *gai_strerror(int code)
{
    switch (code)
    {
      case EAI_AGAIN:
        return "Temporary failure in name resolution";
      case EAI_BADFLAGS:
        return "Bad value for ai_flags";
      case EAI_FAIL:
        return "Non-recoverable failure in name resolution";
      case EAI_FAMILY:
        return "ai_family not supported";
      case EAI_MEMORY:
        return "Memory allocation failure";
      case EAI_NODATA:
        return "No address associated with hostname";
      case EAI_NONAME:
        return "Name or service not known";
      case EAI_SERVICE:
        return "Servname not supported for ai_socktype";
      case EAI_SOCKTYPE:
        return "ai_socktype not supported";
      default:
        return "Unknown error";
    }
}

/**
 * Get a system error message
 */
char *_win_strerror(int errnum)
{
  char *error;

  switch (errnum)
    {
#ifdef EPERM
    case EPERM:
      error = "Not super-user";
      break;
#endif
#ifdef ENOENT
    case ENOENT:
      error = "No such file or directory";
      break;
#endif
#ifdef ESRCH
    case ESRCH:
      error = "No such process";
      break;
#endif
#ifdef EINTR
    case EINTR:
      error = "Interrupted system call";
      break;
#endif
#ifdef EIO
    case EIO:
      error = "I/O error";
      break;
#endif
#ifdef ENXIO
    case ENXIO:
      error = "No such device or address";
      break;
#endif
#ifdef E2BIG
    case E2BIG:
      error = "Arg list too long";
      break;
#endif
#ifdef ENOEXEC
    case ENOEXEC:
      error = "Exec format error";
      break;
#endif
#ifdef EBADF
    case EBADF:
      error = "Bad file number";
      break;
#endif
#ifdef ECHILD
    case ECHILD:
      error = "No children";
      break;
#endif
#ifdef EAGAIN
    case EAGAIN:
      error = "Resource unavailable or operation would block, try again";
      break;
#endif
#ifdef ENOMEM
    case ENOMEM:
      error = "Not enough memory";
      break;
#endif
#ifdef EACCES
    case EACCES:
      error = "Permission denied";
      break;
#endif
#ifdef EFAULT
    case EFAULT:
      error = "Bad address";
      break;
#endif
#ifdef ENOTBLK
    case ENOTBLK:
      error = "Block device required";
      break;
#endif
#ifdef EBUSY
    case EBUSY:
      error = "Mount device busy";
      break;
#endif
#ifdef EEXIST
    case EEXIST:
      error = "File exists";
      break;
#endif
#ifdef EXDEV
    case EXDEV:
      error = "Cross-device link";
      break;
#endif
#ifdef ENODEV
    case ENODEV:
      error = "No such device";
      break;
#endif
#ifdef ENOTDIR
    case ENOTDIR:
      error = "Not a directory";
      break;
#endif
#ifdef EISDIR
    case EISDIR:
      error = "Is a directory";
      break;
#endif
#ifdef EINVAL
    case EINVAL:
      error = "Invalid argument";
      break;
#endif
#ifdef ENFILE
    case ENFILE:
      error = "Too many open files in system";
      break;
#endif
#ifdef EMFILE
    case EMFILE:
      error = "Too many open files";
      break;
#endif
#ifdef ENOTTY
    case ENOTTY:
      error = "Not a typewriter";
      break;
#endif
#ifdef ETXTBSY
    case ETXTBSY:
      error = "Text file busy";
      break;
#endif
#ifdef EFBIG
    case EFBIG:
      error = "File too large";
      break;
#endif
#ifdef ENOSPC
    case ENOSPC:
      error = "No space left on device";
      break;
#endif
#ifdef ESPIPE
    case ESPIPE:
      error = "Illegal seek";
      break;
#endif
#ifdef EROFS
    case EROFS:
      error = "Read only file system";
      break;
#endif
#ifdef EMLINK
    case EMLINK:
      error = "Too many links";
      break;
#endif
#ifdef EPIPE
    case EPIPE:
      error = "Broken pipe";
      break;
#endif
#ifdef EDOM
    case EDOM:
      error = "Math arg out of domain of func";
      break;
#endif
#ifdef ERANGE
    case ERANGE:
      error = "Math result not representable";
      break;
#endif
#ifdef ENOMSG
    case ENOMSG:
      error = "No message of desired type";
      break;
#endif
#ifdef EIDRM
    case EIDRM:
      error = "Identifier removed";
      break;
#endif
#ifdef ECHRNG
    case ECHRNG:
      error = "Channel number out of range";
      break;
#endif
#ifdef EL2NSYNC
    case EL2NSYNC:
      error = "Level 2 not synchronized";
      break;
#endif
#ifdef L3HLT
    case L3HLT:
      error = "Level 3 halted";
      break;
#endif
#ifdef EL3RST
    case EL3RST:
      error = "Level 3 reset";
      break;
#endif
#ifdef ELNRNG
    case ELNRNG:
      error = "Link number out of range";
      break;
#endif
#ifdef EUNATCH
    case EUNATCH:
      error = "Protocol driver not attached";
      break;
#endif
#ifdef ENOCSI
    case ENOCSI:
      error = "No CSI structure available";
      break;
#endif
#ifdef EL2HLT
    case EL2HLT:
      error = "Level 2 halted";
      break;
#endif
#ifdef ENOLCK
    case ENOLCK:
      error = "No record locks available";
      break;
#endif
#ifdef EBADE
    case EBADE:
      error = "Invalid exchange";
      break;
#endif
#ifdef EBADR
    case EBADR:
      error = "Invalid request descriptor";
      break;
#endif
#ifdef EXFULL
    case EXFULL:
      error = "Exchange full";
      break;
#endif
#ifdef ENOANO
    case ENOANO:
      error = "No anode";
      break;
#endif
#ifdef EBADRQC
    case EBADRQC:
      error = "Invalid request code";
      break;
#endif
#ifdef EBADSLT
    case EBADSLT:
      error = "Invalid slot";
      break;
#endif
#ifdef EDEADLOCK
    case EDEADLOCK:
      error = "File locking deadlock error";
      break;
#endif
#ifdef EBFONT
    case EBFONT:
      error = "Bad font file fmt";
      break;
#endif
#ifdef ENOSTR
    case ENOSTR:
      error = "Device not a stream";
      break;
#endif
#ifdef ENODATA
    case ENODATA:
      error = "No data (for no delay io)";
      break;
#endif
#ifdef ETIME
    case ETIME:
      error = "Timer expired";
      break;
#endif
#ifdef ENOSR
    case ENOSR:
      error = "Out of streams resources";
      break;
#endif
#ifdef ENONET
    case ENONET:
      error = "Machine is not on the network";
      break;
#endif
#ifdef ENOPKG
    case ENOPKG:
      error = "Package not installed";
      break;
#endif
#ifdef EREMOTE
    case EREMOTE:
      error = "The object is remote";
      break;
#endif
#ifdef ENOLINK
    case ENOLINK:
      error = "The link has been severed";
      break;
#endif
#ifdef EADV
    case EADV:
      error = "Advertise error";
      break;
#endif
#ifdef ESRMNT
    case ESRMNT:
      error = "Srmount error";
      break;
#endif
#ifdef ECOMM
    case ECOMM:
      error = "Communication error on send";
      break;
#endif
#ifdef EPROTO
    case EPROTO:
      error = "Protocol error";
      break;
#endif
#ifdef EMULTIHOP
    case EMULTIHOP:
      error = "Multihop attempted";
      break;
#endif
#ifdef ELBIN
    case ELBIN:
      error = "Inode is remote (not really error)";
      break;
#endif
#ifdef EDOTDOT
    case EDOTDOT:
      error = "Cross mount point (not really error)";
      break;
#endif
#ifdef EBADMSG
    case EBADMSG:
      error = "Trying to read unreadable message";
      break;
#endif
#ifdef ENOTUNIQ
    case ENOTUNIQ:
      error = "Given log. name not unique";
      break;
#endif
#ifdef EBADFD
    case EBADFD:
      error = "f.d. invalid for this operation";
      break;
#endif
#ifdef EREMCHG
    case EREMCHG:
      error = "Remote address changed";
      break;
#endif
#ifdef ELIBACC
    case ELIBACC:
      error = "Can't access a needed shared lib";
      break;
#endif
#ifdef ELIBBAD
    case ELIBBAD:
      error = "Accessing a corrupted shared lib";
      break;
#endif
#ifdef ELIBSCN
    case ELIBSCN:
      error = ".lib section in a.out corrupted";
      break;
#endif
#ifdef ELIBMAX
    case ELIBMAX:
      error = "Attempting to link in too many libs";
      break;
#endif
#ifdef ELIBEXEC
    case ELIBEXEC:
      error = "Attempting to exec a shared library";
      break;
#endif
#ifdef ENOSYS
    case ENOSYS:
      error = "Function not implemented";
      break;
#endif
#ifdef ENMFILE
    case ENMFILE:
      error = "No more files";
      break;
#endif
#ifdef ENOTEMPTY
    case ENOTEMPTY:
      error = "Directory not empty";
      break;
#endif
#ifdef ENAMETOOLONG
    case ENAMETOOLONG:
      error = "File or path name too long";
      break;
#endif
#ifdef ELOOP
    case ELOOP:
      error = "Too many symbolic links";
      break;
#endif
#ifdef EOPNOTSUPP
    case EOPNOTSUPP:
      error = "Operation not supported on transport endpoint";
      break;
#endif
#ifdef EPFNOSUPPORT
    case EPFNOSUPPORT:
      error = "Protocol family not supported";
      break;
#endif
#ifdef ECONNRESET
    case ECONNRESET:
      error = "Connection reset by peer";
      break;
#endif
#ifdef ENOBUFS
    case ENOBUFS:
      error = "No buffer space available";
      break;
#endif
#ifdef EAFNOSUPPORT
    case EAFNOSUPPORT:
      error = "Address family not supported by protocol family";
      break;
#endif
#ifdef EPROTOTYPE
    case EPROTOTYPE:
      error = "Protocol wrong type for socket";
      break;
#endif
#ifdef ENOTSOCK
    case ENOTSOCK:
      error = "Socket operation on non-socket";
      break;
#endif
#ifdef ENOPROTOOPT
    case ENOPROTOOPT:
      error = "Protocol not available";
      break;
#endif
#ifdef ESHUTDOWN
    case ESHUTDOWN:
      error = "Can't send after socket shutdown";
      break;
#endif
#ifdef ECONNREFUSED
    case ECONNREFUSED:
      error = "Connection refused";
      break;
#endif
#ifdef EADDRINUSE
    case EADDRINUSE:
      error = "Address already in use";
      break;
#endif
#ifdef ECONNABORTED
    case ECONNABORTED:
      error = "Connection aborted";
      break;
#endif
#ifdef ENETUNREACH
    case ENETUNREACH:
      error = "Network is unreachable";
      break;
#endif
#ifdef ENETDOWN
    case ENETDOWN:
      error = "Network interface is not configured";
      break;
#endif
#ifdef ETIMEDOUT
    case ETIMEDOUT:
      error = "Connection timed out";
      break;
#endif
#ifdef EHOSTDOWN
    case EHOSTDOWN:
      error = "Host is down";
      break;
#endif
#ifdef EHOSTUNREACH
    case EHOSTUNREACH:
      error = "Host is unreachable";
      break;
#endif
#ifdef EINPROGRESS
    case EINPROGRESS:
      error = "Connection already in progress";
      break;
#endif
#ifdef EALREADY
    case EALREADY:
      error = "Socket already connected";
      break;
#endif
#ifdef EDESTADDRREQ
    case EDESTADDRREQ:
      error = "Destination address required";
      break;
#endif
#ifdef EMSGSIZE
    case EMSGSIZE:
      error = "Message too long";
      break;
#endif
#ifdef EPROTONOSUPPORT
    case EPROTONOSUPPORT:
      error = "Unknown protocol";
      break;
#endif
#ifdef ESOCKTNOSUPPORT
    case ESOCKTNOSUPPORT:
      error = "Socket type not supported";
      break;
#endif
#ifdef EADDRNOTAVAIL
    case EADDRNOTAVAIL:
      error = "Address not available";
      break;
#endif
#ifdef ENETRESET
    case ENETRESET:
      error = "Connection aborted by network";
      break;
#endif
#ifdef EISCONN
    case EISCONN:
      error = "Socket is already connected";
      break;
#endif
#ifdef ENOTCONN
    case ENOTCONN:
      error = "Socket is not connected";
      break;
#endif
#ifdef ETOOMANYREFS
    case ETOOMANYREFS:
      error = "Too many references: cannot splice";
      break;
#endif
#ifdef EPROCLIM
    case EPROCLIM:
      error = "Too many processes";
      break;
#endif
#ifdef EUSERS
    case EUSERS:
      error = "Too many users";
      break;
#endif
#ifdef EDQUOT
    case EDQUOT:
      error = "Disk quota exceeded";
      break;
#endif
#ifdef ESTALE
    case ESTALE:
      error = "Unknown error";
      break;
#endif
#ifdef ENOTSUP
    case ENOTSUP:
      error = "Not supported";
      break;
#endif
#ifdef ENOMEDIUM
    case ENOMEDIUM:
      error = "No medium (in tape drive)";
      break;
#endif
#ifdef ENOSHARE
    case ENOSHARE:
      error = "No such host or network path";
      break;
#endif
#ifdef ECASECLASH
    case ECASECLASH:
      error = "Filename exists with different case";
      break;
#endif
    case 0:
      error = "No error";
      break;
    default:
    	{
    		char szPanic[250];
    		
	      error = "Unknown error";

	      sprintf(szPanic, "Unknown error %i in PlibC _win_strerror()\n", errnum);
	      __plibc_panic(0, szPanic);
	      break;
    	}
    }

  return error;
}

/* end of strerror.c */
