
  // data types

  //structures

  // classes

  // free functions
  m.def("generateFrameEpipolar", &GenerateFrameEpipolar, R"pbdoc(
        Generate epipolar images for frame model)pbdoc");
  m.def("generateRationalEpipolar", &GenerateRationalEpipolar, R"pbdoc(
        Generate epipolar images for rational model)pbdoc");
