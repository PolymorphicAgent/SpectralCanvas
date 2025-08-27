QT       += core gui multimedia quick quickwidgets svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/progressmessagebox.cpp \
    src/audiofile.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/progressmessagebox.h \
    src/audiofile.h \
    src/mainwindow.h

FORMS += \
    ui/mainwindow.ui

DISTFILES += \
    todo.txt \
    README.md

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# include fftw
win32: LIBS += $$PWD/libs/fftw-3.3.10/lib/libfftw3l-3.a

INCLUDEPATH += $$PWD/libs/fftw-3.3.10/include
DEPENDPATH += $$PWD/libs/fftw-3.3.10/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/fftw-3.3.10/lib/libfftw3l-3.a

# make sure that libfftw3l-3.dll is available at runtime
FFTW_DLL_PATH = $$PWD/libs/fftw-3.3.10/bin/libfftw3l-3.dll

win32: QMAKE_POST_LINK += cmd /c copy /Y \"$$replace(FFTW_DLL_PATH, /, \\)\" \"$$replace(OUT_PWD, /, \\)\"
else: QMAKE_POST_LINK += cp \"$$FFTW_DLL_PATH\" \"$$OUT_PWD/\"; echo DLL copied

RESOURCES += \
    resources/resources.qrc

