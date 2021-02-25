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

//#include <memory.h>

#include <cassert>
#include <cstring>
#include <sstream>
#include <ios>
#include <vector>

typedef CoSupport::String::QuotedString QS;

using CoSupport::String::Environment;
using CoSupport::String::DoubleQuotedString;
using CoSupport::String::SingleQuotedString;
using CoSupport::String::QuotedString;
using CoSupport::String::DequotingStatus;
using CoSupport::String::DequotingError;
using CoSupport::String::QuoteMode;

using CoSupport::String::dequote;

struct TestCases {
  TestCases(
      const char                          *qs
    , std::initializer_list<const char *>  more
    , std::initializer_list<const char *>  results
    , std::initializer_list<const char *>  errors)
    : quotedStrings(qs)
  {
    {
      size_t i = 0;
      quotedStringMore = new const char *[more.size()+1];
      for (const char *str : more)
        quotedStringMore[i++] = str;
      quotedStringMore[i] = nullptr;
      assert(i == more.size());
    }
    {
      size_t i = 0;
      quotedStringResults = new const char *[results.size()+1];
      for (const char *result : results)
        quotedStringResults[i++] = result;
      quotedStringResults[i] = nullptr;
      assert(i == results.size());
    }
    {
      size_t i = 0;
      quotedStringsWithErrors = new const char *[errors.size()+1];
      for (const char *error : errors)
        quotedStringsWithErrors[i++] = error;
      quotedStringsWithErrors[i] = nullptr;
      assert(i == errors.size());
    }
  }

  ~TestCases() {
    delete[] quotedStringMore;
    delete[] quotedStringResults;
    delete[] quotedStringsWithErrors;
  }

  const char  *quotedStrings;
  const char **quotedStringMore;
  const char **quotedStringResults;
  const char **quotedStringsWithErrors;
};

TestCases quotedStringTests(
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
      "TEST"
    }
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
    , "TEST"
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
    });

TestCases singleQuotedStringTests(
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
  , {}
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
    });

TestCases doubleQuotedStringTests(
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
  , {}
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
    });

TestCases singleNoQuotesStringTests(
    ""
  , {
      "TE'ST"
    }
  , {
      "TE'ST"
    }
  , {
    });

TestCases doubleNoQuotesStringTests(
    ""
  , {
      "\\n\"\\r"
    }
  , {
      "\n\"\r"
    }
  , {
    });

TestCases singleQuotedStringFailTests(
    ""
  , {
    }
  , {
    }
  , {
      "'FAIL'"
    });

TestCases singleNoQuotesStringFailTests(
    ""
  , {
    }
  , {
    }
  , {
      "FAIL"
    });

TestCases doubleQuotedStringFailTests(
    ""
  , {
    }
  , {
    }
  , {
      "\"FAIL\""
    });

TestCases doubleNoQuotesStringFailTests(
    ""
  , {
    }
  , {
    }
  , {
      "FAIL"
    });

const char *var1 = "FOO=BAR";
const char *var2 = "BAR=FOO";
const char *var3 = "0=FLUMMY";
const char *var4 = "9=FLAMMY";
const char *var5 = "10=FOOBAR";
const char *var6 = "0FOO=BATZ";
const char *var7 = "_=UNDERSCORE";
const char *envArray[] = { var1, var2, var3, var4, var5, var6, var7, nullptr };

