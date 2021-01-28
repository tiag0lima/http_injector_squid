#include "UserData.h"

#include "DefaultResponse.h"
#include "Util.h"

#include <QRegExp>
#include <QFile>

#include <QDebug>

UserData::UserData(QString usersPath, QObject *parent) : QObject(parent) {
	mUsersPath = usersPath;
}


void UserData::parseRequest(QStringList list) {
	mName = parseField<QString>(list, "name:(\\ ){0,}(([a-zA-Z0-9]){1,})", "None", 2);
	if (mName == "None") { 
		mErrorCode = UserData::BAD_REQUEST;
		return;
	}
	mVersion = parseField(list, "version:(\\ ){0,}([0-9].[0-9])", 1.0, 2);
	
	if (loadUserConfigFile(mName)) {
		if (isExpired())
			mErrorCode = UserData::ERROR_CODE::USER_EXPIRED;
		else if (isTooOld())
			mErrorCode = UserData::ERROR_CODE::VERSION_TOO_OLD;
	}
}



QString UserData::getResponse(bool &ok) {
	ok = mErrorCode == UserData::ERROR_CODE::NONE;

	switch (mErrorCode) {
		case UserData::ERROR_CODE::BAD_REQUEST:
			return HTTP_BAD_REQUEST;
		case UserData::ERROR_CODE::INTERNAL_ERROR:
			return HTTP_INTERNAL_ERROR;
		case UserData::ERROR_CODE::USER_NOT_FOUND:
			return tr(HTTP_USER_NOT_FOUND).replace("[name]", mName.toUpper());
		case UserData::ERROR_CODE::USER_EXPIRED:
			return mExpiredResponse;
		case UserData::ERROR_CODE::VERSION_TOO_OLD:
			return mUpgradeResponse;
	}
	return mOkResponse;
}


bool UserData::loadUserConfigFile(QString name) {
	QFile file(mUsersPath+"/"+name);
	if (not file.open(QIODevice::ReadOnly))
		mErrorCode = UserData::USER_NOT_FOUND;
	else {
		QStringList list = QString(file.readAll().data()).split('\n');
		file.close();
		setConfig(list);
		return true;
	}
	return false;
}


void UserData::setConfig(QStringList list) {
	 mTime = parseDateTime(list);
  	 auto info = getMapInfo();
	 mCurrentVersion = parseVars(parseField<QString>(list, "version:(\\ ){0,}([0-9].[0-9]|\\[version\\])", HTTP_ERROR, 2), info).toFloat();
	 mOkResponse = parseVars(parseField<QString>(list, "ok_response:(\\ ){0,}(.*)", HTTP_OK, 2), info);
	 mUpgradeResponse = parseVars(parseField<QString>(list, "upgrade_response:(\\ ){0,}(.*)", HTTP_NEED_UPGRADE, 2), info);
	 mExpiredResponse = parseVars(parseField<QString>(list, "expired_response:(\\ ){0,}(.*)", HTTP_EXPIRED, 2), info);
	 mErrorReponse = parseVars(parseField<QString>(list, "error_response:(\\ ){0,}(.*)", HTTP_ERROR, 2), info);

}


QMap<QString, QString> UserData::getMapInfo() {
	QMap<QString, QString> info;
	info["[name]"] = mName.toUpper();
	info["[version]"] = mVersion;
	info["[regressive_datetime]"] = getRegressiveDateTime();
	return info;
}


QString UserData::getRegressiveDateTime() {
	QDateTime cdt = QDateTime::currentDateTime();
	qint64 days = cdt.daysTo(mTime);
	qint64 secs = cdt.secsTo(mTime)-(60*60*12*days);
	qint64 hours = secs/(60*60);
	secs -= hours*(60*60);
	qint64 mins = secs/60;
	secs -= mins*60;
	
	QString ret;
	if (days > 0) ret += tr("%1D|").arg(days);
	if (hours > 0) ret += tr("%1H|").arg(hours);
	else if (mins > 0) ret += tr("%1M|").arg(mins);
	else if (secs > 0) ret += tr("%1S|").arg(secs);
	if (not ret.isEmpty()) ret = ret.mid(0, ret.length()-1);
	return ret;
}


