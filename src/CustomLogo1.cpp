#include "CustomLogo1.h"
#include <QTime>
#include <QDebug>

CustomLogo1::CustomLogo1(QObject *parent) : QObject(parent) {
}


QString CustomLogo1::getLogo() {
	QString tm = QTime::currentTime().toString("HH:mm");
	return tr("HTTP/1.0 200 %1  &spades; JESUS TE AMA  &spades;\r\n\r\n").arg(tm);
}

