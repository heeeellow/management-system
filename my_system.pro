# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------
QT += axcontainer core gui sql widgets network
TEMPLATE = app
TARGET = my_system
DESTDIR = ./x64/Release
CONFIG += release plugin
LIBS += -L"."
DEPENDPATH += .
MOC_DIR += .
OBJECTS_DIR += release
UI_DIR += .
RCC_DIR += .
include(my_system.pri)

#Installs the plugin in the plugins directory.
target.path = $$[QT_INSTALL_PLUGINS]
INSTALLS += target
RC_FILE = logo.rc
