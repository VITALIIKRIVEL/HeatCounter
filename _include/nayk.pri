#-------------------------------------------------
#
# Файл для подключения к qmake-файлу всех проектов
#
#-------------------------------------------------

# каталоги для поиска подключаемых файлов:
INCLUDEPATH *= $${PWD}

# заголовочные:
HEADERS *= $${PWD}/crypto.h \
    $${PWD}/log.h \
    $${PWD}/system_utils.h \
    $${PWD}/hw_utils.h \
    $${PWD}/filesys.h \
    $${PWD}/http.h \
    $${PWD}/http_server.h \
    $${PWD}/convert.h \
    $${PWD}/geo.h

# исходники:	
SOURCES *= $${PWD}/crypto.cpp \
    $${PWD}/system_utils.cpp \
    $${PWD}/hw_utils.cpp \
    $${PWD}/filesys.cpp \
    $${PWD}/log.cpp \
    $${PWD}/http_server.cpp \
    $${PWD}/convert.cpp \
    $${PWD}/geo.cpp

contains( QT, axcontainer ) {
    win32 {
        HEADERS *= $${PWD}/xls.h
	SOURCES *= $${PWD}/xls.cpp
    }
}

# если подключен драйвер сети:
contains( QT, network ) {
    HEADERS *= $${PWD}/http_client.h
    SOURCES *= $${PWD}/http_client.cpp
}

# если подключены виджеты:
contains( QT, widgets ) {
    HEADERS *= $${PWD}/highlighter.h \
        $${PWD}/gui_utils.h \
        $${PWD}/ex_controls.h \
        $${PWD}/graph.h

    SOURCES *= $${PWD}/highlighter.cpp \
        $${PWD}/gui_utils.cpp \
        $${PWD}/ex_controls.cpp \
        $${PWD}/graph.cpp

    DEFINES += WIDGETS_SOFT
}

# если подключен драйвер com-порта:
contains( QT, serialport ) {
    HEADERS *= $${PWD}/com_port.h
    SOURCES *= $${PWD}/com_port.cpp
}

# если подключены charts:
contains( QT, sql ) {
    HEADERS *= $${PWD}/db_client.h
    SOURCES *= $${PWD}/db_client.cpp
}

# если подключен sql:
contains( QT, charts ) {
    HEADERS *= $${PWD}/diagram.h
    SOURCES *= $${PWD}/diagram.cpp
}

# для релиз-сборки:
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

# Перевод (включение в основном файле проекта):
#TRANSLATIONS += $${PWD}/translations/nayk_ru.ts
#nayk_tr.commands = $${PWD}/translations/nayk_ru.ts -qm $${PWD}/translations/nayk_ru.qm
#PRE_TARGETDEPS += nayk_tr
#QMAKE_EXTRA_TARGETS += nayk_tr

unix:BUILD_DATE = '$(shell date +%Y-%m-%d)'
win32:BUILD_DATE = '$(shell echo %DATE:~6,4%-%DATE:~3,2%-%DATE:~0,2%)'
DEFINES += APP_BUILD_DATE=\\\"$$BUILD_DATE\\\"

