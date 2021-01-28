#ifndef __STATUS_H__
#define __STATUS_H__

#define HTTP_OK "[http_ok] <font color=\"green\">OK</font>[crlf][crlf]"
#define HTTP_INTERNAL_ERROR "HTTP/1.0 500 <font color=\"yellow\">INTERNAL ERROR</font>[crlf][crlf]"
#define HTTP_BAD_REQUEST "HTTP/1.0 400 <font color=\"red\">BAD REQUEST</font>[crlf][crlf]<h1 color=\"red\">BAD REQUEST.</h1>[crlf][crlf]"
#define HTTP_USER_NOT_FOUND "HTTP/1.0 404 <font color=\"purple\">USER [name] NOT FOUND</font>[crlf][crlf]"
#define HTTP_NEED_UPGRADE "HTTP/1.0 426 <font color=\"cyan\">UPGRADE REQUIRED</font>[crlf][crlf]" 
#define HTTP_EXPIRED "HTTP/1.0 423 <font color=\"red\">LOCKED</font>[crlf][crlf]"
#define HTTP_ERROR "HTTP/1.0 401 <font color=\"red\">UNAUTHORIZED</font>[crlf][crlf]"


#endif
