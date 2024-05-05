#pragma once

#include <boost/json.hpp>

struct Point {
  double x;
  double y;
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& value,
                const Point& p);

Point tag_invoke(boost::json::value_to_tag<Point>,
                 const boost::json::value& value);

struct Coefficients {
  double a;
  double b;
  double r;
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& value,
                const Coefficients& lc);

struct RequestData {
  std::vector<Point> points;
};

RequestData tag_invoke(boost::json::value_to_tag<RequestData>,
                       const boost::json::value& value);

struct ResponseData {
  Coefficients coefficients;
  std::vector<Point> calculated_points;
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& value,
                const ResponseData& rd);

struct ErrorData {
  int code;
  std::string message;
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& value,
                const ErrorData& ed);
