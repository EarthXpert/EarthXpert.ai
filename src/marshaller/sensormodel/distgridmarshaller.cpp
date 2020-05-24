/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/distgridmarshaller.h"
#include "core/exceptions/exexcept.h"

#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <limits>

using earthxpert::DistGridMarshaller;
using earthxpert::DistortionGridF;

/**
 *  Serialize a distortion grid into a text file.
 *
 *  \param [in] gridFile name of the file
 *  \param [in] distGrid pointer to a DistortionGrid object
 *  \throw if distGrid is null, or contains no grid data.
 *  \throw if the file cannot be open for writing
 */
void DistGridMarshaller::Serialize(const std::string &gridFile,
                                   DistortionGridF const *const distGrid) {
  if (!distGrid || !distGrid->HasGrid()) {
    throw std::invalid_argument("Given distortion grid should contain valid data.");
  }

  std::ofstream outStream(gridFile.c_str());
  if (!outStream.good()) {
    throw std::invalid_argument("Cannot create file.");
  }

  Serialize(outStream, distGrid);
}

/**
 *  Serialize a distortion grid into a std::ostream.
 *
 *  \param [in/out] outStream destination stream
 *  \param [in] distGrid pointer to a DistortionGrid object
 *  \throw if the output stream is not valid
 *  \throw if distGrid is null, or contains no grid data
 */
void DistGridMarshaller::Serialize(std::ostream &outStream,
                                   DistortionGridF const *const distGrid) {
  if (!outStream.good()) {
    throw std::invalid_argument("Invalid output stream");
  }
  if (!distGrid || !distGrid->HasGrid()) {
    throw std::invalid_argument("Given distortion grid should contain valid data.");
  }

  unsigned int gridCols{};
  unsigned int gridRows{};
  std::vector<float> vX;
  std::vector<float> vY;
  std::vector<float> vdX;
  std::vector<float> vdY;

  const auto bOk = distGrid->FetchGrid(gridCols, gridRows, vX, vY, vdX, vdY);
  if (!bOk) {
    throw std::exception("Given distortion grid is invalid");
  }

  outStream << "file_version: 1\n\n";

  outStream << "begin gridgroup \"Image deformation grids\"\n";
  outStream << "    enabled:     yes\n";
  outStream << "    order_index: 0\n";
  outStream << "    camera_name: \"" << distGrid->GetCameraName() << "\"\n";
  outStream << "    description: \"BINGO self calibration grid - from all "
               "additional parameters\"\n\n";

  outStream << "    begin grid \"Image deformation grid 1\"\n";
  outStream << "        enabled:       yes\n";
  outStream << "        inverse_scale: 1.000000\n";
  outStream << "        columns:       " << gridCols << "\n";
  outStream << "        rows:          " << gridRows << "\n";
  outStream << "        description:   \"Created by program BgExport\"\n\n";

  outStream << "        begin gridpoints\n";

  const size_t numPts = size_t(gridCols) * size_t(gridRows);
  for (size_t k = 0; k < numPts; ++k) {
    // format "            %11.4f %11.4f %11.4f %11.4f" TODO
    outStream << vX[k] - vdX[k] << vY[k] - vdY[k] << vX[k] << vY[k];
  }

  outStream << "        end gridpoints\n";

  outStream << "    end grid\n";
  outStream << "end gridgroup\n";
}

/**
 *  Deserialize a calibration grid from a file.
 *
 *  \param [in] gridFile name of the grid file
 *  \return new instance of DistortionGrid
 *  \throw if the file cannot be opened for reading
 */
std::unique_ptr<DistortionGridF>
DistGridMarshaller::Deserialize(const std::string &gridFile) {
  std::ifstream inStream(gridFile.c_str());
  if (!inStream.good()) {
    throw std::invalid_argument("Failed to open file.");
  }
  return Deserialize(inStream);
}

/**
 *  Deserialize a calibration grid from std::istream.
 *
 *  \param [in] inStream input stream.
 *  \return new instance of DistortionGrid
 *  \throw if the stream is not good
 *  \throw if the information inside the stream is not properly formated
 */
std::unique_ptr<DistortionGridF>
DistGridMarshaller::Deserialize(std::istream &inStream) {
  if (!inStream.good()) {
    throw std::invalid_argument("Invalid stream");
  }

  int gridCols{0};
  int gridRows{0};
  std::string camName;

  std::string line;

  while (!inStream.eof()) {
    size_t pos;

    std::getline(inStream, line);
    if (std::string::npos != (pos = line.find("camera_name:"))) {
      camName = line.substr(pos + 13);
      camName.erase(std::remove(std::begin(camName), std::end(camName), '\"'),
                    std::end(camName));
    } else if (std::string::npos != (pos = line.find("columns:"))) {
      gridCols = std::stoi(line.substr(pos + 8));
    } else if (std::string::npos != (pos = line.find("rows:"))) {
      gridRows = std::stoi(line.substr(pos + 5));
    }

    if (std::string::npos != line.find("begin gridpoints"))
      break;
  }

  if (0 == gridCols || 0 == gridRows) {
    throw std::exception("Invalid Z/I grid format");
  }

  const size_t numGridPts = size_t(gridCols) * size_t(gridRows);
  std::vector<float> vX(numGridPts);
  std::vector<float> vY(numGridPts);
  std::vector<float> vdX(numGridPts);
  std::vector<float> vdY(numGridPts);

  for (size_t i = 0; i < numGridPts; ++i) {
    std::getline(inStream, line);
    boost::trim(line);

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(" \t\n"),
                 boost::algorithm::token_compress_on);

    if (4 != tokens.size()) {
      throw std::exception("Each line of grid point must contain 4 numbers");
    }

    vX[i] = std::stod(tokens[2]);
    vY[i] = std::stod(tokens[3]);
    vdX[i] = vX[i] - std::stod(tokens[0]);
    vdY[i] = vY[i] - std::stod(tokens[1]);
  }

  std::getline(inStream, line);
  if (std::string::npos == line.find("end gridpoints")) {
    throw std::exception("Invalid Z/I grid format");
  }

  std::getline(inStream, line); // skip "    end grid"
  std::getline(inStream, line); // skip "end gridgroup"

  return std::make_unique<DistortionGridF>(camName, vX, vY, vdX, vdY);
}
