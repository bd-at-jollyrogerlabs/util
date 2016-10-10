// Copyright 2016 by Brian Davis, all rights reserved.

#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>

namespace jrl
{

using IntPair = std::pair<int, int>;
using HelpPair = std::pair<char, const char *>;

template<typename FirstType, typename SecondType>
std::ostream &
operator<<(std::ostream &strm, const typename std::pair<FirstType, SecondType> &val)
{
  strm << "(" << val.first << "," << val.second << ")";
  return strm;
}

} // namespace jrl

#include "jrl_macro"
#include "hash_map" // also includes hash_set
#include "stl_usability/algorithm_usability"
#include "stl_usability/ostream_innerator"

using namespace jrl;

using HashSet = hash_set<int>;
using HashMap = hash_map<int, int>;

using namespace std;
namespace po = boost::program_options;

/**
 * Line iteration proxy, cribbed from stackoverflow:
 * http://stackoverflow.com/questions/1567082/how-do-i-iterate-over-cin-line-by-line-in-c/1567703
 */
// @todo move this into usability
class Line
{
  std::string line_;
public:
  friend std::istream &operator>>(std::istream &strm, Line &line)
  {
    std::getline(strm, line.line_);
    return strm;
  }

  operator std::string() const
  {
    return line_;
  }

  operator const char *() const
  {
    return line_.c_str();
  }
};

template<typename IStreamIterator>
struct StreamRange
{
  auto
  begin()
  {
    return IStreamIterator(cin);
  }

  auto
  end()
  {
    return IStreamIterator();
  }
};

namespace
{

// command language
const char HELP_CMD = '?';

const char INSERT_CMD = 'i';
const char EMPLACE_CMD = 'm';
const char EMPLACE_HINT_CMD = 'M';
const char ERASE_CMD = 'e';
const char ITERATE_CMD = 'T';
const char CLEAR_CMD = 'c';
const char FIND_CMD = 'f';
const char LOAD_FACTOR_CMD = 'l';
const char SIZE_CMD = 's';

const vector<HelpPair> COMMANDS{
  make_pair(INSERT_CMD, "insert"),
  make_pair(EMPLACE_CMD, "emplace"),
  make_pair(EMPLACE_HINT_CMD, "emplace (hint)"),
  make_pair(ERASE_CMD, "erase"),
  make_pair(ITERATE_CMD, "iterate"),
  make_pair(CLEAR_CMD, "clear"),
  make_pair(FIND_CMD, "find"),
  make_pair(LOAD_FACTOR_CMD, "load factor"),
  make_pair(SIZE_CMD, "size")
};

/********** globals **********/

HashSet hs_g;
HashMap hm_g;

} // unnamed namespace

/********** support functions **********/

void
usage(const char * const program, const po::options_description &desc)
{
  cerr << "usage: " << program << desc << endl;
  exit(1);
}

string
trim(const string& line)
{
  THROW_ON_FAIL((' ' == line[1]), runtime_error, "second character of line ["
		<< line << "] was not a space as expected");
  return line.substr(2);
}

using Tokenizer = boost::tokenizer<>;
using TokenIterator = Tokenizer::iterator;

void
increment(TokenIterator &itr, TokenIterator end, const char *info)
{
  THROW_ON_FAIL((end != itr), runtime_error, "no " << info << " supplied");
  ++itr;
  THROW_ON_FAIL((end != itr), runtime_error, "no " << info << " supplied");
}

void
doInsert(TokenIterator itr, TokenIterator end, const bool isMap)
{
  increment(itr, end, "key");
  int key  = stoi(*itr);
  if (isMap) {
    increment(itr, end, "value");
    int value = stoi(*itr);
    hm_g.insert(make_pair(key, value));
  }
  else {
    hs_g.insert(key);
  }
  cout << "inserted" << endl;
}

void
doEmplace(TokenIterator itr, TokenIterator end, const bool isMap)
{
  increment(itr, end, "key");
  int key  = stoi(*itr);
  if (isMap) {
    increment(itr, end, "value");
    int value = stoi(*itr);
    hm_g.emplace(move(key), move(value));
  }
  else {
    hs_g.emplace(move(key));
  }
  cout << "emplaced" << endl;
}

void
doEmplaceHint(TokenIterator itr, TokenIterator end, const bool isMap)
{
  increment(itr, end, "key");
  int key  = stoi(*itr);
  if (isMap) {
    increment(itr, end, "value");
    int value = stoi(*itr);
    hm_g.emplace_hint(hm_g.cend(), move(key), move(value));
  }
  else {
    hs_g.emplace_hint(hs_g.cend(), move(key));
  }
  cout << "emplaced" << endl;
}

