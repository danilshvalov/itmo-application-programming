#pragma once

#include <boost/json.hpp>

struct Point {
  double x;
  double y;
};

void tag_invoke(boost::json::value_from_tag, boost::json::value &value,
                const Point &p);

Point tag_invoke(boost::json::value_to_tag<Point>,
                 const boost::json::value &value);

struct Coefficients {
  double a;
  double b;
  double r;
};

Coefficients tag_invoke(boost::json::value_to_tag<Coefficients>,
                        const boost::json::value &value);

struct RequestData {
  std::vector<Point> points;
};

void tag_invoke(boost::json::value_from_tag, boost::json::value &value,
                const RequestData &rd);

struct ResponseData {
  Coefficients coefficients;
  std::vector<Point> calculated_points;
};

ResponseData tag_invoke(boost::json::value_to_tag<ResponseData>,
                        const boost::json::value &value);
