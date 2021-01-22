#include "TSocket.h"
#include "status.h"
#include <unistd.h>


TSocket::TSocket(qintptr desc, QObject *parent) 
	: QTcpSocket(parent) {
	build();
	configure();
	setSocketDescriptor(desc);
}

void TSocket::build() {
	mStream = new QTcpSocket(static_cast<QObject*>(this));

}

bool TSocket::connectStream(QString host, qintptr port) {
	mStream->connectToHost(host, port);
	return mStream->waitForConnected(10000);	
}

void TSocket::configure() {
	setFirstIncommingMessage([](QTcpSocket*) { return false; });	
	

	QObject::connect (this, &QTcpSocket::readyRead, [&] () {
		if (mIsFirstMessage) {
			mIsFirstMessage = false;
			if (not mFirstMessage(this))
				disconnectFromHost();
			return;
		}
		if (not alreadyClose) {
			mStream->write(readAll());
		}
	});

	
	QObject::connect (mStream, &QTcpSocket::readyRead, [&] () {	
		if (not alreadyClose) {
			write(mStream->readAll());
		}
	});

	QObject::connect (this, &QTcpSocket::disconnected, [&]() { 
		if (not alreadyClose) {
			alreadyClose = true;
			if (mStream && mStream->isOpen()) mStream->disconnectFromHost();
			emit onClose(this);
		}
	});

	QObject::connect (mStream, &QTcpSocket::disconnected, [&]() {
		if (not alreadyClose) {
			alreadyClose = true;
			if (isOpen()) disconnectFromHost();
			emit onClose(this);
		}
	});

}



void TSocket::setFirstIncommingMessage(std::function<bool (TSocket *)> firstMessage) {
	mFirstMessage = firstMessage;
}	





