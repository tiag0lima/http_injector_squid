#ifndef __DATA_H__
#define __DATA_H__

#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>

class UserData : public QObject {

public:
	UserData(QString usersPath, QObject *parent=nullptr);
	
	void parseRequest(QStringList);
	QString getResponse(bool &ok);

	QString name() { return mName; }
	qintptr version() { return mVersion; }
	bool isExpired() { return QDateTime::currentDateTime() > mTime; }
	bool isTooOld() { return mCurrentVersion > mVersion; }

	QDateTime dateTime() { return mTime; };
	QString okReponse() { return mOkResponse; }
	QString errorResponse() { return mErrorReponse; }
	QString upgradeResponse() { return mUpgradeResponse; }

private:
	void build();
	void configure();
	void setConfig(QStringList);
	bool loadUserConfigFile(QString);
	QMap<QString, QString> getMapInfo();
	QString getRegressiveDateTime();

	QString mUsersPath;

	QString mName{QString()};
	float mVersion{0};
	
	QDateTime mTime{QDateTime()};
	QString mOkResponse{QString()};
	QString mErrorReponse{QString()};
	QString mUpgradeResponse{QString()};
	QString mExpiredResponse{QString()};
	float mCurrentVersion{0};

	enum ERROR_CODE {
		BAD_REQUEST,
		INTERNAL_ERROR,
		USER_NOT_FOUND,
		USER_EXPIRED,
		VERSION_TOO_OLD,
		NONE
	};

	UserData::ERROR_CODE mErrorCode{UserData::ERROR_CODE::NONE};
};


#endif
