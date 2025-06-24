// experiment.h
#pragma once
#include <QString>
#include "./core/segmentmontecarlo.h"

struct Experiment {
    int    id      = -1;
    SegmentMonteCarlo::Params params;
    double S_est   = 0.0;
    double error   = 0.0;
    QString ts;
};
