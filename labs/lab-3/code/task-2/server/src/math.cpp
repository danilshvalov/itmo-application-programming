#include "math.hpp"

Coefficients FindCoefficients(const std::vector<Point>& points) {
  double n = points.size();
  double sum_x_y = 0;
  double sum_x = 0;
  double sum_y = 0;
  double sum_squared_x = 0;
  double sum_squared_y = 0;
  for (const Point& p : points) {
    sum_x_y += p.x * p.y;
    sum_x += p.x;
    sum_y += p.y;
    sum_squared_x += std::pow(p.x, 2);
    sum_squared_y += std::pow(p.y, 2);
  }
  double sum_x_squared = std::pow(sum_x, 2);
  double sum_y_squared = std::pow(sum_y, 2);

  double a =
      (n * sum_x_y - sum_x * sum_y) / (n * sum_squared_x - sum_x_squared);
  double b = (sum_squared_x * sum_y - sum_x * sum_x_y) /
             (n * sum_squared_x - sum_x_squared);
  double r = (n * sum_x_y - sum_x * sum_y) /
             std::sqrt((n * sum_squared_x - sum_x_squared) *
                       (n * sum_squared_y - sum_y_squared));
  return Coefficients{a, b, r};
}

std::vector<Point> FindCalculatedPoints(const std::vector<Point>& points,
                                        const Coefficients& coefficients) {
  std::vector<Point> result;
  result.reserve(points.size());
  for (const Point& p : points) {
    result.push_back(Point{p.x, coefficients.a * p.x + coefficients.b});
  }
  return result;
}
