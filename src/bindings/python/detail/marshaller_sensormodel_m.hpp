
// data types

//structures

// classes

// free functions
m.def("saveRationalModel", &SaveRationalModel, R"pbdoc(
        Save rational model to a text file)pbdoc");
m.def("loadRationalModel", &LoadRationalModel, R"pbdoc(
        Load rational model from a text file)pbdoc");
