#ifndef SWAN_OPENAL_BUFFER_HPP
#define SWAN_OPENAL_BUFFER_HPP

#include <AL/al.h>
#include <AL/alut.h>
#include <string>

namespace SWAN
{
	namespace AL
	{
		/// Structure of OpenAL buffer properties. Used by Buffer::GetProperties().
		struct BufferProps {
			/// Size in bytes of buffer's data.
			ALint Size = -1;

			/// Bit depth for each channel of the buffer.
			ALint BitDepth = -1;

			/// Frequency (Hz) of the buffer's data.
			ALint Frequency = -1;

			/// Number of channels the buffer has.
			ALint Channels = -1;

			/// Length of buffer's data in samples.
			/// (Note: This value is calculated, OpenAL can't tell you directly.)
			double LenSamples = -1;

			/// Length of buffer's data in seconds.
			/// (Note: This value is calculated, OpenAL can't tell you directly.)
			double Duration = -1;

			/// Is the buffer mono (one audio channel)?
			inline bool IsMono() const { return Channels == 1; }

			/// Is the buffer stereo (two audio channels)?
			inline bool IsStereo() const { return Channels == 2; }

			/// Are these properties valid?
			inline bool IsValid() const
			{
				return Frequency > 0
				       && Channels > 0
				       && BitDepth > 0
				       && Size > 0;
			}
		};

		/// An OpenAL buffer for storing sound data.
		struct Buffer {
			/// Creates an uninitialized OpenAL buffer.
			/// Note: This buffer will have an ID of 0. Use LoadFileALUT() to add data to it.
			Buffer() {}

			/// Creates a buffer and loads the given audio file.
			/// Note: File must be an 8 or 16 bit .wav.
			Buffer(const char* filename) { LoadFileALUT(filename); }

			/// Deleted copy constructor.
			Buffer(const Buffer& other) = delete;
			/// Deleted copy operator.
			Buffer& operator=(const Buffer& other) = delete;

			/// Move constructor.
			Buffer(Buffer&& other)
			    : id(std::move(other.id))
			{
				other.id = 0;
			}

			/// Move operator.
			Buffer& operator=(Buffer&& other)
			{
				id = std::move(other.id);
				other.id = 0;
				return *this;
			}

			/// Destructor.
			~Buffer()
			{
				if(IsInitialized())
					alDeleteBuffers(1, &id);
			}

			/// Loads a .wav file into the buffer using FreeALUT.
			/// Note: File must be an 8 or 16 bit .wav.
			void LoadFileALUT(const char* filename) { id = alutCreateBufferFromFile(filename); }

			/// Retrieves the properties of the given buffer.
			const BufferProps GetProperties() const
			{
				BufferProps res;

				alGetBufferi(id, AL_SIZE, &res.Size);
				alGetBufferi(id, AL_BITS, &res.BitDepth);
				alGetBufferi(id, AL_FREQUENCY, &res.Frequency);
				alGetBufferi(id, AL_CHANNELS, &res.Channels);

				res.LenSamples = res.Size * 8.0 / double(res.Channels * res.BitDepth);
				res.Duration = res.LenSamples / res.Frequency;

				return res;
			}

			/// Is this buffer properly initialized?
			inline bool IsInitialized() const { return id != 0; }

			/// Returns the ID of the buffer.
			inline ALuint GetID() const { return id; }

		  private:
			/// The ID of the OpenAL buffer.
			ALuint id = 0;
		};
	} // namespace AL
} // namespace SWAN
#endif
