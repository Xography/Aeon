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
#include <algorithm>

#include <Aeon/XML/SAXHandler.h>

namespace Aeon { namespace XML {
	// ------------------------------------------------------------------------
	void SAXHandler::_StartElement(const XML_Char* name, const XML_Char** atts)
	{
		attributes.Clear();

		for (int i = 0; *atts; ++i)
		{
			const XMLString& key =   (*atts++);
			const XMLString& value = (*atts++);

			attributes.Insert(key, value);
		}

		StartElement(XMLString(name), attributes);
	}

	// ------------------------------------------------------------------------
	void SAXHandler::_EndElement(const XML_Char* name)
	{
		EndElement(XMLString(name));
	}

	// ------------------------------------------------------------------------
	void SAXHandler::_CharacterData(const XML_Char* s, int len)
	{
		XMLString data(s, len);
		CharacterData(data);
	}

	// ------------------------------------------------------------------------
	void SAXHandler::_ProcessingInstruction(const XML_Char* target, const XML_Char* data)
	{
		ProcessingInstruction(XMLString(target), XMLString(data));
	}

	// ------------------------------------------------------------------------
	void SAXHandler::_CommentData(const XML_Char* data)
	{
		CommentData(XMLString(data));
	}

	// ------------------------------------------------------------------------
	void SAXHandler::_CDataStart(void)
	{
		StartCData();
	}

	// ---------------------------------------------------------------------------
	void SAXHandler::_CDataEnd(void)
	{
		EndCData();
	}
}}
