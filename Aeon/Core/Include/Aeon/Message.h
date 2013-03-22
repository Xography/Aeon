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
#ifndef Message_h__
#define Message_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Attribute.h>
#include <Aeon/AttributeContainer.h>
#include <Aeon/Export.h>

namespace Aeon {
	/*
	 * TypeID Code:
	 * 
	 * Copyright 2010 Mohammad Abdulfatah. All rights reserved.
	 * 
	 * Licensed under the MIT license:
	 *
	 *     http://www.opensource.org/licenses/mit-license.php
	 *
	 */
	AEON_EXPORT uint32_t GenerateMessageID(const char* typeName);

	/**
	 * @brief Message runtime type information template.
	 */
	template<typename T> class MessageType
	{
	public:
		static uint32_t id()
		{
	#if defined (_MSC_VER)
			return GenerateMessageID(__FUNCTION__);
	#elif defined(__GNUC__)
			return GenerateMessageID(__PRETTY_FUNCTION__);
	#else
			return GenerateMessageID(typeid(T).name());
	#endif
		}
	private:
		MessageType();        
	};

	// Macro used to declare new messages
#define BEGIN_DECLARE_MESSAGE(typename_) class typename_ : public Aeon::Message { \
	public: typename_(uint32_t type) : Message(type) {} \
	public: void Initialise() { \


#define BEGIN_DECLARE_MESSAGE_EXTERNAL(typename_, _EXPORT) class _EXPORT typename_ : public Aeon::Message { \
public: typename_(uint32_t type) : Message(type) {} \
public: void Initialise() { \

#define END_DECLARE_MESSAGE() }}; \

#define DECLARE_ATTRIBUTE(type_, name_) AddAttribute<type_>(ATTR(name_)); \

#define TYPE_MATCH(msg_,_type_) msg_->GetType() == Aeon::MessageType<type_>::id()

	/**
	 * @brief Message base class.
	 */
	class AEON_EXPORT Message : public AttributeContainer
	{
	public:
		/**
		 * Constructor.
		 */
		Message(uint32_t type);

		/**
		 * Destructor.
		 */
		virtual ~Message()
		{

		}

	public:
		/**
		 * Get the type of this message.
		 */
		uint32_t GetType()
		{
			return type;
		}

	public:
		/**
		 * Creates a new Message of the given Type T (whereas T has to derive from this Message class).
		 */
		template<typename T> static T* Create()
		{
			return new T(MessageType<T>::id());
		}

		/**
		 * Destroy the given message.
		 */
		static void Destroy(Message* ptr);


	public:
		void Initialise() {}
		void AttributeUpdated(const AttributeKey& key) {}

	protected:
		/**
		 * The message type.
		 */
		uint32_t type;
	};
}

#endif // Message_h__