// Delimiters are ":;_" variables are envArray.
TestCases quotedStringDelimVarTests(
    "A\tB:B 'C;C' D;D E;E \"F:F\" G\\:G H_"
    "\\$FOO$FOO\\${FOO}${FOO}$0FOO${0FOO}$9FOO${9FOO}$10${10};"
    "\"\\$FOO$FOO\\${FOO}${FOO}$0FOO${0FOO}$9FOO${9FOO}$10${10}\";"
    "'\\$FOO$FOO\\${FOO}${FOO}$0FOO${0FOO}$9FOO${9FOO}$10${10}';"
    "'$';'$_';'${';'${_';'${}';\"$_\";\"${_}\";"
  , {
    }
  , {
      "A\tB"
    , "B C;C D"
    , "D E"
    , "E F:F G:G H"
    , "$FOO""BAR""${FOO}""BAR""FLUMMY""FOO""BATZ""FLAMMY""FOO""0""FOOBAR"
    , "$FOO""BAR""${FOO}""BAR""FLUMMY""FOO""BATZ""FLAMMY""FOO""0""FOOBAR"
    , "\\$FOO$FOO\\${FOO}${FOO}$0FOO${0FOO}$9FOO${9FOO}$10${10}"
    , "$", "$_", "${", "${_", "${}", "UNDERSCORE", "UNDERSCORE"
    }
  , {
      "$"
    , "$_"
    , "${"
    , "${_"
    , "${"
    , "${_"
    , "${}"
    , "\"$\""
    , "\"${\""
    , "\"${FOO\""
    , "\"${FOO\"}\""
    , "\"${}\""
    , "\"${BAR'}\""
    , "\"${BAR\\\"}\""
    });

TestCases doubleQuotedStringVarTests(
    "\"\\$FOO$FOO\\${FOO}${FOO}$0FOO${0FOO}$9FOO${9FOO}$10${10}\""
  , {
    }
  , {
      "$FOO""BAR""${FOO}""BAR""FLUMMY""FOO""BATZ""FLAMMY""FOO""0""FOOBAR"
    }
  , {
      "\"$\""
    , "\"${\""
    , "\"${FOO\""
    , "\"${FOO\"}\""
    , "\"${}\""
    });

TestCases singleQuotedStringVarTests(
    "'${FOO}''${_}'"
  , {
    }
  , {
      "BAR", "UNDERSCORE"
    }
  , {
      "'${BAR'}'"
    , "'${FOO\"}'"
    });

TestCases singleNoQuotesStringVarTests(
    ""
  , {
      "${FOO}", "${_}"
    }
  , {
      "BAR", "UNDERSCORE"
    }
  , {
      "$"
    , "${"
    , "${}"
    , "${FOO"
    , "${FOO\"}"
    , "${BAR'}"
    });

TestCases doubleNoQuotesStringVarTests(
    ""
  , {
      "${FOO}", "${_}"
    }
  , {
      "BAR", "UNDERSCORE"
    }
  , {
      "$"
    , "${"
    , "${}"
    , "${FOO"
    , "${FOO\"}"
    , "${BAR'}"
    });

