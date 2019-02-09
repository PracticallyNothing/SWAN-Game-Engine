#ifndef SWAN_OPENAL_CONTEXT_HPP
#define SWAN_OPENAL_CONTEXT_HPP

#include <AL/al.h>
#include <AL/alc.h>

#include "Device.hpp"

namespace SWAN
{
	namespace AL
	{
		/// A context for OpenAL to run in.
		struct Context {
			/// Creates a context using the given device.
			Context(const Device& dev)
			{
				contextPtr = alcCreateContext(dev.GetDeviceHandle(), nullptr);
				if(!MakeCurrent()) {
					// LOG("ERROR: Failed to make context current");
				}
			}

			/// Makes the context the current context.
			bool MakeCurrent() { return alcMakeContextCurrent(contextPtr); }

			/// Deleted copy constructor.
			Context(const Context& other) = delete;
			/// Deleted copy operator.
			Context& operator=(const Context& other) = delete;

			/// Move constructor.
			Context(Context&& other)
			    : contextPtr(std::move(other.contextPtr)) { other.contextPtr = nullptr; }

			/// Move operator.
			Context& operator=(Context&& other)
			{
				contextPtr = std::move(other.contextPtr);
				other.contextPtr = nullptr;
				return *this;
			}

			/// Destructor.
			~Context()
			{
				if(contextPtr != nullptr) {
					alcMakeContextCurrent(nullptr);
					alcDestroyContext(contextPtr);
				}
			}

			/// Returns a handle to the actual OpenAL context.
			ALCcontext* GetContextHandle() const { return contextPtr; }

		  private:
			/// A pointer to the actual OpenAL context.
			ALCcontext* contextPtr;
		};
	} // namespace AL
} // namespace SWAN

#endif
