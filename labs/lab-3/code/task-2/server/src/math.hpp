#pragma once

#include <vector>

#include "models.hpp"

Coefficients FindCoefficients(const std::vector<Point>& points);

std::vector<Point> FindCalculatedPoints(const std::vector<Point>& points,
                                        const Coefficients& coefficients);
