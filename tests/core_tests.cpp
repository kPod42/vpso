#include <QtTest>
#include "core/segmentmontecarlo.h"

class CoreTests : public QObject {
    Q_OBJECT
private slots:
    void estimateGetsCloser();
};

void CoreTests::estimateGetsCloser()
{
    SegmentMonteCarlo::Params p;
    p.R = 1; p.C = 0; p.o = SegmentMonteCarlo::Vertical;

    SegmentMonteCarlo small(p);   small.m_p.N = 5'000;
    SegmentMonteCarlo big  (p);   big  .m_p.N = 200'000;

    QVERIFY( std::abs( big.run() - M_PI ) <
            std::abs( small.run() - M_PI ) );
}
QTEST_MAIN(CoreTests)
#include "core_tests.moc"
