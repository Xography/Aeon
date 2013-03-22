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
#ifndef IUserInterface_h__
#define IUserInterface_h__

#include <Aeon/Subsystem/ISubsystem.h>

namespace Aeon {
	/**
	 *  @brief Provides an interface for 2d user interface subsystems.
	 */
	class AEON_SUBSYSTEM_EXPORT IUserInterface : public ISubsystem
	{
	public:
		/**
		 * Returns the subsystem type.
		 *
		 * @see ISubsystem
		 */
		SubsystemType GetSubsystemType() const override
		{
			return SUBSYS_USERINTERFACE;
		}

	public:
		/**
		 * Updates the user interface.
		 */
		virtual void Update() = 0;

		/**
		 * Renders the user interface.
		 */
		virtual void Render() = 0;

		/**
		 * Callback used to informs the subsystem that the window has been resized to the specified dimensions. This is useful for widgets that depend on screen size.
		 */
		virtual void InformWindowResized(int /*w*/, int /*h*/) {};
	};
}

#endif // IUserInterface_h__
