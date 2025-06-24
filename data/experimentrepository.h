// experimentrepository.h
#pragma once
#include "Experiment.h"
#include <QVector>

class ExperimentRepository
{
public:
    bool insert(const Experiment &e);
    QVector<Experiment> list() const;
};
