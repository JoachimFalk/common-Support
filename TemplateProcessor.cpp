/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 * 
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * --- This software and any associated documentation is provided "as is" 
 * 
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#include <CoSupport/String/TemplateProcessor.hpp>

#include <stdexcept>
#include <sstream>
#include <iostream>

#include <errno.h>

#include <boost/regex.hpp>

namespace CoSupport { namespace String {

namespace {

  static const boost::regex reTemplateVariable("<@([[:alnum:]]+)@>");

} // namespace Anonymous

TemplateProcessor::TemplateProcessor(const std::string &templateFileName)
  : templateFileName(templateFileName),
    templateFile(templateFileName.c_str(), std::ios::in) {
  if (!templateFile.is_open()) {
    std::ostringstream msg;
    msg << "Can't open template file '"
        << templateFileName << "' :"
        << strerror(errno);
    throw std::runtime_error(msg.str());
  }
}

void TemplateProcessor::fillIn(
    const std::string &outputFileName,
    const std::map<std::string, std::string> &fields) const {
  std::ofstream out(outputFileName.c_str(), std::ios::out);
  if (!out.is_open()) {
    std::ostringstream msg;
    msg << "Can't open template output file '"
        << outputFileName << "' :"
        << strerror(errno);
    throw std::runtime_error(msg.str());
  }
  
  fillIn(out, fields);
  
  if (out.bad()) {
    std::ostringstream msg;
    msg << "Can't write to output file '"
        << outputFileName << "' :"
        << strerror(errno);
    throw std::runtime_error(msg.str());
  }
}

void TemplateProcessor::fillIn(
    std::ostream &out,
    const std::map<std::string, std::string> &fields) const {
  templateFile.clear();
  templateFile.seekg(0, std::ios::beg);
  
  while (templateFile.good() && out.good()) {
    std::string templateStringLine;
    std::getline(templateFile, templateStringLine);
    
    std::string templateStringLineNew;
    size_t      templateStringLinePos = 0;
    
    // iterate over all template variables in the line
    for (boost::regex_iterator<std::string::const_iterator>
           iter(
             templateStringLine.begin(),
             templateStringLine.end(),
             reTemplateVariable);
         iter !=  boost::regex_iterator<std::string::const_iterator>();
         ++iter) {
      // (*iter)[0] is the whole match string, e.g., "<@TEMPLATEVAR@>"
      // (*iter)[1] is the name of the template variable, e.g., "TEMPLATEVAR"
      
      // Append stuff before template match
      templateStringLineNew.append(
        templateStringLine.begin() + templateStringLinePos,
        templateStringLine.begin() + iter->position((std::size_t) 0));
      // Advance templateStringLinePos after "<@TEMPLATEVAR@>"
      templateStringLinePos =
        iter->position((std::size_t) 0) + iter->length(0);
      // Find template variable definition
      std::map<std::string, std::string>::const_iterator viter =
        fields.find((*iter)[1]);
      if (viter != fields.end()) {
        std::string subst      = viter->second;
        size_t      column     = templateStringLineNew.length();
        bool        needIndent = false;
        
        while (subst.length()) {
          if (needIndent) {
            templateStringLineNew.append("\n");
            for (size_t many = column; many > 0; --many)
              templateStringLineNew.append(" ");
          } else
            needIndent = true;
          size_t nlpos = subst.find('\n');
          templateStringLineNew.append(subst.substr(0, nlpos));
          if (nlpos < std::string::npos)
            subst = subst.substr(nlpos + 1); // the +1 skips the '\n'
          else
            subst = "";
        }
      } else {
        templateStringLineNew
          .append("!!!")
          .append((*iter)[1])
          .append(":UNDEF!!!");
      }
    }
    // Append rest of template file line
    templateStringLineNew.append(
      templateStringLine.begin() + templateStringLinePos,
      templateStringLine.end());
    
    out << templateStringLineNew << '\n';
  }
  if (templateFile.bad()) {
    std::ostringstream msg;
    msg << "Can't read from template file '"
        << templateFileName << "' :"
        << strerror(errno);
    throw std::runtime_error(msg.str());
  }
}

} } // namespace CoSupport::String

