/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "sensormodel/utils.h"

#include <map>

namespace earthxpert {
namespace detail {
  // sensor names & types
  static std::map<std::string, SensorType> sSensorN2T = {
  {"undefined", SensorType::eUNDEFINED},
  {"ads", SensorType::eADS},
  {"frame", SensorType::eFRAME},
  {"pushbroom", SensorType::ePUSHBROOM},
  {"rolling", SensorType::eROLLING},
  {"affine", SensorType::eAFFINE},
  {"network", SensorType::eNETWORK},
  {"rational", SensorType::eRATIONAL} };

  static std::map<SensorType, std::string> sSensorT2N = {
  {SensorType::eUNDEFINED, "undefined"},
  {SensorType::eADS, "ads"},
  {SensorType::eFRAME, "frame"},
  {SensorType::ePUSHBROOM, "pushbroom"},
  {SensorType::eROLLING, "rolling"},
  {SensorType::eAFFINE, "affine"},
  {SensorType::eNETWORK, "network"},
  {SensorType::eRATIONAL, "rational"} };
}

/**
 *  \brief Convert sensor name to type
 *
 *  \param [in] name sensor name
 *  \return Returns sensor type
 */
SensorType SensorNameToType(const std::string& name) {
  SensorType type{ SensorType::eUNDEFINED };
  const auto it = detail::sSensorN2T.find(name);
  if (detail::sSensorN2T.end() != it) {
    type = it->second;
  }

  return type;
}

/**
 *  \brief Convert sensor type to name
 *
 *  \param [in] type sensor type
 *  \return Returns sensor name
 */
std::string SensorTypeToName(const SensorType type) {
  std::string name{ "undefined" };
  const auto it = detail::sSensorT2N.find(type);
  if (detail::sSensorT2N.end() != it) {
    name = it->second;
  }

  return name;
}
}