#include "splashdialog.h"
#include "ui_splashdialog.h"

SplashDialog::SplashDialog(int msecTimeout, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SplashDialog)
    , fade(this, "windowOpacity")
{
    ui->setupUi(this);

    connect(&fade, &QPropertyAnimation::finished, this, [this]{
        emit closed();   // сообщаем, что заставка уходит
        close();
    });

    /* внешний вид */
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowOpacity(1.0);

    /* анимация: 1 → 0 за 400 мс */
    fade.setStartValue(1.0);
    fade.setEndValue  (0.0);
    fade.setDuration  (400);
    connect(&fade, &QPropertyAnimation::finished, this, &QDialog::close);

    /* таймер автозакрытия */
    closeTim.setSingleShot(true);
    closeTim.setInterval(msecTimeout);
    connect(&closeTim, &QTimer::timeout, this, [this]{ fade.start(); });
    closeTim.start();
}

SplashDialog::~SplashDialog()
{
    delete ui;
}
