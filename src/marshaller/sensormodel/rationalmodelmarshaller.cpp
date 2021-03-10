/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "marshaller/sensormodel/rationalmodelmarshaller.h"
#include "core/exceptions/exexcept.h"
#include "core/utils/stringutils.h"
#include "sensormodel/rationalmodel.h"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <map>

using namespace earthxpert;
using utils::CompareI;

namespace {
  // rpb entries
  std::map<std::string, int> sRpbEntryMap = {
    {"satId", 0},
    {"bandId", 1},
    {"SpecId", 2},
    {"errBias", 3},
    {"errRand", 4},
    {"lineOffset", 5},
    {"sampOffset", 6},
    {"latOffset", 7},
    {"longOffset", 8},
    {"heightOffset", 9},
    {"lineScale", 10},
    {"sampScale", 11},
    {"latScale", 12},
    {"longScale", 13},
    {"heightScale", 14},
    {"lineNumCoef", 15},
    {"lineDenCoef", 16},
    {"sampNumCoef", 17},
    {"sampDenCoef", 18},
    {"lineCorCoef", 19},
    {"sampCorCoef", 20} };

  // rpc entries
  std::map<std::string, int> sRpcEntryMap = {
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
    {"SAMP_DEN_COEFF", 13},
    {"LINE_COR_COEFF", 14},
    {"SAMP_COR_COEFF", 15} };

  constexpr size_t kRPC_COEFF_KEY_LEN = 14;  // lenght of coefficient key's string component
  constexpr size_t kRPC_COEFF_KEY_IDX = 15;  // position of index component
}

/**
 *  \brief seralize a rational model to a .rpb file or a rpc.txt file
 *
 *  \param[in] modelFile the file to insert model coefficients
 *  \param[in] ratModel the rational model to be seralized
 */
