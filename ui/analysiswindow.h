/************************  analysiswindow.h  *************************/
#pragma once
#include <QDialog>
#include <memory>

class QSqlTableModel;
class QSortFilterProxyModel;

namespace Ui { class AnalysisWindow; }

class AnalysisWindow : public QDialog
{
    Q_OBJECT
public:
    explicit AnalysisWindow(QWidget *parent = nullptr);
    ~AnalysisWindow() override;

private slots:
    /* переключение режимов */
    void showTable();
    void showGraph();

    /* изменение «K-последних» и типа графика */
    void onSbLastKChanged(int k);
    void onChartTypeChanged(int);

private:
    Ui::AnalysisWindow *ui {};

    /* модели */
    std::unique_ptr<QSqlTableModel>        model;
    std::unique_ptr<QSortFilterProxyModel> proxy;

    /* пересчёт сводки и построение графика */
    void refreshSummary();
    void drawChart();
};