template <typename T>
bool testQuotingDequoting(TestCases const &tc) {
  bool errorFlag = false;

  T str;

  {
    std::stringstream in(tc.quotedStrings, std::ios::in);

    unsigned int resultIndex = 0;

    while (in.good()) {
  //  std::cout << "peek: " << QS(1, in.peek()) << std::endl;
      in >> str;
      if (in.fail())
        break;
      if (in.eof())
        break;
      if (tc.quotedStringResults[resultIndex] == nullptr) {
        std::cout << "INTERNAL ERROR: " << QS(str) << std::endl;
        errorFlag = true;
      } else if (str != tc.quotedStringResults[resultIndex]) {
        std::cout << "BAD: " << QS(str) << " != " << QS(tc.quotedStringResults[resultIndex]) << std::endl;
        errorFlag = true;
        ++resultIndex;
      } else {
        std::cout << "GOOD: " << QS(str) << std::endl;
        ++resultIndex;
      }
    }
    if (in.bad() || !in.eof()) {
      std::cout << "Read error while reading in strings" << std::endl;
      errorFlag = true;
    }
    for (const char **inStrs = tc.quotedStringMore; *inStrs != nullptr; ++inStrs) {
      in.str(*inStrs);
      in.seekg(0, std::ios::beg);
      in.clear();
      in >> str;
      if (!in.good() || in.peek() != EOF) {
        std::cout << "Read error while reading in string " << QS(*inStrs) << std::endl;
        errorFlag = true;
      }
      if (tc.quotedStringResults[resultIndex] == nullptr) {
        std::cout << "INTERNAL ERROR: " << QS(str) << std::endl;
        errorFlag = true;
      } else if (str != tc.quotedStringResults[resultIndex]) {
        std::cout << "BAD: " << QS(str) << " != " << QS(tc.quotedStringResults[resultIndex]) << std::endl;
        errorFlag = true;
        ++resultIndex;
      } else {
        std::cout << "GOOD: " << QS(str) << std::endl;
        ++resultIndex;
      }
    }

    while (tc.quotedStringResults[resultIndex] != nullptr) {
      std::cout << "BAD: nullptr != " << QS(tc.quotedStringResults[resultIndex]) << std::endl;
      errorFlag = true;
      ++resultIndex;
    }
  }
  // Expected errors
  {
    for (unsigned errorIndex = 0; tc.quotedStringsWithErrors[errorIndex]; ++errorIndex) {
      std::stringstream in(tc.quotedStringsWithErrors[errorIndex], std::ios::in);

      //  std::cout << "peek: " << QS(1, in.peek()) << std::endl;
      in >> str;
      if (in.fail()) {
        std::cout << "GOOD: " << QS(tc.quotedStringsWithErrors[errorIndex]) << " failed as expected." << std::endl;
      } else {
        std::cout << "BAD: " << QS(tc.quotedStringsWithErrors[errorIndex]) << " did not fail but resulted in " << QS(str) << "!" << std::endl;
        errorFlag = true;
      }
    }
  }
  // Round trip
  {
    std::stringstream        quoted;
    std::vector<std::string> expected;

    for (unsigned resultIndex = 0; tc.quotedStringResults[resultIndex]; ++resultIndex) {
      quoted << " " << T(tc.quotedStringResults[resultIndex]);
      if (!quoted.fail())
        expected.push_back(tc.quotedStringResults[resultIndex]);
      else
        quoted.clear();
    }
    for (unsigned errorIndex = 0; tc.quotedStringsWithErrors[errorIndex]; ++errorIndex) {
      quoted << " " << T(tc.quotedStringsWithErrors[errorIndex]);
      if (!quoted.fail())
        expected.push_back(tc.quotedStringsWithErrors[errorIndex]);
      else
        quoted.clear();
    }
    quoted << " " << T(tc.quotedStrings);
    if (!quoted.fail())
      expected.push_back(tc.quotedStrings);
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
        errorFlag = true;
      } else if (str != expected[expectedIndex]) {
        std::cout << "BAD: " << QS(str) << " != " << QS(expected[expectedIndex]) << std::endl;
        errorFlag = true;
      } else if (expectedIndex < randomStart) {
        std::cout << "GOOD: " << QS(str) << std::endl;
      }
      ++expectedIndex;
    }

    if (quoted.bad() || !quoted.eof()) {
      std::cout << "Read error while reading in strings" << std::endl;
      errorFlag = true;
    }
    while (expectedIndex < expected.size()) {
      std::cout << "BAD: nullptr != " << QS(expected[expectedIndex]) << std::endl;
      errorFlag = true;
      ++expectedIndex;
    }
  }
  return errorFlag;
}

