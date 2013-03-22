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
#ifndef NullInput_h__
#define NullInput_h__

#include <Aeon/Subsystem/IInput.h>

namespace Aeon {
	/**
	 * @brief Null implementation of the input subsystem.
	 */
	class AEON_SUBSYSTEM_EXPORT NullInput : public IInput
	{
	public:
		DECLARE_SUBSYSTEM(NullInput,"Null Input")

	public:
		/**
		 * Overrides for ISubsystem functions.
		 *
		 * @see ISubsystem
		 */
		uint8_t GetCurrentModifiers() const override { return 0; };
		bool IsKeyDown(Aeon::Key key) const override { return false; };

		void Initialise(CmdArgs& /*args*/) override { }
		void Shutdown() override { }
		void Update() override { }

	public:
		/**
		 * Overrides for IMessageHandler.
		 *
		 * @see IMessageHandler
		 */
		void OnMessage(Aeon::Message* message) override { };
	};
}

#endif // NullInput_h__
