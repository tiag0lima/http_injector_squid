TEMPLATE = 	app

HEADERS = 	../include/DefaultResponse.h include/FirstRequest.h \
		../include/Parser.h include/TSocket.h include/UserData.h \
		../include/Util.h include/squid.h ../include/Data.h


SOURCES = 	src/FirstRequest.cpp src/TSocket.cpp src/UserData.cpp \
		../src/Util.cpp src/main.cpp src/squid.cpp


INCLUDEPATH = 	include/ ../include/

OBJECTS_DIR = 	./objects
MOC_DIR = 	./objects

DESTDIR = 	../
TARGET = 	squid

QT += 		core network
QT -=		gui

CONFIG += 	c++17



