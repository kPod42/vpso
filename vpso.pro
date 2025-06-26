############################################################################
# vpso.pro – добавить CONFIG+=unit_tests для сборки модульных тестов      #
############################################################################
CONFIG += c++17
QT     += sql                # БД нужна в любой сборке

# ───── общие исходники (core + DAO) ─────
CORE_SOURCES = \
    core/segmentmontecarlo.cpp \
    data/databasemanager.cpp    \
    data/experimentrepository.cpp

CORE_HEADERS = \
    core/segmentmontecarlo.h    \
    data/experiment.h           \
    data/databasemanager.h      \
    data/experimentrepository.h


# ──────────────────────────  UNIT-TESTS  ────────────────────────────────
contains(CONFIG, unit_tests) {

    message(*** Building UNIT-TESTS ***)

    TEMPLATE = app
    TARGET   = vpso_tests

    CONFIG  += testcase console
    QT      += testlib

    SOURCES += tests/core_tests.cpp $$CORE_SOURCES
    HEADERS += $$CORE_HEADERS       # достаточно заголовков с Q_OBJECT

} else {                             # ────────── GUI-приложение ──────────

    message(*** Building APPLICATION ***)

    TEMPLATE = app
    TARGET   = vpso

    QT      += widgets printsupport
    CONFIG  -= console               # без консольного окна под Windows

    # --- QCustomPlot ----------------------------------------------------
    INCLUDEPATH += $$PWD/3rdparty
    SOURCES     += $$PWD/3rdparty/qcustomplot.cpp
    HEADERS     += $$PWD/3rdparty/qcustomplot.h

    # --- ваши исходники UI ---------------------------------------------
    SOURCES += \
        main.cpp \
        ui/mainwindow.cpp \
        ui/analysiswindow.cpp \
        $$CORE_SOURCES

    HEADERS += \
        ui/mainwindow.h \
        ui/analysiswindow.h\         # собственный класс-обёртка, не ui_*.h \
        ui/FilterLastK.h\
        $$CORE_HEADERS

    FORMS += \
        ui/mainwindow.ui \
        ui/analysiswindow.ui

    RESOURCES += resources.qrc
}

HEADERS += \
    core/loghandler.h \
    ui/FilterLastK.h \
    ui/aboutdialog.h \
    ui/splashdialog.h

FORMS += \
    ui/aboutdialog.ui \
    ui/splashdialog.ui

SOURCES += \
    core/loghandler.cpp \
    ui/aboutdialog.cpp \
    ui/splashdialog.cpp
