// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <cstring>
#include <cassert>

#include <map>
#include <list>
#include <iostream>

#include <CoSupport/compatibility-glue/nullptr.h>

#include <CoSupport/Streams/DebugOStream.hpp>

#include <CoSupport/XML/Xerces/common.hpp>
#include <CoSupport/XML/Xerces/Handler.hpp>

#include <boost/filesystem.hpp>

#include "test1-xsd.c" // Get test1.xsd
#include "test2-dtd.c" // Get test2.dtd

namespace fs = boost::filesystem;

using CoSupport::Streams::Color;

int main(int argc, char *argv[]) {
  CoSupport::Streams::DebugOStream out(std::cout);

  const char *testdata = getenv("TESTDATA");
  assert(testdata != nullptr);

  bool allTestsPassed = true;

/*{
    CoSupport::XML::Xerces::Handler handler;

    handler.setTopElementName(XMLCH("top"));
    handler.setDTDUrl(XMLCH("test.dtd"));
    handler.createEmpty();
    handler.save("test-xml1.xml");
  }*/
  const char *xsdTestsExpectedFail[] = {
      "test1a.xml",
      "test1b.xml",
      "test1c.xml",
      "test1d.xml"
    };
  const char *xsdTestsExpectedPass[] = {
      "test1e.xml",
      "test1f.xml",
      "test1g.xml",
      "test1h.xml"
    };

  for (size_t i = 0;
       i < sizeof(xsdTestsExpectedFail)/sizeof(xsdTestsExpectedFail[0]);
       ++i) {
    fs::path testXml = fs::path(testdata) / xsdTestsExpectedFail[i];
    try {
      CoSupport::XML::Xerces::Handler handler;

      handler.setTopElementName(XMLCH("test1_top"));
      handler.setXSDUrl(XMLCH("test1-intern.xsd"));
      handler.setXSD(test1XSD, sizeof(test1XSD)-1);
      handler.load(testXml.native());
      out << Color::Red() << "ERROR: unexpected pass for parsing " << testXml << Color::Auto() << std::endl;
      allTestsPassed = false;
    } catch (std::runtime_error &e) {
      out << Color::Green() << "OK: expected fail for parsing " << testXml << ": " << e.what() << Color::Auto() << std::endl;
    }
  }
  for (size_t i = 0;
       i < sizeof(xsdTestsExpectedPass)/sizeof(xsdTestsExpectedPass[0]);
       ++i) {
    fs::path testXml = fs::path(testdata) / xsdTestsExpectedPass[i];
    try {
      CoSupport::XML::Xerces::Handler handler;

      handler.setTopElementName(XMLCH("test1_top"));
      handler.setXSDUrl(XMLCH("test1-intern.xsd"));
      handler.setXSD(test1XSD, sizeof(test1XSD)-1);
      handler.load(testXml.native());
      out << Color::Green() << "OK: expected pass for parsing " << testXml << Color::Auto() << std::endl;
    } catch (std::runtime_error &e) {
      out << Color::Red() << "ERROR: unexpected fail for parsing " << testXml << ": " << e.what() << Color::Auto() << std::endl;
      allTestsPassed = false;
    }
  }

  const char *dtdTestsExpectedFail[] = {
      "test2a.xml",
      "test2b.xml",
      "test2c.xml",
      "test2d.xml"
    };
  const char *dtdTestsExpectedPass[] = {
//    "test2e.xml", //FIXME: This test fails because we can't force usage of a DTD if none is given
      "test2f.xml",
      "test2g.xml",
//    "test2h.xml"  //FIXME: This test fails because we can't force usage of a DTD if none is given
    };

  for (size_t i = 0;
       i < sizeof(dtdTestsExpectedFail)/sizeof(dtdTestsExpectedFail[0]);
       ++i) {
    fs::path testXml = fs::path(testdata) / dtdTestsExpectedFail[i];
    try {
      CoSupport::XML::Xerces::Handler handler;

      handler.setTopElementName(XMLCH("test2_top"));
      handler.setDTDUrl(XMLCH("test2-intern.dtd"));
      handler.setDTD(test2DTD, sizeof(test2DTD)-1);
      handler.load(testXml.native());
      out << Color::Red() << "ERROR: unexpected pass for parsing " << testXml << Color::Auto() << std::endl;
      allTestsPassed = false;
    } catch (std::runtime_error &e) {
      out << Color::Green() << "OK: expected fail for parsing " << testXml << ": " << e.what() << Color::Auto() << std::endl;
    }
  }
  for (size_t i = 0;
       i < sizeof(dtdTestsExpectedPass)/sizeof(dtdTestsExpectedPass[0]);
       ++i) {
    fs::path testXml = fs::path(testdata) / dtdTestsExpectedPass[i];
    try {
      CoSupport::XML::Xerces::Handler handler;

      handler.setTopElementName(XMLCH("test2_top"));
      handler.setDTDUrl(XMLCH("test2-intern.dtd"));
      handler.setDTD(test2DTD, sizeof(test2DTD)-1);
      handler.load(testXml.native());
      out << Color::Green() << "OK: expected pass for parsing " << testXml << Color::Auto() << std::endl;
    } catch (std::runtime_error &e) {
      out << Color::Red() << "ERROR: unexpected fail for parsing " << testXml << ": " << e.what() << Color::Auto() << std::endl;
      allTestsPassed = false;
    }
  }

  if (allTestsPassed) {
    out << Color::Green() << "OK: Passed all tests!" << Color::Auto() << std::endl;
    return 0;
  } else {
    out << Color::Red() << "ERROR: Some tests failed!" << Color::Auto() << std::endl;
    return 1;
  }
}
