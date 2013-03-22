/*******************************************************************************
 *
 * This file is part of the AEON Framework.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 * 
 * See LICENSE.txt for licensing details.
 * 
 ******************************************************************************/
#ifndef SAXParser_h__
#define SAXParser_h__

#include <string>

#include <Aeon/Platform/Platform.h>

#include <Aeon/XML/AeonXMLFwd.h>
#include <Aeon/XML/AeonXMLConf.h>
#include <Aeon/XML/Export.h>

namespace Aeon {
	namespace XML {
	#ifdef AEON_WINDOWS
		typedef int ssize_t; // Do this here since other libraries may also define ssize_t.
	#endif

	typedef char XML_Char;
	class Parser;

	/**
	 * Quick exception class.
	 *
	 * @note I'm not using the Framework's Exception class to enable stand-alone usage of the XML Parser wrapper.
	 */
	class AEON_XML_EXPORT SAXParserException : public std::exception
	{
	public:
		/**
		 * Constructor which accepts own error descriptions.
		 */
		explicit SAXParserException(const std::string& desc):
			description(desc)
		{

		}

		/**
		 * Destructor.
		 */
		virtual ~SAXParserException() throw()
		{

		}

		/**
		 * Overriding the std::exception what.
		 */
		const char* what() const throw() override
		{
			return description.c_str();
		}

	private:
		/**
		 * A description of the exception.
		 */
		std::string description;
	};

	/**
	 * The actual SAX parser implementation.
	 */
	class AEON_XML_EXPORT SAXParser
	{
	public:
		/**
		 * Creates a new Parser, sets up all required Handlers via bootstrapping the System with a user-defined chunk size; the chunk
		 * size is the buffer used while stream-reading the file.
		 *
		 * @param handler The SAX Event Handler class, must implement the  SAXHandlerInterface or any Adapter.
		 * @param chunkSize Buffer size to use while reading parsing the filestream.
		 */
		SAXParser(Aeon::XML::SAXHandlerInterface* handler, size_t chunkSize = EXPAT_CHUNK_SIZE);

		/**
		 * Destructor.
		 */
		virtual ~SAXParser();

	public:
		/**
		* Get the current expat error code, XML_ERROR. Refer to the expat manual for interpretation of the specific codes.
		* 
		* @return The error code.
		*/
		virtual unsigned int XmlError();

		/**
		* The parser's current status, should be XML_STATUS_OK while parsing, XML_STATUS_ERROR means there is something quite wrong.
		*
		* @return The parser's current status.
		*/
		virtual unsigned int XmlStatus();

		/**
		 * Check whether the parser is ready to start parsing, useful for checking if there was anything going wrong during initialization.
		 * 
		 * @return whether the parser is ready to start parsing.
		 */
		virtual bool IsReady();

		/**
		 * Returns the Last error which occurred with the accompanying description taken from the expat library.
		 *
		 * @return description of the last error.
		 */
		virtual std::string GetXmlErrorString();

	public:
		/**
		 * Parses the given file. Throws SAXParserException if anything crucial goes wrong, so should surround the statement with try catch.
		 *
		 * @return false if anything goes wrong, true if the end of the file was reached without any errors.
		 */
		virtual bool Parse(const std::string& filename);
		virtual void ParseString(const std::string& str);

	private:
		/**
		 * Used to initalized the parser.
		 */
		void Bootstrap();

		/**
		 * Reads a block from _file.
		 */
		ssize_t Readblock();

	private:
		/**
		 * XML elements and attributes.
		 */
		static void HandleElementStart(void* userData, const XML_Char* name, const XML_Char** atts);
		static void HandleElementEnd(void* userData, const XML_Char* name);

		/**
		 * Character data.
		 */
		static void HandleCharacterData(void* userData, const XML_Char* s, int len);

		/**
		 * Processing instructions
		 */
		static void HandleProcessingInstruction(void* userData, const XML_Char* target, const XML_Char* data);

		/**
		 * Comments.
		 */
		static void HandleComments(void* userData, const XML_Char* data);

		/**
		 * Default handler.
		 */
		static void HandleDefault(void* userData, const XML_Char* s, int len);

		/**
		 * CDATA
		 */
		static void HandleCDATAStart(void* userData);
		static void HandleCDATAEnd(void* userData);

		/**
		 * DOCTYPE
		 */
		static void HandleDoctypeDeclStart(void* userData, const XML_Char* doctypeName, const XML_Char* systemId, const XML_Char* publicId, int hasInternalSubset);
		static void HandleDoctypeDeclEnd(void* userData);
		
		/**
		 * Notations.
		 */
		static void HandleNotationDecl(void* userData, const XML_Char* notationName, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId);

		/**
		 * Namespaces.
		 */
		static void HandleStartNamespaceDecl(void* userData, const XML_Char* prefix, const XML_Char* uri);
		static void HandleEndNamespaceDecl(void* userData, const XML_Char* prefix);

		/**
		 * Entity handlers.
		 */
		static void HandleEntityDecl(void* userData, const XML_Char* entityName, int isParamEntity, const XML_Char* value, int valueLength, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId, const XML_Char* notationName);
		static void HandleSkippedEntity(void* userData, const XML_Char* entityName, int isParameterEntity);
		static void HandleUnparsedEntityDecl(void* userData, const XML_Char* entityName, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId, const XML_Char* notationName);

	private:
		/**
		 * A pointer to the sax handler used.
		 */
		SAXHandlerInterface* handler;
		
		Parser* parser;


		/**
		 * Just a plain old buffer used during reading.
		 */
		XML_Char* buffer;

		/**
		 * Keeps track of buffer sizes.
		 */
		size_t bufsize;

		/**
		 * Indicates if the parser is ready.
		 */
		bool ready;

		/**
		 * Keeps track of any failures during initalization.
		 */ 
		bool failure;

		/**
		 * File used during read operations.
		 */
		FILE* file;

		/**
		 * The file to read.
		 */
		std::string fname;
	};
}}

#endif // SAXParser_h__
