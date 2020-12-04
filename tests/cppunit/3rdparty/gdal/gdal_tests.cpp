#include "3rdparty/gtest/gtest_main_gdal.h"

#include <iostream>
#include <memory>

using namespace earthxpert;

TEST(gdal, CreateSRS) {
  auto wgs84Geog = std::make_unique<OGRSpatialReference>();
  ASSERT_TRUE(nullptr != wgs84Geog) << "returned nullptr";
  wgs84Geog->importFromEPSG(4326);

  std::string wgs84Wkt = R"wkt(GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137.0,298.257223563]],PRIMEM["Greenwich",0.0],UNIT["Degree",0.0174532925199433],AUTHORITY["EPSG",4326]])wkt";
  auto wgs84Geog2 = std::make_unique<OGRSpatialReference>();
  //wgs84Geog2->importFromESRI();

  std::unique_ptr<OGRSpatialReference> wgs84Geog3{ OGRSpatialReference::GetWGS84SRS()};
  ASSERT_TRUE(nullptr != wgs84Geog3) << "returned nullptr";
  ASSERT_TRUE(wgs84Geog->IsSameGeogCS(wgs84Geog3.get()));
}

/**
 *  \brief test the relationship between the horizontal position 
 *  and the vertical height between different datum
 */
TEST(gdal, CT3D) {
  // WGS84 GEOCS
  auto wgs84Geog = std::make_unique<OGRSpatialReference>();
  ASSERT_TRUE(nullptr != wgs84Geog) << "returned nullptr";
  wgs84Geog->importFromEPSG(4326);

  std::string wgs84Wkt = R"wkt(GEOGCS["GCS_WGS_1984",DATUM["D_WGS_1984",SPHEROID["WGS_1984",6378137.0,298.257223563]],PRIMEM["Greenwich",0.0],UNIT["Degree",0.0174532925199433],AUTHORITY["EPSG",4326]])wkt";
  auto wgs84GeogEsri = std::make_unique<OGRSpatialReference>();
  ASSERT_TRUE(nullptr != wgs84GeogEsri) << "returned nullptr";
  char* wgs84Esri[2] = { wgs84Wkt.data(), nullptr };
  wgs84GeogEsri->importFromESRI(wgs84Esri);
  ASSERT_TRUE(wgs84Geog->IsSameGeogCS(wgs84GeogEsri.get()));

  // NAD27 GEOCS
  auto nad27Geog = std::make_unique<OGRSpatialReference>();
  ASSERT_TRUE(nullptr != nad27Geog) << "returned nullptr";
  nad27Geog->importFromEPSG(4267);

  std::string nad27Wkt = R"wkt(GEOGCS["GCS_North_American_1927",DATUM["D_North_American_1927",SPHEROID["Clarke_1866",6378206.4,294.9786982]],PRIMEM["Greenwich",0.0],UNIT["Degree",0.0174532925199433],AUTHORITY["EPSG",4267]])wkt";
  auto nad27GeogEsri = std::make_unique<OGRSpatialReference>();
  ASSERT_TRUE(nullptr != nad27GeogEsri) << "returned nullptr";
  char* nad27Esri[2] = { nad27Wkt.data(), nullptr };
  nad27GeogEsri->importFromESRI(nad27Esri);
  ASSERT_TRUE(nad27Geog->IsSameGeogCS(nad27GeogEsri.get()));

  // UTM17 PROJCS
  auto utm17Wgs84 = std::make_unique<OGRSpatialReference>();
  ASSERT_TRUE(nullptr != utm17Wgs84) << "returned nullptr";
  utm17Wgs84->importFromEPSG(32617);

  std::string utm17Wkt = R"wkt(PROJCS["WGS_1984_UTM_Zone_17N", GEOGCS["GCS_WGS_1984", DATUM["D_WGS_1984", SPHEROID["WGS_1984", 6378137.0, 298.257223563]], PRIMEM["Greenwich", 0.0], UNIT["Degree", 0.0174532925199433]], PROJECTION["Transverse_Mercator"], PARAMETER["False_Easting", 500000.0], PARAMETER["False_Northing", 0.0], PARAMETER["Central_Meridian", -81.0], PARAMETER["Scale_Factor", 0.9996], PARAMETER["Latitude_Of_Origin", 0.0], UNIT["Meter", 1.0], AUTHORITY["EPSG", 32617]])wkt";
  auto utm17Wgs84Esri = std::make_unique<OGRSpatialReference>();
  ASSERT_TRUE(nullptr != utm17Wgs84Esri) << "returned nullptr";
  char* utm17Esri[2] = { utm17Wkt.data(), nullptr };
  utm17Wgs84Esri->importFromESRI(utm17Esri);
  ASSERT_TRUE(utm17Wgs84->IsSame(utm17Wgs84Esri.get()));

  std::cout.precision(16);

  // between NAD27 and WGS84
  //wgs84Geog->SetVertCS();
  //nad27Geog->SetVertCS();
  std::unique_ptr<OGRCoordinateTransformation>
    ctWgs84_Nad27{ OGRCreateCoordinateTransformation(wgs84Geog.get(), nad27Geog.get()) };

  double lon = -79.347015;  // Toronto
  double lat = 43.651070;
  double hgt = 10000;
  ctWgs84_Nad27->Transform(1, &lon, &lat, &hgt);
  std::cout << "lon: " << lon << " lat: " << lat << " hgt: " << hgt << std::endl;

  // WGS84
  std::unique_ptr<OGRCoordinateTransformation>
    ctWgs84_utm17{ OGRCreateCoordinateTransformation(wgs84Geog.get(), utm17Wgs84.get()) };
  ASSERT_TRUE(nullptr != ctWgs84_utm17) << "returned nullptr";
  std::unique_ptr<OGRCoordinateTransformation>
    ctUtm17_Wgs84{ OGRCreateCoordinateTransformation(utm17Wgs84.get(), wgs84Geog.get()) };
  ASSERT_TRUE(nullptr != ctUtm17_Wgs84) << "returned nullptr";

  double east{lon}, north{lat};
  ctWgs84_utm17->Transform(1, &east, &north);
  std::cout << "east: " << east << " north: " << north << std::endl;

  double lonN{ east }, latN{ north };
  ctUtm17_Wgs84->Transform(1, &lonN, &latN);
  std::cout << "lonN: " << lonN << " latN: " << latN << std::endl;
}
