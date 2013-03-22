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
#include <Aeon/Subsystem/ISubsystem.h>

namespace Aeon
{



	// -------------------------------------------------------------------------
	ISubsystem::ISubsystem() : subsystemRTTI("Subsystem")
	{

	}

	// -------------------------------------------------------------------------
	ISubsystem::~ISubsystem()
	{

	}

	// -------------------------------------------------------------------------
	void ISubsystem::SetSubsystemName(const String& rtti)
	{
		subsystemRTTI.Set(rtti);
	}

	// -------------------------------------------------------------------------
	const SubsystemRTTI& ISubsystem::GetSubsystemName() const
	{
		return subsystemRTTI;
	}


	// -------------------------------------------------------------------------
	SubsystemRTTI::SubsystemRTTI(const String& name) : rtti(name)
	{

	}

	// -------------------------------------------------------------------------
	bool SubsystemRTTI::operator==(const SubsystemRTTI& rhs) const
	{
		return rtti.compare(rhs.rtti) == 0;
	}

	// -------------------------------------------------------------------------
	bool SubsystemRTTI::operator==(const String& rhs) const
	{
		return rtti.compare(rhs) == 0;
	}

	// -------------------------------------------------------------------------
	const String& SubsystemRTTI::Classname() const
	{
		return rtti;
	}

	// -------------------------------------------------------------------------
	void SubsystemRTTI::Set(const String& name)
	{
		rtti = name;
	}

}

