1. Introduction

Design and implementation of sensor geometry models, including frame, rational, and pending (pushbroom, ADS, rolling-shutter), which describe the imaging relationship between the ground space and the imagery space.

List of fully implemented models: frame, rational.

2. Third-party libraries used:

a) GDAL

OGRSpatialReference: spatial reference system of the ground space.
OGRCoordTrasformation:

b) OpenCV

Mat1d: 3x3 rotation matrix
