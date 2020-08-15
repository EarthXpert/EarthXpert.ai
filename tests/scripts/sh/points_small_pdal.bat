set CWD_DIR=%~d0%~p0
cd D:\home\workspace\install\windows\bin\release
pdal pipeline D:/home/workspace/TestData/PointClouds/points_small_pdal_pipeline.json
cd %CWD_DIR%