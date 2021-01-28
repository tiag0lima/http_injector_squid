
#ifndef __TSOCKET_H__
#define __TSOCKET_H__

#include <QTcpSocket>
#include <QObject>
#include <functional>


class TSocket : public QTcpSocket {
	Q_OBJECT
public:
	TSocket(qintptr desc, QObject *parent=nullptr);
	void setFirstIncommingMessage(std::function<bool (TSocket *)>);

	void setStreamHost(QString host) { mStreamHost = host; }
	void setStreamPort(qintptr port) { mStreamPort = port; }
	QString streamHost() { return mStreamHost; }
	qintptr streamPort() { return mStreamPort; }
	bool connectStream(QString host, qintptr port);
	void setUsersFolder(QString usersFolder) { mUsersFolder = usersFolder; }
	QString usersFolder() { return mUsersFolder; }


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
	bool mIsFirstMessage{true}, mHasSecondMessage{false};
	QString mStreamHost;
	QString mUsersFolder;
	int mStreamPort;
};




#endif
