#include <QCoreApplication>
#include <functional>
#include <QObject>
#include <QString>
#include <QDebug>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <QMap>
#include "Parser.h"
#include "Menu.h"
#include "Data.h"
#include "Util.h"
#include "UserManager.h"
#include "CustomLogo.h"


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

	Menu menu(data);
	UserManager userManager(data.USERS_FOLDER);
	CustomLogo customLogo;

	auto onClose = [&] () { pgm_exit(app, 0); };


	auto updateServicesStates = [&]() {
		bool squidState = userManager.squidState();
		bool sshState = userManager.sshState();

		menu.setSquidState(squidState);
		menu.setSshState(sshState);
		customLogo.setServiceState("squid", 80, squidState);
		customLogo.setServiceState("ssh", 22, sshState);
	};


	QObject::connect (&menu, &Menu::exit, onClose);
	QObject::connect (&menu, &Menu::addNewUser, &userManager, &UserManager::addUser);
	QObject::connect (&menu, &Menu::deleteUser, &userManager, &UserManager::removeUser);
	
	QObject::connect (&menu, &Menu::changeSquidState, &userManager, &UserManager::setSquidState); 
	QObject::connect (&menu, &Menu::changeSshState, &userManager, &UserManager::setSshState);
	QObject::connect (&menu, &Menu::updateUser, [&](QString name){
		int max = userManager.maxLogin(name);
		menu.updateUserFields(name, max, userManager.userFileInfo(name));
	});
	QObject::connect (&menu, &Menu::listUsers, [&](){
		QStringList users = userManager.usersList();
		QMap<QString, QPair<int, int>> usersInfo;
		for (QString user : users) {
			int logged = userManager.lenLogged(user);
			int max = userManager.maxLogin(user);
			usersInfo[user] = QPair<int, int>(logged, max);
		}
		menu.showUsers(usersInfo);
	});
	QObject::connect (&userManager, &UserManager::ready, [&](bool ok){
		updateServicesStates();
		menu.setUsersList(userManager.usersList());
		delay(0);
		menu.run();
	});


	menu.setUsersList(userManager.usersList());
	menu.setLogo(std::bind(&CustomLogo::getLogo, &customLogo));
	updateServicesStates();
	menu.run();

	app.exec();
	return 0;
}

