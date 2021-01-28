#ifndef __UTIL_H__
#define __UTIL_H__

#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QVariant>
#include <QRegExp>
#include <QMap>


QString getField(QStringList , QRegExp, int);
QString getField(QStringList , QString, int);
QString parseVars(QString, QMap<QString, QString> more=QMap<QString, QString>());
QDateTime parseDateTime(QStringList);


template <class T>
T parseField(QStringList list, QRegExp key, T dft, int idx=0) {	
	QString res = getField(list, key, idx);
	if (res == QString())
		return dft;
	return QVariant(res).value<T>();
}


template<class T>
T parseField(QStringList list, QString key, T dft, int idx=0) {
	return parseField(list, QRegExp(key), dft, idx);
}


int execute(QString);
bool isServiceRunning(QString);
void stopService(QString);
void delay(int);

#endif
