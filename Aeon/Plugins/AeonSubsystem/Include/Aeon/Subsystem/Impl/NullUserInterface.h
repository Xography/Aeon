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
#ifndef NullUserInterface_h__
#define NullUserInterface_h__

#include <Aeon/Subsystem/IUserInterface.h>

namespace Aeon {
	/**
	 *  @brief Null implementation of the UI subsystem.
	 */
	class AEON_SUBSYSTEM_EXPORT NullUserInterface : public IUserInterface
	{
		DECLARE_SUBSYSTEM(NullUserInterface,"Null User Interface")

	public:
		/**
		 * Overrides for ISubsystem functions.
		 *
		 * @see ISubsystem
		 */
		void Initialise(CmdArgs& /*args*/) override {}
		void Shutdown() override {}
		
	public:
		/**
		 * Overrides for IUserInterface functions.
		 *
		 * @see IUserInterface
		 */
		void Update() override {}
		void Render() override {}

	public:
		virtual void OnMessage(Aeon::Message* message) { }
	};
}

#endif // NullAudio_h__
