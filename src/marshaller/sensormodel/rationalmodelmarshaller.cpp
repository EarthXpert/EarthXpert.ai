/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/rationalmodelmarshaller.h"
#include "core/utils/stringutils.h"
#include "sensormodel/rationalmodel.h"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <map>
#include <stdexcept>

using earthxpert::ISensorModel;
using earthxpert::RationalModel;
using earthxpert::RationalModelMarshaller;
using earthxpert::utils::CompareI;

// rpb entries
static std::map<std::string, int> sRpbEntryMap = {
  {"lineOffset", 0}, 
  {"sampOffset", 1},
  {"latOffset", 2},
  {"longOffset", 3},
  {"heightOffset", 4},
  {"lineScale", 5},
  {"sampScale", 6},
  {"latScale", 7},
  {"longScale", 8},
  {"heightScale", 9},
  {"lineNumCoef", 10},
  {"lineDenCoef", 11},
  {"sampNumCoef", 12},
  {"sampDenCoef", 13},
  {"errBias", 14},
  {"errRand", 15} };

// rpc entries
static std::map<std::string, int> sRpcEntryMap = {
  {"LINE_OFF", 0},
  {"SAMP_OFF", 1},
  {"LAT_OFF", 2},
  {"LONG_OFF", 3},
  {"HEIGHT_OFF", 4},
  {"LINE_SCALE", 5},
  {"SAMP_SCALE", 6},
  {"LAT_SCALE", 7},
  {"LONG_SCALE", 8},
  {"HEIGHT_SCALE", 9},
  {"LINE_NUM_COEFF", 10},
  {"LINE_DEN_COEFF", 11},
  {"SAMP_NUM_COEFF", 12},
  {"SAMP_DEN_COEFF", 13} };

static constexpr size_t kRPC_COEFF_KEY_LEN = 14;  // lenght of coefficient key's string component
static constexpr size_t kRPC_COEFF_KEY_IDX = 15;  // position of index component

/**
 *  \brief seralize a rational model to a .rpb file or a rpc.txt file
 *
 *  \param[in] modelFile the file to insert model coefficients
 *  \param[in] rationalModel the rational model to be seralized
 */
void RationalModelMarshaller::Serialize(const std::string &modelFile,
    RationalModel const *const rationalModel) {
  const auto coefFormat = GetCoefFormat(modelFile);

  std::ofstream outStream(modelFile.c_str());
  if (!outStream.good()) {
    throw std::invalid_argument("Cannot create file.");
  }

  if (eRPB == coefFormat) {
    return SerializeRpb(outStream, rationalModel);
  }
  else if (eRPC == coefFormat) {
    return SerializeRpc(outStream, rationalModel);
  }
  else {
    throw std::invalid_argument("Not a rational model file.");
  }
}

/**
 *  \brief read rational model data from .RPB or rpc.txt file
 *
 *  \param [in] modelFile the name of the input file
 *  \return the pointer to the rational model object
 *  \throw if the file cannot be opened for reading
 */
std::unique_ptr<RationalModel>
RationalModelMarshaller::Deserialize(const std::string &modelFile) {
  const auto coefFormat = GetCoefFormat(modelFile);

  std::ifstream inStream(modelFile.c_str());
  if (!inStream.good()) {
    throw std::invalid_argument("Failed to open file.");
  }

  if (eRPB == coefFormat) {
    return DeserializeRpb(inStream);
  } else if (eRPC == coefFormat) {
    return DeserializeRpc(inStream);
  } else {
    throw std::invalid_argument("Not a rational model file."); 
  }
}

/**
 *  \brief parse coefficeints format from the model's filename, i.e., .RPB or rpc.txt
 *
 *  \param [in] modelFile the name of the input file
 */
RationalModelMarshaller::CoefFormat 
RationalModelMarshaller::GetCoefFormat(const std::string& modelFile) const {
  if (CompareI(modelFile.substr(modelFile.length() - 4), ".rpb")) {
    return eRPB;
  }
  else if (CompareI(modelFile.substr(modelFile.length() - 8), "_rpc.txt")) {
    return eRPC;
  }
  else {
    throw std::invalid_argument("Not a rational model file.");
  }
}

/**
 *  \brief seralize a rational model into a .rpb file
 *
 *  \param[in] outStream output stream
 *  \param[in] rationalModel the rational model to be seralized
 */
void RationalModelMarshaller::SerializeRpb(std::ostream& outStream,
  const RationalModel* const rationalModel) {

}

/**
 *  \brief seralize a rational model into a rpc.txt file
 *
 *  \param[in] outStream output stream
 *  \param[in] rationalModel the rational model to be seralized
 */
void RationalModelMarshaller::SerializeRpc(std::ostream& outStream,
  const RationalModel* const rationalModel) {
}

/**
 *  \brief Deserialize from .RPB file
 *
 *  \param [in] inStream input stream
 */
