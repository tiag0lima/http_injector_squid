#include "Util.h"
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <unistd.h>
#include <iostream>
#include <stdio_ext.h>

QString getField(QStringList list, QRegExp key, int idx) {
	for (QString s : list) {
		int index = key.indexIn(s);
		if (index > -1)
			return key.capturedTexts()[idx];
	}
	return QString();
}

QString getField(QStringList list, QString s, int idx) {
	return getField(list, QRegExp(s), idx);
}


QString parseVars(QString s, QMap<QString, QString> more) {
	more["[date]"] = QDate::currentDate().toString();
	more["[time]"] = QTime::currentTime().toString();
	more["[datetime]"] = QDateTime::currentDateTime().toString();
	more["[crlf]"] = "\r\n";
	more["[lf]"] = "\n";
	more["[http_ok]"] = "HTTP/1.0 200";
	more["[http_upgrade]"] = "HTTP/1.0 426";
	more["[http_expired]"] = "HTTP/1.0 423";
	more["[http_error]"] = "HTTP/1.0 400";

	
	for (QMap<QString, QString>::iterator it = more.begin(); it != more.end(); it++)
		s.replace(it.key(), it.value());
	return s.remove(QRegExp("^(\ ){1,}"));
}


QDateTime parseDateTime(QStringList list) {
 	QStringList formats;
	formats << "d-M-y h:m" << "d-M-yy h:m" << "d-M-yyyy h:m";
	formats << "d/M/y h:m" << "d/M/yy h:m" << "d/M/yyyy h:m";
	formats << "d-M-y" << "d-M-yy" << "d-M-yyyy";
	formats << "d/M/y" << "d/M/yy" << "d/M/yyyy";
	QString stm = parseField<QString>(list, "datetime:(\\ ){0,}(([0-9]){1,}[-/]([0-9]){1,}[-/]([0-9]){1,}( [0-9]{1,}:([0-9]){1,})?)", "01-01-11 00:00", 2);
	for (QString format : formats) {
		QDateTime dt = QDateTime::fromString(stm, format);
		if (dt.isValid())
			return dt;
	}
	return QDateTime();
}


int execute(QString cmd) {

	FILE *fp = popen(cmd.toLatin1(), "r");
	char s[1024];
	if (fp == NULL) {
	       qDebug() << "can't execute command "+cmd;
       	       return -1;
	}
	if (fgets(s, 1024, fp) != NULL)
		qDebug() << s;
	return pclose(fp);

}

void delay(int l) {
	for (int x = l; x > 0; x--) {
		std::cout << "\r\033[J\033[" << 35-(x%4) << "m" << x; 
		std::cout.flush();
		sleep(1);
	}
	std::cout << "\r\033[J\033[0mPRESS [ENTER] TO CONTINUE...";
	getchar();
	std::cout << "continue";
	std::cout.flush();
}
	


bool isServiceRunning(QString service) {
	FILE *proc = popen(QString("ps -axu | grep %1 | grep -v grep").arg(service).toLatin1(), "r");
	char tmp[10];
	bool status = false;
	if (not proc)
		qDebug() << "Can't check service "+service;
	else if (fgets(tmp, 10, proc) != NULL)
		status = true;
	fclose(proc);
	return status;
}

void stopService(QString service) {	
	FILE *proc = popen(QString("ps -axu | grep %1 | grep -v grep | sed -E \"s/(\ ){1,}/\ /g\" | cut -d ' ' -f2").arg(service).toLatin1(), "r");
	char *line;
	size_t line_size, line_buff_size=0;
	if (not proc)
		qDebug() << "Can't check service "+service;
	else while ((getline(&line, &line_buff_size, proc)) > 0) {
		system(QString("kill -9 %1").arg(line).toLatin1());
	}
	fclose(proc);
}
