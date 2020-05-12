/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "marshaller/isensormodelmarshaller.h"
#include "sensormodel/rationalmodel.h"

#include <istream>
#include <ostream>
#include <string>

namespace watxtract {
/**
 *  The class RationalModelMarshaller serialize and deserialize rational models.
 *
 *  \detail An example .RPB file is given below:

 *  \codeblock
 *
 *  An example _rpc.txt file is given below:
 *
 *  \codeblock
 */
class WXMARSHALLER_EXPORTS RationalModelMarshaller {
public:
  // foramt of coefficients in mdoel file
  enum CoefFormat {
    eRPB,   // RPC00B
    eRPC    // RPC00A
  };

  void SetImageSize(const int numCols, const int numRows) {
    imgNumCols = numCols;
    imgNumRows = numRows;
  }

  /**
   *  \brief seralize a rational model into a .rpb file or a rpc.txt file
   *
   *  \param[in] modelFile the file to insert model coefficients
   *  \param[in] rationalModel the rational model to be seralized
   */
  void Serialize(const std::string& modelFile, 
    const RationalModel* const rationalModel);

  /**
   *  \brief deserialize rational model from a .RPB or rpc.txt file
   *
   *  \param [in] modelFile the name of the input file
   *  \return the pointer to the rational model object
   */
  std::unique_ptr<ISensorModel> Deserialize(const std::string& modelFile);

  // GDAL helper
  /**
   *  \brief GeoTIFF/NITF tags
   */
private:
  int imgNumCols{};
  int imgNumRows{};

  CoefFormat GetCoefFormat(const std::string& modelFile) const;

  /**
   *  \brief seralize a rational model into a .rpb file
   *
   *  \param[in] outStream output stream
   *  \param[in] rationalModel the rational model to be seralized
   */
  void SerializeRpb(std::ostream& outStream,
    const RationalModel* const rationalModel);

  /**
   *  \brief seralize a rational model into a rpc.txt file
   *
   *  \param[in] outStream output stream
   *  \param[in] rationalModel the rational model to be seralized
   */
  void SerializeRpc(std::ostream& outStream,
    const RationalModel* const rationalModel);

  /**
   *  \brief Deserialize .RPB file
   *
   *  \param [in] inStream input stream
   */
  std::unique_ptr<ISensorModel> DeserializeRpb(std::istream& inStream);

  /**
   *  \brief Deserialize _rpc.txt file
   *
   *  \param [in] inStream input stream
   */
  std::unique_ptr<ISensorModel> DeserializeRpc(std::istream& inStream);

  void InsertCoefs(std::ostream& outStream,
    const std::array<double, RationalModel::NumCoef>& coefs);
  void ExtractCoefs(std::istream& inStream,
    std::array<double, RationalModel::NumCoef>& coefs);
};
} // namespace watxtract
