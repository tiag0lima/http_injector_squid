#include "UserManager.h"
#include <QProcess>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QDebug>
#include <unistd.h>
#include "Util.h"

#define USER 0
#define PASS 1


UserManager::UserManager(QString userPath, QObject *parent) : QObject(parent) {
	mUserPath = userPath;
}





int UserManager::lenLogged(QString name) {
	QStringList list;
	QProcess proc;
	proc.start("ps", QStringList() << "-ax");
	proc.waitForStarted();
	proc.waitForFinished();

	if (proc.exitCode() != 0) {
		qDebug() << proc.readAllStandardError();
		return NULL;
	}
	QStringList out = tr(proc.readAllStandardOutput().data()).split('\n');
	return out.filter(QRegExp(name+"@pts")).count(); 
}



bool UserManager::addUser(QStringList cts, qint64 days, qint64 max, QMap<QString, QString> params, bool emitReady) {
	QDateTime dt;
	dt = QDateTime::currentDateTime().addDays(days);
	params["datetime"] = dt.toString("d-M-yyyy h:m");
	if (not createUser(cts[USER], cts[PASS], dt)) {
		if (emitReady) emit ready(false);
		return false;
	} else if (not createUserFile(cts[USER], params)) {
		if (emitReady) emit ready(false);
		return false;
	}
	setMaxLogin(cts[USER], max);
	if (emitReady) emit ready(true);
	return true;
}


bool UserManager::updateUser(QStringList cts, qint64 days, qint64 max, QMap<QString, QString> args, bool emitReady) {
	if (not removeUser(cts[USER])) {
		if (emitReady) emit ready(false);
		return false;
	} else if (not addUser(cts, days, max, args)) {
		if (emitReady) emit ready(false);
		return false;
	}
	if (emitReady) emit ready(true);
	return true;
}



bool UserManager::createUser(QString name, QString password, QDateTime dt) {
	qDebug() << "appending user " << name;
	QString cmd = "useradd ";
	QStringList args;
	args << "-e "+dt.toString("yyyy-MM-dd");
	args << "--no-create-home";
	args << "--shell /bin/false";
	args << name;
	cmd += args.join(" ");
	if (execute(cmd) != 0) {
		qDebug() << "Can't append user " << name;
		return false;
	}
	cmd = tr("echo \"%1:%2\" | chpasswd").arg(name).arg(password);
	if (execute(cmd) != 0) {
		qDebug() << "Can't set password to user " << name;
		return false;
	}
	qDebug() << "User" << name << "created with sucess.";
	return true;
}


bool UserManager::removeUser(QString name, bool emitReady) {
	if (not deleteUser(name)) {
		if (emitReady) emit ready(false);
		return false;
	}
	if (not deleteUserFile(name)) {
		if (emitReady) emit ready(false);
		return false;
	}

	if (emitReady) emit ready(true);
	return true;
}


bool UserManager::deleteUser(QString name) {
	qDebug() << "removing user " << name;
	QString cmd = "deluser ";
	QStringList args;
	args << "--remove-all-files";
	args << "--remove-home";
	args << name;
	args << "2> /dev/null";
	args << "> /dev/null";
	cmd += args.join(" ");
	if (execute(cmd) != 0) {
		qDebug() << "Can't delete user " << name;
		return false;
	}
	removeLimit(name);
	qDebug() << "User" << name << "removed with sucess.";
	return true;
}


bool UserManager::createUserFile(QString name, QMap<QString, QString> params) {
	qDebug() << "Creating " << mUserPath+"/"+name;
	QString s;
	QFile f(mUserPath+"/"+name);
	f.open(QIODevice::WriteOnly | QIODevice::Text);
	if (not f.isOpen())
		return false;
	for (QMap<QString, QString>::iterator it=params.begin(); it != params.end(); it++) {
		s = it.key();
		s += ": ";
		s += it.value();
		s += "\n";
		f.write(s.toLatin1());
	}
	f.flush();
	f.close();
	qDebug() << mUserPath+"/"+name << " created with sucess";
	return true;
}