void
doErase(TokenIterator &itr, TokenIterator end, const bool isMap)
{
  increment(itr, end, "key");
  int key  = stoi(*itr);
  if (isMap) {
    hm_g.erase(key);
  }
  else {
    hs_g.erase(key);
  }
  cout << "erased or not present" << endl;
}

void
doIterate(const bool isMap)
{
  cout << "entries: [";
  if (isMap) {
    copy(hm_g, ostream_innerator<IntPair>(cout, ","));
  }
  else {
    copy(hs_g, ostream_innerator<int>(cout, ","));
  }
  cout << "]" << endl;
}

void
doClear(const bool isMap)
{
  if (isMap) {
    hm_g.clear();
  }
  else {
    hs_g.clear();
  }
  cout << "cleared" << endl;
}

#define DO_FIND(container)				\
  do {							\
    auto entry = container.find(stoi(key));		\
    if (container.end() == entry) {			\
      cout << "not found" << endl;			\
    }							\
    else {						\
      cout << "entry: [" << *entry << "]" << endl;	\
    }							\
  } while (0)

void
doFind(const string& line, const bool isMap)
{
  auto key = trim(line);
  if (isMap) {
    DO_FIND(hm_g);
  }
  else {
    DO_FIND(hs_g);
  }
}

#undef DO_FIND

void
doLoadFactor(const bool isMap)
{
  if (isMap) {
    cout << "load factor [" << hm_g.load_factor() << "]" << endl;
  }
  else {
    cout << "load factor [" << hs_g.load_factor() << "]" << endl;
  }
}

void
doSize(const bool isMap) {
  if (isMap) {
    cout << "size [" << hm_g.size() << "]" << endl;
    return;
  }
  cout << "size [" << hs_g.size() << "]" << endl;
}

int
main(const int argc,
     const char *argv[])
{
  try {
    po::variables_map options;
    po::options_description desc("options");
    desc.add_options()
      /* --help or -h produces help message */
      ("help,h", "help message")
      ("map,m", "test hash map (default is to test hash set)");
    po::store(po::parse_command_line(argc, argv, desc), options);
    if (options.count("help")) {
      usage(argv[0], desc);
    }
    const bool isTestMap = (1 == options.count("map"));

    cout << "enter commands (? for help, EOF to terminate): " << endl;
    for (const string &line : StreamRange<istream_iterator<Line>>()) {
      try {
	Tokenizer tokenizer(line);
	TokenIterator itr = tokenizer.begin();
	switch (line[0]) {
	case HELP_CMD:
	  cout << "commands:\n";
	  copy(COMMANDS, ostream_innerator<HelpPair>(cout, "\n"));
	  cout << endl;
	  break;

	case INSERT_CMD:
	  doInsert(itr, tokenizer.end(), isTestMap);
	  break;

	case EMPLACE_CMD:
	  doEmplace(itr, tokenizer.end(), isTestMap);
	  break;

	case EMPLACE_HINT_CMD:
	  doEmplaceHint(itr, tokenizer.end(), isTestMap);
	  break;

	case ERASE_CMD:
	  doErase(itr, tokenizer.end(), isTestMap);
	  break;

	case ITERATE_CMD:
	  doIterate(isTestMap);
	  break;

	case CLEAR_CMD:
	  doClear(isTestMap);
	  break;

	case FIND_CMD:
	  doFind(line, isTestMap);
	  break;

	case LOAD_FACTOR_CMD:
	  doLoadFactor(isTestMap);
	  break;

	case SIZE_CMD:
	  doSize(isTestMap);
	  break;

	default:
	  cerr << "ERROR: unknown command '" << line[0] << "'" << endl;
	}
      }
      catch (const exception& exc) {
	cerr << "ERROR: caught exception processing line: " << exc.what() << endl;
      }
      catch (...) {
	cerr << "ERROR: caught unknown exception processing line" << endl;
      }
      cout << ": ";
    }
  }
  catch (const exception& exc) {
    cerr << "ERROR: caught exception at top level: " << exc.what() << endl;
    return 1;
  }
  catch (...) {
    cerr << "ERROR: caught unknown exception at top level" << endl;
    return 1;
  }
  return 0;
}

#if 0

// simple_example_1.cpp
#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>

int main(){
   using namespace std;
   using namespace boost;
   string s = "This is,  a test";
   tokenizer<> tok(s);
   for(tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg){
       cout << *beg << "\n";
   }
}

#endif
