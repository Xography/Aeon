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
#ifndef SAXHandlerAdapter_h__
#define SAXHandlerAdapter_h__

#include <Aeon/XML/SAXHandlerInterface.h>
#include <Aeon/XML/Export.h>

namespace Aeon { 
	namespace XML {
	/**
	 * Adapter for the SAXHandler Interface, every function is implemented
	 * as an empty method.
	 * This adapter comes in handy if you don't want to fill your implementation
	 * with empty function definitions.
	 */
	class AEON_XML_EXPORT SAXHandlerAdapter : public SAXHandlerInterface {
	public:
		/**
		 * Overrides for SAXHandlerInterface.
		 *
		 * @see SAXHandlerInterface
		 */
		virtual void _StartElement(const XML_Char* /*name*/, const XML_Char** /*atts*/) { }
		virtual void _EndElement(const XML_Char* /*name*/) { }
		virtual void _CharacterData(const XML_Char* /*s*/, int /*len*/) { }
		virtual void _ProcessingInstruction(const XML_Char* /*target*/, const XML_Char* /*data*/) { }
		virtual void _CommentData(const XML_Char* /*data*/) { }
		virtual void _DefaultHandler(const XML_Char* /*s*/, int /*len*/) { }
		virtual void _CDataStart() { }
		virtual void _CDataEnd() { }
		virtual void _DoctypeDeclStart(const XML_Char* /*doctypeName*/, const XML_Char* /*systemId*/, const XML_Char* /*publicId*/, int /*hasInternalSubset*/) { }
		virtual void _DoctypeDeclEnd() { }
		virtual void _NotationDecl(const XML_Char* /*notationName*/, const XML_Char* /*base*/, const XML_Char* /*systemId*/, const XML_Char* /*publicId*/) { }
		virtual void _StartNamespaceDecl(const XML_Char* /*prefix*/, const XML_Char* /*uri*/) { }
		virtual void _EndNamespaceDecl(const XML_Char* /*prefix*/) { }
		virtual void _EntityDecl(const XML_Char* /*entityName*/, int /*isParamEntity*/, const XML_Char* /*value*/, int /*valueLength*/, const XML_Char* /*base*/, const XML_Char* /*systemId*/, const XML_Char* /*publicId*/, const XML_Char* /*notationName*/) { }
		virtual void _SkippedEntity(const XML_Char* /*entityName*/, int /*isParameterEntity*/) { }
		virtual void _UnparsedEntityDecl(const XML_Char* /*entityName*/, const XML_Char* /*base*/, const XML_Char* /*systemId*/, const XML_Char* /*publicId*/, const XML_Char* /*notationName*/) { }
	};
}}

#endif // SAXHandlerAdapter_h__