bool testQuotingDequotingHelper(
    TestCases const &tc
  , QuoteMode qm
  , const char *delim
  , const char **env
  , unsigned int &resultIndex
  , const char *start, const char *end
  , const char       *&in
  , std::stringstream &inStream)
{
  bool errorFlag = false;
  try {
    ptrdiff_t pos = in - start;

    std::string str = dequote(in, end, qm, delim, env);

    if (tc.quotedStringResults[resultIndex] == nullptr) {
      std::cout << "INTERNAL ERROR: " << QS(str) << std::endl;
      return true;
    } else if (str != tc.quotedStringResults[resultIndex]) {
      std::cout << "BAD: " << QS(str) << " != " << QS(tc.quotedStringResults[resultIndex]) << std::endl;
      errorFlag = true;
    } else {
      std::cout << "GOOD: " << QS(str) << std::endl;
    }

    const char                        *inPtr;
    std::string                        other;
    DequotingStatus status;

    inPtr = start + pos;
    status = dequote(other, inPtr, end, qm, delim, env);
    if (status != DequotingStatus::OK ||
        inPtr != in || other != str) {
      std::cout << "INCONSISTENT1: " << status << ", " << (inPtr != in) << " || " << QS(other) << " != " << QS(str) << std::endl;
      errorFlag = true;
    }
    inPtr = start + pos;
    other = dequote(inPtr, qm, delim, env);
    if (inPtr != in || other != str) {
      std::cout << "INCONSISTENT2: " << (inPtr != in) << " || " << QS(other) << " != " << QS(str) << std::endl;
      errorFlag = true;
    }
    inPtr = start + pos;
    status = dequote(other, inPtr, qm, delim, env);
    if (status != DequotingStatus::OK ||
        inPtr != in || other != str) {
      std::cout << "INCONSISTENT3: " << status << ", " << (inPtr != in) << " || " << QS(other) << " != " << QS(str) << std::endl;
      errorFlag = true;
    }
    inStream.seekg(pos, std::ios::beg);
    other = dequote(inStream, qm, delim, env);
    if (!inStream.good() || inStream.tellg() != (in-start) || other != str) {
      std::cout << "INCONSISTENT4: " << inStream.tellg() << " != " << (in-start) << " || " << QS(other) << " != " << QS(str) << std::endl;
      errorFlag = true;
    }
    inStream.seekg(pos, std::ios::beg);
    status = dequote(other, inStream, qm, delim, env);
    if (status != DequotingStatus::OK ||
        !inStream.good() || inStream.tellg() != (in-start) || other != str) {
      std::cout << "INCONSISTENT5: " << inStream.tellg() << " != " << (in-start) << " || " << QS(other) << " != " << QS(str) << std::endl;
      errorFlag = true;
    }
    std::string duplicate(start+pos, in);
    const char *dupIn  = duplicate.c_str();
    const char *dupEnd = dupIn + duplicate.length();
    other = dequote(qm, dupIn, dupEnd, env);
    if (other != str) {
      std::cout << "INCONSISTENT6: " << QS(other) << " != " << QS(str) << std::endl;
      errorFlag = true;
    }
    status = dequote(other, qm, dupIn, dupEnd, env);
    if (status != DequotingStatus::OK ||
        other != str) {
      std::cout << "INCONSISTENT7: " << QS(other) << " != " << QS(str) << std::endl;
      errorFlag = true;
    }
    other = dequote(qm, dupIn, env);
    if (other != str) {
      std::cout << "INCONSISTENT8: " << QS(other) << " != " << QS(str) << std::endl;
      errorFlag = true;
    }
    status = dequote(other, qm, dupIn, env);
    if (status != DequotingStatus::OK ||
        other != str) {
      std::cout << "INCONSISTENT9: " << QS(other) << " != " << QS(str) << std::endl;
      errorFlag = true;
    }
    duplicate += "G A R B A G E";
    dupIn  = duplicate.c_str();
    dupEnd = dupIn + duplicate.length();
    try {
      other.clear();
      other = dequote(qm, dupIn, dupEnd, env);
      status =  DequotingStatus::OK;
    } catch (DequotingError const &e) {
      status = e.getError();
    }
    if (status != DequotingStatus::TRAILING_GARBAGE &&
        (status != DequotingStatus::OK || other != str + "G A R B A G E")) {
      std::cout << "INCONSISTENT10: " << QS(other) << " != " << QS(str+"G A R B A G E") << std::endl;
      errorFlag = true;
    }
    other.clear();
    status = dequote(other, qm, dupIn, dupEnd, env);
    if (status != DequotingStatus::TRAILING_GARBAGE &&
        (status != DequotingStatus::OK || other != str + "G A R B A G E")) {
      std::cout << "INCONSISTENT11: " << QS(other) << " != " << QS(str+"G A R B A G E") << std::endl;
      errorFlag = true;
    }
    try {
      other.clear();
      other = dequote(qm, dupIn, env);
      status =  DequotingStatus::OK;
    } catch (DequotingError const &e) {
      status = e.getError();
    }
    if (status != DequotingStatus::TRAILING_GARBAGE &&
        (status != DequotingStatus::OK || other != str + "G A R B A G E")) {
      std::cout << "INCONSISTENT12: " << QS(other) << " != " << QS(str+"G A R B A G E") << std::endl;
      errorFlag = true;
    }
    other.clear();
    status = dequote(other, qm, dupIn, env);
    if (status != DequotingStatus::TRAILING_GARBAGE &&
        (status != DequotingStatus::OK || other != str + "G A R B A G E")) {
      std::cout << "INCONSISTENT13: " << QS(other) << " != " << QS(str+"G A R B A G E") << std::endl;
      errorFlag = true;
    }
    ++resultIndex;
  } catch (DequotingError const &e) {
    std::cout << "INTERNAL ERROR: " << e.what() << std::endl;
  }
  return errorFlag;
}

