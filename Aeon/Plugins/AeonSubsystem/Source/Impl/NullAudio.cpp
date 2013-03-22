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
#include <Aeon/Subsystem/Impl/NullAudio.h>
#include <Aeon/Message.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	// Null Implementation for an AudioFile
	// -------------------------------------------------------------------------
	class NullAudioFile : public virtual IAudioFile
	{
	public:
		void Play()
		{ 
			LOG_DEBUG << "IAudioFile::Play()";
		}

		void Stop()
		{
			LOG_DEBUG << "IAudioFile::Stop()";
		}

		uint8_t GetVolume()
		{
			LOG_DEBUG << "IAudioFile::GetVolume() = 0";
			return 0;
		}

		void SetVolume(uint8_t volume)
		{
			LOG_DEBUG << "IAudioFile::SetVolume(" << volume << ")";
		}

		void Update(uint32_t msec)
		{
			LOG_DEBUG << "IAudioFile::Update(" << msec << ")";
		}
	};

	// -------------------------------------------------------------------------
	// Null Implementation for a Sound Effect
	// -------------------------------------------------------------------------
	class NullSoundEffect : public NullAudioFile, public SoundEffect
	{
	public:
		virtual void SetSound3D(bool snd3D)
		{
			LOG_DEBUG << "SoundEffect::SetSound3D(" << snd3D << ")";
		}

		virtual bool IsSound3D()
		{
			LOG_DEBUG << "SoundEffect::IsSound3D() = false";
			return false;
		}

		virtual void SetPosition(const Vector3& /*position*/)
		{
			LOG_DEBUG << "SoundEffect::SetPosition(Vec3)";
		}

		virtual const Vector3& GetPosition()
		{
			LOG_DEBUG << "SoundEffect::GetPosition()";
			return pos;
		}

		NullSoundEffect():
		    pos(0.f,0.f,0.f)
		{

		}

	private:
		Vector3 pos;
	};

	// -------------------------------------------------------------------------
	// Null Implementation for an Ambient Sound
	// -------------------------------------------------------------------------
	class NullAmbientSound : public NullAudioFile, public AmbientSound 
	{
	public:
		virtual void SetLooped(bool looped)
		{
			LOG_DEBUG << "AmbientSound::SetLooped(" << looped << ")";

		}

		virtual bool GetLooped()
		{
			LOG_DEBUG << "AmbientSound::GetLooped() = false";
			return false;
		}
	};

	// -------------------------------------------------------------------------
	// Null Implementation for a Music Track
	// -------------------------------------------------------------------------
	class NullMusicTrack : public NullAudioFile, public MusicTrack
	{
		virtual void FadeOut(uint32_t msec_duration)
		{
			LOG_DEBUG << "MusicTrack::FadeOut("<< msec_duration <<")";
		}

		virtual void FadeIn(uint32_t msec_duration)
		{
			LOG_DEBUG << "MusicTrack::FadeIn("<< msec_duration <<")";
		}

		virtual void SetLooped(bool looped)
		{
			LOG_DEBUG << "MusicTrack::SetLooped(" << looped << ")";
		}

		virtual bool GetLooped()
		{
			LOG_DEBUG << "MusicTrack::GetLooped() = false";
			return false;
		}
	};

	// -------------------------------------------------------------------------
	// Null Implementation for a Layered Track
	// -------------------------------------------------------------------------
	class NullLayeredTrack : public NullMusicTrack, public LayeredTrack
	{
		virtual MusicTrack* GetLayer(MusicLayerTrigger /*trigger*/)
		{
			LOG_DEBUG << "LayeredTrack::GetLayer() = 0";
			return 0;
		}

		virtual void SetLayer(MusicLayerTrigger trigger, MusicTrack* /*track*/)
		{
			LOG_DEBUG << "LayeredTrack::SetLayer("<< trigger << ",0)";			
		}

		virtual void SetLooped(bool looped)
		{
			LOG_DEBUG << "LayeredTrack::SetLooped(" << looped << ")";
		}

		virtual bool GetLooped()
		{
			LOG_DEBUG << "LayeredTrack::GetLooped() = false";
			return false;
		}
	};

	// -------------------------------------------------------------------------
	void NullAudio::Initialise(CmdArgs& args)
	{

	}

	// -------------------------------------------------------------------------
	void NullAudio::Shutdown()
	{

	}

	// -------------------------------------------------------------------------
	SoundEffect* NullAudio::CreateSoundEffect(const String& id)
	{
		return new NullSoundEffect();
	}

	// -------------------------------------------------------------------------
	SoundEffect* NullAudio::CreateSoundEffect(uint32_t fastLookupID)
	{
		return new NullSoundEffect();
	}

	// -------------------------------------------------------------------------
	void NullAudio::DestroySoundEffect(SoundEffect* ptr)
	{
		delete ptr;
	}

	// -------------------------------------------------------------------------
	AmbientSound* NullAudio::CreateAmbientSound(const String& id)
	{
		return new NullAmbientSound();
	}

	// -------------------------------------------------------------------------
	AmbientSound* NullAudio::CreateAmbientSound(uint32_t fastLookupID)
	{
		return new NullAmbientSound();
	}

	// -------------------------------------------------------------------------
	void NullAudio::DestroyAmbientSound(AmbientSound* ptr)
	{
		delete ptr;
	}

	// -------------------------------------------------------------------------
	MusicTrack* NullAudio::CreateMusicTrack(const String& id)
	{
		return new NullMusicTrack();
	}

	// -------------------------------------------------------------------------
	MusicTrack* NullAudio::CreateMusicTrack(uint32_t fastLookupID)
	{
		return new NullMusicTrack();
	}

	// -------------------------------------------------------------------------
	void NullAudio::DestroyMusicTrack(MusicTrack* ptr)
	{
		delete ptr;
	}

	// -------------------------------------------------------------------------
	LayeredTrack* NullAudio::CreateLayeredTrack(const String& id)
	{
		return new NullLayeredTrack();
	}

	// -------------------------------------------------------------------------
	LayeredTrack* NullAudio::CreateLayeredTrack(uint32_t fastLookupID)
	{
		return new NullLayeredTrack();
	}

	// -------------------------------------------------------------------------
	void NullAudio::DestroyLayeredTrack(LayeredTrack* ptr)
	{
		delete ptr;
	}

	// -------------------------------------------------------------------------
	void NullAudio::StopAllSounds()
	{

	}

	// -------------------------------------------------------------------------
	void NullAudio::StopPlayback()
	{

	}

	// -------------------------------------------------------------------------
	void NullAudio::PausePlayback()
	{

	}

	// -------------------------------------------------------------------------
	void NullAudio::ResumePlayback()
	{

	}

	// -------------------------------------------------------------------------
	void NullAudio::OnMessage(Aeon::Message* message)
	{
		if(!message) return;

		LOG_DEBUG << "Received message: " << message->GetType();

	}

}
