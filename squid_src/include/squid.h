#ifndef __SQUID_H__
#define __SQUID_H__

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QList>

#include <functional>
#include "TSocket.h"

class squid : public QTcpServer {
	Q_OBJECT
public:
	squid(QObject *parent=nullptr);

	void run();
	
	void setUsersFolder(QString);
	void setSourceHost(QString);
	QString sourceHost();
	void setSourcePort(qintptr);
	int sourcePort();

	void setDestinationHost(QString);
	void setDestinationPort(qintptr);
	QString destinationHost();
	qintptr destinationPort();

signals:
	void exit(int);

protected:
	void incomingConnection(qintptr desc);

private:
	void configure();

	QString mSourceHost,
		mStreamHost;
	qintptr mSourcePort, 
		mStreamPort;

	QTcpServer *mServer;
	QString mUsersFolder;
};




#endif
