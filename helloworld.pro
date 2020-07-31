TEMPLATE = app
LANGUAGE = C++
QT += core gui opengl widgets
CONFIG += c++11

CONFIG += debug_and_release warn_on
QMAKE_CXXFLAGS += /MP

QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE   += $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

CONFIG(debug,debug|release){
        TARGET = helloworldd
        DESTDIR  = ../build/debug
        TEMP_DESTDIR = ../build/intermediate/debug/$$TARGET
}
CONFIG(release,debug|release){
        TARGET = helloworld
        DESTDIR  = ../build/release
        TEMP_DESTDIR = ../build/intermediate/release/$$TARGET
}

MOC_DIR         = $$TEMP_DESTDIR/moc
RCC_DIR         = $$TEMP_DESTDIR/rcc
UI_DIR          = $$TEMP_DESTDIR/qui
OBJECTS_DIR     = $$TEMP_DESTDIR/obj

INCLUDEPATH  +=  \
        ../third_party/math \

HEADERS += \
	Camera.h \
	glwidget.h \
	Drawable.h \
	osgNode.h \

SOURCES += \
    main.cpp \
    glwidget.cpp \
	Drawable.cpp \
	osgNode.cpp \