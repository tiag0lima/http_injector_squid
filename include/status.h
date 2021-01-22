#ifndef __STATUS_H__
#define __STATUS_H__


#define HTTP_OK "HTTP/1.0 200 OK\r\n\r\nEverything is ok\r\n\r\n"
#define HTTP_INTERNAL_ERROR "HTTP/1.0 500 INTERNAL ERROR\r\n\r\n"
#define HTTP_BAD_REQUEST "HTTP/1.0 401 BAD REQUEST\r\n\r\n<h1 color=\"red\">UNKNOWN REQUEST.</h1>\r\n\r\n"
#define HTTP_NEED_UPGRADE "HTTP/1.0 406 Current config is too old, Upgrade is needed..\r\n\r\n"

#endif
