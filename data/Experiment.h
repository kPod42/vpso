#pragma once
#include "core/segmentmontecarlo.h"
#include "qdatetime.h"

struct Experiment {
    SegmentMonteCarlo::Params params;
    double     S_est {};
    double     error {};
    QDateTime  ts    { QDateTime::currentDateTime() };   // ← сразу задаём
};
