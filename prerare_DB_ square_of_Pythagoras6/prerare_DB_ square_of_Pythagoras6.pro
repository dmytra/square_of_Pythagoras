HEADERS   = window.h
SOURCES   = main.cpp \
            window.cpp
QT += sql widgets
requires(qtConfig(combobox))

RESOURCES += \
    res.qrc

DISTFILES += \
    file_for_DB/man \
    file_for_DB/popularnist-prizvis_in_ua \
    file_for_DB/whoman


