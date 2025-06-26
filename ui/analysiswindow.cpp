/************************  analysiswindow.cpp  ***********************/
#include "analysiswindow.h"
#include "qsqlerror.h"
#include "ui_analysiswindow.h"
#include "../data/databasemanager.h"

#include <QMenuBar>
#include <QActionGroup>
#include <QSqlTableModel>
#include "FilterLastK.h"
#include <QtMath>              // qSqrt, qMin, qMax

AnalysisWindow::AnalysisWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AnalysisWindow)
{
    ui->setupUi(this);                         // грузим .ui

    /* ===== 1. Модель experiments из БД ========================= */
    model = std::make_unique<QSqlTableModel>(this, DatabaseManager::db());
    model->setTable("experiments");
    if (!model->select())
        qWarning() << "SELECT error:" << model->lastError();

    /* ===== 2. Прокси-фильтр K-последних ======================== */
    auto *flt = new FilterLastK(this);
    flt->setSourceModel(model.get());

    int idxTs = model->fieldIndex("ts");
    if (idxTs >= 0)
        flt->sort(idxTs, Qt::DescendingOrder);
    else
        qWarning() << "experiments: column 'ts' not found";

    proxy.reset(flt);                           // proxy теперь один-единственный
    ui->experimentTable->setModel(proxy.get());
    ui->experimentTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    /* ===== 3. Меню «Вид»  ====================================== */
    auto *bar = new QMenuBar(this);
    auto *grp = new QActionGroup(bar); grp->setExclusive(true);

    QAction *actTable = bar->addAction(tr("Таблица"));
    actTable->setCheckable(true);  actTable->setChecked(true);
    grp->addAction(actTable);

    QAction *actGraph = bar->addAction(tr("Графики"));
    actGraph->setCheckable(true);  grp->addAction(actGraph);

    if (!layout()) setLayout(new QVBoxLayout);
    layout()->setMenuBar(bar);

    connect(actTable, &QAction::triggered, this, &AnalysisWindow::showTable);
    connect(actGraph, &QAction::triggered, this, &AnalysisWindow::showGraph);

    /* ===== 4. Остальные сигналы  =============================== */
    connect(ui->sbLastK, &QSpinBox::valueChanged,
            this,        &AnalysisWindow::onSbLastKChanged);
    connect(ui->cbChartType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,           &AnalysisWindow::onChartTypeChanged);

    /* первичный расчёт / график */
    onSbLastKChanged(ui->sbLastK->value());
    drawChart();
}

AnalysisWindow::~AnalysisWindow() { delete ui; }

/* ---------- меню Вид -------------------------------------------- */
void AnalysisWindow::showTable()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void AnalysisWindow::showGraph()
{
    ui->stackedWidget->setCurrentIndex(1);
}

/* ---------- фильтр «последние K» -------------------------------- */
void AnalysisWindow::onSbLastKChanged(int k)
{
    static_cast<FilterLastK*>(proxy.get())->setLastK(k);
    refreshSummary();
    drawChart();
}

/* ---------- смена типа графика ---------------------------------- */
void AnalysisWindow::onChartTypeChanged(int) { drawChart(); }

/* ---------- сводная статистика слева ----------------------------- */
void AnalysisWindow::refreshSummary()
{
    const int rows = proxy->rowCount();
    const int colS = model->fieldIndex("S_est");

    if (!rows) {
        ui->avgS->setText("—");
        ui->stdS->setText("—");
        ui->minS->setText("—");
        ui->maxS->setText("—");
        return;
    }

    double sum=0, sumSq=0, mn=1e100, mx=-1e100;
    for (int r=0; r<rows; ++r) {
        const double v = proxy->index(r, colS).data().toDouble();
        sum += v; sumSq += v*v;
        mn = qMin(mn, v); mx = qMax(mx, v);
    }
    const double mean = sum / rows;
    const double std  = qSqrt(qMax(0.0, sumSq/rows - mean*mean));

    ui->avgS->setText(QString::number(mean,'g',6));
    ui->stdS->setText(QString::number(std ,'g',6));
    ui->minS->setText(QString::number(mn  ,'g',6));
    ui->maxS->setText(QString::number(mx  ,'g',6));
}

/* ---------- построение графика ---------------------------------- */
void AnalysisWindow::drawChart()
{
    auto *plt = ui->customPlot;
    plt->clearPlottables(); plt->clearGraphs();

    const int rows = proxy->rowCount();
    if (!rows) { plt->replot(); return; }

    QVector<double> x, y; x.reserve(rows); y.reserve(rows);

    const int cN  = model->fieldIndex("N");
    const int cR  = model->fieldIndex("R");
    const int cS  = model->fieldIndex("S_est");
    const int cEr = model->fieldIndex("error");

    for (int r = rows-1; r >= 0; --r) {
        switch (ui->cbChartType->currentIndex()) {
        case 0: plt->xAxis->setLabel("N — количество точек");
            plt->yAxis->setLabel("S(N) — оценка площади");
            x << proxy->index(r,cN).data().toDouble();
            y << proxy->index(r,cS).data().toDouble(); break;   // S vs N
        case 1: plt->xAxis->setLabel("R — радиус круга");
            plt->yAxis->setLabel("S(R) — оценка площади");
            x << proxy->index(r,cR).data().toDouble();
            y << proxy->index(r,cS).data().toDouble(); break;   // S vs R
        case 2: plt->xAxis->setLabel("S — оценка площади");
            plt->yAxis->setLabel("ΔS — погрешность");
            x << proxy->index(r,cS).data().toDouble();
            y << proxy->index(r,cEr).data().toDouble(); break;  // error vs S
        }
    }

    auto *g = plt->addGraph();
    g->setLineStyle(QCPGraph::lsNone);
    g->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 4));
    g->setData(x, y);

    plt->rescaleAxes();
    plt->replot();
}
