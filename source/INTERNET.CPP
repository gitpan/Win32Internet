/*
 * Internet.CPP
 * 07 Nov 96 by Aldo Calpini <dada@divinf.it>
 *
 * XS interface to the Win32 Internet Functions (WININET.DLL)
 * based on Registry.CPP written by Jesse Dougherty
 *
 * Version: 0.06 18 Dec 96
 *
 */

#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>
#include <winver.h>

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
// Section for the constant definitions.
#define CROAK croak
#define MAX_LENGTH 2048
#define TMPBUFSZ 1024

// VALUES FILLED IN BY PerlCallback
DWORD status=-1;
static CPerl* myPerl;

static time_t ft2timet(FILETIME *ft)
{
	SYSTEMTIME st;
	struct tm tm;

	FileTimeToSystemTime(ft, &st);
	tm.tm_sec = st.wSecond;
	tm.tm_min = st.wMinute;
	tm.tm_hour = st.wHour;
	tm.tm_mday = st.wDay;
	tm.tm_mon = st.wMonth - 1;
	tm.tm_year = st.wYear - 1900;
	tm.tm_wday = st.wDayOfWeek;
	tm.tm_yday = -1;
	tm.tm_isdst = -1;
	return mktime (&tm);
}

#define SUCCESSRETURNED(x)	(x == ERROR_SUCCESS)
#define INETRETURN(x) XSRETURN_IV(SUCCESSRETURNED(x))

#define SETIV(index,value) sv_setiv(ST(index), value)
#define SETPV(index,string) sv_setpv(ST(index), string)
#define SETPVN(index, buffer, length) sv_setpvn(ST(index), (char*)buffer, length)
#define SETHKEY(index, hkey)	SETIV(index,(long)hkey)

#define NEW(x,v,n,t)  (v = (t*)safemalloc((MEM_SIZE)((n) * sizeof(t))))
#define PERLSvIV(sv) (SvIOK(sv) ? SvIVX(sv) : sv_2iv(sv))
#define PERLSvPV(sv, lp) (SvPOK(sv) ? ((lp = SvCUR(sv)), SvPVX(sv)) : sv_2pv(sv, &lp))

#define SvHKEY(index) (HKEY)((unsigned long) PERLSvIV(index))

#define PERLPUSHMARK(p) if (++markstack_ptr == markstack_max)	\
			markstack_grow();			\
		    *markstack_ptr = (p) - stack_base

#define PERLXPUSHs(s)	do {\
 		if (stack_max - sp < 1) {\
			    sp = stack_grow(sp, sp, 1);\
			}\
  (*++sp = (s)); } while (0)



