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
#ifndef NullRenderer_h__
#define NullRenderer_h__

#include <Aeon/Subsystem/IRenderer.h>

namespace Aeon {
	/**
	 *  @brief Null implementation of the renderer subsystem.
	 */
	class AEON_SUBSYSTEM_EXPORT NullRenderer : public IRenderer
	{
		DECLARE_SUBSYSTEM(NullRenderer,"Null Renderer")

	public:
		/**
		 * Returns the time passed since the last frame was rendered.
		 *
		 * @see IRenderer
		 */
		virtual float GetTimeSinceLastFrame() const override { return 0.0f; }

	public:
		/**
		 * Overrides for ISubsystem functions.
		 *
		 * @see ISubsystem
		 */
		virtual void Initialise(CmdArgs& /*args*/) override { }
		virtual void Shutdown() override { }

	public:
		/**
		 * Overrides for IRenderer functions.
		 *
		 * @see IRenderer
		 */
		virtual void Update() override { }
		virtual void Render() override { }
		virtual void ResumeSceneRendering() override { }
		virtual void PauseSceneRendering() override { }
		virtual bool IsRunning() override { return false; }
	
	public:
		void OnMessage(Aeon::Message* message) override { }

	public:
		
	};
}
#endif // NullRenderer_h__
