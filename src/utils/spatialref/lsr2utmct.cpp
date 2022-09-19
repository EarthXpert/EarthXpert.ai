#include "utils/lsr2utmct.h"
#include "core/math/rotation.h"
#include "core/spatialref/ccsutils.h"

using namespace earthxpert;
using namespace earthxpert::math;
using namespace Eigen;

LsrToUtmCT::LsrToUtmCT(const Point3d& origin) {
  ee.SetLsrOrigin(origin);
  Initialize();
}

void LsrToUtmCT::Initialize() {
  wgs84Srs = std::make_unique<OGRSpatialRef>();
  wgs84Srs->importFromEPSG(4326);  // WGS84
  utmSrs = std::make_unique<OGRSpatialRef>();
  //std::string utmWkt{ R"wkt(+proj=utm +zone=37 +datum=WGS84 +units=m +no_defs)wkt" };
  //utmSrs->importFromProj4(utmWkt.c_str());
  utmSrs->importFromEPSG(32637);
  g2mCt = std::make_unique<OgrCt>(wgs84Srs.get(), utmSrs.get());

  const char* datum = "WGE";
  ellParam = std::make_unique<ccs::GeodeticParameters>(ccs_ct::geodetic, ccs_ht::ellipsoidHeight);
  geocParam = std::make_unique<ccs::CoordinateSystemParameters>(ccs_ct::geocentric);
  utmParam = std::make_unique<ccs::UTMParameters>(ccs_ct::universalTransverseMercator); // 37, 0

  ccsGeodToGeoc = std::make_unique<ccs2>(datum, ellParam.get(), datum, geocParam.get());
  ccsGeocToUtm = std::make_unique<ccs2>(datum, geocParam.get(), datum, utmParam.get());
}

/**
 * 
 *  \param [in] pb scale, three rotation angles and three translations
 *  \return utm easting, northing
 */
Point2d LsrToUtmCT::Convert(const Point3d* lsr, const double* pb) const {
  Vector3d lsrS = Vector3d(lsr->x, lsr->y, lsr->z) * pb[0]; // scale

  Matrix3d rot;
  EulerXYZToMatrix(pb+1, rot);
  Vector3d lsrR = rot * lsrS;                               // rotate

  Vector3d lsrT = lsrR + Vector3d(pb[4], pb[5], pb[6]);     // translate

  const Point3d lph = ee.LSRToLPH(lsrT[0], lsrT[1], lsrT[2]); // LSR -> Geographic

  //Point3d utmT;
  //g2mCt->Transform(lph, utmT);
  //const auto utm = Point3To2(utmT);

  const auto geoc = ConvertGeodeticToGeocentric(*ccsGeodToGeoc, lph);
  const auto utm = ConvertGeocentricToUtm(*ccsGeocToUtm, geoc);
  return utm;
}
