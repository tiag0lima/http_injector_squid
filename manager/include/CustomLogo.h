#ifndef __CUSTOM_LOGO_H__
#define __CUSTOM_LOGO_H__


#include <QObject>
#include <QPair>
#include <QMap>

class CustomLogo : public QObject {

public:
	CustomLogo(QObject *parent=nullptr);

	QString getLogo();
	
	void setServiceState(QString, int, bool);
private:
	

	QString repeatCh(int , char);
	QString putAtmiddle(int , QString);
	QString toLogo(QStringList);
	QStringList servicesInfo();
	QStringList devicesInfo();

	QPair<qint64, qint64> getMemoryPercent();
	QPair<qint64, qint64> getSwapPercent();
	int getCpuPercent();
	QMap<QString, QPair<int, bool>> mServices;

};



#endif
