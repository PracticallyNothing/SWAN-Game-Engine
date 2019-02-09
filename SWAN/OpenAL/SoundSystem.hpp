#ifndef SWAN_SOUND_SYSTEM_HPP
#define SWAN_SOUND_SYSTEM_HPP

#include <AL/alut.h>
#include <string>
#include <vector>

#include "Buffer.hpp"
#include "Context.hpp"
#include "Device.hpp"
#include "Listener.hpp"
#include "Source.hpp"

#include "Core/Logging.hpp"

namespace SWAN
{
	using namespace std::literals;
	/// An enumeration for different audio attenuation models.
	enum class DistanceModel {
		/// Audio will not get attenuated with distance.
		None,
		/// Volume will realistically lower with the inverse of the distance,
		/// but won't disappear beyond the max distance of the AL::Source.
		Inverse,
		/// Volume will realistically lower with the inverse of the distance
		/// and will not go higher than the original level,
		/// but won't disappear beyond the max distance of the AL::Source.
		InverseClamped,
		/// Volume will linearly lower with distance,
		/// disappearing beyond the AL::Source's max distance.
		Linear,
		/// Volume will linearly lower with distance and will not go higher than the original level,
		/// disappearing beyond the AL::Source's max distance.
		LinearClamped,
		/// Volume will exponentially decrease with the power of the AL::Source's roloff factor.
		Exponential,
		/// Volume will exponentially decrease with the power of the AL::Source's roloff factor
		/// and will not go higher than the original level.
		ExponentialClamped,
	};

	/// A structure for managing sound.
	struct SoundSystem {
		/// Constructor.
		SoundSystem() : device(), context(device), listener(), sources(), buffers()
		{
			alutInitWithoutContext(nullptr, nullptr);
			SetDistanceModel(DistanceModel::InverseClamped);
			Log("Audio", "Initialized sound system with OpenAL driver \""s + device.GetName() + "\".", LogLevel::Success);
		}

		/// Destructor.
		~SoundSystem()
		{
			for(AL::Source* src : sources)
				delete src;
			for(AL::Buffer* buff : buffers)
				delete buff;
		}

		/// Sets the distance model.
		void SetDistanceModel(DistanceModel dm)
		{
			ALenum e;

			switch(dm) {
				case DistanceModel::None: e = AL_NONE; break;
				case DistanceModel::Linear: e = AL_LINEAR_DISTANCE; break;
				case DistanceModel::Inverse: e = AL_INVERSE_DISTANCE; break;
				case DistanceModel::Exponential: e = AL_EXPONENT_DISTANCE; break;
				case DistanceModel::LinearClamped: e = AL_LINEAR_DISTANCE_CLAMPED; break;
				case DistanceModel::InverseClamped: e = AL_INVERSE_DISTANCE_CLAMPED; break;
				case DistanceModel::ExponentialClamped: e = AL_EXPONENT_DISTANCE_CLAMPED; break;
			}

			alDistanceModel(e);
		}

		/// Creates a buffer from the given file.
		const AL::Buffer* AddBuffer(const char* filename)
		{
			buffers.push_back(new AL::Buffer(filename));
			return buffers.back();
		}

		/// Creates an AL::Source without a buffer.
		AL::Source* AddSource(float pitch = 1,
		                      float gain = 1,
		                      vec3 pos = { 0, 0, 0 },
		                      vec3 vel = { 0, 0, 0 },
		                      bool looping = false)
		{
			sources.push_back(new AL::Source(pitch, gain, pos, vel, looping));
			return sources.back();
		}

		/// Creates an AL::Source with a buffer,
		/// which holds the contents of the given file.
		AL::Source* AddSourceWithBuffer(const char* filename,
		                                float pitch = 1,
		                                float gain = 1,
		                                vec3 pos = { 0, 0, 0 },
		                                vec3 vel = { 0, 0, 0 },
		                                bool looping = false)
		{
			AL::Source* res = AddSource(pitch, gain, pos, vel, looping);
			res->SetBuffer(AddBuffer(filename));
			return res;
		}
		/// Creates an AL::Source with the given buffer.
		AL::Source* AddSourceWithBuffer(const AL::Buffer* buffer,
		                                float pitch = 1,
		                                float gain = 1,
		                                vec3 pos = { 0, 0, 0 },
		                                vec3 vel = { 0, 0, 0 },
		                                bool looping = false)
		{
			AL::Source* res = AddSource(pitch, gain, pos, vel, looping);
			res->SetBuffer(buffer);
			return res;
		}

		/// The OpenAL device.
		AL::Device device;

		/// The OpenAL context.
		AL::Context context;

		/// The OpenAL listener.
		AL::Listener listener;

		/// A vector of every OpenAL source.
		std::vector<AL::Source*> sources;

		/// A vector of every OpenAL buffer.
		std::vector<AL::Buffer*> buffers;
	};
} // namespace SWAN

#endif
