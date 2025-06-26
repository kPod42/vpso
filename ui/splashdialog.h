#pragma once
#include <QDialog>
#include <QTimer>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
namespace Ui { class SplashDialog; }
QT_END_NAMESPACE

class SplashDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SplashDialog(int msecTimeout = 2000, QWidget *parent = nullptr);
    ~SplashDialog() override;

signals:
    void closed();                 // ← испустим перед окончательным закрытием

private:
    Ui::SplashDialog  *ui;
    QTimer             closeTim;
    QPropertyAnimation fade;
};
