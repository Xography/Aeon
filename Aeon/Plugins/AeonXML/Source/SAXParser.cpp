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
#include <Aeon/XML/SAXParser.h>
#include <string.h>
#include "expat.h"
#include <Aeon/XML/SAXHandlerInterface.h>

namespace Aeon { namespace XML {
	
	/**
		* The XML parser pimpl.
		*/
	class Parser
	{
	public:
		XML_Parser expatParser;

		/**
		 * The XML error that occurred, if any.
		 */
		XML_Error error;
		
		/**
		 * The XML status.
		 */
		XML_Status status;
	};
	
	// ------------------------------------------------------------------------
	SAXParser::SAXParser(SAXHandlerInterface* handler, size_t chunkSize /* = EXPAT_CHUNK_SIZE */):
		handler(handler),
		parser(new Parser()),
		bufsize(chunkSize),
		ready(false),
		failure(false),
		file(0),
		fname("")
	{
		Bootstrap();
	}

	// ------------------------------------------------------------------------
	SAXParser::~SAXParser()
	{
		ready = false;
		parser = 0;

		delete buffer;

		buffer = 0;

		if(file)
		{
			fclose(file);
			file = 0;
		}

		handler = 0;
	}

	// ------------------------------------------------------------------------
	unsigned int SAXParser::XmlError()
	{
		return parser->error;
	}

	// ------------------------------------------------------------------------
	std::string SAXParser::GetXmlErrorString()
	{
		return XML_ErrorString(parser->error);
	}

	// ------------------------------------------------------------------------
	unsigned int SAXParser::XmlStatus()
	{
		return parser->status;
	}

	// ------------------------------------------------------------------------
	bool SAXParser::IsReady()
	{
		return ready;
	}

	// ------------------------------------------------------------------------
	bool SAXParser::Parse(const std::string& filename)
	{
		Bootstrap();

		// Open file
		fname = filename;
		file = fopen(fname.c_str(), "r");

		if(file == nullptr) failure = true;

		ssize_t bytes_read;

		// Ensure that the parser is ready
		if(failure)
			throw SAXParserException("Failed to open file: "+fname);

		if(!ready)
			throw SAXParserException("Parser not ready.");

		// Loop, reading the XML source block by block 
		while((bytes_read = Readblock()) >= 0)
		{
			if(bytes_read > 0)
			{
				XML_Status status = XML_Parse(parser->expatParser, buffer, bytes_read,
					XML_FALSE);

				// In case of Error
				if(status != XML_STATUS_OK)
				{
					status = status;
					parser->error = XML_GetErrorCode(parser->expatParser);
					fclose(file);
					ready = false;

					throw SAXParserException(fname + ": " +
						GetXmlErrorString());
				}
			}

			// Break on successful "short read", in event of EOF
			if(parser->error == XML_ERROR_FINISHED)
				break;
		}

		// Finalize the parser
		if((parser->status == XML_STATUS_OK) || (parser->error == XML_ERROR_FINISHED))
		{
			XML_Parse(parser->expatParser, buffer, 0, XML_TRUE);
			XML_ParserFree(parser->expatParser);
			return true;
		}

		XML_ParserFree(parser->expatParser);

		// Return false in case of error
		return false;
	}
	// ------------------------------------------------------------------------
	void SAXParser::ParseString(const std::string& str)
	{
		Bootstrap();

		// Ensure that the parser is ready
		if(failure)
			throw SAXParserException("Failed to open file: "+fname);

		ssize_t _strlen = str.length();

		if(!ready)
			throw SAXParserException("Parser not ready.");

		// Loop, reading the XML source block by block 
		if(_strlen > 0)
		{
			XML_Status status = XML_Parse(parser->expatParser, str.c_str(), _strlen,
				XML_FALSE);

			// In case of Error
			if(parser->status != XML_STATUS_OK)
			{
				parser->status = parser->status;
				parser->error = XML_GetErrorCode(parser->expatParser);
				ready = false;

				throw SAXParserException(fname + ": " + GetXmlErrorString());
			}
		}

		// Finalize the parser
		if((parser->status == XML_STATUS_OK) || (parser->error == XML_ERROR_FINISHED))
		{
			XML_Parse(parser->expatParser, buffer, 0, XML_TRUE);
			XML_ParserFree(parser->expatParser);
		}
		
		else
			XML_ParserFree(parser->expatParser);
	}

