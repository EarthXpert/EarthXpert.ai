#include <cstdlib>
#include <functional>
#include <iostream>

using FunctionType = std::function<double(const nc::NdArray<double>&, const nc::NdArray<double>&)>;

// https://en.wikipedia.org/wiki/Gaussian_function
double baseGaussianFunction(const nc::NdArray<double>& coordinates,
  const nc::NdArray<double>& betas) {
  const auto x = coordinates.at(0);
  const auto y = coordinates.at(1);

  const auto a = betas.at(0);
  const auto x0 = betas.at(2);
  const auto y0 = betas.at(3);
  const auto sigmaX = betas.at(4);
  const auto sigmaY = betas.at(5);

  return a * nc::exp(-(nc::square(x - x0) / (2.0 * nc::square(sigmaX)) + nc::square(y - y0) / (2.0 * nc::square(sigmaY))));
}

// It is desired to find a curve (model function) of the form
double gaussianFunction(const nc::NdArray<double>& coordinates,
  const nc::NdArray<double>& betas) {
  const auto dcOffset = betas.at(1);
  return baseGaussianFunction(coordinates, betas) + dcOffset;
}

/**
 *  TestCase 0:
 */
TEST(numcpp, Gaussian2D) {
  // create some data points to describe a two-dimensional gaussian function
  auto coords = nc::arange<double>(-3.0, 3.1, 0.1);
  auto [y, x] = nc::meshgrid<double>(coords, coords);
  auto coordinates = nc::vstack({ x.flatten(), y.flatten() }).transpose();

  // randomize some truth values for the model parameters
  nc::random::seed(666); // set the random seed for repeatability

  // make some random "truth" parameters for generating data
  const auto aTruth = nc::random::randFloat<double>(900.0, 1100.0);
  const auto dcOffsetTruth = nc::random::randFloat<double>(90.0, 100.0);
  const auto x0Truth = nc::random::randFloat<double>(-0.5, 0.5);
  const auto y0Truth = nc::random::randFloat<double>(-0.5, 0.5);
  const auto sigmaXTruth = nc::random::randFloat<double>(0.9, 1.1);
  const auto sigmaYTruth = nc::random::randFloat<double>(0.9, 1.1);

  nc::NdArray<double> betasTruth = { aTruth, dcOffsetTruth, x0Truth, y0Truth, sigmaXTruth, sigmaYTruth };

  // make some "measurements"
  nc::NdArray<double> measurements(1, coordinates.shape().rows);
  const auto cSlice = coordinates.cSlice();
  for (nc::uint32 i = 0; i < measurements.size(); ++i) {
    measurements[i] = gaussianFunction(coordinates(i, cSlice), betasTruth);
  }

  // add some noise to the measurements
  auto noise = nc::random::randN<double>(measurements.shape()) * 10.0; // STD = 10 of noise
  measurements += noise;

  // partial derivative of gaussianFunction with respect to a
  FunctionType delFdelA = [](const nc::NdArray<double>& coordinates,
    const nc::NdArray<double>& betas) -> double {
      const auto a = betas.at(0);
      return baseGaussianFunction(coordinates, betas) / a;
  };

  // partial derivative of gaussianFunction with respect to dcOffset
  FunctionType delFdelDcOffset = [](const nc::NdArray<double>& /*coordinates*/,
    const nc::NdArray<double>& /*betas*/) -> double {
      return 1.0;
  };

  // partial derivative of gaussianFunction with respect to x0
  FunctionType delFdelX0 = [](const nc::NdArray<double>& coordinates,
    const nc::NdArray<double>& betas) -> double {
      const auto x = coordinates.at(0);

      const auto x0 = betas.at(2);
      const auto sigmaX = betas.at(4);

      return (x - x0) / nc::square(sigmaX) * baseGaussianFunction(coordinates, betas);
  };

  // partial derivative of gaussianFunction with respect to y0
  FunctionType delFdelY0 = [](const nc::NdArray<double>& coordinates,
    const nc::NdArray<double>& betas) -> double {
      const auto y = coordinates.at(1);

      const auto y0 = betas.at(3);
      const auto sigmaY = betas.at(5);

      return (y - y0) / nc::square(sigmaY) * baseGaussianFunction(coordinates, betas);
  };

  // partial derivative of gaussianFunction with respect to sigmaX
  FunctionType delFdelSigmaX = [](const nc::NdArray<double>& coordinates,
    const nc::NdArray<double>& betas) -> double {
      const auto x = coordinates.at(0);

      const auto x0 = betas.at(2);
      const auto sigmaX = betas.at(4);

      return (nc::square(x - x0) / nc::power(sigmaX, 3)) * baseGaussianFunction(coordinates, betas);
  };

  // partial derivative of gaussianFunction with respect to sigmaY
  FunctionType delFdelSigmaY = [](const nc::NdArray<double>& coordinates,
    const nc::NdArray<double>& betas) -> double {
      const auto y = coordinates.at(1);

      const auto y0 = betas.at(2);
      const auto sigmaY = betas.at(4);

      return (nc::square(y - y0) / nc::power(sigmaY, 3)) * baseGaussianFunction(coordinates, betas);
  };

  // starting with the initial estimates of and calculating after 5 iterations
  const nc::uint32 numIterations = 5;
  const double aGuess = aTruth + nc::random::randN<double>() * 5.0;
  const double dcOffsetGuess = dcOffsetTruth + nc::random::randN<double>() * 5.0;
  const double x0Guess = x0Truth + nc::random::randN<double>() * 0.2;
  const double y0Guess = y0Truth + nc::random::randN<double>() * 0.2;
  const double sigmaXGuess = sigmaXTruth + nc::random::randN<double>() * 0.2;
  const double sigmaYGuess = sigmaYTruth + nc::random::randN<double>() * 0.2;

  auto [betas, rms] = nc::linalg::gaussNewtonNlls(numIterations,
    coordinates, measurements, FunctionType(gaussianFunction),
    { delFdelA, delFdelDcOffset, delFdelX0, delFdelY0, delFdelSigmaX, delFdelSigmaY },
    aGuess, dcOffsetGuess, x0Guess, y0Guess, sigmaXGuess, sigmaYGuess);

  nc::NdArray<double> initialGuess = { aGuess, dcOffsetGuess,
    x0Guess, y0Guess, sigmaXGuess, sigmaYGuess };

  std::cout << "truth values  = " << betasTruth;
  std::cout << "initial guess = " << initialGuess;
  std::cout << "beta values   = " << betas;
  std::cout << "RMS = " << rms << '\n';
}
