QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    AControl \
    AControl/PControl \
    AControl/FControl \
    AControl/CControl \
    AEditor \
    AEditor/PEditor \
    AEditor/FEditor \
    AEditor/CEditor \
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
    AControl/AControl.cpp \
    AControl/CControl/ccontrol.cpp \
    AControl/FControl/fcontrol.cpp \
    AControl/PControl/pcontrol.cpp \
    AEditor/AEditor.cpp \
    AEditor/CEditor/ceditor.cpp \
    AEditor/FEditor/feditor.cpp \
    AEditor/PEditor/peditor.cpp \
    Converter/converter_10_p.cpp \
    Converter/converter_p_10.cpp \
    MainWindow/mainwindow.cpp \
    main.cpp \

HEADERS += \
    AControl/AControl.h \
    AControl/CControl/ccontrol.h \
    AControl/FControl/fcontrol.h \
    AControl/PControl/pcontrol.h \
    AControl/control.h \
    AEditor/AEditor.h \
    AEditor/CEditor/ceditor.h \
    AEditor/FEditor/feditor.h \
    AEditor/PEditor/peditor.h \
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