void RationalModelMarshaller::Serialize(const std::string &modelFile,
    RationalModel const *const ratModel) {
  const auto fmt = GetFormat(modelFile);

  std::ofstream ofs(modelFile.c_str());
  if (!ofs.good()) 
    throw std::invalid_argument(_("Cannot create file."));

  if (eRPB == fmt) {
    return SerializeRpb(ofs, ratModel);
  }
  else if (eRPC == fmt) {
    return SerializeRpc(ofs, ratModel);
  }
  else {
    throw std::invalid_argument(_("Not a rational model file."));
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
  const auto fmt = GetFormat(modelFile);

  std::ifstream ifs(modelFile.c_str());
  if (!ifs.good()) 
    throw std::invalid_argument(_("Failed to open file."));

  if (eRPB == fmt) {
    return DeserializeRpb(ifs);
  } 
  else if (eRPC == fmt) {
    return DeserializeRpc(ifs);
  } 
  else {
    throw std::invalid_argument(_("Not a rational model file.")); 
  }
}

/**
 *  \brief parse coefficeints format from the model's filename, i.e., .RPB or rpc.txt
 *
 *  \param [in] modelFile the name of the input file
 */
RationalModelMarshaller::RMFormat 
RationalModelMarshaller::GetFormat(const std::string& modelFile) const {
  if (CompareI(modelFile.substr(modelFile.length() - 4), ".rpb")) {
    return eRPB;
  }
  else if (CompareI(modelFile.substr(modelFile.length() - 8), "_rpc.txt")) {
    return eRPC;
  }
  else {
    throw std::invalid_argument(_("Not a rational model file."));
  }
}

/**
 *  \brief seralize a rational model into a .rpb file
 *
 *  \param[in] outS output stream
 *  \param[in] ratModel the rational model to be seralized
 */
void RationalModelMarshaller::SerializeRpb(std::ostream& outS,
  const RationalModel* const ratModel) {

  outS << "BEGIN_GROUP = IMAGE\n";

  outS << "END_GROUP = IMAGE\n";
  outS << "END;\n";

  if (!ratModel->mCorrFlag) return;

  outS << "BEGIN_GROUP = CORR\n";
  outS << "END_GROUP = CORR";
}

/**
 *  \brief seralize a rational model into a rpc.txt file
 *
 *  \param[in] outS output stream
 *  \param[in] ratModel the rational model to be seralized
 */
void RationalModelMarshaller::SerializeRpc(std::ostream& outS,
  const RationalModel* const ratModel) {
  outS << std::setprecision(9);

  auto nfc = ratModel->nfc; // normalization factors
  outS << "LINE_OFF: " << nfc[0] << " pixels\n";
  outS << "SAMP_OFF: " << nfc[1] << " pixels\n";
  outS << "LAT_OFF: " << nfc[2] << " degrees\n";
  outS << "LONG_OFF: " << nfc[3] << " degrees\n";
  outS << "HEIGHT_OFF: " << nfc[4] << " meters\n";
  outS << "LINE_SCALE: " << nfc[5] << " pixels\n";
  outS << "SAMP_SCALE: " << nfc[6] << " pixels\n";
  outS << "LAT_SCALE: " << nfc[7] << " degrees\n";
  outS << "LONG_SCALE: " << nfc[8] << " degrees\n";
  outS << "HEIGHT_SCALE: " << nfc[9] << " meters\n";

  outS << std::setprecision(15) 
    << std::setiosflags(std::ios::scientific | std::ios::showpos);

  // rpc coefficients
  for (int i = 0; i < RationalModel::NumCoef; ++i) {
    outS << "LINE_NUM_COEFF_" << i + 1 << ": " << ratModel->lnc[i] << "\n";
  }
  for (int i = 0; i < RationalModel::NumCoef; ++i) {
    outS << "LINE_DEN_COEFF_" << i + 1 << ": " << ratModel->ldc[i] << "\n";
  }
  for (int i = 0; i < RationalModel::NumCoef; ++i) {
    outS << "SAMP_NUM_COEFF_" << i + 1 << ": " << ratModel->snc[i] << "\n";
  }
  for (int i = 0; i < RationalModel::NumCoef; ++i) {
    outS << "SAMP_DEN_COEFF_" << i + 1 << ": " << ratModel->sdc[i] << "\n";
  }

  if (!ratModel->mCorrFlag) return;

  // correction coeffcients
  for (int i = 0; i < RationalModel::NumCorr; ++i) {
    outS << "LINE_COR_COEFF_" << i + 1 << ": " << ratModel->mLineCorr[i] << "\n";
  }
  for (int i = 0; i < RationalModel::NumCorr; ++i) {
    outS << "SAMP_COR_COEFF_" << i + 1 << ": " << ratModel->mSampCorr[i] << "\n";
  }
}

/**
 *  \brief Deserialize from .RPB file
 *
 *  \param [in] inS input stream
 *  \todo string_view
 */
std::unique_ptr<RationalModel>
RationalModelMarshaller::DeserializeRpb(std::istream& inS) {
  double errBias{}, errRand{};
  std::array<double, rnf::NumFact> nfc{};
  std::array<double, RationalModel::NumCoef> lineNumCoef{};
  std::array<double, RationalModel::NumCoef> lineDenCoef{};
  std::array<double, RationalModel::NumCoef> sampNumCoef{};
  std::array<double, RationalModel::NumCoef> sampDenCoef{};

  std::string line;
  while (!inS.eof()) {
    std::getline(inS, line);
    boost::trim(line);
    if (line.empty()) continue;

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(" =;,\t\n"),
      boost::algorithm::token_compress_on);

    if (tokens.empty() || 3 < tokens.size()) 
      throw std::exception(_("Each line contains 1 to 3 tokens"));

    const auto& key = tokens[0];
    
    auto it = sRpbEntryMap.find(key);
    if (sRpbEntryMap.end() == it) continue;
    
    switch (it->second) {
    case 3: // errBias
      errBias = std::stod(tokens[1]);
      break;
    case 4: // errRand
      errRand = std::stod(tokens[1]);
      break;
    case 5: // range 5 ... 14
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
      nfc.at(it->second) = std::stod(tokens[1]);
      break;
    case 15: // lineNumCoef
      ExtractCoefs(inS, lineNumCoef);
      break;
    case 16: // lineDenCoef
      ExtractCoefs(inS, lineDenCoef);
      break;
    case 17: // sampNumCoef
      ExtractCoefs(inS, sampNumCoef);
      break;
    case 18: // sampDenCoef
      ExtractCoefs(inS, sampDenCoef);
      break;
    case 19: // lineCorCoef
      //ExtractCorr(inS, lineCorCoef);
      break;
    case 20: // sampCorCoef
      //ExtractCorr(inS, sampCorCoef);
      break;
    default:
      assert(false && "wrong entry.");
    }
  }

  auto groundSRS = std::make_shared<OGRSpatialRef>("WGS84");
  auto ratModel = std::make_unique<RationalModel>(mNumCols, mNumRows,
    nfc, lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, groundSRS);
  ratModel->SetErrors(errBias, errRand);

  return ratModel;
}