bool UserManager::deleteUserFile(QString name) {
	QString pn = mUserPath+"/"+name;
	qDebug() << "removing "+pn;
	if (execute("rm -rf "+pn) != 0) {
		qDebug() << "can't remove "+pn;
		return false;
	}
	qDebug() << pn << "remove with sucess.";
	return true;
}



QStringList UserManager::usersList() {
	QDir dir(mUserPath);
	QStringList ret = dir.entryList(QStringList() << "*");
	ret.removeAll(".");
	ret.removeAll("..");
	return ret;
}


QMap<QString, QString> UserManager::userFileInfo(QString name) {
	QMap<QString, QString> ret;
	QFile file(mUserPath+"/"+name);
	file.open(QIODevice::ReadOnly);
	if (not file.isOpen()) {
		qDebug() << file.errorString();
		return QMap<QString, QString>();
	}

	QString line, key, value;
	QStringList ls;
	while (true) {
		line = file.readLine(1024).data();
		if (line.isEmpty())
			break;
		ls = line.split(':');
		key = ls[0];
		ls.removeAt(0);
		value = ls.join(':');
		ret[key] = value.split('\n')[0];
	}
	file.close();
	return ret;
}



void UserManager::setMaxLogin(QString name, int max) {
	removeLimit(name);
	QFile file("/etc/security/limits.conf");
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        if (not file.isOpen()) {
                qDebug() << file.errorString();
		return;
	}
	file.write(tr("%1\thard\tmaxlogins\t%2\n").arg(name).arg(max).toLatin1());
	file.flush();
	file.close();
}


void UserManager::removeLimit(QString name) {
	QString cmd;
	cmd = "cat /etc/security/limits.conf | grep -v "+name;
	cmd += " > /tmp/limits.conf";
	cmd += "&& mv /tmp/limits.conf /etc/security/limits.conf";
	
	system(cmd.toLatin1());
}

int UserManager::maxLogin(QString name) {
	QFile file("/etc/security/limits.conf");
	file.open(QIODevice::ReadOnly);
	if (not file.isOpen()) {
		qDebug() << file.errorString();
		return NULL;
	}
	QStringList list = tr(file.readAll().data()).split('\n').filter(QRegExp("^"+name+"\thard\tmaxlogins\t([0-9]){1,}$"));
	if (list.count() == 0) return 0;
	file.close();
	return list[0].split('\t')[3].toInt();
}


bool UserManager::squidState() {
	return isServiceRunning("squid");
}


bool UserManager::sshState() {
	return isServiceRunning("ssh");
}


void UserManager::setSquidState(bool state, Data data) {
	if (state == false) {
		qDebug() << "Stoping squid";
		stopService("squid");
		if (squidState() == false)
			qDebug() << "Squid stopped with sucess.";
		else
			qDebug() << "Can't stop squid.";
	} else {
		qDebug() << "Starting squid";
		QStringList args;
		args << tr("--source-host %1").arg(data.SOURCE_HOST);
		args << tr("--source-port %1").arg(data.SOURCE_PORT);
		args << tr("--dest-host %1").arg(data.DEST_HOST);
		args << tr("--dest-port %1").arg(data.DEST_PORT);
		args << tr("--users-folder %1").arg(data.USERS_FOLDER);
		system(tr("./squid %1 >> ./squid.log 2>> squid.log&").arg(args.join(' ')).toLatin1());
		if (squidState() == true)
			qDebug() << "Squid started with sucess.";
		else
			qDebug() << "Can't stat squid.";

	}

	emit ready(true);
}


void UserManager::setSshState(bool state) {
	if (state == true) {
		qDebug() << "Starting ssh.";
		system("/etc/init.d/ssh start");
		if (sshState() == false) {
			qDebug() << "Can't start ssh.";
			emit ready(false);
		}
		else
			qDebug() << "Ssh started with sucess.";
	} else {
		qDebug() << "Stopping ssh.";
		system("/etc/init.d/ssh stop");
		if (sshState() == true) {
			qDebug() << "Can't stop ssh.";
			emit ready(false);
		}
		else
			qDebug() << "Ssh stopped with sucess.";
	}
	emit ready(true);
}

