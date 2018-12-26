#ifndef SWAN_OPENAL_SOURCE_HPP
#define SWAN_OPENAL_SOURCE_HPP

#include <AL/al.h>
#include <AL/alc.h>

#include "Device.hpp"
#include "Buffer.hpp"
#include "Maths/Vector.hpp"

namespace SWAN {
    namespace AL {
	/// Structure of OpenAL source properties.
	/// Used by Source::GetProperties(), Source::SetProperties()
	/// and Source::ReadProperties().
	struct SourceProps {
	    /// The pitch of the source's sound.
	    /// The default level is 1.
	    float Pitch = 1;
	    /// The gain of the source's sound.
	    /// The default value is 1.
	    float Gain = 1;
	    /// The position in 3D space of the source.
	    /// The default value is origin (0,0,0).
	    vec3 Position = {0,0,0};
	    /// The velocity of the source.
	    /// The default value is (0,0,0).
	    vec3 Velocity = {0,0,0};
	    /// Whether the source loops after its audio stops playing.
	    /// False by default.
	    bool Looping = false;

	    /// Whether the source's position is relative to the current listener.
	    /// False by default.
	    bool Relative = false;

	    /// How low the source's gain can go. Default is 0.
	    float MinGain = 0;
	    /// How high the source's gain can go. Default is 1.
	    float MaxGain = 1;

	    /// The maximum distance the source can be heard at.
	    /// Note: The value of this applies in every case,
	    ///       but logically, this only works when the distance model
	    ///       has been set to Linear. See SoundSystem::SetDistanceModel().
	    float MaxDistance;
	    /// How fast the gain of the source will lower with distance.
	    /// Default value is 1.
	    float RoloffFactor = 1;
	    /// The distance at which the source will be heard at half volume.
	    float ReferenceDistance;
	};

	/// An OpenAL source that plays audio.
	struct Source {
	    /// Enumeration for the state of the source (whether it's playing or not).
	    enum class State
	    {
		/// The source either doesn't have any audio to play, or the audio is at 00:00.
		Initial,
		/// The source is currently playing its audio. A call to Play() will restart it.
		Playing,
		/// The source has paused its audio. A call to Play() will resume the playback.
		Paused,
		/// The source's audio has finished playback. A call to Play() will start from the beginning.
		Stopped
	    };
	    /// Is this source paused?

	    /// Returns the current position in the audio.
	    inline float GetCurrPlaybackPos() const { return GetFloatValue(AL_SEC_OFFSET); }
	    /// Jumps to the given amount of seconds in the audio.
	    inline void SetCurrPlaybackPos(float seconds) { alSourcef(id, AL_SEC_OFFSET, seconds); }

	    /// Is this source's state initial?
	    inline bool IsInitial() const {return GetState() == State::Initial;}
	    /// Is this source playing?
	    inline bool IsPlaying() const {return GetState() == State::Playing;}
	    /// Is this source paused?
	    inline bool IsPaused()  const {return GetState() == State::Paused;}
	    /// Is this source stopped?
	    inline bool IsStopped() const {return GetState() == State::Stopped;}

	    /// Returns the current state of the source.
	    State GetState() const
	    {
		int source_state = GetIntValue(AL_SOURCE_STATE);

		switch(source_state)
		{
		case AL_INITIAL: return State::Initial;
		case AL_PLAYING: return State::Playing;
		case AL_PAUSED: return State::Paused;
		case AL_STOPPED: return State::Stopped;
		}

		return State::Initial;
	    }

	    /// Constructs a source without audio.
	    Source(float pitch = 1,
			 float gain = 1,
			 vec3 pos = {0,0,0},
			 vec3 vel = {0,0,0},
			 bool looping = false)
	    {
		alGenSources((ALuint)1, &id);
		properties = { pitch, gain, pos, vel, looping };
		SetProperties(properties);
	    }

