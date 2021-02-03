/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/exmarshaller.h"
#include "sensormodel/rationalmodel.h"

#include <istream>
#include <ostream>

namespace earthxpert {
/**
 *  The class RationalModelMarshaller serialize and deserialize rational models.
 *
 *  \detail An example .RPB file is given below:

 *  \codeblock
 *
 *  An example _rpc.txt file is given below:
 *
 *  \codeblock
 *
 *  \todo append correction polynomials to .RPB & _rpc.txt
 */
class EXMARSHALLER_EXPORTS RationalModelMarshaller {
public:
  // foramt of coefficients in mdoel file
  enum RMFormat {
    eRPB,   // RPC00B
    eRPC    // RPC00A
  };

  void SetImageSize(const int numCols, const int numRows) {
    mNumCols = numCols;
    mNumRows = numRows;
  }

  /**
   *  \brief seralize a rational model into a .rpb file or a rpc.txt file
   *
   *  \param[in] modelFile the file to insert model coefficients
   *  \param[in] rationalModel the rational model to be seralized
   */
  void Serialize(const std::string& modelFile, const RationalModel* rationalModel);

  /**
   *  \brief deserialize rational model from a .RPB or rpc.txt file
   *
   *  \param [in] modelFile the name of the input file
   *  \return the pointer to the rational model object
   */
  std::unique_ptr<RationalModel> Deserialize(const std::string& modelFile);

  // GDAL helper
  /**
   *  \brief GeoTIFF/NITF tags
   */
private:
  int mNumCols{};
  int mNumRows{};

  RMFormat GetFormat(const std::string& modelFile) const;

  /**
   *  \brief seralize a rational model into a .rpb file
   *
   *  \param[in] outStream output stream
   *  \param[in] rationalModel the rational model to be seralized
   */
  void SerializeRpb(std::ostream& outStream, const RationalModel* rationalModel);

  /**
   *  \brief seralize a rational model into a rpc.txt file
   *
   *  \param[in] outStream output stream
   *  \param[in] rationalModel the rational model to be seralized
   */
  void SerializeRpc(std::ostream& outStream, const RationalModel* rationalModel);

  /**
   *  \brief Deserialize .RPB file
   *
   *  \param [in] inStream input stream
   */
  std::unique_ptr<RationalModel> DeserializeRpb(std::istream& inStream);

  /**
   *  \brief Deserialize _rpc.txt file
   *
   *  \param [in] inStream input stream
   */
  std::unique_ptr<RationalModel> DeserializeRpc(std::istream& inStream);

  void InsertCoefs(std::ostream& outStream,
    const std::array<double, RationalModel::NumCoef>& coefs);
  void ExtractCoefs(std::istream& inStream,
    std::array<double, RationalModel::NumCoef>& coefs);
};

using RMFormat = RationalModelMarshaller::RMFormat;
using UPRationalModelMarshaller = std::unique_ptr<RationalModelMarshaller>;
using SPRationalModelMarshaller = std::shared_ptr<RationalModelMarshaller>;

// helper free functions
void inline SaveRationalModel(const std::string& modelFile,
  const RationalModel* rationalModel) {
  RationalModelMarshaller marshaller;
  marshaller.Serialize(modelFile, rationalModel);
}

std::unique_ptr<RationalModel> inline 
LoadRationalModel(const std::string& modelFile) {
  RationalModelMarshaller marshaller;
  return marshaller.Deserialize(modelFile);
}
} // namespace earthxpert
