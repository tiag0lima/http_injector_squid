#ifndef __USER_MANAGER_H__
#define __USER_MANAGER_H__


#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QString>
#include <QMap>
#include "Data.h"

class UserManager : public QObject {
	Q_OBJECT
public:
	UserManager(QString, QObject *parent=nullptr);

	bool addUser(QStringList, qint64, qint64, QMap<QString, QString>, bool emitReady=true);
	bool updateUser(QStringList, qint64, qint64, QMap<QString, QString>, bool emitReady=true);
	bool removeUser(QString, bool emitReady=true);
	QStringList usersList();
	QMap<QString, QString> userFileInfo(QString);
	
	int lenLogged(QString);
	void setMaxLogin(QString, int);
	void removeLimit(QString);
	int maxLogin(QString);
	bool squidState();
	bool sshState();
	void setSquidState(bool, Data data);
	void setSshState(bool);

signals:
	void ready(bool);

private:
	bool createUser(QString, QString, QDateTime);
	bool createUserFile(QString, QMap<QString, QString>);
	bool deleteUserFile(QString);
	bool deleteUser(QString);

	QString mUserPath;
};



#endif
