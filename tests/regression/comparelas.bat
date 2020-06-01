@echo off
set EX_SCRIPT_DIR=%~d0%~p0
set EX_WORKSPACE_DIR=D:\home\workspace

set EX_WORKING_DIR=%EX_WORKSPACE_DIR%/install\windows\bin\release

set EX_TESTDATA_LAS_DIR=%EX_WORKSPACE_DIR%/TestData\PointClouds
set EX_TEST_OUT_DIR=%EX_WORKSPACE_DIR%/build\windows\vs2019ex\tests\output

set EX_GDAL_DIR=%EX_WORKSPACE_DIR%/3rd_party\windows\gdal_2.4.1\bin\release
set EX_LASTOOLS_DIR=%EX_WORKSPACE_DIR%/3rd_party\windows\lastools_19.05.07\bin\release

cd %EX_WORKING_DIR%

pointcloudwriter_tests.exe
REM %EX_LASTOOLS_DIR%/lasinfo64.exe %EX_TESTDATA_LAS_DIR%/points_small.las
REM %EX_LASTOOLS_DIR%/lasinfo64.exe %EX_TEST_OUT_DIR%/points_small_t.las
REM %EX_LASTOOLS_DIR%/lasinfo64.exe %EX_TEST_OUT_DIR%/points_small_t2.las

%EX_LASTOOLS_DIR%/lasinfo64.exe %EX_TESTDATA_LAS_DIR%/guided_tour_c3d/DEM/DSM.las
%EX_LASTOOLS_DIR%/lasinfo64.exe %EX_TEST_OUT_DIR%/DSM_t.las

cd %EX_SCRIPT_DIR%