bool testQuotingDequoting(
    TestCases const &tc
  , QuoteMode qm
  , const char *delim = nullptr
  , const char **env = nullptr)
{
  bool errorFlag = false;

  {
    unsigned int resultIndex = 0;

    {
      const char *start = tc.quotedStrings;
      const char *end   = start + strlen(start);

      if (!delim)
        while (start != end && isspace(*start))
          ++start;

      std::stringstream inStream(start, std::ios::in);

      for (const char *in = start; in != end;) {
        if (in == end)
          break;
        errorFlag |= testQuotingDequotingHelper(tc, qm, delim, env, resultIndex, start, end, in, inStream);
        if (!delim)
          while (in != end && isspace(*in))
            ++in;
        else
          if (in != end && strchr(delim, *in))
            ++in;
      }
    }
    for (const char **inStrs = tc.quotedStringMore; *inStrs != nullptr; ++inStrs) {
      const char *start = *inStrs;
      const char *end   = start + strlen(start);
      std::stringstream inStream(start, std::ios::in);

      const char *in = start;
      errorFlag |= testQuotingDequotingHelper(tc, qm, delim, env, resultIndex, start, end, in, inStream);
      if (in != end) {
        std::cout << "Read error while reading in string " << QS(*inStrs) << std::endl;
        errorFlag = true;
      }
    }
    while (tc.quotedStringResults[resultIndex] != nullptr) {
      std::cout << "BAD: nullptr != " << QS(tc.quotedStringResults[resultIndex]) << std::endl;
      errorFlag = true;
      ++resultIndex;
    }
  }
  // Expected errors
  {
    unsigned errorIndex = 0;

    for (const char *in = tc.quotedStringsWithErrors[errorIndex];
         in != nullptr;
         in = tc.quotedStringsWithErrors[++errorIndex])
    {
      const char *start = in;
      const char *end   = in + strlen(in);

      std::stringstream inStream(in, std::ios::in);

      std::string str;
      DequotingStatus error = dequote(str, in, end, qm, delim, env);

      if (error != DequotingStatus::OK) {
        std::cout << "GOOD: " << QS(tc.quotedStringsWithErrors[errorIndex]) << " failed as expected with " << error << "." << std::endl;
      } else {
        std::cout << "BAD: " << QS(tc.quotedStringsWithErrors[errorIndex]) << " did not fail but resulted in " << QS(str) << "!" << std::endl;
        errorFlag = true;
      }

      const char                        *inPtr;
      std::string                        other;
      DequotingStatus status;

      inPtr = start;
      try {
        other.clear();
        other = dequote(inPtr, end, qm, delim, env);
        status =  DequotingStatus::OK;
      } catch (DequotingError const &e) {
        status = e.getError();
      }
      if (status != error || inPtr != in) {
        std::cout << "ERROR_INCONSISTENT1: " << status << " != " << error << " || " << (inPtr != in) << " || " << QS(other) << " != " << QS(str) << std::endl;
        errorFlag = true;
      }
      inPtr = start;
      status = dequote(other, inPtr, qm, delim, env);
      if (status != error ||
          inPtr != in || other != str) {
        std::cout << "ERROR_INCONSISTENT2: " << status << " != " << error << " || " << (inPtr != in) << " || " << QS(other) << " != " << QS(str) << std::endl;
        errorFlag = true;
      }
      inPtr = start;
      try {
        other.clear();
        other = dequote(inPtr, qm, delim, env);
        status =  DequotingStatus::OK;
      } catch (DequotingError const &e) {
        status = e.getError();
      }
      if (status != error || inPtr != in) {
        std::cout << "ERROR_INCONSISTENT3: " << status << " != " << error << " || " << (inPtr != in) << " || " << QS(other) << " != " << QS(str) << std::endl;
        errorFlag = true;
      }
      inStream.seekg(0, std::ios::beg);
      inStream.clear();
      status = dequote(other, inStream, qm, delim, env);
      if (inStream.good() || status != error ||
          (inStream.clear(), inStream.tellg()) != (in-start) || other != str) {
        std::cout << "ERROR_INCONSISTENT4: " << status << " != " << error << " || " << inStream.tellg() << " != " << (in-start) << " || " << QS(other) << " != " << QS(str) << std::endl;
        errorFlag = true;
      }
      inStream.seekg(0, std::ios::beg);
      inStream.clear();
      try {
        other.clear();
        other = dequote(inStream, qm, delim, env);
        status =  DequotingStatus::OK;
      } catch (DequotingError const &e) {
        status = e.getError();
      }
      if (inStream.good() || status != error ||
          (inStream.clear(), inStream.tellg()) != (in-start)) {
        std::cout << "ERROR_INCONSISTENT5: " << status << " != " << error << " || " << inStream.tellg() << " != " << (in-start) << " || " << QS(other) << " != " << QS(str) << std::endl;
        errorFlag = true;
      }
      if (error == DequotingStatus::NO_DELIMITERS_ALLOWED_FOR_QM)
        continue;

      std::string duplicate(start, in);
      const char *dupStart = duplicate.c_str();
      const char *dupEnd   = dupStart + duplicate.length();
      status = dequote(other, qm, dupStart, dupEnd, env);
      if (status != error || other != str) {
        std::cout << "ERROR_INCONSISTENT6: " << status << " != " << error << " || " << QS(other) << " != " << QS(str) << std::endl;
        errorFlag = true;
      }
      try {
        other = dequote(qm, dupStart, dupEnd, env);
        status =  DequotingStatus::OK;
      } catch (CoSupport::String::DequotingError const &e) {
        status = e.getError();
      }
      if (status != error || other != str) {
        std::cout << "ERROR_INCONSISTENT7: " << status << " != " << error << " || " << QS(other) << " != " << QS(str) << std::endl;
        errorFlag = true;
      }
      status = dequote(other, qm, dupStart, env);
      if (status != error || other != str) {
        std::cout << "ERROR_INCONSISTENT8: " << status << " != " << error << " || " << QS(other) << " != " << QS(str) << std::endl;
        errorFlag = true;
      }
      try {
        other = dequote(qm, dupStart, env);
        status =  DequotingStatus::OK;
      } catch (DequotingError const &e) {
        status = e.getError();
      }
      if (status != error || other != str) {
        std::cout << "ERROR_INCONSISTENT9: " << status << " != " << error << " || " << QS(other) << " != " << QS(str) << std::endl;
        errorFlag = true;
      }
    }
  }

  return errorFlag;
}

