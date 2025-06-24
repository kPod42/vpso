#pragma once
#include <QVector>
#include <QPointF>
#include <random>

class SegmentMonteCarlo
{
public:
    enum Orientation { Vertical, Horizontal };

    struct Params {
        double R  = 1.0;
        double x0 = 0.0;
        double y0 = 0.0;
        double C  = 0.0;
        Orientation o = Vertical;
        int    N  = 10'000;
    };

    explicit SegmentMonteCarlo(Params p);

    double run();                                   // оценка площади

    const QVector<QPointF>& hits()   const { return m_hits;   }   // точки-попадания
    const QVector<QPointF>& misses() const { return m_misses; }   // точки-промахи

private:
    Params  m_p;
    QVector<QPointF> m_hits;                        // попали в сегмент
    QVector<QPointF> m_misses;                      // в круге, но не в сегменте
};
