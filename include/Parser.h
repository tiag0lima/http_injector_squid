#ifndef __PARSER_H__
#define __PARSER_H__

#include <QDir>
#include <QDebug>
#include <QString>
#include <QVariant>
#include <QStringList>
#include "Data.h"

void show_usage(QString reason, QString name) {
	qDebug() << reason 
		<< "\n\nusage: " << name << " [options]\n"
		<< "\noptions:"
		<< "\n\t--source-port source_port\tdefault 80"
		<< "\n\t--dest-port dest_port\tdefault 22"
		<< "\n\t--source-host source_host\tdefault 0.0.0.0"
		<< "\n\t--dest-host destination_host\tdefault 127.0.0.1"
		<< "\n\t--users-folder users_folder\tdefault /var/squid/users"
		<< "\n\t--help\t\t\tshow this message";
}


QStringList toQStringList(char *s[], int len) {
	QStringList ret;
	for (int x = 0; x < len; x++)
		ret << QString(s[x]);
	return ret;
}

int has_parameter(QStringList sl, QString s) {
	int len = sl.length();
	for (int p = 0; p < len; p++)
		if (sl[p].contains(s))
			return p;
	return NULL;
}


QString getParameter(QStringList &params, QString param) {
	int parg = has_parameter(params, param);
	QString ret = QString();
	if (parg > 0) {
		ret = params[parg+1];
		params.removeAt(parg+1);
		params.removeAt(parg);
	}
	return ret;
}


bool fix_dir(QString dirname) {
	if (not QDir().mkpath(dirname)) {
		qDebug() << "Cannot create path " << dirname;
		return false;
	}
	return true;
}


template<class T>
void setParameter(T &d, QString s) {
	if (s != QString())
		d = QVariant(s).value<T>();
}


bool parse_parameter(char *argv[], int argc, Data &data) {
	QStringList mArgs = toQStringList(argv, argc);

	auto incorrect = [mArgs](QString reason) {
		show_usage(reason, mArgs[0]);
		return false;
	};

	if (has_parameter(mArgs, "--help"))
		return incorrect("");
	
	setParameter(data.SOURCE_PORT, getParameter(mArgs, "--source-port"));
	setParameter(data.DEST_PORT, getParameter(mArgs, "--dest-port"));

	setParameter(data.SOURCE_HOST,  getParameter(mArgs, "--source-host"));
	setParameter(data.DEST_HOST, getParameter(mArgs, "--dest-host"));

	setParameter(data.USERS_FOLDER, getParameter(mArgs, "--users-folder"));

	mArgs.removeAll(argv[0]); //remove ./thisProgram
	if (mArgs.count() > 0)
		return incorrect("unknown:\n\t"+mArgs.join("\n\t"));
	
	return fix_dir(data.USERS_FOLDER);
}



#endif
