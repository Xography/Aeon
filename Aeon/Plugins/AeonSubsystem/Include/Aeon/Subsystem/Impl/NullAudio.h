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
#ifndef NullAudio_h__
#define NullAudio_h__

#include <Aeon/Logging/Logging.h>

#include <Aeon/Subsystem/IAudio.h>

#ifdef _MSC_VER
#	pragma warning (disable : 4250) // We are aware what we're getting ourselves into...
#endif // _MSC_VER

namespace Aeon {
	/**
	 *  @brief Null implementation of the audio subsystem.
	 */
	class AEON_SUBSYSTEM_EXPORT NullAudio : public IAudio
	{
		DECLARE_SUBSYSTEM(NullAudio,"Null Audio")

	public:
		/**
		 * Overrides for ISubsystem functions.
		 *
		 * @see ISubsystem
		 */
		void Initialise(CmdArgs& args) override;
		void Shutdown() override;

	public:
		/**
		 * Overrides for IAudio functions.
		 *
		 * @see IAudio
		 */
		SoundEffect* CreateSoundEffect(const String& id) override;
		SoundEffect* CreateSoundEffect(uint32_t fastLookupID) override;
		void DestroySoundEffect(SoundEffect* ptr) override;
		AmbientSound* CreateAmbientSound(const String& id) override;
		AmbientSound* CreateAmbientSound(uint32_t fastLookupID) override;
		void DestroyAmbientSound(AmbientSound* ptr) override;
		MusicTrack* CreateMusicTrack(const String& id) override;
		MusicTrack* CreateMusicTrack(uint32_t fastLookupID) override;
		void DestroyMusicTrack(MusicTrack* ptr) override;
		LayeredTrack* CreateLayeredTrack(const String& id) override;
		LayeredTrack* CreateLayeredTrack(uint32_t fastLookupID) override;
		void DestroyLayeredTrack(LayeredTrack* ptr) override;
		void StopAllSounds() override;
		void StopPlayback() override;
		void PausePlayback() override;
		void ResumePlayback() override;
	public:
		void OnMessage(Aeon::Message* message);
	};
}

#endif // NullAudio_h__
