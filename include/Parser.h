#ifndef __PARSER_H__
#define __PARSER_H__

#include <QString>
#include <QStringList>

typedef struct Data {
	int SOURCE_PORT;
	int DEST_PORT;
	QString SOURCE_HOST = "0.0.0.0";
	QString DEST_HOST = "127.0.0.1";
} Data;

void show_usage(QString reason, QString name) {
	qDebug() << reason 
		<< "\n\nusage: " << name << " [options] <SOURCE_PORT> <DESTINATION_PORT>\n"
		<< "\n\tSOURCE_PORT\t\tit will accept connections on port"
		<< "\n\tDESTINATION_PORT\twill redirect all input to port"
		<< "\n\noptions:"
		<< "\n\t--source_host source_host\tchange source host"
		<< "\n\t--dest_host destination_host\tchange destination host"
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


bool parse_parameter(char *argv[], int argc, Data &data) {
	QStringList mArgs = toQStringList(argv, argc);

	auto incorrect = [mArgs](QString reason) {
		show_usage(reason, mArgs[0]);
		return false;
	};

	if (has_parameter(mArgs, "--help"))
		return incorrect("");
	
	QString source_host = getParameter(mArgs, "--source_host");
	if (source_host != QString())
		data.SOURCE_HOST = source_host;
	

	QString dest_host = getParameter(mArgs, "--dest_host");
	if (dest_host != QString())
		data.DEST_HOST = dest_host;

	if (mArgs.length() != 3)
		return incorrect(QString("[%1] SOURCE_PORT and DESTINATION_PORT not found.").arg(mArgs.length()));

	bool ok;
	data.SOURCE_PORT = mArgs[1].toInt(&ok);
	if (not ok) return incorrect("SOURCE_PORT unknown");

	data.DEST_PORT = mArgs[2].toInt(&ok);
	if (not ok) return incorrect("DESTINATION_PORT unknown");

	return true;
}

#endif
