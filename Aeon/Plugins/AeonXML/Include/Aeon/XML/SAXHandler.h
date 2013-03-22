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
#ifndef SAXHandler_h__
#define SAXHandler_h__

#include <Aeon/XML/XMLString.h>
#include <Aeon/XML/XMLAttributes.h>
#include <Aeon/XML/SAXHandlerAdapter.h>
#include <Aeon/XML/Export.h>

namespace Aeon { 
	namespace XML {
	/** 
	 * An abstract class, making example usage of the SAXHandlerInterface (and the SAXHandlerAdapter). This implementation wraps the Char Arrays nicely
	 * into (std::)Strings (called XMLString) and Attributes into Attribute-Lists (std::vector based). This comes at the cost of speed, of course, but
	 * saves the user from implementing an own Handler using the Raw data.
	 *
	 * @note This Handler is non-validating, it is only able to read XML Elements and their attributes, CharacterData (Between tags), comments and CData. 
	 * @warning UTF-8 is supported, whereas UTF-16 support would require a re-implementation of the XMLString class to use std::wstring!
	 */
	class AEON_XML_EXPORT SAXHandler : public SAXHandlerAdapter
	{
	public:
		// Receivers to wrap the data into a more convenient format.
		void _StartElement(const XML_Char* name, const XML_Char** atts);
		void _EndElement(const XML_Char* name);
		void _CharacterData(const XML_Char* s, int len);
		void _ProcessingInstruction(const XML_Char* target, const XML_Char* data);
		void _CommentData(const XML_Char* data);
		void _CDataStart();
		void _CDataEnd();

		/**
		 * Start of an element tag.
		 *
		 * @param name the name of the tag.
		 * @param attributes the attributes, accessible via .get(<Name>)
		 */
		virtual void StartElement(const XMLString& name, const XMLAttributes& attributes) = 0;

		/**
		 * End of an element tag.
		 *
		 * @param name the name of the element.
		 */
		virtual void EndElement(const XMLString& name) = 0;

		/**
		 * CharacterData, usually text between XML tags.
		 *
		 * @param data the data, still contains whitespaces.
		 */
		virtual void CharacterData(const XMLString& data) = 0;

		/**
		* Processing Instructions.
		*
		* @param target the instructions' target.
		* @param data the instruction data.
		*/
		virtual void ProcessingInstruction(const XMLString& /*target*/, const XMLString& /*data*/) { }

		/**
		 * A XML Comment.
		 *
		 * @param data the comment's contents
		 */
		virtual void CommentData(const XMLString& /*data*/) { }

		/**
		 * Marks the start of a CDATA Tag.
		 */
		virtual void StartCData() { }

		/**
		 * Marks the end of a CDATA Tag.
		 */
		virtual void EndCData() { }

	private:
		/**
		 * The actual XML attributes.
		 */
		XMLAttributes attributes;
	};
}}

#endif // SAXHandler_h__
