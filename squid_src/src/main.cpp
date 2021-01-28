#include <QCoreApplication>
#include <functional>
#include <QObject>
#include <QString>
#include <QDebug>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <QMap>
#include "squid.h"
#include "Parser.h"
#include "Util.h"


void pgm_exit(QCoreApplication &app, int code) {
	std::cout << "\r\033[J" << std::endl;	
	app.exit(code);
	exit(code);
}


int main(int argc, char *argv[]) {
	Data data;
	if (not parse_parameter(argv, argc, data))
		return 127;
	
	QCoreApplication app(argc, argv);

	squid sq;

	auto onClose = [&] () { pgm_exit(app, 0); };
	QObject::connect (&sq, &squid::exit, onClose);

	sq.setSourceHost(data.SOURCE_HOST);
	sq.setSourcePort(data.SOURCE_PORT);
	sq.setDestinationHost(data.DEST_HOST);
	sq.setDestinationPort(data.DEST_PORT);
	sq.setUsersFolder(data.USERS_FOLDER);
	sq.run();
	
	app.exec();
	return 0;
}

