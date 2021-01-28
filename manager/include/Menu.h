#ifndef __MENU_H__
#define __MENU_H__

#include <QPair>
#include <QString>
#include <QObject>
#include "Screen.h"
#include "UserManager.h"
#include "Data.h"


class Menu : public QObject {
	Q_OBJECT
public:
	Menu(Data data, QObject *parent=nullptr);

	void setLogo(std::function<QString (void)>);
	bool updateUserFields(QString, int, QMap<QString, QString>);
	void setUsersList(QStringList);
	void showUsers(QMap<QString, QPair<int,int>>);
	void setSquidState(bool);
	void setSshState(bool);
	void run();

signals:
	void addNewUser(QStringList, qint64, qint64, QMap<QString, QString>, bool);
	void deleteUser(QString, bool);
	void updateUser(QString);
	void listUsers();
	void changeSquidState(bool, Data);
	void changeSshState(bool);
	void exit();

private:
	void build();
	void configure();
	void showVars();
	int getMainMenu();
	bool getNewUser();
	bool getDeleteUser();
	bool getUpdateUser();
	void onChangeSquid();
	
	Screen *mScreen;
	QStringList mUsersList;
	QStringList mStates;

	enum {
		APPEND_USER,
		DELETE_USER,
		UPDATE_USER,
		LIST_USERS,	
		CHANGE_SQUID,
		CHANGE_SSH,
		EXIT
	};

	bool mSquidState;
	bool mSshState;
	Data mData;
};



#endif
