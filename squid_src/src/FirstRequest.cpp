#include "FirstRequest.h"
#include "DefaultResponse.h"
#include "UserData.h"
#include "Util.h"
#include <QRegExp>
#include <QFile>


bool parseFirstRequest(TSocket *s) {
	QString req = s->readAll().data();
	qDebug() << req;

	int req_len = req.length();
	if (req.length() > 1024) {
		s->write(parseVars(HTTP_BAD_REQUEST).toLatin1());
		s->flush();
		qDebug() << "UNKNOW REQUESTS SIZE";
		return false;
	}
	

	bool ok;
	QStringList list = req.remove('\r').split('\n');
	UserData data(s->usersFolder());
	data.parseRequest(list);
	QString toSend = parseVars(data.getResponse(ok));
	qDebug() << toSend;

	if (not s->connectStream(s->streamHost(), s->streamPort())) {
		s->write(parseVars(HTTP_INTERNAL_ERROR).toLatin1());
		s->flush();
		qDebug() << "error connect internal";
		return false;
	}

	s->write(toSend.toLatin1());
	s->flush();
	return ok;
}




