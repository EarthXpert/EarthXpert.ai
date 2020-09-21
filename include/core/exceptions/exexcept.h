/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include <stdexcept>
#include "3rdparty/gettext/exgettext.h"

namespace earthxpert {
/**
 *  \brief Reports errors when data is invalid
 */
class DataError : public std::exception {
public:
  /**
   *  \param [in] message The message string
   */
  explicit DataError(const std::string& message)
    : exception(message.c_str()) {}

  explicit DataError(const char *message)
    : exception(message) {}
};

/**
 *  \brief Reports errors when doing epipolar transformation
 */
class EpipolarError : public std::exception {
public:
  /**
   *  \param [in] message The message string
   */
  explicit EpipolarError(const std::string& message)
    : exception(message.c_str()) {}

  explicit EpipolarError(const char* message)
    : exception(message) {}
};

/**
 *  \brief Reports errors being lacking of valid license
 */
class InvalidLicense : public std::exception {
public:
  /**
   *  \param [in] message The message string
   */
  explicit InvalidLicense(const std::string& message)
    : exception(message.c_str()) {}

  explicit InvalidLicense(const char *message)
    : exception(message) {}
};

  /**
 *  \brief Reports IO relevant errors
 */
class IOError : public std::exception {
public:
  /**
   *  \param [in] message The message string
   */
  explicit IOError(const std::string& message)
    : exception(message.c_str()) {}

  explicit IOError(const char *message)
    : exception(message) {}
};

/**
 *  \brief Reports sensor model relevant errors such as failure of I2G or G2I
 */
class SensorError : public std::exception {
public:
  /**
   *  \param [in] message The message string
   */
  explicit SensorError(const std::string& message)
    : exception(message.c_str()) {}

  explicit SensorError(const char* message)
    : exception(message) {}
};

/**
 *  \brief Reports sensor model relevant errors such as failure of SfM
 */
class SfMError : public std::exception {
public:
  /**
   *  \param [in] message The message string
   */
  explicit SfMError(const std::string& message)
    : exception(message.c_str()) {}

  explicit SfMError(const char* message)
    : exception(message) {}
};
} // namespace earthxpert
