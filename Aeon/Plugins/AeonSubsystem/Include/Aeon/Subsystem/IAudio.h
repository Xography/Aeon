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
#ifndef IAudio_h__
#define IAudio_h__
#pragma once

#include <Aeon/Subsystem/ISubsystem.h>
#include <Aeon/AeonTypes.h>
#include <Aeon/Types/Vector3.h>

namespace Aeon {
	/**
	 * @brief Audio file interface, reepresenting a set of functions common to audio files.
	 **/
	class AEON_SUBSYSTEM_EXPORT IAudioFile
	{
	public:
		/**
		 * Destructor.
		 */
		virtual ~IAudioFile()
		{
		}

	public:
		/**
		 * Plays the audio file.
		 */
		virtual void Play() = 0;
		
		/**
		 * Stops playback of the audio file.
		 */
		virtual void Stop() = 0;
		
		/**
		 * Get the currently set volume of the audio file (default: 100)
		 * @return the volume
		 */
		virtual uint8_t GetVolume() = 0;
		
		/**
		 * Set the volume of the audio file (default: 100)
		 * @param volume the volume
		 */
		virtual void SetVolume(uint8_t volume) = 0;

		/**
		 * Update the audio file (in msec)
		 * @param msec the amount of milliseconds passed since last frame.
		 */
		virtual void Update(uint32_t msec) = 0;

	};

	/**
	 * @brief A sound effect, either in 2D or 3D space.
	 */
	class AEON_SUBSYSTEM_EXPORT SoundEffect : public virtual IAudioFile
	{
	public:
		/**
		 * Destructor.
		 */
		virtual ~SoundEffect()
		{
		}

	public:
		/**
		 * Enable 3D positioning (else it'll play in 2D)
		 * 
		 * @param snd3D whether to play sound in 3D or not.
		 */
		virtual void SetSound3D(bool snd3D) = 0;
		
		/**
		 * Checks if 3D sound is enabled.
		 * 
		 * @return whether the sound plays in 3D or not.
		 */
		virtual bool IsSound3D() = 0;
		
		/**
		 * Set the position of the sound (if 3D enabled).
		 * 
		 * @param position vector with the 3d world space position of the sound.
		 */
		virtual void SetPosition(const Vector3& position) = 0;

		
		/**
		 * Get the position of the sound (if 3D enabled).
		 * 
		 * @return position vector (0 0 0 if 3D is disabled).
		 */
		virtual const Vector3& GetPosition() = 0;

	};

	/**
	 * @brief An ambient sound playing in the background.
	 **/
	class AEON_SUBSYSTEM_EXPORT AmbientSound : public virtual IAudioFile
	{
	public:
		/**
		 * Destructor.
		 */
		virtual ~AmbientSound()
		{
		}

	public:
		/**
		 * Set whether to loop the playback or not.
		 * @param looped 
		 */
		virtual void SetLooped(bool looped) = 0;
		
		/**
		 * Get whether playback is looped or not.
		 * 
		 * @return whether looped playback is enabled or not. 
		 */
		virtual bool GetLooped() = 0;
	};

	/**
	 * @brief A music track playing in the background.
	 */
	class AEON_SUBSYSTEM_EXPORT MusicTrack : public virtual IAudioFile
	{
	public:
		/**
		 * Destructor.
		 */
		virtual ~MusicTrack()
		{
		}

	public:
		/**
		 * 
		 */
		virtual void FadeOut(uint32_t msec_duration) = 0;
		
		/**
		 * 
		 */
		virtual void FadeIn(uint32_t msec_duration) = 0;
		
		/**
		 * 
		 */
		virtual void SetLooped(bool looped) = 0;
		
		/**
		 * 
		 */
		virtual bool GetLooped() = 0;

	};

	/**
	 * @brief A more complex type of music track consisting of seperate layers that 
	 * can be faded it an out on demand.
	 **/
	class AEON_SUBSYSTEM_EXPORT LayeredTrack : public virtual IAudioFile 
	{
	public:
		enum MusicLayerTrigger {
			MUS_LAYER_COMBAT = 0,
			MUS_LAYER_ALWAYS,
			MUS_LAYER_DANGER,
			MUS_LAYER_DRASTIC,

			// Keep me at the end
			_MUS_LAYER_ELEM_SIZE
		};
	public:
		/**
		 * Destructor.
		 */
		virtual ~LayeredTrack() 
		{
		}

	public:
		/**
		 * Get the Music Track bound to the given layer (or NULL if there is none).
		 * 
		 * @return the music track.
		 **/
		virtual MusicTrack* GetLayer(MusicLayerTrigger trigger) = 0;
		
		/**
		 * Add a music track tied to a specific layer (so it enables in
		 * a certain situation).
		 * 
		 * @param MusicTrack the music track
		 * @param trigger the trigger (i.e. situation) the track is tied to.
		 **/
		virtual void SetLayer(MusicLayerTrigger trigger, MusicTrack* track) = 0;
		
		/**
		 * 
		 */
		virtual void SetLooped(bool looped) = 0;

		/**
		 * 
		 */
		virtual bool GetLooped() = 0;
	};
	
	/**
	 * @brief Common audio Subystem interface.
	 **/
	class AEON_SUBSYSTEM_EXPORT IAudio : public ISubsystem 
	{
	public:
		/**
		 * 
		 */
		virtual SoundEffect* CreateSoundEffect(const String& id) = 0;
		
		/**
		 * 
		 */
		virtual SoundEffect* CreateSoundEffect(uint32_t fastLookupID) = 0;
		
		/**
		 * 
		 */
		virtual void DestroySoundEffect(SoundEffect* ptr) = 0;

	public:
		/**
		 * 
		 */
		virtual AmbientSound* CreateAmbientSound(const String& id) = 0;
		
		/**
		 * 
		 */
		virtual AmbientSound* CreateAmbientSound(uint32_t fastLookupID) = 0;
		
		/**
		 * 
		 */
		virtual void DestroyAmbientSound(AmbientSound* ptr) = 0;

	public:
		/**
		 * 
		 */
		virtual MusicTrack* CreateMusicTrack(const String& id) = 0;
		
		/**
		 * 
		 */
		virtual MusicTrack* CreateMusicTrack(uint32_t fastLookupID) = 0;
		
		/**
		 * 
		 */
		virtual void DestroyMusicTrack(MusicTrack* ptr) = 0;

	public:
		/**
		 * 
		 */
		virtual LayeredTrack* CreateLayeredTrack(const String& id) = 0;
		
		/**
		 * 
		 */
		virtual LayeredTrack* CreateLayeredTrack(uint32_t fastLookupID) = 0;
		
		/**
		 * 
		 */
		virtual void DestroyLayeredTrack(LayeredTrack* ptr) = 0;

	public:
		/**
		 * 
		 */
		virtual void StopAllSounds() = 0;
		
		/**
		 * 
		 */
		virtual void StopPlayback() = 0;
		
		/**
		 * 
		 */
		virtual void PausePlayback() = 0;
		
		/**
		 * 
		 */
		virtual void ResumePlayback() = 0;

	public:
		/**
		 * 
		 */
		SubsystemType GetSubsystemType() const
		{
			return SUBSYS_AUDIO;
		}
	};
}

#endif // IAudio_h__