	    /// Constructs a source with the given buffer's contents as audio.
	    Source(const Buffer* buffer,
			 float pitch = 1,
			 float gain = 1,
			 vec3 pos = {0,0,0},
			 vec3 vel = {0,0,0},
			 bool looping = false)
	    {
		alGenSources((ALuint)1, &id);

		properties = { pitch, gain, pos, vel, looping,
			       false, 0, 1, 10, 1, 5};
	
		SetProperties(properties);
		SetBuffer(buffer);
	    }

	    /// Copy constructor.
	    Source(const Source& other) {
		alGenSources((ALuint)1, &id);

		SetBuffer(other.buffer);
		SetProperties(other.properties);
	    }

	    /// Copy operator.
	    Source& operator=(const Source& other) {
		alGenSources((ALuint)1, &id);

		SetBuffer(other.buffer);
		SetProperties(other.properties);

		return *this;
	    }

	    /// Move constructor.
	    Source(Source&& other) {
		id = std::move(other.id);
		other.id = 0;

		SetBuffer(std::move(other.buffer));
		SetProperties(std::move(other.properties));
	    }

	    /// Move operator.
	    Source& operator=(Source&& other) {
		id = std::move(other.id);
		other.id = 0;

		SetBuffer(std::move(other.buffer));
		SetProperties(std::move(other.properties));
		return *this;
	    }

	    /// Destructor.
	    ~Source() {
		if(id != 0)
		    alDeleteSources(1, &id);
	    }

	    /// Sets the given buffer as an audio source.
	    void SetBuffer(const Buffer* buffer) {
		SetIntValue(buffer->GetID(), AL_BUFFER);
		this->buffer = buffer;
	    }

	    /// Sets the pitch of the source's audio.
	    void SetPitch(float value)  { SetFloatValue(value, AL_PITCH);   properties.Pitch   = value; }
	    /// Sets the gain of the source's audio.
	    void SetGain(float value)   { SetFloatValue(value, AL_GAIN);    properties.Gain    = value; }
	    /// Sets whether the source's audio loops after it reaches the end.
	    void SetLooping(bool value) { SetBoolValue (value, AL_LOOPING); properties.Looping = value; }

	    /// Sets the source's position in 3D space.
	    void SetPosition(vec3 pos)   {SetVec3Value(pos, AL_POSITION); properties.Position = pos;}
	    /// Sets the source's velocity.
	    void SetVelocity(vec3 vel)   {SetVec3Value(vel, AL_VELOCITY); properties.Velocity = vel;}
	    /// Sets whether the source's position is relative to the position of the listener.
	    void SetRelative(bool value) {SetBoolValue(value, AL_SOURCE_RELATIVE); properties.Relative = value; }

	    /// Sets the minimum gain this source can have.
	    void SetMinGain(float value) {SetFloatValue(value, AL_MIN_GAIN); properties.MinGain = value; }
	    /// Sets the maximum gain this source can have.
	    void SetMaxGain(float value) {SetFloatValue(value, AL_MAX_GAIN); properties.MaxGain = value; }
	    /// Sets the maximum distance, at which the source can be heard.
	    void SetMaxDistance(float value) {SetFloatValue(value, AL_MAX_DISTANCE); properties.MaxDistance = value; }
	    /// Sets how fast the audio level of the source drops with distance.
	    void SetRoloffFactor(float value) { SetFloatValue(value, AL_ROLLOFF_FACTOR); properties.RoloffFactor = value; }
	    /// Sets the distance at which the source will be heard at half volume.
	    void SetReferenceDistance(float value) { SetFloatValue(value, AL_REFERENCE_DISTANCE); properties.ReferenceDistance = value; }

	    /// Copies over the properties from the given props struct.
	    void SetProperties(SourceProps props) {
		SetPitch(props.Pitch);
		SetGain(props.Gain);
		SetPosition(props.Position);
		SetVelocity(props.Velocity);
		SetLooping(props.Looping);
		SetRelative(props.Relative);
		SetMinGain(props.MinGain);
		SetMaxGain(props.MaxGain);
		SetMaxDistance(props.MaxDistance);
		SetRoloffFactor(props.RoloffFactor);
		SetReferenceDistance(props.ReferenceDistance);
	    }

