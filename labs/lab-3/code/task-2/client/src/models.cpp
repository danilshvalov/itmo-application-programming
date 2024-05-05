#include "models.hpp"

void tag_invoke(boost::json::value_from_tag, boost::json::value &value,
                const Point &p) {
  value = {
      {"x", p.x},
      {"y", p.y},
  };
}

Point tag_invoke(boost::json::value_to_tag<Point>,
                 const boost::json::value &value) {
  const boost::json::object &object = value.as_object();
  return Point{
      object.at("x").as_double(),
      object.at("y").as_double(),
  };
}

Coefficients tag_invoke(boost::json::value_to_tag<Coefficients>,
                        const boost::json::value &value) {
  const boost::json::object &object = value.as_object();
  return Coefficients{
      object.at("a").as_double(),
      object.at("b").as_double(),
      object.at("r").as_double(),
  };
}

void tag_invoke(boost::json::value_from_tag, boost::json::value &value,
                const RequestData &rd) {
  value = {
      {"points", boost::json::value_from(rd.points)},
  };
}

ResponseData tag_invoke(boost::json::value_to_tag<ResponseData>,
                        const boost::json::value &value) {
  const boost::json::object &object = value.as_object();
  return ResponseData{
      boost::json::value_to<Coefficients>(object.at("coefficients")),
      boost::json::value_to<std::vector<Point>>(object.at("calculated_points")),
  };
}
