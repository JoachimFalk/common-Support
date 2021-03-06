// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
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

//#include "DebugOStream.hpp"

#include <CoSupport/compatibility-glue/nullptr.h>

#include <CoSupport/XML/Xerces/Handler.hpp>
#include <CoSupport/XML/Xerces/StdOstreamFormatTarget.hpp>
#include <CoSupport/XML/Xerces/StdIstreamInputSource.hpp>

#include <boost/noncopyable.hpp>
//#include <boost/smart_ptr/scoped_ptr.hpp>



#include <xercesc/validators/common/Grammar.hpp>

#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>
#include <xercesc/framework/XMLGrammarDescription.hpp>

#include <xercesc/dom/DOMImplementationRegistry.hpp>

//#include <xercesc/validators/DTD/DTDValidator.hpp>
//#include <xercesc/validators/schema/SchemaValidator.hpp>

#include <xercesc/util/XercesVersion.hpp>

#include <sstream>

#include <algorithm>    // std::find_if
#include <functional>   // std::not1

#include <xercesc/dom/DOMErrorHandler.hpp>
#if XERCES_VERSION_MAJOR >= 3
# include <xercesc/util/XMLEntityResolver.hpp>
#elif XERCES_VERSION_MAJOR == 2
# include <xercesc/dom/DOMEntityResolver.hpp>
# include <xercesc/internal/XMLGrammarPoolImpl.hpp>
#else //XERCES_VERSION_MAJOR == 2
# error "Oops neither Xerces version 2 nor 3"
#endif //XERCES_VERSION_MAJOR != 2 && XERCES_VERSION_MAJOR != 3

namespace CoSupport { namespace XML { namespace Xerces {

  /**
   * Simple error handler to be installed in a parser
   */
  class DOMErrorHandler
  : public XN::DOMErrorHandler,
    private boost::noncopyable {
  public:
    DOMErrorHandler(std::ostream &outDbg);

    /**
     * Implementation of the DOM ErrorHandler interface
     */
    bool handleError(const XN::DOMError &domError);

    /**
     * Check if an error occurred.
     */
    bool parseFailed()
      { return failed; }
  private:
    std::ostream &outDbg;
    bool          failed;
  };

  DOMErrorHandler::DOMErrorHandler(std::ostream &outDbg)
    : outDbg(outDbg), failed(false) {}

  /**
   * Implementation of the DOM ErrorHandler interface
   */
  bool DOMErrorHandler::handleError(const XN::DOMError &domError){
    outDbg << "Xerces DOMError of severity ";
    switch (domError.getSeverity()) {
      case XN::DOMError::DOM_SEVERITY_WARNING:
        outDbg << "warning at file ";
        break;
      case XN::DOMError::DOM_SEVERITY_ERROR:
        outDbg << "error at file ";
        failed = true;
        break;
      case XN::DOMError::DOM_SEVERITY_FATAL_ERROR:
        outDbg << "fatal error at file ";
        failed = true;
        break;
#ifndef NDEBUG
      default:
        assert(!"Unhandled domError.getSeverity() level!");
#endif
    }
    XMLCh const *uri = domError.getLocation()->getURI();
    outDbg << (uri ? NStr(uri) : NStr("unknown file or stream"))
         << ", line " << domError.getLocation()->getLineNumber()
         << ", char " << domError.getLocation()->getColumnNumber()
         << ": " << static_cast<const XMLCh *const>(domError.getMessage()) << std::endl;
    return !failed;
  }

