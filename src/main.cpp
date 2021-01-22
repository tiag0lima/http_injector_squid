#include <QCoreApplication>
#include <functional>
#include <QString>
#include <QDebug>
#include "squid.h"

#include "CustomLogo1.h"
#include "Parser.h"



int main(int argc, char *argv[]) {
	Data data;
	if (not parse_parameter(argv, argc, data))
		return 127;
	
	QCoreApplication app(argc, argv);
	

	CustomLogo1 clogo;
	std::function<QString (void)> customLogo = [&clogo](){return clogo.getLogo();};


	
	squid sq;
	sq.setCustomLogo(customLogo);
	sq.setSourceHost(data.SOURCE_HOST);
	sq.setSourcePort(data.SOURCE_PORT);
	sq.setDestinationHost(data.DEST_HOST);
	sq.setDestinationPort(data.DEST_PORT);

	sq.run();

	app.exec();
	return 0;
}

