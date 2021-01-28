#include "squid.h"

#include <QDebug>
#include <QThread>
#include <unistd.h>
#include <sys/socket.h>
#include "FirstRequest.h"
#define MAX 1024

squid::squid(QObject *parent) : QTcpServer(parent){
};


void squid::run() {
	return configure();
}


void squid::configure() {
	QHostAddress sourceHost = QHostAddress(mSourceHost);
	if (not listen(sourceHost, mSourcePort)) {
		qDebug() << "can't listen on " << mSourceHost << ":" << mSourcePort;
		emit exit(0);
		return;
	}

	qDebug() << "listening on " << mSourceHost << ":" << mSourcePort
		<< " -> forwarding " << mStreamHost << ":" << mStreamPort;
}


void squid::incomingConnection(qintptr desc) {
	qDebug() << "new connection incoming";

	TSocket *sock = new TSocket(desc);
	connect (sock, &TSocket::onClose, [](TSocket *s) {
		qDebug() << "deleting one connection";
		s->deleteLater();
	});

	sock->setFirstIncommingMessage(parseFirstRequest);
	sock->setStreamHost(mStreamHost);
	sock->setStreamPort(mStreamPort);
	sock->setUsersFolder(mUsersFolder);
}



void squid::setUsersFolder(QString usersFolder) {
	mUsersFolder =  usersFolder;
}


void squid::setSourceHost(QString host) {
	mSourceHost = host;
}

QString squid::sourceHost() {
	return mSourceHost;
}

void squid::setSourcePort(qintptr port) {
	mSourcePort = port;
}

int squid::sourcePort() {
	return mSourcePort;
}

void squid::setDestinationHost(QString host) {
	mStreamHost = host;
}

void squid::setDestinationPort(qintptr port) {
	mStreamPort = port;
}

QString squid::destinationHost() {
	return mStreamHost;
}

qintptr squid::destinationPort() {
	return mStreamPort;
}

