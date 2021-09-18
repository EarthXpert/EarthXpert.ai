from enum import Enum, unique

@unique
class SensorType(Enum):
  eUNDEFINED = -1
  # physical sensor models
  eMT_ADS = 0       # Indicates an aerial push-broom sensor
  eMT_FRAME = 1     # Indicates a frame based pin-hole camera
  eMT_PUSHBROOM = 2 # Indicates a push-broom sensor
  eMT_ROLLING = 3   # Indicates an aerial push-broom sensor
  eMT_SAR = 4       # Indicates a synthetic aperture radar
  # replacement sensor models
  eMT_AFFINE = 5    # Indicates an affine model
  eMT_NETWORK = 6   # Indicates a neural network model
  eMT_RATIONAL = 7  # Indicates a rational polynomial model
  NumSensorTypes = 8
  