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
#ifndef XMLAttributes_h__
#define XMLAttributes_h__

#include <vector>

#include <Aeon/XML/AeonXMLFwd.h>
#include <Aeon/XML/XMLString.h>
#include <Aeon/XML/Export.h>

namespace Aeon { namespace XML {
	/**
	 * A very basic XML Attribute representation, consisting of a key and a value.
	 */
	struct Attribute
	{
		XMLString key;
		XMLString value;
	};

	typedef std::vector<Attribute> AttributesVector;

	/**
	 * Very basic and quick implementation of a supporting XMLAttributes class helping to (temporarily) store XML Element Attributes in a vector.
	 */
	class AEON_XML_EXPORT XMLAttributes
	{
	public:
		/**
		 * Retrieves the value of the attribute at the specified position.
		 */
		const XMLString& Get(const int pos) const
		{
			return vec.at(pos).value;
		}

		/**
		 * Retrieves the amount of attributes.
		 */
		int Size() const
		{
			return vec.size();
		}

		/**
		 * Retrieves a vector containing all the attributes.
		 */
		const AttributesVector& Vector() const
		{
			return vec;
		}

	public:
		/**
		 * Inserts a new Attribute Pair (Key and Value) into the Vector.
		 *
		 * @param key the key of the attribute.
		 * @param value the value of the attribute.
		 */
		void Insert(const XMLString& key, const XMLString& value)
		{
			Attribute _a;
			_a.key = key;
			_a.value = value;
			vec.push_back(_a);
		}

		/**
		 * Look up a key of the attribute list, if the key is found, the value will be returned, an empty XMLString otherwise.
		 *
		 * @param key the key of the attribute.
		 * @return the value of the attribute.
		 */
		const XMLString& Get(const XMLString& key) const
		{
			long pos = find(key);

			if(pos != -1)	return vec.at(pos).value;
			else			{ static XMLString empty; return empty; }
		}

		/**
		* Clears the attribute list.
		*/
		void Clear()
		{
			vec.clear();
		}

	private:
		// Used to find an element in the  list, return -1 if element is not found
		long find(const XMLString& key) const
		{
			long i = 0;

			for (AttributesVector::const_iterator it = vec.begin(); it != vec.end(); ++it)
			{
				if(it->key == key)
					return i;

				++i;
			}

			return -1;
		}

	private:
		/**
		 * Vector containing all the XML attributes.
		 */
		AttributesVector vec;
	};
}}

#endif // XMLAttributes_h__