	// ------------------------------------------------------------------------
	void SAXParser::Bootstrap()
	{
		// Allocate buffer
		buffer = new XML_Char[bufsize];
		memset(buffer, 0, bufsize * sizeof(XML_Char));

		// Create Parser
		parser->expatParser = XML_ParserCreate(nullptr);

		if(parser->expatParser && buffer)
		{
			ready = true;

			// Register Handlers
			XML_SetUserData(parser->expatParser, this);
			XML_SetElementHandler(parser->expatParser, &SAXParser::HandleElementStart, SAXParser::HandleElementEnd);
			XML_SetCharacterDataHandler(parser->expatParser, &SAXParser::HandleCharacterData);
			XML_SetProcessingInstructionHandler(parser->expatParser, &SAXParser::HandleProcessingInstruction);
			XML_SetCommentHandler(parser->expatParser, &SAXParser::HandleComments);
			XML_SetCdataSectionHandler(parser->expatParser, &SAXParser::HandleCDATAStart, &SAXParser::HandleCDATAEnd);
			XML_SetDoctypeDeclHandler(parser->expatParser, SAXParser::HandleDoctypeDeclStart, &SAXParser::HandleDoctypeDeclEnd);
			XML_SetUnparsedEntityDeclHandler(parser->expatParser, &SAXParser::HandleUnparsedEntityDecl);
			XML_SetNotationDeclHandler(parser->expatParser, &SAXParser::HandleNotationDecl);
			XML_SetDoctypeDeclHandler(parser->expatParser, &SAXParser::HandleDoctypeDeclStart, &SAXParser::HandleDoctypeDeclEnd);
			XML_SetEntityDeclHandler(parser->expatParser, &SAXParser::HandleEntityDecl);
			XML_SetSkippedEntityHandler(parser->expatParser, &SAXParser::HandleSkippedEntity);

			XML_SetDefaultHandler(parser->expatParser, &SAXParser::HandleDefault);
		}
		
		else
		{
			// If anything goes wrong, we 'silently' fail and register the error.
			delete buffer;
			buffer = 0;
			failure = true;
			ready = false;
		}
	}

	// ------------------------------------------------------------------------
	ssize_t SAXParser::Readblock()
	{
		if(file == nullptr)
			return -1;

		size_t _s = fread(buffer, sizeof(buffer)[0], bufsize, file);
		ssize_t code = static_cast<ssize_t>(_s);

		if(_s < bufsize)
		{
			// If we're at the end of the file
			if(feof(file))
			{
				parser->error = XML_ERROR_FINISHED;
				return _s;
			}

			// If there was an error reading the chunk
			if(ferror(file))
			{
				long s = _s;
				parser->status = XML_STATUS_ERROR;
				parser->error = XML_ERROR_NO_ELEMENTS;
				fseek(file, -s, SEEK_CUR);
			}
		}

		// If we've finished reading the file.
		if(_s == 0)
		{
			parser->status = XML_STATUS_OK;
			parser->error = XML_ERROR_FINISHED;
			code = -1;
		}

		return code;
	}

	// ------------------------------------------------------------------------
	// Static handlers, very repetitive stuff ahead.
	// ------------------------------------------------------------------------
	void SAXParser::HandleElementStart(void* userData, const XML_Char* name, const XML_Char** atts)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_StartElement(name, atts);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleElementEnd(void* userData, const XML_Char* name)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_EndElement(name);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleCharacterData(void* userData, const XML_Char* s, int len)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_CharacterData(s, len);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleProcessingInstruction(void* userData, const XML_Char* target, const XML_Char* data)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_ProcessingInstruction(target, data);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleComments(void* userData, const XML_Char* data)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_CommentData(data);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleDefault(void* userData, const XML_Char* s, int len)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_DefaultHandler(s, len);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleCDATAStart(void* userData)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_CDataStart();
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleCDATAEnd(void* userData)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_CDataEnd();
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleDoctypeDeclStart(void* userData, const XML_Char* doctypeName, const XML_Char* systemId, const XML_Char* publicId, int hasInternalSubset)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_DoctypeDeclStart(doctypeName, systemId, publicId, hasInternalSubset);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleDoctypeDeclEnd(void* userData)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_DoctypeDeclEnd();
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleNotationDecl(void* userData, const XML_Char* notationName, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_NotationDecl(notationName, base, systemId, publicId);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleStartNamespaceDecl(void* userData, const XML_Char* prefix, const XML_Char* uri)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_StartNamespaceDecl(prefix, uri);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleEndNamespaceDecl(void* userData, const XML_Char* prefix)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_EndNamespaceDecl(prefix);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleEntityDecl(void* userData, const XML_Char* entityName, int isParamEntity, const XML_Char* value, int valueLength, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId, const XML_Char* notationName)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_EntityDecl(entityName, isParamEntity, value, valueLength, base, systemId, publicId, notationName);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleSkippedEntity(void* userData, const XML_Char* entityName, int isParameterEntity)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_SkippedEntity(entityName, isParameterEntity);
	}

	// ------------------------------------------------------------------------
	void SAXParser::HandleUnparsedEntityDecl(void* userData, const XML_Char* entityName, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId, const XML_Char* notationName)
	{
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if(self->handler)
			self->handler->_UnparsedEntityDecl(entityName, base, systemId, publicId, notationName);
	}
}}