int main(int argc, char *argv[]) {
  bool error = false;

  std::cout << "Testing CoSupport::String::QuotedString" << std::endl;
  error |= testQuotingDequoting<QuotedString>(quotedStringTests);
  std::cout << "Testing CoSupport::String::QuoteMode::AUTO" << std::endl;
  error |= testQuotingDequoting(quotedStringTests, QuoteMode::AUTO);
  std::cout << "Testing CoSupport::String::QuoteMode::AUTO with delimiters and variables" << std::endl;
  error |= testQuotingDequoting(quotedStringDelimVarTests, QuoteMode::AUTO, ":;_", envArray);

  std::cout << "Testing CoSupport::String::SingleQuotedString" << std::endl;
  error |= testQuotingDequoting<SingleQuotedString>(singleQuotedStringTests);
  std::cout << "CoSupport::String::QuoteMode::SINGLE_WITH_QUOTES" << std::endl;
  error |= testQuotingDequoting(singleQuotedStringTests, QuoteMode::SINGLE_WITH_QUOTES);
  std::cout << "CoSupport::String::QuoteMode::SINGLE_WITH_QUOTES and delimiters" << std::endl;
  error |= testQuotingDequoting(singleQuotedStringFailTests, QuoteMode::SINGLE_WITH_QUOTES, ":;_");
  std::cout << "CoSupport::String::QuoteMode::SINGLE_WITH_QUOTES and variables" << std::endl;
  error |= testQuotingDequoting(singleQuotedStringVarTests, QuoteMode::SINGLE_WITH_QUOTES, nullptr, envArray);
  std::cout << "CoSupport::String::QuoteMode::SINGLE_NO_QUOTES" << std::endl;
  error |= testQuotingDequoting(singleNoQuotesStringTests, QuoteMode::SINGLE_NO_QUOTES);
  std::cout << "CoSupport::String::QuoteMode::SINGLE_NO_QUOTES and delimiters" << std::endl;
  error |= testQuotingDequoting(singleNoQuotesStringFailTests, QuoteMode::SINGLE_NO_QUOTES, "@");
  std::cout << "CoSupport::String::QuoteMode::SINGLE_NO_QUOTES and variables" << std::endl;
  error |= testQuotingDequoting(singleNoQuotesStringVarTests, QuoteMode::SINGLE_NO_QUOTES, nullptr, envArray);

  std::cout << "Testing CoSupport::String::DoubleQuotedString" << std::endl;
  error |= testQuotingDequoting<DoubleQuotedString>(doubleQuotedStringTests);
  std::cout << "CoSupport::String::QuoteMode::DOUBLE_WITH_QUOTES" << std::endl;
  error |= testQuotingDequoting(doubleQuotedStringTests, QuoteMode::DOUBLE_WITH_QUOTES);
  std::cout << "CoSupport::String::QuoteMode::DOUBLE_WITH_QUOTES and delimiters" << std::endl;
  error |= testQuotingDequoting(doubleQuotedStringFailTests, QuoteMode::DOUBLE_WITH_QUOTES, "!");
  std::cout << "CoSupport::String::QuoteMode::DOUBLE_WITH_QUOTES and variables" << std::endl;
  error |= testQuotingDequoting(doubleQuotedStringVarTests, QuoteMode::DOUBLE_WITH_QUOTES, nullptr, envArray);
  std::cout << "CoSupport::String::QuoteMode::DOUBLE_NO_QUOTES" << std::endl;
  error |= testQuotingDequoting(doubleNoQuotesStringTests, QuoteMode::DOUBLE_NO_QUOTES);
  std::cout << "CoSupport::String::QuoteMode::DOUBLE_NO_QUOTES and delimiters" << std::endl;
  error |= testQuotingDequoting(doubleNoQuotesStringFailTests, QuoteMode::DOUBLE_NO_QUOTES, "\n");
  std::cout << "CoSupport::String::QuoteMode::DOUBLE_NO_QUOTES and variables" << std::endl;
  error |= testQuotingDequoting(doubleNoQuotesStringVarTests, QuoteMode::DOUBLE_NO_QUOTES, nullptr, envArray);

  return error ? -1 : 0;
}
