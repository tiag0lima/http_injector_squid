#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <QObject>
#include <QMap>
#include <QString>
#include <termios.h>
#include <functional>

class Screen : public QObject {

public:
	Screen(QObject *parent=nullptr);

	int getChooseOption(QMap<int, QString>);
	QString getString(QString title, QString dft="", bool passMode=false, bool not_clean=false);
	QString getPassword(QString title, bool not_clean=false);
	void setLogo(std::function<QString (void)>);
private:
	void printOptions(QMap<int, QString>, int focus=-1);
	void focusLine(int line);
	void parseIn(int);
	void parseIn2(int);


	QMap<int, QString> mOptions;
	QString mAll_in;
	char mLasts[3];
	int mCurrentPos;
	bool mIsNav;
	std::function<QString (void)> mLogo;
};



#endif