DWORD
constant(CPerl* pPerl,char *name, int arg)
{
    errno = 0;
    switch (*name) {
    case 'A':
		break;
    case 'B':
		break;
	case 'C':
		break;
    case 'D':
		break;
    case 'E':
		break;
    case 'F':
		break;
    case 'G':
		break;
    case 'H':
		if(strncmp(name, "HTTP_", 5) == 0)
			switch(name[5]) {
			case 'A':
				if (strEQ(name, "HTTP_ADDREQ_FLAG_ADD"))
					#ifdef HTTP_ADDREQ_FLAG_ADD
						return HTTP_ADDREQ_FLAG_ADD;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "HTTP_ADDREQ_FLAG_REPLACE"))
					#ifdef HTTP_ADDREQ_FLAG_REPLACE
						return HTTP_ADDREQ_FLAG_REPLACE;
					#else
						goto not_there;
					#endif
				break;
			case 'Q':
				if(strncmp(name, "HTTP_QUERY_", 11) == 0)
					switch(name[11]) {
					case 'A':
						if (strEQ(name, "HTTP_QUERY_ALLOW"))
							#ifdef HTTP_QUERY_ALLOW
								return HTTP_QUERY_ALLOW;
							#else
								goto not_there;
							#endif
						break;
					case 'C':
						if (strEQ(name, "HTTP_QUERY_CONTENT_DESCRIPTION"))
							#ifdef HTTP_QUERY_CONTENT_DESCRIPTION
								return HTTP_QUERY_CONTENT_DESCRIPTION;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_CONTENT_ID"))
							#ifdef HTTP_QUERY_CONTENT_ID
								return HTTP_QUERY_CONTENT_ID;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_CONTENT_LENGTH"))
							#ifdef HTTP_QUERY_CONTENT_LENGTH
								return HTTP_QUERY_CONTENT_LENGTH;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_CONTENT_TRANSFER_ENCODING"))
							#ifdef HTTP_QUERY_CONTENT_TRANSFER_ENCODING
								return HTTP_QUERY_CONTENT_TRANSFER_ENCODING;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_CONTENT_TYPE"))
							#ifdef HTTP_QUERY_CONTENT_TYPE
								return HTTP_QUERY_CONTENT_TYPE;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_COST"))
							#ifdef HTTP_QUERY_COST
								return HTTP_QUERY_COST;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_CUSTOM"))
							#ifdef HTTP_QUERY_CUSTOM
								return HTTP_QUERY_CUSTOM;
							#else
								goto not_there;
							#endif
						break;
					case 'D':
						if (strEQ(name, "HTTP_QUERY_DATE"))
							#ifdef HTTP_QUERY_DATE
								return HTTP_QUERY_DATE;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_DERIVED_FROM"))
							#ifdef HTTP_QUERY_DERIVED_FROM
								return HTTP_QUERY_DERIVED_FROM;
							#else
								goto not_there;
							#endif
						break;
					case 'E':
						if (strEQ(name, "HTTP_QUERY_EXPIRES"))
							#ifdef HTTP_QUERY_EXPIRES
								return HTTP_QUERY_EXPIRES;
							#else
								goto not_there;
							#endif
						break;
					case 'F':
						if (strEQ(name, "HTTP_QUERY_FLAG_REQUEST_HEADERS"))
							#ifdef HTTP_QUERY_FLAG_REQUEST_HEADERS
								return HTTP_QUERY_FLAG_REQUEST_HEADERS;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_FLAG_SYSTEMTIME"))
							#ifdef HTTP_QUERY_FLAG_SYSTEMTIME
								return HTTP_QUERY_FLAG_SYSTEMTIME;
							#else
								goto not_there;
							#endif
						break;
					case 'L':
						if (strEQ(name, "HTTP_QUERY_LANGUAGE"))
							#ifdef HTTP_QUERY_LANGUAGE
								return HTTP_QUERY_LANGUAGE;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_LAST_MODIFIED"))
							#ifdef HTTP_QUERY_LAST_MODIFIED
								return HTTP_QUERY_LAST_MODIFIED;
							#else
								goto not_there;
							#endif
						break;
					case 'M':
						if (strEQ(name, "HTTP_QUERY_MESSAGE_ID"))
							#ifdef HTTP_QUERY_MESSAGE_ID
								return HTTP_QUERY_MESSAGE_ID;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_MIME_VERSION"))
							#ifdef HTTP_QUERY_MIME_VERSION
								return HTTP_QUERY_MIME_VERSION;
							#else
								goto not_there;
							#endif
						break;
					case 'P':
						if (strEQ(name, "HTTP_QUERY_PRAGMA"))
							#ifdef HTTP_QUERY_PRAGMA
								return HTTP_QUERY_PRAGMA;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_PUBLIC"))
							#ifdef HTTP_QUERY_PUBLIC
								return HTTP_QUERY_PUBLIC;
							#else
								goto not_there;
							#endif
						break;
					case 'R':
						if (strEQ(name, "HTTP_QUERY_RAW_HEADERS"))
							#ifdef HTTP_QUERY_RAW_HEADERS
								return HTTP_QUERY_RAW_HEADERS;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_RAW_HEADERS_CRLF"))
							#ifdef HTTP_QUERY_RAW_HEADERS_CRLF
								return HTTP_QUERY_RAW_HEADERS_CRLF;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_REQUEST_METHOD"))
							#ifdef HTTP_QUERY_REQUEST_METHOD
								return HTTP_QUERY_REQUEST_METHOD;
							#else
								goto not_there;
							#endif
						break;
					case 'S':
						if (strEQ(name, "HTTP_QUERY_SERVER"))
							#ifdef HTTP_QUERY_SERVER
								return HTTP_QUERY_SERVER;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_STATUS_CODE"))
							#ifdef HTTP_QUERY_STATUS_CODE
								return HTTP_QUERY_STATUS_CODE;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_STATUS_TEXT"))
							#ifdef HTTP_QUERY_STATUS_TEXT
								return HTTP_QUERY_STATUS_TEXT;
							#else
								goto not_there;
							#endif
						break;
					case 'U':
						if (strEQ(name, "HTTP_QUERY_URI"))
							#ifdef HTTP_QUERY_URI
								return HTTP_QUERY_URI;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "HTTP_QUERY_USER_AGENT"))
							#ifdef HTTP_QUERY_USER_AGENT
								return HTTP_QUERY_USER_AGENT;
							#else
								goto not_there;
							#endif
						break;
					case 'V':
						if (strEQ(name, "HTTP_QUERY_VERSION"))
							#ifdef HTTP_QUERY_VERSION
								return HTTP_QUERY_VERSION;
							#else
								goto not_there;
							#endif
						break;
					case 'W':
						if (strEQ(name, "HTTP_QUERY_WWW_LINK"))
							#ifdef HTTP_QUERY_WWW_LINK
								return HTTP_QUERY_WWW_LINK;
							#else
								goto not_there;
							#endif
						break;
					}
				break;
			}
		break;
    case 'I':
		if(strncmp(name, "ICU_", 4) == 0)
			switch(name[4]) {
			case 'B':
				if(strEQ(name, "ICU_BROWSER_MODE")) 
					#ifdef ICU_BROWSER_MODE
						return ICU_BROWSER_MODE;
					#else
						goto not_there;
					#endif
				break;
			case 'D':
				if(strEQ(name, "ICU_DECODE"))
					#ifdef ICU_DECODE
						return ICU_DECODE;
					#else
						goto not_there;
					#endif
				break;
			case 'E':
				if(strEQ(name, "ICU_ENCODE_SPACES_ONLY")) 
					#ifdef ICU_ENCODE_SPACES_ONLY
						return ICU_ENCODE_SPACES_ONLY;
					#else
						goto not_there;
					#endif
				if(strEQ(name, "ICU_ESCAPE")) 
					#ifdef ICU_ESCAPE
						return ICU_ESCAPE;
					#else
						goto not_there;
					#endif
				break;
			case 'N':
				if(strEQ(name, "ICU_NO_ENCODE")) 
					#ifdef ICU_NO_ENCODE
						return ICU_NO_ENCODE;
					#else
						goto not_there;
					#endif
				if(strEQ(name, "ICU_NO_META")) 
					#ifdef ICU_NO_META
						return ICU_NO_META;
					#else
						goto not_there;
					#endif
				break;
			case 'U':
				if(strEQ(name, "ICU_USERNAME")) 
					#ifdef ICU_USERNAME
						return ICU_USERNAME;
					#else
						goto not_there;
					#endif
				break;
			}
		if(strncmp(name, "INTERNET_", 9) == 0)
			switch(name[9]) {
			case 'C':
				if(strEQ(name, "INTERNET_CONNECT_FLAG_PASSIVE"))
					#ifdef INTERNET_CONNECT_FLAG_PASSIVE
						return INTERNET_CONNECT_FLAG_PASSIVE;
					#else
						goto not_there;
					#endif
				break;
			case 'F':
				if(strEQ(name, "INTERNET_FLAG_ASYNC"))
					#ifdef INTERNET_FLAG_ASYNC
						return INTERNET_FLAG_ASYNC;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_HYPERLINK"))
					#ifdef INTERNET_FLAG_HYPERLINK
						return INTERNET_FLAG_HYPERLINK;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_KEEP_CONNECTION"))
					#ifdef INTERNET_FLAG_KEEP_CONNECTION
						return INTERNET_FLAG_KEEP_CONNECTION;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_MAKE_PERSISTENT"))
					#ifdef INTERNET_FLAG_MAKE_PERSISTENT
						return INTERNET_FLAG_MAKE_PERSISTENT;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_NO_AUTH"))
					#ifdef INTERNET_FLAG_NO_AUTH
						return INTERNET_FLAG_NO_AUTH;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_NO_AUTO_REDIRECT"))
					#ifdef INTERNET_FLAG_NO_AUTO_REDIRECT
						return INTERNET_FLAG_NO_AUTO_REDIRECT;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_NO_CACHE_WRITE"))
					#ifdef INTERNET_FLAG_NO_CACHE_WRITE
						return INTERNET_FLAG_NO_CACHE_WRITE;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_NO_COOKIES"))
					#ifdef INTERNET_FLAG_NO_COOKIES
						return INTERNET_FLAG_NO_COOKIES;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_READ_PREFETCH"))
					#ifdef INTERNET_FLAG_READ_PREFETCH
						return INTERNET_FLAG_READ_PREFETCH;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_RELOAD"))
					#ifdef INTERNET_FLAG_RELOAD
						return INTERNET_FLAG_RELOAD;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_RESYNCHRONIZE"))
					#ifdef INTERNET_FLAG_RESYNCHRONIZE
						return INTERNET_FLAG_RESYNCHRONIZE;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_TRANSFER_ASCII"))
					#ifdef INTERNET_FLAG_TRANSFER_ASCII
						return INTERNET_FLAG_TRANSFER_ASCII;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_FLAG_TRANSFER_BINARY"))
					#ifdef INTERNET_FLAG_TRANSFER_BINARY
						return INTERNET_FLAG_TRANSFER_BINARY;
					#else
						goto not_there;
					#endif
				break;
			case 'I':
				if(strEQ(name, "INTERNET_INVALID_PORT_NUMBER"))
					#ifdef INTERNET_INVALID_PORT_NUMBER
						return (DWORD) INTERNET_INVALID_PORT_NUMBER;
					#else
						goto not_there;
					#endif
				break;
				if(strEQ(name, "INTERNET_INVALID_STATUS_CALLBACK"))
					#ifdef INTERNET_INVALID_STATUS_CALLBACK
						return (DWORD) INTERNET_INVALID_STATUS_CALLBACK;
					#else
						goto not_there;
					#endif
				break;
			case 'O':
				if(strncmp(name,"INTERNET_OPEN_",14) == 0)
					switch(name[14]) {
					case 'D':
						if(strEQ(name, "INTERNET_OPEN_TYPE_DIRECT"))
							#ifdef INTERNET_OPEN_TYPE_DIRECT
								return INTERNET_OPEN_TYPE_DIRECT;
							#else
								goto not_there;
							#endif
						break;
					case 'P':
						if(strEQ(name, "INTERNET_OPEN_TYPE_PROXY"))
							#ifdef INTERNET_OPEN_TYPE_PROXY
								return INTERNET_OPEN_TYPE_PROXY;
							#else
								goto not_there;
							#endif
						if(strEQ(name, "INTERNET_OPEN_TYPE_PROXY_PRECONFIG"))
							#ifdef INTERNET_OPEN_TYPE_PROXY_PRECONFIG
								return INTERNET_OPEN_TYPE_PROXY_PRECONFIG;
							#else
								goto not_there;
							#endif
						break;
					}
				if(strncmp(name,"INTERNET_OPTION_",16) == 0)
					switch(name[16]) {
					case 'C':
						if(strEQ(name, "INTERNET_OPTION_CONNECT_BACKOFF"))
							#ifdef INTERNET_OPTION_CONNECT_BACKOFF
								return INTERNET_OPTION_CONNECT_BACKOFF;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "INTERNET_OPTION_CONNECT_RETRIES"))
							#ifdef INTERNET_OPTION_CONNECT_RETRIES
								return INTERNET_OPTION_CONNECT_RETRIES;
							#else
	    						goto not_there;
							#endif
						if (strEQ(name, "INTERNET_OPTION_CONNECT_TIMEOUT"))
							#ifdef INTERNET_OPTION_CONNECT_TIMEOUT
	    						return INTERNET_OPTION_CONNECT_TIMEOUT;
							#else
	    						goto not_there;
							#endif
						if (strEQ(name, "INTERNET_OPTION_CONTROL_SEND_TIMEOUT"))
							#ifdef INTERNET_OPTION_CONTROL_SEND_TIMEOUT
	    						return INTERNET_OPTION_CONTROL_SEND_TIMEOUT;
							#else
	    						goto not_there;
							#endif
						if (strEQ(name, "INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT"))
							#ifdef INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT
	    						return INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT;
							#else
								goto not_there;
							#endif
						break;
					case 'D':
						if(strEQ(name, "INTERNET_OPTION_DATA_SEND_TIMEOUT"))
							#ifdef INTERNET_OPTION_DATA_SEND_TIMEOUT
								return INTERNET_OPTION_DATA_SEND_TIMEOUT;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "INTERNET_OPTION_DATA_RECEIVE_TIMEOUT"))
								#ifdef INTERNET_OPTION_DATA_RECEIVE_TIMEOUT
									return INTERNET_OPTION_DATA_RECEIVE_TIMEOUT;
								#else
									goto not_there;
								#endif
						break;
					case 'H':
						if(strEQ(name, "INTERNET_OPTION_HANDLE_TYPE"))
							#ifdef INTERNET_OPTION_HANDLE_TYPE
								return INTERNET_OPTION_HANDLE_TYPE;
							#else
								goto not_there;
							#endif
						break;
					case 'L':
						if (strEQ(name, "INTERNET_OPTION_LISTEN_TIMEOUT"))
							#ifdef INTERNET_OPTION_LISTEN_TIMEOUT
								return INTERNET_OPTION_LISTEN_TIMEOUT;
							#else														
								goto not_there;
							#endif
						break;
					case 'P':
						if (strEQ(name, "INTERNET_OPTION_PASSWORD"))
							#ifdef INTERNET_OPTION_PASSWORD
								return INTERNET_OPTION_PASSWORD;
							#else
								goto not_there;
							#endif
						break;
					case 'R':
						if (strEQ(name, "INTERNET_OPTION_READ_BUFFER_SIZE"))
							#ifdef INTERNET_OPTION_READ_BUFFER_SIZE
								return INTERNET_OPTION_READ_BUFFER_SIZE;
							#else
								goto not_there;
							#endif
						break;
					case 'U':
						if (strEQ(name, "INTERNET_OPTION_USERNAME"))
							#ifdef INTERNET_OPTION_USERNAME
								return INTERNET_OPTION_USERNAME;
							#else
								goto not_there;
							#endif
						if (strEQ(name, "INTERNET_OPTION_USER_AGENT"))
							#ifdef INTERNET_OPTION_USER_AGENT
								return INTERNET_OPTION_USER_AGENT;
							#else
								goto not_there;
							#endif
						break;
					case 'V':
						if (strEQ(name, "INTERNET_OPTION_VERSION"))
							#ifdef INTERNET_OPTION_VERSION
								return INTERNET_OPTION_VERSION;
							#else
								goto not_there;
							#endif
						break;
					case 'W':
						if (strEQ(name, "INTERNET_OPTION_WRITE_BUFFER_SIZE"))
							#ifdef INTERNET_OPTION_WRITE_BUFFER_SIZE
								return INTERNET_OPTION_WRITE_BUFFER_SIZE;
							#else
								goto not_there;
							#endif
						break;
					}
				break;
			case 'S':
				if (strEQ(name, "INTERNET_SERVICE_FTP"))
					#ifdef INTERNET_SERVICE_FTP
						return INTERNET_SERVICE_FTP;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_SERVICE_GOPHER"))
					#ifdef INTERNET_SERVICE_GOPHER
						return INTERNET_SERVICE_GOPHER;
					#else
					    goto not_there;
					#endif
				if (strEQ(name, "INTERNET_SERVICE_HTTP"))
					#ifdef INTERNET_SERVICE_HTTP
						return INTERNET_SERVICE_HTTP;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_RESOLVING_NAME"))
					#ifdef INTERNET_STATUS_RESOLVING_NAME
						return INTERNET_STATUS_RESOLVING_NAME;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_NAME_RESOLVED"))
					#ifdef INTERNET_STATUS_NAME_RESOLVED
						return INTERNET_STATUS_NAME_RESOLVED;
					#else
						goto not_there;
					#endif
				break;
				if (strEQ(name, "INTERNET_STATUS_CONNECTING_TO_SERVER"))
					#ifdef INTERNET_STATUS_CONNECTING_TO_SERVER
						return INTERNET_STATUS_CONNECTING_TO_SERVER;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_CONNECTED_TO_SERVER"))
					#ifdef INTERNET_STATUS_CONNECTED_TO_SERVER
						return INTERNET_STATUS_CONNECTED_TO_SERVER;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_SENDING_REQUEST"))
					#ifdef INTERNET_STATUS_SENDING_REQUEST
						return INTERNET_STATUS_SENDING_REQUEST;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_REQUEST_SENT"))
					#ifdef INTERNET_STATUS_REQUEST_SENT
						return INTERNET_STATUS_REQUEST_SENT;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_RECEIVING_RESPONSE"))
					#ifdef INTERNET_STATUS_RECEIVING_RESPONSE
						return INTERNET_STATUS_RECEIVING_RESPONSE;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_RESPONSE_RECEIVED"))
					#ifdef INTERNET_STATUS_RESPONSE_RECEIVED
						return INTERNET_STATUS_RESPONSE_RECEIVED;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_REDIRECT"))
					#ifdef INTERNET_STATUS_REDIRECT
						return INTERNET_STATUS_REDIRECT;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_CLOSING_CONNECTION"))
					#ifdef INTERNET_STATUS_CLOSING_CONNECTION
						return INTERNET_STATUS_CLOSING_CONNECTION;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_CONNECTION_CLOSED"))
					#ifdef INTERNET_STATUS_CONNECTION_CLOSED
						return INTERNET_STATUS_CONNECTION_CLOSED;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_HANDLE_CREATED"))
					#ifdef INTERNET_STATUS_HANDLE_CREATED
						return INTERNET_STATUS_HANDLE_CREATED;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_HANDLE_CLOSING"))
					#ifdef INTERNET_STATUS_HANDLE_CLOSING
						return INTERNET_STATUS_HANDLE_CLOSING;
					#else
						goto not_there;
					#endif
				if (strEQ(name, "INTERNET_STATUS_REQUEST_COMPLETE"))
					#ifdef INTERNET_STATUS_REQUEST_COMPLETE
						return INTERNET_STATUS_REQUEST_COMPLETE;
					#else
						goto not_there;
					#endif
				break;
			}
		break;
    case 'J':
		break;
    case 'K':
		break;
    case 'L':
		break;
    case 'M':
		break;
    case 'N':
		break;
    case 'O':
		break;
    case 'P':
		break;
    case 'Q':
		break;
    case 'R':
		break;
    case 'S':
		break;
    case 'T':
		break;
    case 'U':
		break;
    case 'V':
		break;
    case 'W':
		break;
    case 'X':
		break;
    case 'Y':
		break;
    case 'Z':
		break;
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}


