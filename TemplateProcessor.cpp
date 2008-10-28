
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
        templateStringLine.begin() + iter->position(0));
      // Advance templateStringLinePos after "<@TEMPLATEVAR@>"
      templateStringLinePos =
        iter->position(0) + iter->length(0);
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

