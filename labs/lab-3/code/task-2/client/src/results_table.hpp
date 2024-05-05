#pragma once

#include <vector>

#include <QWidget>

#include "models.hpp"

class ResultsTable : public QWidget {
public:
  ResultsTable(const std::vector<Point> &points,
               const Coefficients &coefficients, QWidget *parent = nullptr);
};
