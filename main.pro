TEMPLATE = 	app

HEADERS = 	include/squid.h include/TSocket.h \
		include/CustomLogo1.h include/Parser.h \
       		include/status.h include/FirstRequests.h

SOURCES = 	src/main.cpp src/squid.cpp src/TSocket.cpp \
		src/CustomLogo1.cpp src/FirstRequests.cpp

INCLUDEPATH = 	include/

OBJECTS_DIR = 	./objects
MOC_DIR = 	./objects

TARGET = 	main

QT += 		core network
QT -=		gui

CONFIG += 	c++17

