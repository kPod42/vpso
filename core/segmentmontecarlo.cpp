#include "segmentmontecarlo.h"
#include <cmath>

SegmentMonteCarlo::SegmentMonteCarlo(Params p) : m_p(p) {}

double SegmentMonteCarlo::run()
{
    m_hits.clear();
    m_misses.clear();                               // ← очищаем и этот контейнер

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> ux(m_p.x0 - m_p.R, m_p.x0 + m_p.R);
    std::uniform_real_distribution<double> uy(m_p.y0 - m_p.R, m_p.y0 + m_p.R);

    int inside = 0;
    for (int i = 0; i < m_p.N; ++i) {
        const double x = ux(rng);
        const double y = uy(rng);

        const bool inCircle = std::hypot(x - m_p.x0, y - m_p.y0) <= m_p.R;
        bool inSegment = false;
        if (inCircle)
        {
            if (m_p.o == Vertical)          // вертикальная линия x = C
                inSegment = (x >= m_p.C);
            else                            // горизонтальная линия y = C
                inSegment = (y >= m_p.C);
        }

        if (inSegment)
            m_hits.append({x, y});          // зарегистрировали попадание
        else if (inCircle)
            m_misses.append({x, y});        // в круге, но не в сегменте

        inside += inSegment;
    }

    const double rectArea = 4.0 * m_p.R * m_p.R;
    const double p = static_cast<double>(inside) / m_p.N;
    return p * rectArea;                     // Ŝ
}

