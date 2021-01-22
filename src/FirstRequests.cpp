#include "FirstRequests.h"


bool parseFistsRequets(TSocket *s) {
	QString req = s->readAll().data();
	qDebug() << req;
	
	if (not req.contains("CONNECT")) {
		s->write(HTTP_BAD_REQUEST);
		s->flush();
		qDebug() << "CONNECT FIELD NOT FOUND";	
		return false;
	}

	QStringList lreq = req.split(' ');
	int req_len = lreq.length();
	if (req_len < 1 || req_len > 30 || req.length() > 1024) {
		s->write(HTTP_BAD_REQUEST);
		s->flush();
		qDebug() << "UNKNOW REQUESTS SIZE";
		return false;
	}
	
	float client_version = parseVersion(lreq);	
	float server_version = currentVersion();
	qDebug() << "client version " << client_version << endl;
	if (client_version < server_version) {
		s->write(HTTP_NEED_UPGRADE);
		s->flush();
		qDebug() << "OLD VERSION";
		return false;
	}

	if (not s->connectStream(s->streamHost(), s->streamPort())) {
		s->write(HTTP_INTERNAL_ERROR);
		s->flush();
		qDebug() << "error connect internal";
		return false;
	}


	s->write(s->getLogo().toLatin1());
	s->flush();
	return true;
}



float currentVersion() {
	QFile file("/etc/http_squid/version");
	if (not file.open(QIODevice::ReadOnly)) {
		qDebug() << file.errorString();
		return 1.0;
	}
	bool ok;
	float version = file.readLine().toFloat(&ok);
	file.close();
	if (not ok)
		return 1.0;
	return version;
}


float parseVersion(QStringList info) {
	QRegExp version_expr("version\([0-9]?\.[0-9]?\)");
	for (QString s : info)
		if (s.contains(version_expr)) {
			return s.mid(8, s.length()-9).toFloat();
		}
	return 1.0;
}	



