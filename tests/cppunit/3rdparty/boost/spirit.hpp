/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <boost/fusion/include/adapted.hpp>

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/lex.hpp>
#include <boost/spirit/include/qi.hpp>

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace bfu = boost::fusion;
namespace bsp = boost::spirit;
namespace bsq = bsp::qi;

struct Sample {                        //creation of struct Sample
  double one;
  std::string two;
  double three;
  std::string four;
};

BOOST_FUSION_ADAPT_STRUCT(Sample, one, two, three, four);

// use parser iterator to access the elements sequentially in random access Sequence
template <typename Iterator> 
struct SampleParser : bsq::grammar<Iterator, Sample()> {
  SampleParser() : SampleParser::base_type(start) {
    using namespace bsq;

    _11c = repeat(11)[char_];
    start = skip(space)["Sample" >> double_ >> _11c >> double_ >> _11c];
  }

private:
  bsq::rule<Iterator, Sample()> start;
  bsq::rule<Iterator, std::string()> _11c;
};

//using std string to parse input into the struct Sample directly
TEST(bsq, adapt_struct) {
  using It = std::string::const_iterator;
  const std::string input("Sample 421 123778901 +Inf ejgfdsghijk  ");

  It f = input.begin(), l = input.end();
  SampleParser<It> parser;//using sampleparser and naming it as parser 
  Sample example;

  //check it the input given through parser iterator is correct or valid
  if (phrase_parse(f, l, parser, bsq::ascii::space, example)) {
    std::cout << "parsed: " << bfu::as_vector(example) << "\n";
    std::cout << " example.one: " << example.one << "\n";
    std::cout << " example.two: '" << example.two << "'\n";
    std::cout << " example.three: " << example.three << "\n";
    std::cout << " example.four: '" << example.four << "'\n";
  }
  else {
    std::cout << "couldn't parse '" << input << "'\n";
  }

  if (f != l)
    std::cout << "Remaining unparsed input: '" << std::string(f, l) << "'\n";
}
