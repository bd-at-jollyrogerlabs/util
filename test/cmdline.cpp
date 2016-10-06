// Copyright 2016 by Brian Davis, all rights reserved.

#include <iostream>
#include <iterator>
#include <vector>

#include <jrl_macro>
#include <stl_usability/algorithm_usability>
#include <stl_usability/ostream_innerator>

#include "cmdline_config.h"

using namespace std;

/**
 * Line iteration proxy, cribbed from stackoverflow:
 * http://stackoverflow.com/questions/1567082/how-do-i-iterate-over-cin-line-by-line-in-c/1567703
 */
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

const char HELP_CMD = '?';

const char INSERT_CMD = 'i';
const char EMPLACE_CMD = 'm';
const char EMPLACE_HINT_CMD = 'M';
const char ERASE_CMD = 'e';
const char ITERATE_CMD = 'T';
const char CLEAR_CMD = 'c';
const char FIND_CMD = 'f';
const char LOAD_FACTOR_CMD = 'l';

const vector<char> COMMANDS{
  INSERT_CMD,
  EMPLACE_CMD,
  EMPLACE_HINT_CMD,
  ERASE_CMD,
  ITERATE_CMD,
  CLEAR_CMD,
  FIND_CMD,
  LOAD_FACTOR_CMD
};

/********** globals **********/

HashSet hs_g;

} // unnamed namespace

string
trim(const string& line)
{
  THROW_ON_FAIL((' ' == line[1]), runtime_error, "second character of line ["
		<< line << "] was not a space as expected");
  return line.substr(2);
}

#if 0
#define DO_COMMAND(name, action)		\
  void						\
  name(const string& line)			\
  {						\
    auto data = trim(line);			\
    int val = stoi(data);			\
    action;					\
    /* ignore extraneous input */		\
  }
#endif

#define DO_COMMAND(name, action)		\
  void						\
  name(const string& line)			\
  {						\
    auto data = trim(line);			\
    action;					\
    /* ignore extraneous input */		\
  }

DO_COMMAND(doInsert, (hs_g.insert(stoi(data))))

DO_COMMAND(doEmplace, (hs_g.emplace(stoi(data))))

DO_COMMAND(doEmplaceHint, (hs_g.emplace_hint(hs_g.cend(), stoi(data))))

DO_COMMAND(doErase, (hs_g.erase(stoi(data))))

void
doIterate()
{
  cout << "entries: [";
  copy(hs_g, ostream_innerator<int>(cout, ","));
  cout << "]" << endl;
}

void
doClear()
{
  hs_g.clear();
  cout << "cleared" << endl;
}

void
doFind(const string& line)
{
  auto key = trim(line);
  auto entry = hs_g.find(stoi(key));
  if (hs_g.end() == entry) {
    cout << "not found" << endl;
  }
  else {
    cout << "entry: [" << *entry << "]" << endl;
  }
}

void
doLoadFactor()
{
  cout << "load factor [" << hs_g.load_factor() << "]" << endl;
}

#undef DO_COMMAND

int
main(const int argc,
     const char *argv[])
{
  try {
    for (const string &line : StreamRange<istream_iterator<Line>>()) {
      switch (line[0]) {
      case HELP_CMD:
	cout << "commands: ";
	copy(COMMANDS, ostream_innerator<char>(cout, ","));
	cout << endl;
	break;

      case INSERT_CMD:
	doInsert(line);
	cout << "inserted" << endl;
	break;

      case EMPLACE_CMD:
	doEmplace(line);
	cout << "emplaced" << endl;
	break;

      case EMPLACE_HINT_CMD:
	doEmplaceHint(line);
	cout << "emplaced" << endl;
	break;

      case ERASE_CMD:
	doErase(line);
	cout << "erased" << endl;
	break;

      case ITERATE_CMD:
	doIterate();
	// ignore extraneous input
	break;

      case CLEAR_CMD:
	doClear();
	// ignore extraneous input
	break;

      case FIND_CMD:
	doFind(line);
	break;

      case LOAD_FACTOR_CMD:
	doLoadFactor();
	// ignore extraneous input
	break;

      default:
	cerr << "ERROR: unknown command '" << line[0] << "'" << endl;
      }
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
