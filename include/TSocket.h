
#ifndef __TSOCKET_H__
#define __TSOCKET_H__

#include <QTcpSocket>
#include <QObject>
#include <functional>
#include <QThread>


class TSocket : public QTcpSocket {
	Q_OBJECT
public:
	TSocket(qintptr desc, QObject *parent=nullptr);
	void setFirstIncommingMessage(std::function<bool (TSocket *)>);
	void setLogo(std::function<QString (void)> logo) { mLogo = logo; };
	QString getLogo() { return mLogo(); };

	void setStreamHost(QString host) { mStreamHost = host; }
	void setStreamPort(qintptr port) { mStreamPort = port; }
	QString streamHost() { return mStreamHost; }
	qintptr streamPort() { return mStreamPort; }
	bool connectStream(QString host, qintptr port);


signals:
	void onClose(TSocket*);
	
protected:
	void run();

private:
	void build();
	void configure();
	void clean();
	void syncSockets(QTcpSocket *, QTcpSocket *);

	QTcpSocket *mStream;
	bool alreadyClose{false};
	std::function<QString (void)> mLogo;
	std::function<bool (TSocket*)> mFirstMessage;
	bool mIsFirstMessage{true};
	QString mStreamHost;
	int mStreamPort;

};




#endif
