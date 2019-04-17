#-------------------------------------------------
#
# Файл для подключения к qmake-файлу 
#
#-------------------------------------------------

# конфигурация сборки:

# каталоги для поиска подключаемых файлов:
INCLUDEPATH *= $${PWD}
#INCLUDEPATH *= $${PWD}/lua5.2
INCLUDEPATH *= $${PWD}/lua5.3
INCLUDEPATH *= $${PWD}/selene
INCLUDEPATH *= $${PWD}/selene/selene
#INCLUDEPATH *= $${PWD}/LuaBridge
#INCLUDEPATH *= $${PWD}/LuaBridge/detail

#LIBS += -llua5.2
LIBS += -llua5.3
