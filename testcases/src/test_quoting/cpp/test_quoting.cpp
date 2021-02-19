// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2020 FAU -- Joachim Falk <joachim.falk@fau.de>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#include <CoSupport/String/quoting.hpp>
#include <CoSupport/String/DoubleQuotedString.hpp>
#include <CoSupport/String/SingleQuotedString.hpp>
#include <CoSupport/String/QuotedString.hpp>

#include <boost/random/mersenne_twister.hpp>

#include <cassert>
#include <sstream>
#include <ios>
#include <vector>

typedef CoSupport::String::QuotedString QS;

struct TestCases {
  TestCases(
      const char                          *qs
    , std::initializer_list<const char *>  results
    , std::initializer_list<const char *>  errors)
    : quoutedStrings(qs)
  {
    {
      size_t i = 0;
      quoutedStringResults = new const char *[results.size()+1];
      for (const char *result : results)
        quoutedStringResults[i++] = result;
      quoutedStringResults[i] = nullptr;
      assert(i == results.size());
    }
    {
      size_t i = 0;
      quoutedStringsWithErrors = new const char *[errors.size()+1];
      for (const char *error : errors)
        quoutedStringsWithErrors[i++] = error;
      quoutedStringsWithErrors[i] = nullptr;
      assert(i == errors.size());
    }
  }

  ~TestCases() {
    delete[] quoutedStringResults;
    delete[] quoutedStringsWithErrors;
  }

  const char  *quoutedStrings;
  const char **quoutedStringResults;
  const char **quoutedStringsWithErrors;
};

template <typename T>
bool testQuotingDequoting(TestCases const &tc) {
  bool error = false;

  T str;

  {
    std::stringstream in(tc.quoutedStrings, std::ios::in);

    unsigned int resultIndex = 0;

    while (in.good()) {
  //  std::cout << "peek: " << QS(1, in.peek()) << std::endl;
      in >> str;
      if (in.fail())
        break;
      if (in.eof())
        break;
      if (tc.quoutedStringResults[resultIndex] == nullptr) {
        std::cout << "INTERNAL ERROR: " << QS(str) << std::endl;
        error = true;
      } else if (str != tc.quoutedStringResults[resultIndex]) {
        std::cout << "BAD: " << QS(str) << " != " << QS(tc.quoutedStringResults[resultIndex]) << std::endl;
        error = true;
        ++resultIndex;
      } else {
        std::cout << "GOOD: " << QS(str) << std::endl;
        ++resultIndex;
      }
    }

    if (in.bad() || !in.eof()) {
      std::cout << "Read error while reading in strings" << std::endl;
      error = true;
    }
    while (tc.quoutedStringResults[resultIndex] != nullptr) {
      std::cout << "BAD: nullptr != " << QS(tc.quoutedStringResults[resultIndex]) << std::endl;
      error = true;
      ++resultIndex;
    }
  }
  // Expected errors
  {
    for (unsigned errorIndex = 0; tc.quoutedStringsWithErrors[errorIndex]; ++errorIndex) {
      std::stringstream in(tc.quoutedStringsWithErrors[errorIndex], std::ios::in);

      //  std::cout << "peek: " << QS(1, in.peek()) << std::endl;
      in >> str;
      if (in.fail()) {
        std::cout << "GOOD: " << QS(tc.quoutedStringsWithErrors[errorIndex]) << " failed as expected." << std::endl;
      } else {
        std::cout << "BAD: " << QS(tc.quoutedStringsWithErrors[errorIndex]) << " did not fail but resulted in " << QS(str) << "!" << std::endl;
        error = true;
      }
    }
  }
  // Round trip
  {
    std::stringstream        quoted;
    std::vector<std::string> expected;

    for (unsigned resultIndex = 0; tc.quoutedStringResults[resultIndex]; ++resultIndex) {
      quoted << " " << T(tc.quoutedStringResults[resultIndex]);
      if (!quoted.fail())
        expected.push_back(tc.quoutedStringResults[resultIndex]);
      else
        quoted.clear();
    }
    for (unsigned errorIndex = 0; tc.quoutedStringsWithErrors[errorIndex]; ++errorIndex) {
      quoted << " " << T(tc.quoutedStringsWithErrors[errorIndex]);
      if (!quoted.fail())
        expected.push_back(tc.quoutedStringsWithErrors[errorIndex]);
      else
        quoted.clear();
    }
    quoted << " " << T(tc.quoutedStrings);
    if (!quoted.fail())
      expected.push_back(tc.quoutedStrings);
    else
      quoted.clear();

    size_t randomStart = expected.size();

    // Random fuzzing
    boost::random::mt19937 random(4711);
    for (unsigned int randomIndex = 0; randomIndex < 10000; ++randomIndex) {
      unsigned int length = (random() & 0x3FF) + 1;
      std::string str(length, '\0');
      random.generate(str.begin(), str.end());
      quoted << " " << T(str);
      if (!quoted.fail())
        expected.push_back(str);
      else
        quoted.clear();
    }

    T str;

    quoted.seekg(0, std::ios::beg);

//  quoted.clear();
//  std::cout << quoted.good() << ", " << quoted.bad() << ", " << quoted.fail() << ", " << quoted.eof() << std::endl;
//  std::cout << quoted.str() << std::endl;

    size_t expectedIndex = 0;
    while (quoted.good()) {
//    std::cout << "peek: " << QS(1, in.peek()) << std::endl;
      quoted >> str;
//    std::cout << quoted.good() << ", " << quoted.bad() << ", " << quoted.fail() << ", " << quoted.eof() << std::endl;
      if (quoted.fail())
        break;
      if (quoted.eof())
        break;
      if (expectedIndex == expected.size()) {
        std::cout << "INTERNAL ERROR: " << QS(str) << std::endl;
        error = true;
      } else if (str != expected[expectedIndex]) {
        std::cout << "BAD: " << QS(str) << " != " << QS(expected[expectedIndex]) << std::endl;
        error = true;
      } else if (expectedIndex < randomStart) {
        std::cout << "GOOD: " << QS(str) << std::endl;
      }
      ++expectedIndex;
    }

    if (quoted.bad() || !quoted.eof()) {
      std::cout << "Read error while reading in strings" << std::endl;
      error = true;
    }
    while (expectedIndex < expected.size()) {
      std::cout << "BAD: nullptr != " << QS(expected[expectedIndex]) << std::endl;
      error = true;
      ++expectedIndex;
    }
  }
  return error;
}

