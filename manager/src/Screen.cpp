#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "Screen.h"
#include "ScreenUtil.h"

using std::cout, std::cerr, std::cin, std::endl, std::string;

std::ostream& operator<<(std::ostream& out, const QString& s) {
	out << s.toStdString();
	return out;
}


Screen::Screen(QObject *parent) : QObject(parent) {
	mLogo = [](){return QString();};
}



void Screen::setLogo(std::function<QString (void)> logo) {
	mLogo = logo;
}

void Screen::printOptions(QMap<int, QString> options, int focus) {
	cout << "\033[0;0H\033[2J";
	QString logo = mLogo();
        if (not logo.isEmpty())
		cout << logo << endl;
	cout.flush();
	int l=0;
	for (QMap<int, QString>::iterator it = options.begin(); it != options.end(); it++) {
		if (l++ == focus)
			cout << "[*]\t\033[42m";
		else
			cout << "[" <<  it.key() << "]\t";
		cout << QString(it.value()) << endl;
		cout << "\033[40m";
	}

}


void Screen::focusLine(int line) {
	printOptions(mOptions, line);
}


int Screen::getChooseOption(QMap<int, QString> options) {
	printOptions(options);
	mOptions = options;

	termios t, t_saved;
	
	tcgetattr(fileno(stdin), &t);
	t_saved = t;
	
	t.c_lflag &= (~ICANON & ~ECHO);
	t.c_cc[VTIME] = 0;
	t.c_cc[VMIN] = 1;
	
	if (tcsetattr(fileno(stdin), TCSANOW, &t) < 0) {
		cout << "Unable to set terminal to single characte mode" << endl;
		cout << "\r\033[J>> ";
		std::string tmp;
		cin >> tmp;
		mAll_in = tmp.data();
	} else {
		std::streambuf *pbuf = cin.rdbuf();
		char ch;
		mIsNav = false;
		mCurrentPos = 0;
		while (true) {
			if (mIsNav) mAll_in = std::to_string(mCurrentPos).c_str();

			if (mAll_in.toInt() >= mOptions.count()) {
				mAll_in = mAll_in.mid(1, mAll_in.length());
				if (mAll_in.toInt() >= mOptions.count())
					mAll_in = std::to_string(mOptions.count()-1).c_str();
			}
			focusLine(mAll_in.toInt());
		 	cout << "\r\033[J>> " << mAll_in.toInt();
			cout.flush();
			if (pbuf->sgetc() == EOF) break;
			ch = pbuf->sbumpc();
			if (ch == '\n') break;
			
			parseIn(ch);
			
		}
		cout << endl;
		if (tcsetattr(fileno(stdin), TCSANOW, &t_saved) < 0)
			cerr << "Unable to restore terminal" << endl;
	}

	return mAll_in.toInt();;
}



void Screen::parseIn(int ch) {
	switch (ch) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			mAll_in += ch;
			mIsNav = false;
			mCurrentPos = 0;
			break;
		case 0x08:
		case 0x7f:
			mAll_in = mAll_in.mid(0, mAll_in.length()-1);
			mIsNav = false;
			mCurrentPos = 0;
			break;
	};

	mLasts[0] = mLasts[1];	
	mLasts[1] = mLasts[2];	
	mLasts[2] = ch;	
	
	if (mLasts[0] == 27 && mLasts[1] == 91) {
		mIsNav = true;
		switch (ch) {
			case 65:
			case 68:
				if (mCurrentPos <= 0)
				       mCurrentPos = mOptions.count();
				mCurrentPos -= 1;
				break;
			case 66:
			case 67:
				mCurrentPos += 1;
				if (mCurrentPos >= mOptions.count())
				       mCurrentPos = 0;
				break;
			case 70:
				mCurrentPos = mOptions.count()-1;
				break;
			case 72:
				mCurrentPos = 0;
				break;
			default:
				mIsNav = false;
		}
	}
}
	
void Screen::parseIn2(int ch) {
	if (ch == 0x7f && mCurrentPos > 0) {
		mAll_in.remove(mCurrentPos-1, 1);
		mCurrentPos -= 1;
	}
	if (ch > 0x20 and ch < 0x7F || ch == 32) {
		mAll_in.insert(mCurrentPos, ch);
		mCurrentPos += 1;
	}
	
	mLasts[0] = mLasts[1];	
	mLasts[1] = mLasts[2];	
	mLasts[2] = ch;	
	
	if (mLasts[0] == 27 && mLasts[1] == 91) {
		mIsNav = true;
		mCurrentPos -= 2;
		mAll_in.remove(mCurrentPos, 2);
		switch (ch) {
			case 65:
			case 68:
				if (mCurrentPos <= 0)
				       mCurrentPos = mAll_in.length()+1;
				mCurrentPos -= 1;
				break;
			case 66:
			case 67:
				mCurrentPos += 1;
				if (mCurrentPos > mAll_in.length())
				       mCurrentPos = 0;
				break;
			case 70:
				mCurrentPos = mAll_in.length();
				break;
			case 72:
				mCurrentPos = 0;
				break;
			default:
				mIsNav = false;
		}
	}
}
	



QString Screen::getString(QString title, QString dft, bool passmode, bool not_clean) {
	termios t, t_saved;
	mAll_in = dft;
        int n;

        tcgetattr(fileno(stdin), &t);
        t_saved = t;

        t.c_lflag &= (~ICANON & ~ECHO);
        t.c_cc[VTIME] = 0;
        t.c_cc[VMIN] = 1;

        if (tcsetattr(fileno(stdin), TCSANOW, &t) < 0) {
                cout << "Unable to set terminal to single characte mode" << endl;
                cout << "\r\033[J" << title << ": ";
		std::string tmp;
		cin >> tmp;
		mAll_in = tmp.data();
        } else {
                std::streambuf *pbuf = cin.rdbuf();
		char ch;
		QString toprint;
		mIsNav = false;
		mCurrentPos = mAll_in.length();
                while (true) {	
		       	if (not passmode) toprint = mAll_in;
			else {toprint.resize(mAll_in.size()); toprint.fill('*'); }
			if (not not_clean) {
				cout << "\033[0;0H\33[0J";
				QString logo = mLogo();
				if (not logo.isEmpty())
				       cout << logo << endl;
			}
			cout << "\r\033[J" << title << ": " << toprint;
			cout << "\r\033[" << mCurrentPos+1+title.length()+1 << "C";
			cout.flush();
			if (pbuf->sgetc() == EOF) break;
			ch = pbuf->sbumpc();
			if (ch == '\n' and mAll_in.length() > 0) break;
                        parseIn2(ch);
                }
		cout << endl;
                if (tcsetattr(fileno(stdin), TCSANOW, &t_saved) < 0)
                        cerr << "Unable to restore terminal" << endl;
        }

        return mAll_in;
}



QString Screen::getPassword(QString title, bool not_clean) {
	return getString(title, "", true, not_clean);
}