std::unique_ptr<RationalModel>
RationalModelMarshaller::DeserializeRpb(std::istream& inStream) {
  double errBias{}, errRand{};
  std::array<double, rnf::NumFact> normalFators{};
  std::array<double, RationalModel::NumCoef> lineNumCoef{};
  std::array<double, RationalModel::NumCoef> lineDenCoef{};
  std::array<double, RationalModel::NumCoef> sampNumCoef{};
  std::array<double, RationalModel::NumCoef> sampDenCoef{};

  std::string line;
  while (!inStream.eof()) {
    std::getline(inStream, line);
    boost::trim(line);
    if (line.empty()) continue;

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(" =;,\t\n"),
      boost::algorithm::token_compress_on);

    if (tokens.empty() || 3 < tokens.size()) {
      throw std::exception("Each line contains 1 to 3 tokens");
    }

    const auto& key = tokens[0];
    
    auto it = sRpbEntryMap.find(key);
    if (sRpbEntryMap.end() == it) continue;
    
    switch (it->second) {
    case 0: // range 0 ... 8
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      normalFators.at(it->second) = std::stod(tokens[1]);
      break;
    case 10: // lineNumCoef
      ExtractCoefs(inStream, lineNumCoef);
      break;
    case 11: // lineDenCoef
      ExtractCoefs(inStream, lineDenCoef);
      break;
    case 12: // sampNumCoef
      ExtractCoefs(inStream, sampNumCoef);
      break;
    case 13: // sampDenCoef
      ExtractCoefs(inStream, sampDenCoef);
      break;
    case 14: // errBias
      errBias = std::stod(tokens[1]);
      break;
    case 15: // errRand
      errRand = std::stod(tokens[1]);
      break;
    default:
      assert(false);
    }
  }

  auto groundSRS = std::make_shared<SpatialReferenceSystem>("WGS84");
  auto rationalModel = std::make_unique<RationalModel>(mNumCols, mNumRows,
    normalFators, lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, groundSRS);
  rationalModel->SetErrors(errBias, errRand);

  return rationalModel;
}

/**
 *  \brief Deserialize from rpc.txt file
 *
 *  \param [in] inStream input stream
 */
std::unique_ptr<RationalModel>
RationalModelMarshaller::DeserializeRpc(std::istream& inStream) {
  std::array<double, rnf::NumFact> normalFators{};
  std::array<double, RationalModel::NumCoef> lineNumCoef{};
  std::array<double, RationalModel::NumCoef> lineDenCoef{};
  std::array<double, RationalModel::NumCoef> sampNumCoef{};
  std::array<double, RationalModel::NumCoef> sampDenCoef{};
  
  std::string line;
  while (!inStream.eof()) {
    std::getline(inStream, line);
    boost::trim(line);
    if (line.empty()) continue;

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(" :\t\n"),
      boost::algorithm::token_compress_on);

    if (2 > tokens.size()) {
      throw std::exception("Each line contains 2+ tokens");
    }

    const auto& key = tokens[0];
    const auto& value = tokens[1];

    auto it = sRpcEntryMap.find(key);
    if (sRpcEntryMap.end() != it) {
      normalFators.at(it->second) = std::stod(value);
      continue;
    }

    // polynomial coefficients
    it = sRpcEntryMap.find(key.substr(0, kRPC_COEFF_KEY_LEN));
    if (sRpcEntryMap.end() == it) continue;
    
    switch (int idx{}; it->second) {
    case 10: // lineNumCoef
      idx = std::stoi(key.substr(kRPC_COEFF_KEY_IDX)) - 1;
      lineNumCoef[idx] = std::stod(value);
      break;
    case 11: // lineDenCoef
      idx = std::stoi(key.substr(kRPC_COEFF_KEY_IDX)) - 1;
      lineDenCoef[idx] = std::stod(value);
      break;
    case 12: // sampNumCoef
      idx = std::stoi(key.substr(kRPC_COEFF_KEY_IDX)) - 1;
      sampNumCoef[idx] = std::stod(value);
      break;
    case 13: // sampDenCoef
      idx = std::stoi(key.substr(kRPC_COEFF_KEY_IDX)) - 1;
      sampDenCoef[idx] = std::stod(value);
      break;
    default:
      assert(false);
    }
  }

  auto groundSRS = std::make_shared<SpatialReferenceSystem>("WGS84");
  return std::make_unique<RationalModel>(mNumCols, mNumRows, normalFators,
      lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, groundSRS);
}

/**
 *  \brief insert 20 coefficients
 *
 *  \param [in] outStream output stream
 *  \param [in] coefs 20 coefficients
 */
void RationalModelMarshaller::InsertCoefs(std::ostream& outStream,
  const std::array<double, RationalModel::NumCoef>& coefs) {
}

/**
 *  \brief extract 20 coefficients
 *  
 *  \param [in] inStream input stream
 *  \param [out] coefs 20 coefficients
 */
void RationalModelMarshaller::ExtractCoefs(std::istream & inStream,
  std::array<double, RationalModel::NumCoef>& coefs) {
  std::string line;
  for (size_t i = 0; i < RationalModel::NumCoef; ++i) {
    std::getline(inStream, line);
    boost::trim(line);
    coefs[i] = std::stod(line);
  }
}