  // MSVC requires at least one byte in the array!
  static XMLByte const emptyXMLByte[] = { 0 }; 

#if XERCES_VERSION_MAJOR >= 3 // XERCES_VERSION_MAJOR >= 3
  class DOMEntityResolver
  : public XN::XMLEntityResolver,
    private boost::noncopyable {
  public:
    DOMEntityResolver();

    /**
      * Allow the application to resolve external entities.
      *
      * <p>The Parser will call this method before opening any external
      * entity except the top-level document entity (including the
      * external DTD subset, external entities referenced within the
      * DTD, and external entities referenced within the document
      * element): the application may request that the parser resolve
      * the entity itself, that it use an alternative URI, or that it
      * use an entirely different input source.</p>
      *
      * <p>Application writers can use this method to redirect external
      * system identifiers to secure and/or local URIs, to look up
      * public identifiers in a catalogue, or to read an entity from a
      * database or other input source (including, for example, a dialog
      * box).</p>
      *
      * <p>If the system identifier is a URL, the SAX parser must
      * resolve it fully before reporting it to the application.</p>
      *
      * @param resourceIdentifier An object containing the type of
      *        resource to be resolved and the associated data members
      *        corresponding to this type.
      * @return An InputSource object describing the new input source,
      *         or null to request that the parser open a regular
      *         URI connection to the system identifier.
      *         The returned InputSource is owned by the parser which is
      *         responsible to clean up the memory.
      * @exception SAXException Any SAX exception, possibly
      *            wrapping another exception.
      * @exception IOException An IO exception,
      *            possibly the result of creating a new InputStream
      *            or Reader for the InputSource.
      *
      * @see InputSource#InputSource
      * @see XMLResourceIdentifier
      */
      XN::InputSource *resolveEntity(
          XN::XMLResourceIdentifier *resourceIdentifier);
  };

  DOMEntityResolver::DOMEntityResolver() {
  }

  XN::InputSource *DOMEntityResolver::resolveEntity(
      XN::XMLResourceIdentifier *resourceIdentifier) {
/*  std::cout << "DOMEntityResolver::resolveEntity(...)"
        <<  " publicId: " << (resourceIdentifier->getPublicId()
              ? NStr(resourceIdentifier->getPublicId())
              : NStr("nullptr"))
        << ", systemId: " << (resourceIdentifier->getSystemId()
              ? NStr(resourceIdentifier->getSystemId())
              : NStr("nullptr"))
        << ", baseURI: " << (resourceIdentifier->getBaseURI()
              ? NStr(resourceIdentifier->getBaseURI())
              : NStr("nullptr"))
        << std::endl; */
    return
      new XN::MemBufInputSource(emptyXMLByte, sizeof(emptyXMLByte)-1, resourceIdentifier->getSystemId());
  }

#elif XERCES_VERSION_MAJOR == 2 // XERCES_VERSION_MAJOR == 2
  class DOMEntityResolver
  : public XN::DOMEntityResolver,
    private boost::noncopyable {
  public:
    DOMEntityResolver();


    /**
     * Allow the application to resolve external entities.
     *
     * The DOMBuilder will call this method before opening any external
     * entity except the top-level document entity (including the
     * external DTD subset, external entities referenced within the
     * DTD, and external entities referenced within the document
     * element): the application may request that the DOMBuilder resolve
     * the entity itself, that it use an alternative URI, or that it
     * use an entirely different input source.
     */
    XN::DOMInputSource *resolveEntity(
        XMLCh const *const publicId,
        XMLCh const *const systemId,
        XMLCh const *const baseURI);
  };

  DOMEntityResolver::DOMEntityResolver() {
  }

  XN::DOMInputSource *DOMEntityResolver::resolveEntity(
      XMLCh const *const publicId,
      XMLCh const *const systemId,
      XMLCh const *const baseURI) {
/*  std::cout << "DOMEntityResolver::resolveEntity("
        << (publicId ? NStr(publicId) : NStr("nullptr")) << ", "
        << (systemId ? NStr(systemId) : NStr("nullptr")) << ", "
        << (baseURI  ? NStr(baseURI)  : NStr("nullptr"))
        << ")" << std::endl; */
    return
      // We need a wrapper for the empty entity source
      new XN::Wrapper4InputSource(
        new XN::MemBufInputSource(emptyXMLByte, sizeof(emptyXMLByte)-1, systemId),
        true /* free XN::MemBufInputSource */
      );
  }
#endif //XERCES_VERSION_MAJOR == 2

