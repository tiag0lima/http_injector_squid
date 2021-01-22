#ifndef __CUSTOM_LOGO_1_H__
#define __CUSTOM_LOGO_1_H__


#include <QObject>
#include <QString>

class CustomLogo1 : public QObject {
	Q_OBJECT
public:
	CustomLogo1(QObject *parent=nullptr);
	
	QString getLogo();


};



#endif
