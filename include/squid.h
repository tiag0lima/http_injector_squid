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

	void setSourceHost(QString);
	QString sourceHost();
	void setSourcePort(qintptr);
	int sourcePort();

	void setDestinationHost(QString);
	void setDestinationPort(qintptr);
	QString destinationHost();
	qintptr destinationPort();

	void setCustomLogo(std::function<QString (void)>);

protected:
	void incomingConnection(qintptr desc);

private:
	void configure();

	QString mSourceHost{"0.0.0.0"},
		mStreamHost{"127.0.0.1"};
	qintptr mSourcePort{80}, mStreamPort{22};

	QTcpServer *mServer;
	std::function<QString (void)> mLogo;
};




#endif
