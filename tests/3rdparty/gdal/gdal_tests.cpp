#include "3rdparty/gtest/gtest_main_gdal.h"

#include <iostream>
#include <memory>

using namespace earthxpert;

TEST(gdal, CreateWgs84SRS) {
  std::string wgs84Wkt = R"wkt(GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137.0,298.257223563]],PRIMEM["Greenwich",0.0],UNIT["Degree",0.0174532925199433],AUTHORITY["EPSG",4326]])wkt";
  auto wgs84Geog = std::make_unique<OGRSpatialReference>(wgs84Wkt.c_str());

  ASSERT_TRUE(nullptr != wgs84Geog) << "returned nullptr";
}

/**
 *  \brief test the relationship between the horizontal position 
 *  and the vertical height between different datum
 */
TEST(gdal, CT3D) {
  std::string wgs84Wkt = R"wkt(GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137.0,298.257223563]],PRIMEM["Greenwich",0.0],UNIT["Degree",0.0174532925199433],AUTHORITY["EPSG",4326]])wkt";
  auto wgs84Geog = std::make_unique<OGRSpatialReference>(wgs84Wkt.c_str());
  wgs84Geog->importFromEPSG(4326);

  std::string nad27Wkt = R"wkt(GEOGCS["GCS_North_American_1927",DATUM["D_North_American_1927",SPHEROID["Clarke_1866",6378206.4,294.9786982]],PRIMEM["Greenwich",0.0],UNIT["Degree",0.0174532925199433],AUTHORITY["EPSG",4267]])wkt";
  auto nad27Geog = std::make_unique<OGRSpatialReference>(nad27Wkt.c_str());
  nad27Geog->importFromEPSG(4267);

  std::string utm17Wkt = R"wkt(PROJCS["WGS_1984_UTM_Zone_17N", GEOGCS["GCS_WGS_1984", DATUM["D_WGS_1984", SPHEROID["WGS_1984", 6378137.0, 298.257223563]], PRIMEM["Greenwich", 0.0], UNIT["Degree", 0.0174532925199433]], PROJECTION["Transverse_Mercator"], PARAMETER["False_Easting", 500000.0], PARAMETER["False_Northing", 0.0], PARAMETER["Central_Meridian", -81.0], PARAMETER["Scale_Factor", 0.9996], PARAMETER["Latitude_Of_Origin", 0.0], UNIT["Meter", 1.0], AUTHORITY["EPSG", 32617]])wkt";
  auto utm17Wgs84 = std::make_unique<OGRSpatialReference>(nad27Wkt.c_str());
  utm17Wgs84->importFromEPSG(32617);

  std::cout.precision(16);

  // between NAD27 and WGS84
  std::unique_ptr<OGRCoordinateTransformation>
    ctWgs84_Nad27{ OGRCreateCoordinateTransformation(wgs84Geog.get(), nad27Geog.get()) };

  double lon = -79.347015;
  double lat = 43.651070;
  double hgt = 10000;
  ctWgs84_Nad27->Transform(1, &lon, &lat, &hgt);
  std::cout << "lon: " << lon << " lat: " << lat << " hgt: " << hgt << std::endl;

  // WGS84
  std::unique_ptr<OGRCoordinateTransformation>
    ctWgs84_utm17{ OGRCreateCoordinateTransformation(wgs84Geog.get(), utm17Wgs84.get()) };
  std::unique_ptr<OGRCoordinateTransformation>
    ctUtm17_Wgs84{ OGRCreateCoordinateTransformation(utm17Wgs84.get(), wgs84Geog.get()) };

  double east{lon}, north{lat};
  ctWgs84_utm17->Transform(1, &east, &north);
  std::cout << "east: " << east << " north: " << north << std::endl;

  double lonN{ east }, latN{ north };
  ctUtm17_Wgs84->Transform(1, &lonN, &latN);
  std::cout << "lonN: " << lonN << " latN: " << latN << std::endl;
}
