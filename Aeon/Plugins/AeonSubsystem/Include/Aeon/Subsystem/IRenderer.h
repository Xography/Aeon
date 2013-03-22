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
#ifndef IRenderer_h__
#define IRenderer_h__

#include <Aeon/Subsystem/ISubsystem.h>

namespace Aeon {
	/**
	 * @brief Rendering subsystem, providing functionality to output data on the screen.
	 */
	class AEON_SUBSYSTEM_EXPORT IRenderer : public ISubsystem
	{
	public:
		/**
		 * Returns the subsystem type.
		 *
		 * @see ISubsystem
		 */
		SubsystemType GetSubsystemType() const override;

		virtual ~IRenderer() {}

	public:
		/**
		 * Returns the time that elapsed since the last frame was rendered.
		 */
		virtual Aeon::Real GetTimeSinceLastFrame() const = 0;

	public:
		/**
		 * ISubsystem overrides.
		 *
		 * @see ISubsystem
		 */
		virtual void Initialise(CmdArgs& args) = 0;
		virtual void Shutdown() = 0;

	public:
		/**
		 * Updates the renderer.
		 */
		virtual void Update() = 0;

		/**
		 * Renders everything required by the application.
		 */
		virtual void Render() = 0; 

		/**
		 * Pauses rendering of scene objects.
		 */
		virtual void PauseSceneRendering() = 0;

		/**
		 * Resumes rendering scene objects.
		 */
		virtual void ResumeSceneRendering() = 0;

		/**
		 * Indicates whether the renderer is still running (or the window has been closed, a quit requested, ..)
		 */
		virtual bool IsRunning() = 0;

	};
}

#endif // IRenderer_h__