  Handler::Handler()
    : dtdBuf(nullptr),    xsdBuf(nullptr),
      dtdSize(0),      xsdSize(0)
  {
//  // process xml
//  static const XMLCh gLS[] = { XN::chLatin_L, XN::chLatin_S, XN::chNull };
//  domImpl = XN::DOMImplementationRegistry::getDOMImplementation(XMLCH("XML 1.0 Traversal 2.0"));
    domImpl = XN::DOMImplementationRegistry::getDOMImplementation(XMLCH("XML 1.0"));
    assert(domImpl != nullptr);
  }

  void Handler::setDTD(std::string const &dtdStr_) {
    dtdStr   = dtdStr_;
    dtdBuf   = dtdStr.c_str();
    dtdSize  = dtdStr.size();
  }
  void Handler::setDTD(char const *dtdBuf_) {
    dtdStr  = "";
    dtdBuf  = dtdBuf_;
    dtdSize = dtdBuf != nullptr ? strlen(dtdBuf) : 0;
  }
  void Handler::setDTD(char const *dtdBuf_, size_t dtdSize_) {
    dtdStr  = "";
    dtdBuf  = dtdBuf_;
    dtdSize = dtdBuf != nullptr ? dtdSize_ : 0;
  }

  void Handler::setXSD(std::string const &xsdStr_) {
    xsdStr   = xsdStr_;
    xsdBuf   = xsdStr.c_str();
    xsdSize  = xsdStr.size();
  }
  void Handler::setXSD(char const *xsdBuf_) {
    xsdStr  = "";
    xsdBuf  = xsdBuf_;
    xsdSize = xsdBuf != nullptr ? strlen(xsdBuf) : 0;
  }
  void Handler::setXSD(char const *xsdBuf_, size_t xsdSize_) {
    xsdStr  = "";
    xsdBuf  = xsdBuf_;
    xsdSize = xsdBuf != nullptr ? xsdSize_ : 0;
  }

#ifdef _WIN32
  void Handler::load(std::wstring const &xmlFileName) {
    XN::LocalFileInputSource src(static_cast<XMLCh const *>(xmlFileName.c_str()));
    load(src);
  }
#endif //_WIN32

  void Handler::load(XStr const &xmlFileName) {
    XN::LocalFileInputSource src(xmlFileName.c_str());
    load(src);
  }

  void Handler::load(std::istream &xmlInputStream) {
    StdIstreamInputSource src(xmlInputStream);
    load(src);
  }

  void Handler::createEmpty() {
    // build empty XML document
    assert(!topElementName.empty());
    XN::DOMDocumentType *type = dtdUrl.empty() || !xsdUrl.empty()
      ? nullptr
      : domImpl->createDocumentType(topElementName, nullptr, dtdUrl);
    domDocument.reset(domImpl->createDocument(nullptr, topElementName, type));
    if (!xsdUrl.empty()) {
      XN::DOMNode *topTag = getDocument()->getDocumentElement();
      setAttrValueFrom(topTag, XMLCH("xmlns:xsi"), XMLCH("http://www.w3.org/2001/XMLSchema-instance"));
      setAttrValueFrom(topTag, XMLCH("xsi:noNamespaceSchemaLocation"), xsdUrl);
    }
  }

  void Handler::save(std::string const &xmlFileName) const {
    XN::LocalFileFormatTarget dest(XStr(xmlFileName).c_str());
    save(&dest);
  }

  void Handler::save(std::ostream &xmlOutputStream) const {
    StdOstreamFormatTarget dest(xmlOutputStream);
    save(&dest);
    dest.flush();
  }

  const XN::DOMDocument *Handler::getDocument() const
    { return domDocument.get(); }

  XN::DOMDocument *Handler::getDocument()
    { return domDocument.get(); }

  Handler::~Handler() {
  }

  static
  XMLCh const emptyXMLCh[] = { 0 };

