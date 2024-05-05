#include "models.hpp"

void tag_invoke(boost::json::value_from_tag, boost::json::value& value,
                const Point& p) {
  value = {
      {"x", p.x},
      {"y", p.y},
  };
}

Point tag_invoke(boost::json::value_to_tag<Point>,
                 const boost::json::value& value) {
  const boost::json::object& object = value.as_object();
  return Point{
      object.at("x").as_double(),
      object.at("y").as_double(),
  };
}

void tag_invoke(boost::json::value_from_tag, boost::json::value& value,
                const Coefficients& lc) {
  value = {
      {"a", lc.a},
      {"b", lc.b},
      {"r", lc.r},
  };
}

RequestData tag_invoke(boost::json::value_to_tag<RequestData>,
                       const boost::json::value& value) {
  const boost::json::object& object = value.as_object();
  return RequestData{
      boost::json::value_to<std::vector<Point>>(object.at("points")),
  };
}

void tag_invoke(boost::json::value_from_tag, boost::json::value& value,
                const ResponseData& rd) {
  value = {
      {"coefficients", boost::json::value_from(rd.coefficients)},
      {"calculated_points", boost::json::value_from(rd.calculated_points)},
  };
}

void tag_invoke(boost::json::value_from_tag, boost::json::value& value,
                const ErrorData& ed) {
  value = {
      {"code", ed.code},
      {"message", ed.message},
  };
}
