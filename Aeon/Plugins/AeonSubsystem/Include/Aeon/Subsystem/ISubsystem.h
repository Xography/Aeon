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
#ifndef ISubsystem_h__
#define ISubsystem_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/CmdArgs.h>
#include <Aeon/IMessageHandler.h>

#include <Aeon/Subsystem/Export.h>

namespace Aeon {
	/**
	 * Sets up basics, such as factory/RTTI for the subsystem
	 */
	#define DECLARE_SUBSYSTEM(type,name)			\
		public:										\
			type() { SetSubsystemName(name); }		\
			virtual ~type() {}

	enum SubsystemType
	{
		SUBSYS_CUSTOM = 0,			// Custom
		SUBSYS_PFT = 1,				// Platform Tools
		SUBSYS_AUDIO = 2,			// Audio
		SUBSYS_RENDERER = 3,		// Renderer
		SUBSYS_INPUT = 4,			// Input	
		SUBSYS_USERINTERFACE = 5	// User Interface
	};

	struct SubsystemInfo
	{
		const char* subystemName;
		const char* version;
		SubsystemType type;
	};

	/**
	 * Converts the given Subsystem type to a human readable String.
	 */
	AEON_API String GetSubsystemTypeName(SubsystemType type)
	{
		static const Char* const strVal[] =
		{
			"Custom", // SUBSYS_CUSTOM
			"Platform Tools", // SUBSYS_PFT 
			"Logging", // SUBSYS_LOG 
			"Audio", // SUBSYS_AUDIO
			"Renderer", // SUBSYS_RENDERER
			"Input", // SUBSYS_INPUT
			"User Interface" // SUBSYS_USERINTERFACE
		};

		return strVal[type];
	}

	/**
	 *  @brief Wrapper Class used to store subsystem RTTI information.
	 */
	class AEON_SUBSYSTEM_EXPORT SubsystemRTTI 
	{
	public:
		/**
		 * Constructor.
		 */
		SubsystemRTTI(const String& name);
	
	public:
		/**
		 * Returns the class name set via DECLARE_SUBSYSTEM
		 */
		const String& Classname() const;
		
		/**
		 * Set the RTTI (class name).
		 */
		void Set(const String& name);
	
	public:
		bool operator ==(const SubsystemRTTI& rhs) const;
		bool operator ==(const String& rhs) const;

	private:
		/**
		 * RTTI information.
		 */
		String rtti;
	};

	/**
	 * @brief Generic Subsystem interface.
	 */
	class AEON_SUBSYSTEM_EXPORT ISubsystem : public IMessageHandler
	{
	public:
		/**
		 * Constructor.
		 */
		ISubsystem();
		
		/**
		 * Destructor.
		 */
		virtual ~ISubsystem();

	protected:
		/**
		 * Sets the subsystem name, internal use.
		 */
		void SetSubsystemName(const String& rtti);
		
	public:
		/**
		 * Called after creating the subsystem instance. Used to set up the system.
		 */
		virtual void Initialise(CmdArgs& args) = 0;

		/**
		 * Requests the subsystem to immediately shut down.
		 */
		virtual void Shutdown() = 0;

		/**
		 * Returns the exact type of the subsystem, or CUSTOM if the subsystem is not a core subsystem.
		 */
		virtual SubsystemType GetSubsystemType() const = 0;

		/**
		 * Returns the rtti about the class, usually its name.
		 */
		virtual const SubsystemRTTI& GetSubsystemName() const;

	protected:
		/**
		 * Runtime type information.
		 */
		SubsystemRTTI subsystemRTTI;
	};
}

#endif // ISubsystem_h__