  void Handler::load(XN::InputSource &in) {
    try {
      std::ostringstream parseErrors;
      // create an error handler
      DOMErrorHandler errorHandler(parseErrors);
      // create an entitiy resolver
      DOMEntityResolver entityResolver;

      // We need a wrapper for the input source
      XN::Wrapper4InputSource   src(&in, false /* don't free &in */);
#if XERCES_VERSION_MAJOR >= 3
      // Xerces-C++ 3.0.0 and later.
      
      // Get a parser
      ScopedXMLPtr<XN::DOMLSParser> domParser;
      if (!dtdUrl.empty() && xsdUrl.empty()) {
        // We use a DTD for verification
        domParser.reset(domImpl->createLSParser(
            XN::DOMImplementationLS::MODE_SYNCHRONOUS,
            XMLCH("http://www.w3.org/TR/REC-xml")));
      } else if (dtdUrl.empty() && !xsdUrl.empty()) {
        // We use a XSD for verification
        domParser.reset(domImpl->createLSParser(
            XN::DOMImplementationLS::MODE_SYNCHRONOUS,
            XMLCH("http://www.w3.org/2001/XMLSchema")));
      } else {
        domParser.reset(domImpl->createLSParser(
            XN::DOMImplementationLS::MODE_SYNCHRONOUS,
            nullptr));
      }
      
      XN::DOMConfiguration *conf = domParser->getDomConfig();
      
      // Set error handler.
      conf->setParameter(XN::XMLUni::fgDOMErrorHandler, &errorHandler);

      // Discard comment nodes in the document.
      conf->setParameter(XN::XMLUni::fgDOMComments, false);

      // Enable datatype normalization.
      conf->setParameter(XN::XMLUni::fgDOMDatatypeNormalization, true);

      // Do not create EntityReference nodes in the DOM tree. No
      // EntityReference nodes will be created, only the nodes corresponding to
      // their fully expanded substitution text will be created.
      conf->setParameter(XN::XMLUni::fgDOMEntities, false);

      // Do not include ignorable whitespace in the DOM tree.
      conf->setParameter(XN::XMLUni::fgDOMElementContentWhitespace, false);

      if (!xsdUrl.empty() || !dtdUrl.empty()) {
        // Controls whether all validation errors are reported. If this feature
        // is set to true, the document must specify a grammar. If this feature
        // is set to false and document specifies a grammar, that grammar might
        // be parsed but no validation of the document contents will be
        // performed.
        conf->setParameter(XN::XMLUni::fgDOMValidate, true);
        // If true, the parser will validate the document only if a grammar is
        // specified.  If false (default), validation is determined by the
        // state of the fgDOMValidation feature.
        //domParser->setFeature(XN::XMLUni::fgDOMValidateIfSchema, false);
        // Use cached grammar if it exists in the pool. If false (default),
        // then we parse the schema grammar. If the
        // fgXercesCacheGrammarFromParse feature is enabled, then this feature
        // is set to true automatically and any setting to this feature by the
        // user is a no-op.
        conf->setParameter(XN::XMLUni::fgXercesUseCachedGrammarInParse, true);
        // If true, then the parser will not attempt to resolve the entity when
        // the resolveEntity method returns nullptr.  If false (default), then the
        // the parser will attempt to resolve the entity when the resolveEntity
        // method returns nullptr.
        // Don't load XSD or DTD entities from file system or via URL!
        conf->setParameter(XN::XMLUni::fgXercesDisableDefaultEntityResolution, true);
        // Set entity resolver that always returns and empty object.
        conf->setParameter(XN::XMLUni::fgXercesEntityResolver, &entityResolver);
      } else {
        conf->setParameter(XN::XMLUni::fgDOMValidate, false);
        conf->setParameter(XN::XMLUni::fgDOMValidateIfSchema, true);

        // Controls whether a external DTD is loaded. This feature is ignored
        // and the DTD is always loaded when the validation feature is turned
        // on.  If true (default), the parser loads an external DTD.  If false,
        // external DTD are ignored completely.  Note that this feature is
        // ignored and a DTD is always loaded when validation is on.
        //domParser->setFeature(XN::XMLUni::fgXercesLoadExternalDTD, false);
      }

      if (!xsdUrl.empty()) {
        // Enable the parser's schema support. If set to true, then namespace
        // processing must also be turned on.
        conf->setParameter(XN::XMLUni::fgXercesSchema, true);
        // Controls whether or not namespace processing is performed.  If the
        // validation is on, then the document must contain a grammar that
        // supports the use of namespaces
        conf->setParameter(XN::XMLUni::fgDOMNamespaces, true);
        // Controls whether or not the schema itself is fully checked for
        // additional errors that are time-consuming or memory-intensive to
        // discover
        conf->setParameter(XN::XMLUni::fgXercesSchemaFullChecking, false);
        // When this feature is on and the schema validation feature is on,
        // then the parser will ignore the DTD, except for entities.
        conf->setParameter(XN::XMLUni::fgXercesSkipDTDValidation, true);

        if (xsdBuf) {
//        std::cout << "preload xsd " << xsdUrl << std::endl;
          // Preload XSD
//        XN::LocalFileInputSource xsdIn(XStr(xsdUrl).c_str());
          XN::MemBufInputSource xsdIn(
              reinterpret_cast<XMLByte const *>(xsdBuf),
              xsdSize,
              xsdUrl.c_str());
          // We need a wrapper for the XSD source
          XN::Wrapper4InputSource xsdSrc(&xsdIn, false /* don't free &xsdIn */);
          sassert(domParser->loadGrammar(&xsdSrc, XN::Grammar::SchemaGrammarType, true) != nullptr);
        }
        conf->setParameter(XN::XMLUni::fgXercesSchemaExternalSchemaLocation, (void *) emptyXMLCh);
        conf->setParameter(XN::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, (void *) xsdUrl.c_str());
      } else {
        conf->setParameter(XN::XMLUni::fgXercesSchema, false);
      }

      if (!dtdUrl.empty()) {
        if (dtdBuf) {
//        std::cout << "preload dtd " << dtdUrl << std::endl;
          // Preload DTD
//        XN::LocalFileInputSource dtdIn(XStr(dtdUrl).c_str());
          XN::MemBufInputSource dtdIn(
              reinterpret_cast<XMLByte const *>(dtdBuf),
              dtdSize,
              dtdUrl.c_str());
          // We need a wrapper for the DTD source
          XN::Wrapper4InputSource dtdSrc(&dtdIn, false /* don't free &dtdIn */);
          //XN::Grammar *gram = domParser->loadGrammar(dtdSrc, XN::Grammar::DTDGrammarType, false);
          //assert(gram != nullptr);
          //gramPool->cacheGrammar(gram);
          sassert(domParser->loadGrammar(&dtdSrc, XN::Grammar::DTDGrammarType, true) != nullptr);

          // Ignore a cached DTD when an XML document contains both an internal
          // and external DTD, and the use cached grammar from parse option is
          // enabled.  Currently, we do not allow using cached DTD grammar when
          // an internal subset is present in the document. This option will
          // only affect the behavior of the parser when an internal and
          // external DTD both exist in a document (i.e. no effect if document
          // has no internal subset).
          conf->setParameter(XN::XMLUni::fgXercesIgnoreCachedDTD, false);
        }
        conf->setParameter(XN::XMLUni::fgXercesSchemaExternalSchemaLocation, (void *) emptyXMLCh);
        conf->setParameter(XN::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, (void *) emptyXMLCh);
      }

      // If true, then parser caches the grammar in the pool for re-use in subsequent parses.
      // If false (defaul), the parser does not cache the grammar in the pool.
      conf->setParameter(XN::XMLUni::fgXercesCacheGrammarFromParse, false);

/*    // Debug this! Did the preload work?
      XN::RefHashTableOfEnumerator<XN::Grammar> iter = gramPool->getGrammarEnumerator();
      while (iter.hasMoreElements()) {
        XN::Grammar &gram = iter.nextElement();
        std::cout << gram.getGrammarDescription()->getGrammarKey() << std::endl;
      }
 */

//    // reset document pool - clear all previous allocated data
//    domParser->resetDocumentPool();

      // We will release the DOM document ourselves.
      conf->setParameter(XN::XMLUni::fgXercesUserAdoptsDOMDocument, true);
      domDocument.reset(domParser->parse(&src));
#elif XERCES_VERSION_MAJOR == 2
      // Same as above but for Xerces-C++ 2 series, cf.
      // http://xerces.apache.org/xerces-c/program-dom-2.html
      
//    XN::XMLGrammarPool* gramPool(
//      new XN::XMLGrammarPoolImpl(XN::XMLPlatformUtils::fgMemoryManager));
      // Get a parser
      ScopedXMLPtr<XN::DOMBuilder> domParser;
      if (!dtdUrl.empty() && xsdUrl.empty()) {
        // We use a DTD for verification
        domParser.reset(domImpl->createDOMBuilder(
            XN::DOMImplementationLS::MODE_SYNCHRONOUS,
            XMLCH("http://www.w3.org/TR/REC-xml"),
            XN::XMLPlatformUtils::fgMemoryManager/*,
            gramPool*/));
      } else if (dtdUrl.empty() && !xsdUrl.empty()) {
        // We use a XSD for verification
        domParser.reset(domImpl->createDOMBuilder(
            XN::DOMImplementationLS::MODE_SYNCHRONOUS,
            XMLCH("http://www.w3.org/2001/XMLSchema"),
            XN::XMLPlatformUtils::fgMemoryManager/*,
            gramPool*/));
      } else {
        domParser.reset(domImpl->createDOMBuilder(
            XN::DOMImplementationLS::MODE_SYNCHRONOUS,
            nullptr,
            XN::XMLPlatformUtils::fgMemoryManager/*,
            gramPool*/));
      }
      
      // Set error handler.
      domParser->setErrorHandler(&errorHandler);

      // Discard comment nodes in the document.
      domParser->setFeature(XN::XMLUni::fgDOMComments, false);
      
      // Enable datatype normalization.
      domParser->setFeature(XN::XMLUni::fgDOMDatatypeNormalization, true);
      
      // Do not create EntityReference nodes in the DOM tree. No
      // EntityReference nodes will be created, only the nodes corresponding to
      // their fully expanded substitution text will be created.
      domParser->setFeature(XN::XMLUni::fgDOMEntities, false);
      
      // Do not include ignorable whitespace in the DOM tree.
      domParser->setFeature(XN::XMLUni::fgDOMWhitespaceInElementContent, false);
      
      if (!xsdUrl.empty() || !dtdUrl.empty()) {
        // Controls whether all validation errors are reported. If this feature
        // is set to true, the document must specify a grammar. If this feature
        // is set to false and document specifies a grammar, that grammar might
        // be parsed but no validation of the document contents will be
        // performed.
        domParser->setFeature(XN::XMLUni::fgDOMValidation, true);
        // If true, the parser will validate the document only if a grammar is
        // specified.  If false (default), validation is determined by the
        // state of the fgDOMValidation feature.
        //domParser->setFeature(XN::XMLUni::fgDOMValidateIfSchema, false);
        // Use cached grammar if it exists in the pool. If false (default),
        // then we parse the schema grammar. If the
        // fgXercesCacheGrammarFromParse feature is enabled, then this feature
        // is set to true automatically and any setting to this feature by the
        // user is a no-op.
        domParser->setFeature(XN::XMLUni::fgXercesUseCachedGrammarInParse, true);
        // If true, then the parser will not attempt to resolve the entity when
        // the resolveEntity method returns nullptr.  If false (default), then the
        // the parser will attempt to resolve the entity when the resolveEntity
        // method returns nullptr.
        // Don't load XSD or DTD entities from file system or via URL!
        domParser->setFeature(XN::XMLUni::fgXercesDisableDefaultEntityResolution, true);
        // Set entity resolver that always returns and empty object.
        domParser->setEntityResolver(&entityResolver);
      } else {
        domParser->setFeature(XN::XMLUni::fgDOMValidation, false);
        domParser->setFeature(XN::XMLUni::fgDOMValidateIfSchema, true);

        // Controls whether a external DTD is loaded. This feature is ignored
        // and the DTD is always loaded when the validation feature is turned
        // on.  If true (default), the parser loads an external DTD.  If false,
        // external DTD are ignored completely.  Note that this feature is
        // ignored and a DTD is always loaded when validation is on.
        //domParser->setFeature(XN::XMLUni::fgXercesLoadExternalDTD, false);
      }

      if (!xsdUrl.empty()) {
        // Enable the parser's schema support. If set to true, then namespace
        // processing must also be turned on.
        domParser->setFeature(XN::XMLUni::fgXercesSchema, true);
        // Controls whether or not namespace processing is performed.  If the
        // validation is on, then the document must contain a grammar that
        // supports the use of namespaces
        domParser->setFeature(XN::XMLUni::fgDOMNamespaces, true);
        // Controls whether or not the schema itself is fully checked for
        // additional errors that are time-consuming or memory-intensive to
        // discover
        domParser->setFeature(XN::XMLUni::fgXercesSchemaFullChecking, false);
        // When this feature is on and the schema validation feature is on,
        // then the parser will ignore the DTD, except for entities.
        domParser->setFeature(XN::XMLUni::fgXercesSkipDTDValidation, true);

        if (xsdBuf) {
//        std::cout << "preload xsd " << xsdUrl << std::endl;
          // Preload XSD
//        XN::LocalFileInputSource xsdIn(XStr(xsdUrl).c_str());
          XN::MemBufInputSource xsdIn(
              reinterpret_cast<XMLByte const *>(xsdBuf),
              xsdSize,
              xsdUrl.c_str());
          // We need a wrapper for the XSD source
          XN::Wrapper4InputSource xsdSrc(&xsdIn, false /* don't free &xsdIn */);
          sassert(domParser->loadGrammar(xsdSrc, XN::Grammar::SchemaGrammarType, true) != nullptr);
        }
        domParser->setProperty(XN::XMLUni::fgXercesSchemaExternalSchemaLocation, (void *) emptyXMLCh);
        domParser->setProperty(XN::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, (void *) xsdUrl.c_str());
      } else {
        domParser->setFeature(XN::XMLUni::fgXercesSchema, false);
      }
      
      if (!dtdUrl.empty()) {
        if (dtdBuf) {
//        std::cout << "preload dtd " << dtdUrl << std::endl;
          // Preload DTD
//        XN::LocalFileInputSource dtdIn(XStr(dtdUrl).c_str());
          XN::MemBufInputSource dtdIn(
              reinterpret_cast<XMLByte const *>(dtdBuf),
              dtdSize,
              dtdUrl.c_str());
          // We need a wrapper for the DTD source
          XN::Wrapper4InputSource dtdSrc(&dtdIn, false /* don't free &dtdIn */);
          //XN::Grammar *gram = domParser->loadGrammar(dtdSrc, XN::Grammar::DTDGrammarType, false);
          //assert(gram != nullptr);
          //gramPool->cacheGrammar(gram);
          sassert(domParser->loadGrammar(dtdSrc, XN::Grammar::DTDGrammarType, true) != nullptr);

          // Ignore a cached DTD when an XML document contains both an internal
          // and external DTD, and the use cached grammar from parse option is
          // enabled.  Currently, we do not allow using cached DTD grammar when
          // an internal subset is present in the document. This option will
          // only affect the behavior of the parser when an internal and
          // external DTD both exist in a document (i.e. no effect if document
          // has no internal subset).
          domParser->setFeature(XN::XMLUni::fgXercesIgnoreCachedDTD, false);
        }
        domParser->setProperty(XN::XMLUni::fgXercesSchemaExternalSchemaLocation, (void *) emptyXMLCh);
        domParser->setProperty(XN::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, (void *) emptyXMLCh);
      }

      // If true, then parser caches the grammar in the pool for re-use in subsequent parses.
      // If false (defaul), the parser does not cache the grammar in the pool.
      domParser->setFeature(XN::XMLUni::fgXercesCacheGrammarFromParse, false);

/*    // Debug this! Did the preload work?
      XN::RefHashTableOfEnumerator<XN::Grammar> iter = gramPool->getGrammarEnumerator();
      while (iter.hasMoreElements()) {
        XN::Grammar &gram = iter.nextElement();
        std::cout << gram.getGrammarDescription()->getGrammarKey() << std::endl;
      }
 */

//    // reset document pool - clear all previous allocated data
//    domParser->resetDocumentPool();

      // We will release the DOM document ourselves.
      domParser->setFeature (XN::XMLUni::fgXercesUserAdoptsDOMDocument, true);
      domDocument.reset(domParser->parse(src));
#endif // XERCES_VERSION_MAJOR == 2
      //check if parsing failed
      if (errorHandler.parseFailed()) {
        std::string s(parseErrors.str());
        // Trim whitespace and newline at end of string s
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        throw std::runtime_error(s);
      }
      assert(domDocument != nullptr);
      assert(getDocument()->getDocumentElement() != nullptr);
    }
    catch (std::runtime_error &err) {
      // std::runtime_error exceptions don't need a translation
      throw;
    }
    catch (const XN::XMLException &toCatch) {
      throw std::runtime_error(
          "Handler::load: Error while parsing XML file: "
          "XMLException message is \"" + NStr(toCatch.getMessage()) + "\"!");
    }
    catch (const XN::DOMException &toCatch) {
      std::ostringstream msg;

      msg << "Handler::load: Error while parsing XML file: "
             "DOMException message is \"" << toCatch.msg << "\" with code " << toCatch.code;
      const unsigned int maxChars = 2047;
      XMLCh errText[maxChars + 1];
      if (XN::DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
        msg << " \"" << errText << "\"";
      throw std::runtime_error(msg.str().c_str());
    }
    catch (...) {
      throw std::runtime_error("Handler::load: Unexpected exception while parsing XML!");
    }
  }

  void Handler::save(XN::XMLFormatTarget *myFormTarget) const {
#if XERCES_VERSION_MAJOR == 2
    ScopedXMLPtr<XN::DOMWriter> domWriter
      (getDocument()->getImplementation()->createDOMWriter());
    domWriter->setFeature(XN::XMLUni::fgDOMWRTFormatPrettyPrint, true);
    domWriter->writeNode(myFormTarget, *getDocument());
#elif XERCES_VERSION_MAJOR >= 3
    // get a serializer, an instance of DOMLSSerializer
    ScopedXMLPtr<XN::DOMLSSerializer> theSerializer
      (getDocument()->getImplementation()->createLSSerializer());
    ScopedXMLPtr<XN::DOMLSOutput>     theOutputDesc
      (getDocument()->getImplementation()->createLSOutput());
    
    // set user specified output encoding
    theOutputDesc->setEncoding(XMLCH("UTF-8"));
    
    bool gSplitCdataSections = true;
    bool gDiscardDefaultContent = true;
    bool gFormatPrettyPrint = true;
    bool gWriteBOM = true;
    
    // set feature if the serializer supports the feature/mode
    XN::DOMConfiguration *serializerConfig = theSerializer->getDomConfig();
    if (serializerConfig->canSetParameter(XN::XMLUni::fgDOMWRTSplitCdataSections, gSplitCdataSections))
        serializerConfig->setParameter(XN::XMLUni::fgDOMWRTSplitCdataSections, gSplitCdataSections);
    
    if (serializerConfig->canSetParameter(XN::XMLUni::fgDOMWRTDiscardDefaultContent, gDiscardDefaultContent))
        serializerConfig->setParameter(XN::XMLUni::fgDOMWRTDiscardDefaultContent, gDiscardDefaultContent);
    
    if (serializerConfig->canSetParameter(XN::XMLUni::fgDOMWRTFormatPrettyPrint, gFormatPrettyPrint))
        serializerConfig->setParameter(XN::XMLUni::fgDOMWRTFormatPrettyPrint, gFormatPrettyPrint);
    
    if (serializerConfig->canSetParameter(XN::XMLUni::fgDOMWRTBOM, gWriteBOM))
        serializerConfig->setParameter(XN::XMLUni::fgDOMWRTBOM, gWriteBOM);
    
    theOutputDesc->setByteStream(myFormTarget);
    
    // get the DOM representation
    theSerializer->write(getDocument(), theOutputDesc.get());
#endif // XERCES_VERSION_MAJOR >= 3
  }

} } } // namespace CoSupport::XML::Xerces
