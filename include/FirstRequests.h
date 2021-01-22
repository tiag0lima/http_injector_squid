#ifndef __FIRST_REQUESTS_H__
#define __FIRST_REQUESTS_H__

#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QStringList>
#include "status.h"
#include "TSocket.h"


bool parseFistsRequets(TSocket*);
float parseVersion(QStringList);
float currentVersion();

#endif