/**
 *  \brief Deserialize from rpc.txt file
 *
 *  \param [in] inS input stream
 */
std::unique_ptr<RationalModel>
RationalModelMarshaller::DeserializeRpc(std::istream& inS) {
  std::array<double, rnf::NumFact> nfc{};
  std::array<double, RationalModel::NumCoef> lineNumCoef{};
  std::array<double, RationalModel::NumCoef> lineDenCoef{};
  std::array<double, RationalModel::NumCoef> sampNumCoef{};
  std::array<double, RationalModel::NumCoef> sampDenCoef{};
  
  std::string line;
  while (!inS.eof()) {
    std::getline(inS, line);
    boost::trim(line);
    if (line.empty()) continue;

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(" :\t\n"),
      boost::algorithm::token_compress_on);

    if (2 > tokens.size()) 
      throw std::exception(_("Each line contains 2+ tokens"));

    const auto& key = tokens[0];
    const auto& value = tokens[1];

    auto it = sRpcEntryMap.find(key);
    if (sRpcEntryMap.end() != it) {
      nfc.at(it->second) = std::stod(value);
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
    case 14: // lineCorCoef
      //idx = std::stoi(key.substr(kRPC_COEFF_KEY_IDX)) - 1;
      //lineCorCoef[idx] = std::stod(value);
      break;
    case 15: // sampCorCoef
      //idx = std::stoi(key.substr(kRPC_COEFF_KEY_IDX)) - 1;
      //sampCorCoef[idx] = std::stod(value);
      break;
    default:
      assert(false && "wrong entry.");
    }
  }

  auto groundSRS = std::make_shared<OGRSpatialRef>("WGS84");
  return std::make_unique<RationalModel>(mNumCols, mNumRows, nfc,
      lineNumCoef, lineDenCoef, sampNumCoef, sampDenCoef, groundSRS);
}

/**
 *  \brief insert 20 coefficients
 *
 *  \param [in] outS output stream
 *  \param [in] coefs 20 coefficients
 */
void RationalModelMarshaller::InsertCoefs(std::ostream& outS,
  const std::array<double, RationalModel::NumCoef>& coefs) {
}

/**
 *  \brief extract 20 coefficients
 *  
 *  \param [in] inS input stream
 *  \param [out] coefs 20 coefficients
 */
void RationalModelMarshaller::ExtractCoefs(std::istream & inS,
  std::array<double, RationalModel::NumCoef>& coefs) {
  std::string line;
  for (size_t i = 0; i < RationalModel::NumCoef; ++i) {
    std::getline(inS, line);
    boost::trim(line);
    coefs[i] = std::stod(line);
  }
}