XS(XS_Win32__Internet_constant)
{
    dXSARGS;

    if (items != 2) {
		croak("Usage: Win32::Internet::constant(name,arg)");
    }
    {
		char *	name = (char *)SvPV(ST(0),na);
		int	arg = (int)SvIV(ST(1));
		DWORD RETVAL;

		RETVAL = constant(pPerl,name, arg);
		ST(0) = sv_newmortal();
		sv_setiv(ST(0), (IV)RETVAL);
    }
    XSRETURN(1);
}



void WINAPI CALLBACK PerlCallback(HINTERNET h, 
						   DWORD context, 
						   DWORD mystatus, 
						   LPVOID mystatusinfo, 
						   DWORD mystatuslength) {

	CPerl *pPerl = myPerl;
	DWORD myret;
	HINTERNET myhandle;

/*
    // Tried to simply set a variable in Perl
	// with the status info for this context,
	// but it fails with:
	// Modification of a read-only value attempted at testcallback.pl line 10.

  	char mytmp[80];
	char mystring[80];
	SV* myvar;
	SV* myvarcheck;


  	// ultoa((DWORD) h, mystring, 10);
	// strncat(mystring, ".", 1);
	// ultoa((DWORD) context, mytmp, 10);
	// strcat(mystring, mytmp);
	// // why the handle?
	ultoa((DWORD) context, mystring, 10);
	printf("PerlCallback: got context=%d mystring=%s\n",context,mystring);
	myvar=perl_get_sv(mystring, FALSE);
	if(myvar==NULL) {
		printf("PerlCallback: creating new var...\n");
		myvar=perl_get_sv(mystring, TRUE | 0x02);
	}
	if(!SvOK(myvar)) printf("PerlCallback: var is NOT defined...\n");

	myvarcheck=perl_get_sv(mystring, FALSE);
	if(myvarcheck==NULL) printf("PerlCallback: var does not exist!\n");
    if(myvarcheck==myvar) printf("PerlCallback: var exists...\n");
	printf("PerlCallback: populating var...\n");
	sv_setiv(myvar, mystatus);
	printf("PerlCallback: returning...\n");    
*/


	// Let's try with perl_call_method
	// ...to clarify:
	// a C routine 
	// called from Perl
	// callbacks this C routine 
	// that callbacks a Perl routine.
	// ;)

	// if(mystatus!=status) {
		dSP;
		ENTER;
		SAVETMPS;
		PUSHMARK(sp);
		// XPUSHs(sv_2mortal(newSVpv("Win32::Internet\0",0)));
		XPUSHs(sv_2mortal(newSViv(context)));
		XPUSHs(sv_2mortal(newSViv(mystatus)));
		switch(mystatus) {
		case INTERNET_STATUS_HANDLE_CREATED:
			myhandle=(HINTERNET) *(LPHINTERNET)mystatusinfo;
			XPUSHs(sv_2mortal(newSViv((DWORD) myhandle)));
			break;
		case INTERNET_STATUS_RESPONSE_RECEIVED:
		case INTERNET_STATUS_REQUEST_SENT:
			myret=(DWORD) *(LPDWORD)mystatusinfo;
			// printf("PerlCallback: received/sent(%d) %d bytes\n",mystatus,myret);
			XPUSHs(sv_2mortal(newSViv(myret)));
			break;
		default:
			XPUSHs(sv_2mortal(newSViv(0)));
			break;
		}
		PUTBACK;
		// printf("PerlCallback: calling callback with context=%d, status=%d\n",context,mystatus);
		perl_call_pv("Win32::Internet::callback", G_DISCARD);
		FREETMPS;
		LEAVE;
	// }
	// status=mystatus;
	// return;
}


