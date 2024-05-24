QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    AControl \
    AEditor \
    Converter \
    LineEdit \
    MainWindow \
    Memory \
    Processor \
    TANumber \
    TANumber/CNumber \
    TANumber/FNumber \
    TANumber/PNumber

SOURCES += \
    Converter/converter_10_p.cpp \
    Converter/converter_p_10.cpp \
    MainWindow/mainwindow.cpp \
    main.cpp \

HEADERS += \
    AControl/control.h \
    AEditor/editor.h \
    Converter/converter_10_p.h \
    Converter/converter_p_10.h \
    LineEdit/input_line_edit.h \
    MainWindow/mainwindow.h \
    Memory/memory.h \
    Processor/processor.h \
    TANumber.h \
    TANumber/CNumber/cnumber.h \
    TANumber/FNumber/fnumber.h \
    TANumber/PNumber/pnumber.h \
    TANumber/TANumber.h \

FORMS += \
    MainWindow/mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