	    /// Will this source attenuate based on distance?
	    /// For OpenAL Soft, stereo sound sources will not attenuate.
	    bool WillAttenuate(Device& dev) {
		return dev.GetName() == std::string("OpenAL Soft")
		    && buffer->GetProperties().IsMono();
	    }

	    /// Re-read properties according to what OpenAL
	    /// says about them and fill the properties member.
	    void ReadProperties() {
		SetPitch(GetFloatValue(AL_PITCH));
		SetGain(GetFloatValue(AL_GAIN));
		SetPosition(GetVec3Value(AL_POSITION));
		SetVelocity(GetVec3Value(AL_VELOCITY));
		SetLooping(GetBoolValue(AL_LOOPING));
		SetRelative(GetBoolValue(AL_SOURCE_RELATIVE));
		SetMinGain(GetFloatValue(AL_MIN_GAIN));
		SetMaxGain(GetFloatValue(AL_MAX_GAIN));
		SetMaxDistance(GetFloatValue(AL_MAX_DISTANCE));
		SetRoloffFactor(GetFloatValue(AL_ROLLOFF_FACTOR));
		SetReferenceDistance(GetFloatValue(AL_REFERENCE_DISTANCE));
	    }

	    /// Plays the source and sets its state to Playing.
	    /// Note: This spawns another thread, in which the audio is played.
	    ///       Use IsPlaying() to check its state.
	    void Play()   {alSourcePlay(id);}
	    /// Pauses the source and sets its state to Paused.
	    void Pause()  {alSourcePause(id);}
	    /// Puts the source's position back at 00:00 and sets its state to Initial.
	    void Rewind() {alSourceRewind(id);}
	    /// Finishes the source's playback and sets its state to Stopped.
	    void Stop()   {alSourceStop(id);}

	    /// Returns the properties of the source.
	    SourceProps GetProperties() const { return properties; }
	    /// Returns the current audio source of the source.
	    const Buffer* GetBuffer() const { return buffer; }
	    /// Returns the current buffer's properties, or an invalid set of properties if there is no buffer.
	    const BufferProps GetBufferProperties() const { return (buffer == nullptr ? BufferProps() : buffer->GetProperties()); }

	    /// Returns the ID of the source.
	    inline ALuint GetID() const { return id; }

	private:
	    /// ID of the buffer.
	    ALuint id = 0;
	    /// Pointer to the buffer that serves as an audio source.
	    const Buffer* buffer = nullptr;
	    /// A structure with the properties of the source.
	    /// All values should be modified with the Set*() functions.
	    SourceProps properties;

	    /// Retrieves theeee boolean value of the given enum.
	    bool GetBoolValue(ALenum en) const {ALint i; alGetSourcei(id, en, &i); return i == AL_TRUE ? true : false;}
	    /// Retrieves the integer value of the given enum.
	    int GetIntValue(ALenum en) const {ALint i; alGetSourcei(id, en, &i); return i;}
	    /// Retrieves the float value of the given enum.
	    float GetFloatValue(ALenum en) const {ALfloat f; alGetSourcef(id, en, &f); return f;}
	    /// Retrieves the 3-dimensional value of the given enum.
	    fvec3 GetVec3Value(ALenum en) const {fvec3 v; alGetSource3f(id, en, &v.x, &v.y, &v.z); return v; }

	    /// Sets the boolean value of the given enum.
	    void SetBoolValue(bool b, ALenum en) {alSourcei(id, en, b ? AL_TRUE : AL_FALSE);}
	    /// Sets the integer value of the given enum.
	    void SetIntValue(int i, ALenum en) {alSourcei(id, en, i);}
	    /// Sets the float value of the given enum.
	    void SetFloatValue(float f, ALenum en) {alSourcei(id, en, f);}
	    /// Sets the 3-dimensional value of the given enum.
	    void SetVec3Value(vec3 v, ALenum en) {alSource3f(id, en, v.x, v.y, v.z); }
	};

    }
}
#endif
