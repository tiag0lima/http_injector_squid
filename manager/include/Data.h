#ifndef __DATA_H__
#define __DATA_H__


typedef struct Data {
	int SOURCE_PORT = 80;
	int DEST_PORT = 22;
	QString SOURCE_HOST = "0.0.0.0";
	QString DEST_HOST = "127.0.0.1";
	QString USERS_FOLDER = "/var/squid/users";
} Data;


#endif
