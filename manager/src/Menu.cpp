#include "Menu.h"
#include <iostream>
#include <QDebug>
#include <QDateTime>
#include "Util.h"
#include "DefaultResponse.h"


using std::cout;
using std::endl;

Menu::Menu(Data data, QObject *parent) : QObject(parent) {
	build();
	configure();
	mData = data;
}


void Menu::build() {
	mScreen = new Screen(this);

}


void Menu::configure() {
	mStates << "Enable" << "Disable";
	mSquidState = false;
	mSshState = false;
}

void Menu::setLogo(std::function<QString (void)> logo) {
	mScreen->setLogo(logo);
}


void Menu::showVars() {
	cout << "\nwarning: don't use \\r or \\n use [crlf] or [lf] instead." << endl
	     << "info: vars list" << endl
	     << "\t[crlf] [lf] [datetime] [date] [time]" << endl
	     << "\t[http_ok] [http_upgrade] [http_expired] [http_error]" << endl
	     << "\t[name] [version] [regressive_datetime]" << endl << endl;
}


void Menu::setUsersList(QStringList list) {
	mUsersList = list;
}


void Menu::run() {
	while (true) {
		switch(getMainMenu()) {
			case APPEND_USER:
				if (getNewUser()) return;
				break;
			case DELETE_USER:
				if (getDeleteUser()) return;
				break;
			case UPDATE_USER:
				if (getUpdateUser()) return;
				break;
			case LIST_USERS:
				emit listUsers(); return;
				break;
			case CHANGE_SQUID:
				onChangeSquid(); return;
				break;
			case CHANGE_SSH:
				emit changeSshState(!mSshState); return;
				break;
			case EXIT:
				emit exit(); return;
				break;
		}
	}
}



int Menu::getMainMenu() {
	QMap<int, QString> options;
	options[APPEND_USER] = "append user";
	options[DELETE_USER] = "delete user";
	options[UPDATE_USER] = "update user";
	options[LIST_USERS] = "list users";
	options[CHANGE_SQUID] = mStates[mSquidState]+" squid";
	options[CHANGE_SSH] = mStates[mSshState]+" ssh";
	options[EXIT] = "exit";	
	return mScreen->getChooseOption(options);
}


bool Menu::getNewUser() {
	QMap<QString, QString> responses;
	QStringList cts;
	qint64 days;
	qint64 max;
	
	try {
		cts << mScreen->getString("Name");
		cts << mScreen->getString("Password", "", true, true);
		days = mScreen->getString("How much days", "30", false, true).toInt();
		max = mScreen->getString("Max login", "10", false, true).toInt();
			
		showVars();
		responses["ok_response"] = mScreen->getString("ok_response", \
		       	HTTP_OK, false, true);
		responses["upgrade_response"] = mScreen->getString("upgrade_response", \
		       	HTTP_NEED_UPGRADE, false, true);
		responses["expired_response"] = mScreen->getString("upgrade_response", \
		       	HTTP_EXPIRED, false, true);
		responses["error_response"] = mScreen->getString("error_response", \
		       	HTTP_NEED_UPGRADE, false, true);

	
		emit addNewUser(cts, days, max, responses, true);
		mUsersList << cts[0];
		return true;
	} catch (...) {
		return false;
	}
}



bool Menu::getDeleteUser() {
	try {
		QMap<int, QString> options;
		options[0] = "..";
		int p = 1;
		for (QString s : mUsersList) options[p++] = s;
		int user = mScreen->getChooseOption(options);
		if (user == 0) return false;
		QString name = mUsersList[user-1];
		mUsersList.removeAll(name);
		emit deleteUser(name, true);
		return true;
	} catch (...) {
		return false;
	}
}



bool Menu::getUpdateUser() {
	try {
		QMap<int, QString> options;
		options[0] = "..";
		int p = 1;
		for (QString s : mUsersList) options[p++] = s;
		int user = mScreen->getChooseOption(options);
		if (user == 0) return false;
		QString name = mUsersList[user-1];
		emit updateUser(name);
		return true;
	} catch (...) {
		return false;
	}
}



bool Menu::updateUserFields(QString name, int max_default, QMap<QString, QString> fields) {
	QMap<QString, QString> responses;
	QStringList cts;
	qint64 max;
	
	try {
		
		cts << mScreen->getString("Name", name);
		cts << mScreen->getString("Password", "", true, true);
		max = mScreen->getString("Max login", QString::number(max_default), false, true).toInt();
		showVars();
		for (QMap<QString, QString>::iterator it = fields.begin(); it != fields.end(); it++)
			responses[it.key()] = mScreen->getString(it.key(), it.value(), false, true);
		
		QDateTime dt = parseDateTime(QStringList() << "datetime: "+responses["datetime"]);
		qint64 days = QDateTime::currentDateTime().daysTo(dt);

		emit deleteUser(name, false);
		emit addNewUser(cts, days, max, responses, true);
		mUsersList.removeAll(name);
		mUsersList.append(cts[0]);
		return true;
	} catch (...) {
		return false;
	}

}


void Menu::showUsers(QMap<QString, QPair<int, int>> users) {
	try {
		QMap<int, QString> options;
		options[0] = "\033[35;47mUSER\tLOGGED\tMAX\033[0m";
		int p = 1;
		for (QMap<QString, QPair<int, int>>::iterator it = users.begin(); it != users.end(); it++) {
			QString info =  it.key();
			info += "\t"+QString::number(it.value().first);
			info += "\t"+QString::number(it.value().second);
			options[p++] = info;
		}

		int user = mScreen->getChooseOption(options);
		run();
	} catch (...) {}
	 
}


void Menu::onChangeSquid() {
	if (! mSquidState) {
		mData.SOURCE_HOST = mScreen->getString("Source host", mData.SOURCE_HOST, false, false);
		mData.SOURCE_PORT = mScreen->getString("Source port", QString::number(mData.SOURCE_PORT), false, true).toInt();
		mData.DEST_HOST = mScreen->getString("Destination host", mData.DEST_HOST, false, true);
		mData.DEST_PORT = mScreen->getString("Destination port", QString::number(mData.DEST_PORT), false, true).toInt();
		mData.USERS_FOLDER = mScreen->getString("Users folder", mData.USERS_FOLDER, false, true);
	}
	changeSquidState(!mSquidState, mData);
}


void Menu::setSquidState(bool state) {
	mSquidState = state;
}

void Menu::setSshState(bool state) {
	mSshState = state;
}
