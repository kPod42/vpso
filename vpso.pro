############################################################################
# vpso.pro – режим выбирается через CONFIG+=unit_tests                    #
############################################################################
CONFIG += c++17                # общий стандарт C++
QT     += sql                  # sql нужен в любой сборке

# ───── Общие исходники (core + DAO) ─────
CORE_SOURCES = \
    core/segmentmontecarlo.cpp \
    data/databasemanager.cpp    \
    data/experimentrepository.cpp

CORE_HEADERS = \
    core/segmentmontecarlo.h    \
    data/experiment.h           \
    data/databasemanager.h      \
    data/experimentrepository.h

# ──────────────────────────── UNIT-TESTS ────────────────────────────────
contains(CONFIG, unit_tests) {

    message(*** Building UNIT-TESTS ***)

    TEMPLATE = app
    TARGET   = vpso_tests

    CONFIG  += testcase console   # console‐подсистема только тестам
    QT      += testlib            # Qt Test

    SOURCES += tests/core_tests.cpp \
               $$CORE_SOURCES
    HEADERS += $$CORE_HEADERS      # (можно сократить до файлов с Q_OBJECT)

} else {                           # ───────── GUI-приложение ────────────

    message(*** Building APPLICATION ***)

    TEMPLATE = app
    TARGET   = vpso

    QT      += widgets printsupport            # charts больше не нужны
    CONFIG  -= console            # убираем консольное окно под Windows

    # --- QCustomPlot ----------------------------------------------------
    INCLUDEPATH += $$PWD/3rdparty           # где лежит .h
    SOURCES     += $$PWD/3rdparty/qcustomplot.cpp
    HEADERS     += $$PWD/3rdparty/qcustomplot.h

    # ---- ваши исходники формы ----
    SOURCES += main.cpp \
               ui/mainwindow.cpp \
               $$CORE_SOURCES

    HEADERS += ui/mainwindow.h \
               $$CORE_HEADERS

    FORMS     += ui/mainwindow.ui
    RESOURCES += resources.qrc
}