int main(int argc, char *argv[]) {
  bool error = false;


  std::cout << "Testing CoSupport::String::QuotedString" << std::endl;
  error |= testQuotingDequoting<CoSupport::String::QuotedString>(TestCases(
      "A B\tC\fD\vE\rF\nG "
      " foo  "
      "' bar ' \t"
      "\" batz \" \t"
      "foo\\\nbar\t"
      "foo\\ bar\\_\n"
      "hix'hax'\f"
      "hix\"hax\"\v"
      "'hix'hax\r"
      "'hix''hax' "
      "'hix'\"hax\"\n"
      "\"hix\"hax "
      "\"hix\"'hax' "
      "\"hix\"\"hax\" "
      "\"\\$\\`\\!$`!\\x24\\x60\\x21\" "
      "\"\\\"\\a\\b\\e\\f\\n\\r\\t\\v\x1\x01\xF\xFF\1\01\001\3\37\377\\\"\" "
      "'\\t\"\\\n\"' "
      "'\1\2\3\4\5\6\7' "
      "\"\\1\\2\\3\\4\\5\\6\\7\" "
      "\10'\11\12\13\14\15'\16\17\18\19\1A\1B\1C\1D\1E\1F\1a\1b\1c\1d\1e\1f "
      "\"\\10\\11\\12\\13\\14\\15\\16\\17\\18\\19\\1A\\1B\\1C\\1D\\1E\\1F\\1a\\1b\\1c\\1d\\1e\\1f\" "
      "\"\\270\\271\\272\\273\\274\\275\\276\\277\\278\\279\\27A\\27B\\27C\\27D\\27E\\27F\\27a\\27b\\27c\\27d\\27e\\27f\" "
    , {
        "A", "B", "C", "D", "E", "F", "G"
      , "foo"
      , " bar "
      , " batz "
      , "foobar"
      , "foo bar_"
      , "hixhax"
      , "hixhax"
      , "hixhax"
      , "hixhax"
      , "hixhax"
      , "hixhax"
      , "hixhax"
      , "hixhax"
      , "$`!$`!$`!"
      , "\"\a\b\x1B\f\n\r\t\v\x01\x01\x0F\xFF\001\001\001\003\037\377\""
      , "\\t\"\\\n\""
      , "\1\2\3\4\5\6\7"
      , "\1\2\3\4\5\6\7"
      , "\10\11\12\13\14\15\16\17\18\19\1A\1B\1C\1D\1E\1F\1a\1b\1c\1d\1e\1f"
      , "\10\11\12\13\14\15\16\17\18\19\1A\1B\1C\1D\1E\1F\1a\1b\1c\1d\1e\1f"
      , "\270\271\272\273\274\275\276\277\278\279\27A\27B\27C\27D\27E\27F\27a\27b\27c\27d\27e\27f"
      }
    , {
                   "\"\\A\""
      ,            "\"\\B\""
      , "\"\\c\"", "\"\\C\""
      , "\"\\d\"", "\"\\D\""
      ,            "\"\\E\""
      ,            "\"\\F\""
      , "\"\\g\"", "\"\\G\""
      , "\"\\h\"", "\"\\H\""
      , "\"\\i\"", "\"\\I\""
      , "\"\\j\"", "\"\\J\""
      , "\"\\k\"", "\"\\K\""
      , "\"\\l\"", "\"\\L\""
      , "\"\\m\"", "\"\\M\""
      ,            "\"\\N\""
      , "\"\\o\"", "\"\\O\""
      , "\"\\p\"", "\"\\P\""
      , "\"\\q\"", "\"\\Q\""
      ,            "\"\\R\""
      , "\"\\s\"", "\"\\S\""
      ,            "\"\\T\""
      , "\"\\u\"", "\"\\U\""
      ,            "\"\\V\""
      , "\"\\w\"", "\"\\W\""
      , "\"\\x\"", "\"\\X\""
      , "\"\\y\"", "\"\\Y\""
      , "\"\\z\"", "\"\\Z\""
      , "\"\\8\""
      , "\"\\9\""
      , "\"\\400\""
      , "\"\\777\""
      , "\"XXX", "'XXX"
      }));

  std::cout << "Testing CoSupport::String::SingleQuotedString" << std::endl;
  error |= testQuotingDequoting<CoSupport::String::SingleQuotedString>(TestCases(
      "\n'\\a''\\b''\\c''\\d''\\e''\\f''\\g''\\h''\\i''\\j''\\k''\\l''\\m'"
      "\r'\\n''\\o''\\p''\\q''\\r''\\s''\\t''\\u''\\v''\\w''\\x''\\y''\\z'"
      "\t'\\A''\\B''\\C''\\D''\\E''\\F''\\G''\\H''\\I''\\J''\\K''\\L''\\M'"
      "\v'\\N''\\O''\\P''\\Q''\\R''\\S''\\T''\\U''\\V''\\W''\\X''\\Y''\\Z'"
      "\f'\\0'\n'\\1'\r '\\2' '\\3''\\4' '\\5'\n '\\6'\r '\\7' '\\8' '\\9'"
      "' bar '\n"
      "'A\\B'\r"
      "'X\\\nY'\v"
      "'U\\''\\''V'\f"
      "'$`!$`!$`!'"
      "'\1\2\3\4\5\6\7'\t"
      "'\\1\\2\\3\\4\\5\\6\\7\\8\\9\\a\\b\\c\\d\\e\\f\\A\\B\\C\\D\\E\\F'\t"
      "'\10\11\12\13\14\15\16\17\18\19\1A\1B\1C\1D\1E\1F\1a\1b\1c\1d\1e\1f' "
      "'\\10\\11\\12\\13\\14\\15\\16\\17\\18\\19\\1A\\1B\\1C\\1D\\1E\\1F\\1a\\1b\\1c\\1d\\1e\\1f' "
      "'\270\271\272\273\274\275\276\277\278\279\27A\27B\27C\27D\27E\27F\27a\27b\27c\27d\27e\27f'"
      "'\\270\\271\\272\\273\\274\\275\\276\\277\\278\\279\\27A\\27B\\27C\\27D\\27E\\27F\\27a\\27b\\27c\\27d\\27e\\27f'"
      "'\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf\xA\xB\xC\xD\xE\xF'"
      "'\\x1\\x2\\x3\\x4\\x5\\x6\\x7\\x8\\x9\\xa\\xb\\xc\\xd\\xe\\xf\\xA\\xB\\xC\\xD\\xE\\xF'"
      "'\\400'"
    , {
        "\\a", "\\b", "\\c", "\\d", "\\e", "\\f", "\\g", "\\h", "\\i", "\\j", "\\k", "\\l", "\\m"
      , "\\n", "\\o", "\\p", "\\q", "\\r", "\\s", "\\t", "\\u", "\\v", "\\w", "\\x", "\\y", "\\z"
      , "\\A", "\\B", "\\C", "\\D", "\\E", "\\F", "\\G", "\\H", "\\I", "\\J", "\\K", "\\L", "\\M"
      , "\\N", "\\O", "\\P", "\\Q", "\\R", "\\S", "\\T", "\\U", "\\V", "\\W", "\\X", "\\Y", "\\Z"
      , "\\0", "\\1", "\\2", "\\3", "\\4", "\\5", "\\6", "\\7", "\\8", "\\9"
      , " bar "
      , "A\\B"
      , "X\\\nY"
      , "U\\", "\\", "V"
      , "$`!$`!$`!"
      , "\1\2\3\4\5\6\7"
      , "\\1\\2\\3\\4\\5\\6\\7\\8\\9\\a\\b\\c\\d\\e\\f\\A\\B\\C\\D\\E\\F"
      , "\10\11\12\13\14\15\16\17\18\19\1A\1B\1C\1D\1E\1F\1a\1b\1c\1d\1e\1f"
      , "\\10\\11\\12\\13\\14\\15\\16\\17\\18\\19\\1A\\1B\\1C\\1D\\1E\\1F\\1a\\1b\\1c\\1d\\1e\\1f"
      , "\270\271\272\273\274\275\276\277\278\279\27A\27B\27C\27D\27E\27F\27a\27b\27c\27d\27e\27f"
      , "\\270\\271\\272\\273\\274\\275\\276\\277\\278\\279\\27A\\27B\\27C\\27D\\27E\\27F\\27a\\27b\\27c\\27d\\27e\\27f"
      , "\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf\xA\xB\xC\xD\xE\xF"
      , "\\x1\\x2\\x3\\x4\\x5\\x6\\x7\\x8\\x9\\xa\\xb\\xc\\xd\\xe\\xf\\xA\\xB\\xC\\xD\\xE\\xF"
      , "\\400"
      }
    , {
        "X'A'", "\"'XX'\"", "'XXX"
      }));

  std::cout << "Testing CoSupport::String::DoubleQuotedString" << std::endl;
  error |= testQuotingDequoting<CoSupport::String::DoubleQuotedString>(TestCases(
      "\n\"\\a\"\"\\b\"\"\\e\"\"\\f\"\"\\n\"\"\\r\"\"\\t\"\"\\v\""
      "\f\"\\1\"\n\"\\2\"\r \"\\3\"\r\"\\4\"\"\\5\"\v\"\\6\"\n \"\\7\"\r\n"
      "\" batz \" \t"
      "\"foo\"\"bar\"\t"
      "\"foo\\\"\\\"bar\""
      "\"${FOO}`cat /dev/zero`</dev/zero >/dev/null\""
      "\"\\\"\\a\\b\\e\\f\\n\\r\\t\\v\x1\x01\xF\xFF\1\01\001\3\37\377\\\"\""
      "\"\1\2\3\4\5\6\7\" "
      "\"\\1\\2\\3\\4\\5\\6\\7\" "
      "\"\x1\x2\x3\x4\x5\x6\x7\x8\x9\xA\xB\xC\xD\xE\xF\xa\xb\xc\xd\xe\xf\" "
      "\"\\x1\\x2\\x3\\x4\\x5\\x6\\x7\\x8\\x9\\xA\\xB\\xC\\xD\\xE\\xF\\xa\\xb\\xc\\xd\\xe\\xf\" "
      "\"\10\11\12\13\14\15\16\17\18\19\1A\1B\1C\1D\1E\1F\1a\1b\1c\1d\1e\1f\""
      "\"\\10\\11\\12\\13\\14\\15\\16\\17\\18\\19\\1A\\1B\\1C\\1D\\1E\\1F\\1a\\1b\\1c\\1d\\1e\\1f\" "
      "\"\270\271\272\273\274\275\276\277\278\279\27A\27B\27C\27D\27E\27F\27a\27b\27c\27d\27e\27f\" "
      "\"\\270\\271\\272\\273\\274\\275\\276\\277\\278\\279\\27A\\27B\\27C\\27D\\27E\\27F\\27a\\27b\\27c\\27d\\27e\\27f\" "
      "\"\370\371\372\373\374\375\376\377\378\379\37A\37B\37C\37D\37E\37F\37a\37b\37c\37d\37e\37f\""
      "\"\\370\\371\\372\\373\\374\\375\\376\\377\\378\\379\\37A\\37B\\37C\\37D\\37E\\37F\\37a\\37b\\37c\\37d\\37e\\37f\""
      "\"\3770\3771\3772\3773\3774\3775\3776\3777\3778\3779\377A\377B\377C\377D\377E\377F\377a\377b\377c\377d\377e\377f\""
      "\"\\3770\\3771\\3772\\3773\\3774\\3775\\3776\\3777\\3778\\3779\\377A\\377B\\377C\\377D\\377E\\377F\\377a\\377b\\377c\\377d\\377e\\377f\""
      "\"\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF\x10\x30\x10\x31\x10\x32\x10\x33\x10\x34\x10\x35\x10\x36\x10\x37\x10\x38\x10\x39\x10\x41\x10\x46\x10G\x10\x61\x10\x66\x10g\""
      "\"\\xF8\\xF9\\xFA\\xFB\\xFC\\xFD\\xFE\\xFF\\x100\\x101\\x102\\x103\\x104\\x105\\x106\\x107\\x108\\x109\\x10A\\x10F\\x10G\\x10a\\x10f\\x10g\""
    , {
        "\a", "\b", "\x1B", "\f", "\n", "\r", "\t", "\v"
      , "\1", "\2", "\3", "\4", "\5", "\6", "\7"
      , " batz "
      , "foo", "bar"
      , "foo\"\"bar"
      , "${FOO}`cat /dev/zero`</dev/zero >/dev/null"
      , "\"\a\b\x1B\f\n\r\t\v\x01\x01\x0F\xFF\001\001\001\003\037\377\""
      , "\1\2\3\4\5\6\7"
      , "\1\2\3\4\5\6\7"
      , "\x1\x2\x3\x4\x5\x6\x7\x8\x9\xA\xB\xC\xD\xE\xF\xa\xb\xc\xd\xe\xf"
      , "\x1\x2\x3\x4\x5\x6\x7\x8\x9\xA\xB\xC\xD\xE\xF\xa\xb\xc\xd\xe\xf"
      , "\10\11\12\13\14\15\16\17\18\19\1A\1B\1C\1D\1E\1F\1a\1b\1c\1d\1e\1f"
      , "\10\11\12\13\14\15\16\17\18\19\1A\1B\1C\1D\1E\1F\1a\1b\1c\1d\1e\1f"
      , "\270\271\272\273\274\275\276\277\278\279\27A\27B\27C\27D\27E\27F\27a\27b\27c\27d\27e\27f"
      , "\270\271\272\273\274\275\276\277\278\279\27A\27B\27C\27D\27E\27F\27a\27b\27c\27d\27e\27f"
      , "\370\371\372\373\374\375\376\377\378\379\37A\37B\37C\37D\37E\37F\37a\37b\37c\37d\37e\37f"
      , "\370\371\372\373\374\375\376\377\378\379\37A\37B\37C\37D\37E\37F\37a\37b\37c\37d\37e\37f"
      , "\3770\3771\3772\3773\3774\3775\3776\3777\3778\3779\377A\377B\377C\377D\377E\377F\377a\377b\377c\377d\377e\377f"
      , "\3770\3771\3772\3773\3774\3775\3776\3777\3778\3779\377A\377B\377C\377D\377E\377F\377a\377b\377c\377d\377e\377f"
      , "\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF\x10\x30\x10\x31\x10\x32\x10\x33\x10\x34\x10\x35\x10\x36\x10\x37\x10\x38\x10\x39\x10\x41\x10\x46\x10G\x10\x61\x10\x66\x10g"
      , "\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF\x10\x30\x10\x31\x10\x32\x10\x33\x10\x34\x10\x35\x10\x36\x10\x37\x10\x38\x10\x39\x10\x41\x10\x46\x10G\x10\x61\x10\x66\x10g"
      }
    , {
                   "\"\\A\""
      ,            "\"\\B\""
      , "\"\\c\"", "\"\\C\""
      , "\"\\d\"", "\"\\D\""
      ,            "\"\\E\""
      ,            "\"\\F\""
      , "\"\\g\"", "\"\\G\""
      , "\"\\h\"", "\"\\H\""
      , "\"\\i\"", "\"\\I\""
      , "\"\\j\"", "\"\\J\""
      , "\"\\k\"", "\"\\K\""
      , "\"\\l\"", "\"\\L\""
      , "\"\\m\"", "\"\\M\""
      ,            "\"\\N\""
      , "\"\\o\"", "\"\\O\""
      , "\"\\p\"", "\"\\P\""
      , "\"\\q\"", "\"\\Q\""
      ,            "\"\\R\""
      , "\"\\s\"", "\"\\S\""
      ,            "\"\\T\""
      , "\"\\u\"", "\"\\U\""
      ,            "\"\\V\""
      , "\"\\w\"", "\"\\W\""
      , "\"\\x\"", "\"\\X\""
      , "\"\\y\"", "\"\\Y\""
      , "\"\\z\"", "\"\\Z\""
      , "\"\\8\""
      , "\"\\9\""
      , "\"\\400\""
      , "\"\\777\""
      , "\"XXX"
      }));

  return error ? -1 : 0;
}
