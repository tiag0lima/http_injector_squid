TEMPLATE = 	app

HEADERS = 	../include/DefaultResponse.h  ../include/Parser.h \
		../include/Util.h include/Screen.h include/ScreenUtil.h \
		include/UserManager.h include/Menu.h include/CustomLogo.h \
		../include/Data.h


SOURCES = 	../src/Util.cpp src/main.cpp src/Screen.cpp src/UserManager.cpp \
		src/Menu.cpp src/CustomLogo.cpp


INCLUDEPATH += 	include/ ../include


OBJECTS_DIR = 	objects/
MOC_DIR = 	objects/

DESTDIR = 	../
TARGET = 	main

QT += 		core
QT -=		gui

CONFIG += 	c++17