XS(XS_Win32__Internet_InternetSetStatusCallback) {

	DWORD mysize;
	BOOL myquerystatus;
	INTERNET_STATUS_CALLBACK mycallback;

  
	dXSARGS;

	if(items != 1) {
		CROAK("usage: InternetSetStatusCallback($handle);\n");
	}


	mycallback=InternetSetStatusCallback((HINTERNET) PERLSvIV(ST(0)), PerlCallback);
	if(mycallback==INTERNET_INVALID_STATUS_CALLBACK) { 
		XSRETURN_NO;
	} else {
		ST(0)=sv_2mortal(newSViv((DWORD) mycallback));
		XSRETURN(1);
	}
}



XS(XS_Win32__Internet_InternetOpen) {

	HINTERNET myhandle;
	
	dXSARGS;

	myPerl=pPerl;

	if(items != 5) {
		CROAK("usage: InternetOpen($agent, $type, $proxy, $proxybypass, $flags);\n");
	}

	if(myhandle=InternetOpen((LPCTSTR) PERLSvPV(ST(0), na), 
                             (DWORD) PERLSvIV(ST(1)), 
                             (LPCTSTR) PERLSvPV(ST(2), na),
                             (LPCTSTR) PERLSvPV(ST(3), na),
                             (DWORD) PERLSvIV(ST(4)))) {
		XSRETURN_IV((long) myhandle);
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetGetLastResponseInfo) {

	DWORD myerrnum;
	unsigned char mybuf[TMPBUFSZ*2];
	DWORD mybufsz = sizeof(mybuf);
	dXSARGS;

	if(items != 0) {
		CROAK("usage: InternetGetLastReponseInfo();\n");
	}

	if(InternetGetLastResponseInfo(&myerrnum, (LPTSTR) &mybuf, &mybufsz)) {
		// printf("InternetGetLastResponseInfo: myerrnum=%d\n",myerrnum);
		// printf("InternetGetLastResponseInfo: mybuf=%s\n",mybuf);
		ST(0)=sv_2mortal(newSViv(myerrnum));
		ST(1)=sv_2mortal(newSVpv((char *) &mybuf,0));
		XSRETURN(2);
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetQueryDataAvailable) {

	DWORD mydatalen;
    HINTERNET myhandle;
	dXSARGS;

	if(items != 1) {
		CROAK("usage: InternetQueryDataAvailable($handle);\n");
	}

	myhandle=(HINTERNET) PERLSvIV(ST(0));
	if(InternetQueryDataAvailable(myhandle, &mydatalen, 0, 0)) {
        XSRETURN_IV((long) mydatalen);
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetReadFile) {

	HINTERNET myfile;
	DWORD mysize;
	char *mybuf;
	DWORD myread;

	dXSARGS;

	if(items != 2) {
		CROAK("usage: InternetReadFile($handle,$size);\n");
	}

	myfile=(HINTERNET) PERLSvIV(ST(0));
	mysize=(DWORD) PERLSvIV(ST(1));
	mybuf=(char *)safemalloc(mysize);
	if(InternetReadFile(myfile, mybuf, mysize, &myread)) {
        ST(0)=sv_2mortal(newSVpv(mybuf,mysize));
		XSRETURN(1);
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetDllVersion) {

  
	dXSARGS;

	if(items != 0) {
		CROAK("usage: InternetDllVersion();\n");
	}

/*

	// This returns: 1.0 (silly)
	
	DWORD mysize;
	char *mybuf;
	long mybufsz;

    mybuf=(char *)safemalloc(16000);
	mybufsz=16000;

	if(InternetQueryOption((HINTERNET) PERLSvIV(ST(0)),
	                       INTERNET_OPTION_VERSION, 
	                       mybuf, 
						   (DWORD *) &mybufsz)) {

		// printf("InternetQueryVersion.major=%d\n",((LPINTERNET_VERSION_INFO)mybuf)->dwMajorVersion);
		// printf("InternetQueryVersion.minor=%d\n",((LPINTERNET_VERSION_INFO)mybuf)->dwMinorVersion);
		ST(0)=sv_2mortal(newSViv(((LPINTERNET_VERSION_INFO)mybuf)->dwMajorVersion));
		ST(1)=sv_2mortal(newSViv(((LPINTERNET_VERSION_INFO)mybuf)->dwMinorVersion));
		XSRETURN(2);
	} else {
		XSRETURN_NO;
	}

*/

	// Attempting to get WININET.DLL true version

	DWORD infosize;
	DWORD zero;
	void *ver;
	void *verbuf;
	BOOL myerror;

	infosize=GetFileVersionInfoSize("WININET.DLL\0",&zero);
	// printf("InternetQueryVersion: GetFileVersionInfoSize.result=%d\n",infosize);

	ver=(void *)safemalloc(infosize);
	verbuf=(void *)safemalloc(infosize);
	
	myerror=GetFileVersionInfo("WININET.DLL\0",zero,infosize,(LPVOID)ver);
	// printf("InternetQueryVersion: GetFileVersionInfo.result=%d\n",myerror);

	if(VerQueryValue(ver, 
                     TEXT("\\StringFileInfo\\040904B0\\FileVersion"), 
                     &verbuf, 
                     (PUINT) &infosize)) { 
		// TEXT("\\VarFileInfo\\Translation"), // returns 040904B0, hope it is always true

		// printf("InternetQueryVersion: VerQueryValue.result=%d\n",myerror);
		// printf("InternetQueryVersion: infosize=%d\n",infosize);
		// printf("InternetQueryVersion: verbuf=%s\n",verbuf);
		ST(0)=sv_2mortal(newSVpv((char *)verbuf,infosize));
		safefree((char *)verbuf);
		safefree((char *)ver);
		XSRETURN(1);
	} else {
		safefree((char *)verbuf);
		safefree((char *)ver);
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetQueryOption) {

	DWORD mysize;
	char *mybuf;
	long mybufsz;
	DWORD myflag;
	DWORD mynum;
	unsigned char myc;
	int i;
  
	dXSARGS;


	if(items != 2) {
		CROAK("usage: InternetQueryOption($handle,$option);\n");
	}

	mybuf=(char *)safemalloc(16000);
	mybufsz=16000;
	myflag=(DWORD) PERLSvIV(ST(1));

	if(InternetQueryOption((HINTERNET) PERLSvIV(ST(0)),
	                       myflag, 
	                       mybuf, 
						   (DWORD *) &mybufsz)) {
        switch(myflag) {
		case INTERNET_OPTION_VERSION:
			// returns an array
			ST(0)=sv_2mortal(newSViv(((LPINTERNET_VERSION_INFO)mybuf)->dwMajorVersion));
			ST(1)=sv_2mortal(newSViv(((LPINTERNET_VERSION_INFO)mybuf)->dwMinorVersion));
			XSRETURN(2);
			break;
		case INTERNET_OPTION_CONNECT_TIMEOUT:
		case INTERNET_OPTION_CONNECT_RETRIES:
		case INTERNET_OPTION_CONNECT_BACKOFF:
		case INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT:
		case INTERNET_OPTION_CONTROL_SEND_TIMEOUT:
		case INTERNET_OPTION_DATA_RECEIVE_TIMEOUT:
		case INTERNET_OPTION_DATA_SEND_TIMEOUT:
		case INTERNET_OPTION_LISTEN_TIMEOUT:
		case INTERNET_OPTION_HANDLE_TYPE:
		case INTERNET_OPTION_READ_BUFFER_SIZE:
		case INTERNET_OPTION_WRITE_BUFFER_SIZE:
			// returns a number
			// printf("InternetQueryOption: mybufsz=%d\n",mybufsz);
			mynum=0;
			for(i=0;i<4;i++) {
				myc=*(mybuf+i);
				mynum+=myc*( i>0? 256*i : 1);
            }			
			ST(0)=sv_2mortal(newSViv(mynum));
			// ST(0)=sv_2mortal(newSViv((DWORD) *mybuf));

			safefree((char *)mybuf);

			XSRETURN(1);
			break;
		default:
			// returns a string
			ST(0)=sv_2mortal(newSVpv(mybuf,mybufsz));

			safefree((char *)mybuf);

			XSRETURN(1);
			break;
		}
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetSetOption) {

	DWORD mysize;
	void *mybuf;
	long mybufsz;
	BOOL myquerystatus;
	DWORD myflag;
	DWORD mynum;
	BOOL myretval;
  
	dXSARGS;

	if(items != 3) {
		CROAK("usage: InternetSetOption($handle,$option,$value);\n");
	}

	mybuf=(void *)safemalloc(16000);
	myflag=(DWORD) PERLSvIV(ST(1));

    switch(myflag) {
	case INTERNET_OPTION_CONNECT_TIMEOUT:
	case INTERNET_OPTION_CONNECT_RETRIES:
	case INTERNET_OPTION_CONNECT_BACKOFF:
	case INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT:
	case INTERNET_OPTION_CONTROL_SEND_TIMEOUT:			 
	case INTERNET_OPTION_DATA_RECEIVE_TIMEOUT:
	case INTERNET_OPTION_DATA_SEND_TIMEOUT:
	case INTERNET_OPTION_LISTEN_TIMEOUT:
	case INTERNET_OPTION_READ_BUFFER_SIZE:
	case INTERNET_OPTION_WRITE_BUFFER_SIZE:
		// sets a number
		mynum=(DWORD) PERLSvIV(ST(2));
		mysize=sizeof(mynum);
		myretval=InternetSetOption((HINTERNET) PERLSvIV(ST(0)),
	                               myflag, 
	                               (LPVOID) &mynum,
		                           mysize);
		XSRETURN(1);
		break;
	default:
		// sets a string
		mybuf=(LPVOID) PERLSvPV(ST(2), na);
		mysize=sizeof(mybuf);
		myretval=InternetSetOption((HINTERNET) PERLSvIV(ST(0)),
	                               myflag, 
	                               mybuf,
		                           mysize);
		break;
	}

	safefree((char *)mybuf);

	if(myretval) {
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetTimeFromSystemTime) {

    SYSTEMTIME st;
	dXSARGS;
    char mybuffer[1024];

	if(items != 8) {
		CROAK("usage: InternetTimeFromSystemTime($second, $minute, $hour, $day, $month, $year, $dayofweek, $RFC);\n");
	}

	st.wSecond       = PERLSvIV(ST(0));
	st.wMinute       = PERLSvIV(ST(1));
	st.wHour         = PERLSvIV(ST(2));
	st.wDay          = PERLSvIV(ST(3));
	st.wMonth        = PERLSvIV(ST(4));
	st.wYear         = PERLSvIV(ST(5));
	st.wDayOfWeek    = PERLSvIV(ST(6));
	st.wMilliseconds = 0;
	// printf("ITFST %d:%d:%d %d/%d/%d %d %d\n",st.wHour,st.wMinute,st.wSecond,
	//	                                     st.wDay, st.wMonth, st.wYear,
	//										 st.wDayOfWeek,PERLSvIV(ST(7)));

	if(InternetTimeFromSystemTime((CONST SYSTEMTIME *)&st,
								  (DWORD)PERLSvIV(ST(7)),
								  (LPSTR)&mybuffer,
								  (DWORD)sizeof(mybuffer))) {
		ST(0)=sv_2mortal(newSVpv(mybuffer,na));
		XSRETURN(1);
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetTimeToSystemTime) {

    SYSTEMTIME st;
	dXSARGS;

	if(items != 1) {
		CROAK("usage: InternetTimeToSystemTime($time);\n");
	}

	if(InternetTimeToSystemTime((LPCTSTR) PERLSvPV(ST(0),na),
		                        &st, 0)) {
		ST(0)=sv_2mortal(newSViv(st.wSecond));
		ST(1)=sv_2mortal(newSViv(st.wMinute));
		ST(2)=sv_2mortal(newSViv(st.wHour));
		ST(3)=sv_2mortal(newSViv(st.wDay));
		ST(4)=sv_2mortal(newSViv(st.wMonth));
		ST(5)=sv_2mortal(newSViv(st.wYear));
		ST(6)=sv_2mortal(newSViv(st.wDayOfWeek));
		XSRETURN(7);		
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetCrackUrl) {

	URL_COMPONENTS myURL;
	LPCSTR mystring;
	DWORD mysize;
	DWORD myflag;
	dXSARGS;

	if(items != 2) {
		CROAK("usage: InternetCrackUrl($URL, $flags);\n");
	}
 
	mysize=strlen((char *) PERLSvPV(ST(0), na));

	myURL.dwStructSize=sizeof(myURL);

	myURL.lpszScheme=safemalloc(mysize);
	myURL.dwSchemeLength=mysize;
    myURL.lpszHostName=safemalloc(mysize);
	myURL.dwHostNameLength=mysize;
	myURL.nPort=mysize;
	myURL.lpszUserName=safemalloc(mysize);
	myURL.dwUserNameLength=mysize;
	myURL.lpszPassword=safemalloc(mysize);
	myURL.dwPasswordLength=mysize;
	myURL.lpszUrlPath=safemalloc(mysize);
	myURL.dwUrlPathLength=mysize;
	myURL.lpszExtraInfo=safemalloc(mysize);
	myURL.dwExtraInfoLength=mysize;

	if(InternetCrackUrl((LPCSTR) PERLSvPV(ST(0), na), 
                        (DWORD) mysize,
                        (DWORD) PERLSvIV(ST(1)),
		                (LPURL_COMPONENTS) &myURL)) {
		ST(0)=sv_2mortal(newSVpv(myURL.lpszScheme,myURL.dwSchemeLength));
		ST(1)=sv_2mortal(newSVpv(myURL.lpszHostName,myURL.dwHostNameLength));
		ST(2)=sv_2mortal(newSViv(myURL.nPort));
		ST(3)=sv_2mortal(newSVpv(myURL.lpszUserName,myURL.dwUserNameLength));
		ST(4)=sv_2mortal(newSVpv(myURL.lpszPassword,myURL.dwPasswordLength));
		ST(5)=sv_2mortal(newSVpv(myURL.lpszUrlPath,myURL.dwUrlPathLength));
		ST(6)=sv_2mortal(newSVpv(myURL.lpszExtraInfo,myURL.dwExtraInfoLength)); 

		XSRETURN(7);
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetCreateUrl) {

	URL_COMPONENTS myURL;	
	HINTERNET myhandle;
	char *mybuf;
	DWORD mybuflen = 16000;

	dXSARGS;

	if(items != 8) {
		CROAK("usage: InternetCreateUrl($scheme, $hostname, $port, $username, $password, $path, $extrainfo, $flags);\n");
	}

	mybuf=(char *)safemalloc(mybuflen);
    myURL.dwStructSize=sizeof(myURL);

	myURL.lpszScheme=(char *) PERLSvPV(ST(0), na);
	myURL.dwSchemeLength=strlen((char *) PERLSvPV(ST(0), na));
	myURL.lpszHostName=(char *) PERLSvPV(ST(1), na);
	myURL.dwHostNameLength=strlen((char *) PERLSvPV(ST(1), na));
	myURL.nPort=(DWORD) PERLSvIV(ST(2));
	myURL.lpszUserName=(char *) PERLSvPV(ST(3), na);
	myURL.dwUserNameLength=strlen((char *) PERLSvPV(ST(3), na));
	myURL.lpszPassword=(char *) PERLSvPV(ST(4), na);
	myURL.dwPasswordLength=strlen((char *) PERLSvPV(ST(4), na));
	myURL.lpszUrlPath=(char *) PERLSvPV(ST(5), na);
	myURL.dwUrlPathLength=strlen((char *) PERLSvPV(ST(5), na));
	myURL.lpszExtraInfo=(char *) PERLSvPV(ST(6), na);
	myURL.dwExtraInfoLength=strlen((char *) PERLSvPV(ST(6), na));
    if(InternetCreateUrl((LPURL_COMPONENTS) &myURL,
		                 (DWORD) PERLSvIV(ST(7)),
						 (LPSTR) mybuf,
						 (LPDWORD) &mybuflen)) {
		ST(0)=sv_2mortal(newSVpv(mybuf,mybuflen)); 
		safefree((char *)mybuf);
		XSRETURN(1);
	} else {
		safefree((char *)mybuf);
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetCanonicalizeUrl) {

	char *myURL;
	DWORD myURLlen;
	dXSARGS;

	if(items != 2) {
		CROAK("usage: InternetCanonicalizeUrl($URL, $flags);\n");
	}

	myURL=(char *)safemalloc(16000);
	myURLlen=16000;

	if(InternetCanonicalizeUrl((LPCTSTR) PERLSvPV(ST(0), na),
	                           myURL,
							   &myURLlen,
		                       (DWORD) PERLSvIV(ST(1)))) {
		ST(0)=sv_2mortal(newSVpv(myURL,myURLlen));
		safefree((char *)myURL);
		XSRETURN(1);
	} else {
		safefree((char *)myURL);
		XSRETURN_NO;
	}
} 


XS(XS_Win32__Internet_InternetCombineUrl) {

	char *myURL;
	DWORD myURLlen;
	dXSARGS;

	if(items != 3) {
		CROAK("usage: InternetCombineUrl($baseURL, $relativeURL, $flags);\n");
	}

	myURL=(char *)safemalloc(16000);
	myURLlen=16000;

	if(InternetCombineUrl((LPCTSTR) PERLSvPV(ST(0), na),
	                      (LPCTSTR) PERLSvPV(ST(1), na),               
	                      myURL,
						  &myURLlen,
		                  (DWORD) PERLSvIV(ST(2)))) {
		ST(0)=sv_2mortal(newSVpv(myURL,myURLlen));
		safefree((char *)myURL);
		XSRETURN(1);
	} else {
		safefree((char *)myURL);
		XSRETURN_NO;
	}
} 



XS(XS_Win32__Internet_InternetOpenUrl) {
  HINTERNET myhandle;
  dXSARGS;

  if(items != 6) {
    CROAK("usage: InternetOpenUrl($session, $url, $headers, $length, $flags, $context);\n");
  }

  if(myhandle=InternetOpenUrl((HINTERNET) PERLSvIV(ST(0)), 
                              (LPCTSTR) PERLSvPV(ST(1), na), 
                              (LPCTSTR) PERLSvPV(ST(2), na),
                              (DWORD) PERLSvIV(ST(3)),
						      (DWORD) PERLSvIV(ST(4)),
						      (DWORD) PERLSvIV(ST(5)))) {
    XSRETURN_IV((long) myhandle);
  } else {
    XSRETURN_NO;
  }
}


XS(XS_Win32__Internet_InternetConnect) {
	HINTERNET myhandle;
	/*char mystring[80];
	SV* myvar;*/
	
	dXSARGS;
	
	if(items != 8) {
		CROAK("usage: InternetConnect($session, $server, $port, $user, $pass, $service, $flags, $context);\n");
	}

	myhandle=InternetConnect((HINTERNET) PERLSvIV(ST(0)), 
                             (LPCTSTR) PERLSvPV(ST(1), na), 
                             (INTERNET_PORT) PERLSvIV(ST(2)),
                             (LPCTSTR) PERLSvPV(ST(3), na),
                             (LPCTSTR) PERLSvPV(ST(4), na),
                             (DWORD) PERLSvIV(ST(5)),
          			         (DWORD) PERLSvIV(ST(6)),
						     (DWORD) PERLSvIV(ST(7)));
	if(myhandle) {
		/*if((DWORD) PERLSvIV(ST(7))>0) {
			ultoa((DWORD) myhandle, mystring, 10);
			strncpy((char *) &mystring+strlen(mystring), ".", 1);
		
			ultoa((DWORD) PERLSvIV(ST(7)), (char *) &mystring+strlen(mystring), 10);
			printf("InternetConnect: mystring=%s\n",mystring);
			myvar=perl_get_sv(mystring, FALSE);
			if(myvar==NULL) myvar=perl_get_sv(mystring, TRUE);
		}*/
		XSRETURN_IV((long) myhandle);
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetCloseHandle) {

	dXSARGS;

	if(items != 1) {
		CROAK("usage: InternetCloseHandle($handle);\n");
	}

	if(InternetCloseHandle((HINTERNET) PERLSvIV(ST(0)))) {
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}



/*
 *
 * FTP FUNCTIONS
 *
 */

XS(XS_Win32__Internet_FtpGetCurrentDirectory) {

	DWORD myerrnum;
	char mybuf[MAX_PATH];
	DWORD mybufsz = sizeof(mybuf);
	dXSARGS;

	if(items != 1) {
		CROAK("usage: FtpGetCurrentDirectory($handle);\n");
	}

	if(FtpGetCurrentDirectory((HINTERNET) PERLSvIV(ST(0)), (char *) &mybuf, &mybufsz)) {
		XSRETURN_PV(mybuf);
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_FtpSetCurrentDirectory) {

	dXSARGS;

	if(items != 2) {
		CROAK("usage: FtpSetCurrentDirectory($handle,$path);\n");
	}

	if(FtpSetCurrentDirectory((HINTERNET) PERLSvIV(ST(0)), 
		                      (LPCTSTR) PERLSvPV(ST(1), na))) {
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_FtpFindFirstFile) {

	HINTERNET myhandle;
	WIN32_FIND_DATA myfile;
	SYSTEMTIME mytime;
	unsigned long myCreationTime, myLastAccessTime, myLastWriteTime;
	unsigned long myFileSize;	

	dXSARGS;


	if(items != 4) {
		CROAK("usage: FtpFindFirstFile($handle,$pattern,$flags,$context);\n");
	}

	if(myhandle=FtpFindFirstFile((HINTERNET) PERLSvIV(ST(0)), 
          		                 (LPCTSTR) PERLSvPV(ST(1), na),
								 (LPWIN32_FIND_DATA) &myfile,
                                 (DWORD) PERLSvIV(ST(3)),
						         (DWORD) PERLSvIV(ST(4)))) {
		myFileSize = myfile.nFileSizeLow + 
			        (myfile.nFileSizeHigh << sizeof(myfile.nFileSizeLow));
		ST(0)=sv_2mortal(newSViv((long) myhandle));
		ST(1)=sv_2mortal(newSVpv(myfile.cFileName,0));
		ST(2)=sv_2mortal(newSVpv(myfile.cAlternateFileName,0));
		ST(3)=sv_2mortal(newSViv(myFileSize));
		ST(4)=sv_2mortal(newSViv(myfile.dwFileAttributes));


		FileTimeToSystemTime(&myfile.ftCreationTime,(LPSYSTEMTIME) &mytime);	
		ST(5)  = sv_2mortal(newSViv(mytime.wSecond));
		ST(6)  = sv_2mortal(newSViv(mytime.wMinute));
		ST(7)  = sv_2mortal(newSViv(mytime.wHour));
		ST(8)  = sv_2mortal(newSViv(mytime.wDay));
		ST(9)  = sv_2mortal(newSViv(mytime.wMonth));
		ST(10) = sv_2mortal(newSViv(mytime.wYear));

		FileTimeToSystemTime(&myfile.ftLastAccessTime,(LPSYSTEMTIME) &mytime);	
		ST(11) = sv_2mortal(newSViv(mytime.wSecond));
		ST(12) = sv_2mortal(newSViv(mytime.wMinute));
		ST(13) = sv_2mortal(newSViv(mytime.wHour));
		ST(14) = sv_2mortal(newSViv(mytime.wDay));
		ST(15) = sv_2mortal(newSViv(mytime.wMonth));
		ST(16) = sv_2mortal(newSViv(mytime.wYear));

		FileTimeToSystemTime(&myfile.ftLastWriteTime,(LPSYSTEMTIME) &mytime);	
		ST(17) = sv_2mortal(newSViv(mytime.wSecond));
		ST(18) = sv_2mortal(newSViv(mytime.wMinute));
		ST(19) = sv_2mortal(newSViv(mytime.wHour));
		ST(20) = sv_2mortal(newSViv(mytime.wDay));
		ST(21) = sv_2mortal(newSViv(mytime.wMonth));
		ST(22) = sv_2mortal(newSViv(mytime.wYear));
        XSRETURN(23);
		/*
		myCreationTime = myfile.ftCreationTime.dwLowDateTime +
			            (myfile.ftCreationTime.dwHighDateTime << 8);
						 sizeof(myfile.ftCreationTime.dwLowDateTime));
		myLastAccessTime = myfile.ftLastAccessTime.dwLowDateTime +
			              (myfile.ftLastAccessTime.dwHighDateTime << 8);
						   sizeof(myfile.ftLastAccessTime.dwLowDateTime));
		myLastWriteTime = myfile.ftLastWriteTime.dwLowDateTime +
			             (myfile.ftLastWriteTime.dwHighDateTime << 8);
					      sizeof(myfile.ftLastWriteTime.dwLowDateTime));
		 ST(5)=sv_2mortal(newSViv(myCreationTime));
		 ST(6)=sv_2mortal(newSViv(myLastAccessTime));
		 ST(7)=sv_2mortal(newSViv(myLastWriteTime));
		 XSRETURN(8);
		ST(5)=sv_2mortal(newSViv(myfile.ftCreationTime.dwLowDateTime));
		ST(6)=sv_2mortal(newSViv(myfile.ftCreationTime.dwHighDateTime));
		ST(7)=sv_2mortal(newSViv(myfile.ftLastAccessTime.dwLowDateTime));
		ST(8)=sv_2mortal(newSViv(myfile.ftLastAccessTime.dwHighDateTime));
		ST(9)=sv_2mortal(newSViv(myfile.ftLastWriteTime.dwLowDateTime));
		ST(10)=sv_2mortal(newSViv(myfile.ftLastWriteTime.dwHighDateTime));
		*/

	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_InternetFindNextFile) {

	WIN32_FIND_DATA myfile;
	SYSTEMTIME mytime;
	unsigned long myCreationTime, myLastAccessTime, myLastWriteTime;
	unsigned long myFileSize;	

	dXSARGS;


	if(items != 1) {
		CROAK("usage: InternetFindNextFile($handle);\n");
	}

	if(InternetFindNextFile((HINTERNET) PERLSvIV(ST(0)), 
						    (LPWIN32_FIND_DATA) &myfile)) {
		myFileSize = myfile.nFileSizeLow + 
			        (myfile.nFileSizeHigh << sizeof(myfile.nFileSizeLow));
		ST(0)=sv_2mortal(newSViv(1));
		ST(1)=sv_2mortal(newSVpv(myfile.cFileName,0));
		ST(2)=sv_2mortal(newSVpv(myfile.cAlternateFileName,0));
		ST(3)=sv_2mortal(newSViv(myFileSize));
		ST(4)=sv_2mortal(newSViv(myfile.dwFileAttributes));


		FileTimeToSystemTime(&myfile.ftCreationTime,(LPSYSTEMTIME) &mytime);	
		ST(5)  = sv_2mortal(newSViv(mytime.wSecond));
		ST(6)  = sv_2mortal(newSViv(mytime.wMinute));
		ST(7)  = sv_2mortal(newSViv(mytime.wHour));
		ST(8)  = sv_2mortal(newSViv(mytime.wDay));
		ST(9)  = sv_2mortal(newSViv(mytime.wMonth));
		ST(10) = sv_2mortal(newSViv(mytime.wYear));

		FileTimeToSystemTime(&myfile.ftLastAccessTime,(LPSYSTEMTIME) &mytime);	
		ST(11) = sv_2mortal(newSViv(mytime.wSecond));
		ST(12) = sv_2mortal(newSViv(mytime.wMinute));
		ST(13) = sv_2mortal(newSViv(mytime.wHour));
		ST(14) = sv_2mortal(newSViv(mytime.wDay));
		ST(15) = sv_2mortal(newSViv(mytime.wMonth));
		ST(16) = sv_2mortal(newSViv(mytime.wYear));

		FileTimeToSystemTime(&myfile.ftLastWriteTime,(LPSYSTEMTIME) &mytime);	
		ST(17) = sv_2mortal(newSViv(mytime.wSecond));
		ST(18) = sv_2mortal(newSViv(mytime.wMinute));
		ST(19) = sv_2mortal(newSViv(mytime.wHour));
		ST(20) = sv_2mortal(newSViv(mytime.wDay));
		ST(21) = sv_2mortal(newSViv(mytime.wMonth));
		ST(22) = sv_2mortal(newSViv(mytime.wYear));
        XSRETURN(23);
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_FtpCreateDirectory) {

	dXSARGS;

	if(items != 2) {
		CROAK("usage: FtpCreateDirectory($handle,$directory);\n");
	}

	if(FtpCreateDirectory((HINTERNET) PERLSvIV(ST(0)), 
          		          (LPCTSTR) PERLSvPV(ST(1), na))) {
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_FtpRemoveDirectory) {

	dXSARGS;

	if(items != 2) {
		CROAK("usage: FtpRemoveDirectory($handle,$directory);\n");
	}

	if(FtpRemoveDirectory((HINTERNET) PERLSvIV(ST(0)), 
          		          (LPCTSTR) PERLSvPV(ST(1), na))) {
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_FtpGetFile) {

	dXSARGS;

	if(items != 7) {
		CROAK("usage: FtpGetFile($handle,$remotefile,$localfile,$replace,$attr,$flags,$context);\n");
	}

	if(FtpGetFile((HINTERNET) PERLSvIV(ST(0)), 
                  (LPCTSTR) PERLSvPV(ST(1), na),
				  (LPCTSTR) PERLSvPV(ST(2), na),
				  (BOOL) PERLSvIV(ST(3)),
				  (DWORD) PERLSvIV(ST(4)),
				  (DWORD) PERLSvIV(ST(5)),
				  (DWORD) PERLSvIV(ST(6)))) {
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_FtpPutFile) {

	dXSARGS;

	if(items != 5) {
		CROAK("usage: FtpPutFile($handle,$localfile,$remotefile,$flags,$context);\n");
	}

	if(FtpPutFile((HINTERNET) PERLSvIV(ST(0)), 
                  (LPCTSTR) PERLSvPV(ST(1), na),
				  (LPCTSTR) PERLSvPV(ST(2), na),
				  (DWORD) PERLSvIV(ST(3)),
				  (DWORD) PERLSvIV(ST(4)))) {
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_FtpRenameFile) {

	DWORD myerrnum;
	unsigned char mybuf[MAX_PATH*2];
	DWORD mybufsz = sizeof(mybuf);
	dXSARGS;

	if(items != 3) {
		CROAK("usage: FtpRenameFile($handle,$oldname,$newname);\n");
	}

	if(FtpRenameFile((HINTERNET) PERLSvIV(ST(0)), 
		             (LPCTSTR) PERLSvPV(ST(1), na),
					 (LPCTSTR) PERLSvPV(ST(2), na))) {
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_FtpDeleteFile) {

	DWORD myerrnum;
	unsigned char mybuf[MAX_PATH*2];
	DWORD mybufsz = sizeof(mybuf);

	dXSARGS;

	if(items != 2) {
		CROAK("usage: FtpDeleteFile($handle,$filename);\n");
	}

	if(FtpDeleteFile((HINTERNET) PERLSvIV(ST(0)), 
		             (LPCTSTR) PERLSvPV(ST(1), na))) {
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}


/*
 *
 * HTTP FUNCTIONS
 *
 */


XS(XS_Win32__Internet_HttpOpenRequest) {
	HINTERNET myhandle;
  
	dXSARGS;

	if(items != 8) {
		CROAK("usage: HttpOpenRequest($handle, $verb, $object, $version, $referer, $accept, $flags, $context);\n");
	}

	myhandle=HttpOpenRequest((HINTERNET) PERLSvIV(ST(0)), 
                             (LPCTSTR) PERLSvPV(ST(1), na), 
						     (LPCTSTR) PERLSvPV(ST(2), na), 
						     (LPCTSTR) PERLSvPV(ST(3), na), 
						     (LPCTSTR) PERLSvPV(ST(4), na), 
						     (LPCTSTR FAR *) PERLSvPV(ST(5), na), 
          			         (DWORD) PERLSvIV(ST(6)),
						     (DWORD) PERLSvIV(ST(7)));
	if(myhandle) {
		XSRETURN_IV((long) myhandle);
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_HttpSendRequest) {

	dXSARGS;

	if(items != 5) {
		CROAK("usage: HttpSendRequest($handle, $headers, $headerslen, $postdata, $postdatalen);\n");
	}

	if(HttpSendRequest((HINTERNET) PERLSvIV(ST(0)), 
                     (LPCTSTR) PERLSvPV(ST(1), na), 
  	                 (DWORD) PERLSvIV(ST(2)),
  					 (LPVOID) PERLSvPV(ST(3), na), 
  					 (DWORD) PERLSvIV(ST(4)))) {
     /*
	if(HttpSendRequest((HINTERNET) PERLSvIV(ST(0)), 
                       NULL, 0, NULL, 0)) {
	*/
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_HttpQueryInfo) {

	LPVOID mybuf;
	DWORD mysize=16000;
	DWORD myindex=0;
   	DWORD myflag=0;

	dXSARGS;

	mybuf=(void *)safemalloc(mysize);

	if(items != 3) {
		CROAK("usage: HttpQueryInfo($handle, $flags, $header);\n");
	}

	myflag=(unsigned long) PERLSvIV(ST(1));

	// printf("HttpQueryInfo: got flags=%u (header=%s)\n",myflag,(char *) PERLSvPV(ST(2),na)); 

	if((DWORD) PERLSvIV(ST(1))==HTTP_QUERY_CUSTOM) {
		strcpy((char *) mybuf,(char *) PERLSvPV(ST(2),na));
		mysize=strlen((char *) PERLSvPV(ST(2),na));
		// printf("HttpQueryInfo: requesting CUSTOM header '%s' (len=%d)...\n",(char *) mybuf, mysize);
	}

    if(HttpQueryInfo((HINTERNET) PERLSvIV(ST(0)), 
					  myflag, 
					  (LPVOID) mybuf,
					  (LPDWORD) &mysize,
					  (LPDWORD) &myindex)) {
		if(myflag & HTTP_QUERY_FLAG_SYSTEMTIME) {
			// printf("HttpQueryInfo: requested SYSTEMTIME (date is %d/%d/%d)\n",((LPSYSTEMTIME) mybuf)->wDay,((LPSYSTEMTIME) mybuf)->wMonth,((LPSYSTEMTIME) mybuf)->wYear);
			ST(0) = sv_2mortal(newSViv(((LPSYSTEMTIME) mybuf)->wSecond));
			ST(1) = sv_2mortal(newSViv(((LPSYSTEMTIME) mybuf)->wMinute));
			ST(2) = sv_2mortal(newSViv(((LPSYSTEMTIME) mybuf)->wHour));
			ST(3) = sv_2mortal(newSViv(((LPSYSTEMTIME) mybuf)->wDay));
			ST(4) = sv_2mortal(newSViv(((LPSYSTEMTIME) mybuf)->wMonth));
			ST(5) = sv_2mortal(newSViv(((LPSYSTEMTIME) mybuf)->wYear));
			safefree((char *)mybuf);
			XSRETURN(6);
		} else {
			ST(0) = sv_2mortal(newSVpv((char *)mybuf,mysize));
			safefree((char *)mybuf);
			XSRETURN(1);
		}
	} else {
		safefree((char *)mybuf);
		XSRETURN_NO;
	}
}


XS(XS_Win32__Internet_HttpAddRequestHeaders) {

	dXSARGS;
   
	if(items != 3) {
		CROAK("usage: HttpAddRequestHeaders($handle, $header, $flags);\n");
	}

	if(HttpAddRequestHeaders((HINTERNET) PERLSvIV(ST(0)), 
					         (LPCTSTR) PERLSvPV(ST(1),na),
					         (DWORD) -1,
					         (DWORD) PERLSvIV(ST(2)))) {
		XSRETURN_YES;
	} else {
		XSRETURN_NO;
	}
}


XS(boot_Win32__Internet)
{
    dXSARGS;
    char* file = __FILE__;
	char* mythread;

    newXS("Win32::Internet::constant", XS_Win32__Internet_constant, file);

	newXS("Win32::Internet::InternetTimeToSystemTime", XS_Win32__Internet_InternetTimeToSystemTime, file);
	newXS("Win32::Internet::InternetTimeFromSystemTime", XS_Win32__Internet_InternetTimeFromSystemTime, file);
    newXS("Win32::Internet::InternetOpen", XS_Win32__Internet_InternetOpen, file);
    newXS("Win32::Internet::InternetOpenUrl", XS_Win32__Internet_InternetOpenUrl, file);
	newXS("Win32::Internet::InternetCrackUrl", XS_Win32__Internet_InternetCrackUrl, file);
	newXS("Win32::Internet::InternetCreateUrl", XS_Win32__Internet_InternetCreateUrl, file);
	newXS("Win32::Internet::InternetCanonicalizeUrl", XS_Win32__Internet_InternetCanonicalizeUrl, file);
	newXS("Win32::Internet::InternetCombineUrl", XS_Win32__Internet_InternetCombineUrl, file);
    newXS("Win32::Internet::InternetQueryDataAvailable", XS_Win32__Internet_InternetQueryDataAvailable, file);
    newXS("Win32::Internet::InternetReadFile", XS_Win32__Internet_InternetReadFile, file);
    newXS("Win32::Internet::InternetGetLastResponseInfo", XS_Win32__Internet_InternetGetLastResponseInfo, file);
	newXS("Win32::Internet::InternetFindNextFile", XS_Win32__Internet_InternetFindNextFile, file);
    newXS("Win32::Internet::InternetQueryOption", XS_Win32__Internet_InternetQueryOption, file);
    newXS("Win32::Internet::InternetSetOption", XS_Win32__Internet_InternetSetOption, file);
	newXS("Win32::Internet::InternetCloseHandle", XS_Win32__Internet_InternetCloseHandle, file);
	newXS("Win32::Internet::InternetConnect", XS_Win32__Internet_InternetConnect, file);
	newXS("Win32::Internet::InternetSetStatusCallback", XS_Win32__Internet_InternetSetStatusCallback, file);
    newXS("Win32::Internet::InternetDllVersion", XS_Win32__Internet_InternetDllVersion, file);

	newXS("Win32::Internet::FtpGetCurrentDirectory", XS_Win32__Internet_FtpGetCurrentDirectory, file);
	newXS("Win32::Internet::FtpSetCurrentDirectory", XS_Win32__Internet_FtpSetCurrentDirectory, file);
	newXS("Win32::Internet::FtpFindFirstFile", XS_Win32__Internet_FtpFindFirstFile, file);
	newXS("Win32::Internet::FtpCreateDirectory", XS_Win32__Internet_FtpCreateDirectory, file);
	newXS("Win32::Internet::FtpRemoveDirectory", XS_Win32__Internet_FtpRemoveDirectory, file);
	newXS("Win32::Internet::FtpGetFile", XS_Win32__Internet_FtpGetFile, file);
	newXS("Win32::Internet::FtpPutFile", XS_Win32__Internet_FtpPutFile, file);
	newXS("Win32::Internet::FtpRenameFile", XS_Win32__Internet_FtpRenameFile, file);
	newXS("Win32::Internet::FtpDeleteFile", XS_Win32__Internet_FtpDeleteFile, file);

	newXS("Win32::Internet::HttpOpenRequest", XS_Win32__Internet_HttpOpenRequest, file);
	newXS("Win32::Internet::HttpSendRequest", XS_Win32__Internet_HttpSendRequest, file);
	newXS("Win32::Internet::HttpQueryInfo", XS_Win32__Internet_HttpQueryInfo, file);
	newXS("Win32::Internet::HttpAddRequestHeaders", XS_Win32__Internet_HttpAddRequestHeaders, file);

	ST(0) = &sv_yes;
    XSRETURN(1);
}
