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
#ifndef SAXHandlerInterface_h__
#define SAXHandlerInterface_h__

#include <Aeon/XML/AeonXMLConf.h>
#include <Aeon/XML/Export.h>

namespace Aeon { namespace XML {
	/**
	 * Interface, every Handler passed to the Parser is required to implement this Interface - Or any Adapter to it. The method
	 * descriptions are taken from the Expat reference.
	 *
	 * @note Not everything Expat parses is represented here, although the majority of handlers should be available.
	 */
	class AEON_XML_EXPORT SAXHandlerInterface
	{
	public:
		/**
		 * Handler for start (and empty) tags. Attributes are passed as a pointer to a vector of char pointers. Each attribute seen in
		 * a start (or empty) tag occupies 2 consecutive places in this vector: the attribute name followed by the attribute value. 
		 * These pairs are terminated by a null pointer.
		 * 
		 * @note An empty tag generates a call to both start and end handlers (in that order).
		 */
		virtual void _StartElement(const XML_Char* name, const XML_Char** atts) = 0;

		/**
		 * Handler for end (and empty) tags. As noted above, an empty tag generates a call to both start and end handlers.
		 */
		virtual void _EndElement(const XML_Char* name) = 0;

		/**
		 * Text handler. The string is NOT null-terminated. You have to use the length argument to deal with the end of the string.
		 * A single block of contiguous text free of markup may still result in sequence of calls to this handler. In other words,
		 * if you're searching for a pattern in the text, it may be split across calls to this handler. 
		 */
		virtual void _CharacterData(const XML_Char* s, int len) = 0;

		/**
		 * Handler for processing instructions. The target is the first word in the processing instruction. The data is the rest of the
		 * characters in it after skipping all whitespace after the initial word.
		 */
		virtual void _ProcessingInstruction(const XML_Char* target, const XML_Char* data) = 0;

		/**
		 * Handler for comments. The data is all text inside the comment delimiters.
		 */
		virtual void _CommentData(const XML_Char* data) = 0;

		/**
		 * Handler for any characters in the document which wouldn't otherwise be handled. This includes both data for which no handlers can
		 * be set (like some kinds of DTD declarations) and data which could be reported but which currently has no handler set. The characters
		 * are passed exactly as they were present in the XML document except that they will be encoded in UTF-8 or UTF-16. Line boundaries
		 * are not normalized. Note that a byte order mark character is not passed to the default handler. There are no guarantees about how
		 * characters are divided between calls to the default handler: for example, a comment might be split between multiple calls. Setting
		 * the handler with this call has the side effect of turning off expansion of references to internally defined general entities. Instead
		 * these references are passed to the default handler.
		 */
		virtual void _DefaultHandler(const XML_Char* s, int len) = 0;

		/**
		 * This Handler that gets called at the beginning of a CDATA section.
		 */
		virtual void _CDataStart(void) = 0;

		/**
		 * This handler that gets called at the end of a CDATA section.
		 */
		virtual void _CDataEnd(void) = 0;

		/**
		 * This Handler is called at the start of a DOCTYPE declaration, before any external or internal subset is parsed. Both sysid and pubid 
		 * may be NULL. The has_internal_subset will be non-zero if the DOCTYPE declaration has an internal subset.
		 */
		virtual void _DoctypeDeclStart(const XML_Char* doctypeName, const XML_Char* systemId, const XML_Char* publicId, int hasInternalSubset) = 0;

		/**
		 * This Handler is called at the end of a DOCTYPE declaration, after parsing any external subset.
		 */
		virtual void _DoctypeDeclEnd(void) = 0;

		/**
		 * Handler that receives notation declarations.
		 */
		virtual void _NotationDecl(const XML_Char* notationName, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId) = 0;

		/**
		 * Handler to be called when a namespace is declared. Namespace declarations occur inside start tags. But the namespace declaration
		 * start handler is called before the start tag handler for each namespace declared in that start tag.
		 */
		virtual void _StartNamespaceDecl(const XML_Char* prefix, const XML_Char* uri) = 0;

		/**
		 * Handler to be called when leaving the scope of a namespace declaration. This will be called, for each namespace declaration, 
		 * after the handler for the end tag of the element in which the namespace was declared.
		 */
		virtual void _EndNamespaceDecl(const XML_Char* prefix) = 0;

		/**
		 * Handler that will be called for all entity declarations. The is_parameter_entity argument will be non-zero in the case of 
		 * parameter entities and zero otherwise. For internal entities (<!ENTITY foo "bar">), value will be non-NULL and systemId, publicId,
		 * and notationName will all be NULL. The value string is not NULL terminated; the length is provided in the value_length parameter. Do
		 * not use value_length to test for internal entities, since it is legal to have zero-length values. Instead check for whether or not value is NULL.
		 *
		 * @note The notationName argument will have a non-NULL value only for unparsed entity declarations.
		 */
		virtual void _EntityDecl(const XML_Char* entityName, int isParamEntity, const XML_Char* value, int valueLength, const XML_Char* base,
			const XML_Char* systemId, const XML_Char* publicId, const XML_Char* notationName) = 0;
		/**
		 * Handler for skipped entities. This is called in two situations:
		 *
		 * 1. An entity reference is encountered for which no declaration has been read and this is not an error.
		 *
		 * 2. An internal entity reference is read, but not expanded, because XML_SetDefaultHandler has been called.
		 *
		 * The is_parameter_entity argument will be non-zero for a parameter entity and zero for a general entity.
		 *
		 * @note skipped parameter entities in declarations and skipped general entities in attribute values cannot be reported, because the event would be
		 * out of sync with the reporting of the declarations or attribute values
		 */
		virtual void _SkippedEntity(const XML_Char* entityName, int isParameterEntity) = 0;

		/**
		 * Handler that will be called for all entity declarations. The is_parameter_entity argument will be non-zero in the case of parameter entities and zero otherwise.
		 *
		 * For internal entities (<!ENTITY foo "bar">), value will be non-NULL and systemId, publicId, and notationName will all be NULL. The value string is not NULL
		 * terminated; the length is provided in the value_length parameter. Do not use value_length to test for internal entities, since it is legal to have zero-length
		 * values. Instead check for whether or not value is NULL.
		 *
		 * The notationName argument will have a non-NULL value only for unparsed entity declarations.
		 */
		virtual void _UnparsedEntityDecl(const XML_Char* entityName, const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId, const XML_Char* notationName) = 0;
	};
}}

#endif // SAXHandlerInterface_h__
