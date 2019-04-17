
INCLUDEPATH *= $$PWD

linux {
    QT        += x11extras
    CONFIG    += link_pkgconfig
    PKGCONFIG += x11
}

win32-mingw {
    QMAKE_CXXFLAGS += -Wliteral-suffix
}
win32-msvc* {
    LIBS += -luser32
}

HEADERS += \
    $$PWD/qglobalshortcut.h

win32 {
    SOURCES += \
        $$PWD/qglobalshortcut_win.cpp
}

linux {
    SOURCES += \
        $$PWD/qglobalshortcut_x11.cpp
}


