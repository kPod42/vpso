#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>                 // std::sin / std::cos, если нужно
#include <QVector>

/* ──────────── конструктор ──────────── */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initPlot();   // создаём circle, lim, gHits, gMisses
    buildFrame(); // первый «пустой» кадр
}

/* ──────────── initPlot ──────────── */
void MainWindow::initPlot()
{
    QCustomPlot *plt = ui->customPlot;

    plt->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    QPen axisPen(Qt::black, 1.5);
    plt->xAxis->setBasePen(axisPen);
    plt->yAxis->setBasePen(axisPen);

    plt->xAxis->grid()->setSubGridVisible(true);
    plt->yAxis->grid()->setSubGridVisible(true);

    /* заранее создаём графики попаданий / промахов */
    gHits   = plt->addGraph();
    gMisses = plt->addGraph();
    gHits  ->setLineStyle(QCPGraph::lsNone);
    gMisses->setLineStyle(QCPGraph::lsNone);

    QCPScatterStyle hitStyle   (QCPScatterStyle::ssDisc,
                             QPen(Qt::green),  QBrush(Qt::green), 3);
    QCPScatterStyle missStyle  (QCPScatterStyle::ssDisc,
                              QPen(Qt::red),    QBrush(Qt::red),   3);

    gHits  ->setScatterStyle(hitStyle);
    gMisses->setScatterStyle(missStyle);

    /* окружность и ограничивающая прямая создаём тоже единожды */
    circle = new QCPCurve(plt->xAxis, plt->yAxis);
    lim    = new QCPItemLine(plt);
    lim->setPen(QPen(Qt::red));
}

/* ──────────── запуск расчёта ──────────── */

void MainWindow::runSimulation()
{
    mc = std::make_unique<SegmentMonteCarlo>(mcParams);
    const double area = mc->run();

    drawScene();                                       // ← а не drawPoints()
    ui->edtS->setText(tr("S ≈ %1").arg(area, 0, 'g', 10));
}

/* ──────────── построение окружности + линии ──────────── */
void MainWindow::buildFrame()
{
    /* вызывается из drawScene() после изменения mcParams
       или один раз в конструкторе для пустой сцены */

    const int nSeg = 180;
    QVector<double> cx(nSeg), cy(nSeg);
    for (int i = 0; i < nSeg; ++i)
    {
        double t = 2 * M_PI * i / (nSeg - 1);
        cx[i] = mcParams.x0 + mcParams.R * std::cos(t);
        cy[i] = mcParams.y0 + mcParams.R * std::sin(t);
    }
    circle->setData(cx, cy);

    if (mcParams.o == SegmentMonteCarlo::Vertical)   // x = C
    {
        lim->start->setCoords(mcParams.C, mcParams.y0 - mcParams.R * 1.2);
        lim->end  ->setCoords(mcParams.C, mcParams.y0 + mcParams.R * 1.2);
    }
    else                                             // y = C
    {
        lim->start->setCoords(mcParams.x0 - mcParams.R * 1.2, mcParams.C);
        lim->end  ->setCoords(mcParams.x0 + mcParams.R * 1.2, mcParams.C);
    }
}

/* ──────────── вывод точек ──────────── */
void MainWindow::drawScene()
{
    QCustomPlot *plt = ui->customPlot;

    /* 1. обновляем окружность и ограничивающую линию */
    buildFrame();

    /* 2. очищаем старые точки, не создавая новых графиков */
    gHits  ->data()->clear();
    gMisses->data()->clear();

    const auto &hits   = mc->hits();
    const auto &misses = mc->misses();

    QVector<double> hx, hy, mx, my;
    hx.reserve(hits.size());   hy.reserve(hits.size());
    mx.reserve(misses.size()); my.reserve(misses.size());

    for (const QPointF &p : hits)   { hx << p.x(); hy << p.y(); }   // ← без +x0/+y0
    for (const QPointF &p : misses) { mx << p.x(); my << p.y(); }

    gHits  ->setData(hx, hy);
    gMisses->setData(mx, my);

    /* 3. подгоняем диапазон осей */
    const double R = mcParams.R;
    plt->xAxis->setRange(mcParams.x0 - R * 1.3, mcParams.x0 + R * 1.3);
    plt->yAxis->setRange(mcParams.y0 - R * 1.3, mcParams.y0 + R * 1.3);

    plt->replot();
}

/* ──────────── слот кнопки Старт ──────────── */
void MainWindow::on_btnStart_clicked()
{
    mcParams.R  = ui->sbR ->value();
    mcParams.C  = ui->sbC ->value();
    mcParams.x0 = ui->sbX0->value();
    mcParams.y0 = ui->sbY0->value();
    mcParams.N  = ui->sbN ->value();
    mcParams.o  = (ui->cbOrient->currentIndex() == 0)
                     ? SegmentMonteCarlo::Vertical
                     : SegmentMonteCarlo::Horizontal;

    runSimulation();
}

MainWindow::~MainWindow() { delete ui; }
