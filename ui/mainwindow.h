#pragma once
#include <QMainWindow>
#include <memory>

#include "../3rdparty/qcustomplot.h"                 // ← путь ищется через INCLUDEPATH
#include "core/SegmentMonteCarlo.h"
#include "data/experimentrepository.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    std::unique_ptr<ExperimentRepository> repo;

private slots:
    void on_btnStart_clicked();

private:
    /* ---- авто-сгенерённое UI ---- */
    Ui::MainWindow *ui = nullptr;

    /* ---- Monte-Carlo модель ---- */
    SegmentMonteCarlo::Params               mcParams;
    std::unique_ptr<SegmentMonteCarlo>      mc;

    /* ---- элементы QCustomPlot ---- */
    QCPGraph     *gHits   = nullptr;
    QCPGraph     *gMisses = nullptr;
    QCPCurve     *circle  = nullptr;
    QCPItemLine  *lim     = nullptr;

    /* ---- инициализация / отрисовка ---- */
    void initPlot();      // настраиваем оси и создаём графики
    void buildFrame();    // окружность + ограничивающая прямая
    void drawScene();     // точки + вызов buildFrame()
    void runSimulation(); // пересчёт модели + drawScene();
};
