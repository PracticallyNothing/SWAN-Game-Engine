#ifndef SWAN_OPENAL_DEVICE_HPP
#define SWAN_OPENAL_DEVICE_HPP

#include <AL/al.h>
#include <AL/alc.h>

#include <string>
#include <utility>

namespace SWAN {
    namespace AL {
	/// An OpenAL device for playing sounds.
	struct Device {
	    /// Initializes an OpenAL device.
	    Device()
	    {
		devicePtr = alcOpenDevice(nullptr);
		// if(devicePtr) LOG("INFO: OpenAL device opened.\n");
		// else          LOG("ERROR: OpenAL device failed to open.\n");
	    }

	    /// Deleted copy constructor.
	    Device(const Device& other) = delete;
	    /// Deleted copy operator.
	    Device& operator=(const Device& other) = delete;

	    /// Move constructor.
	    Device(Device&& other)
		: devicePtr(std::move(other.devicePtr))
	    {
		other.devicePtr = nullptr;
	    }

	    /// Move operator.
	    Device& operator=(Device&& other) {
		devicePtr = std::move(other.devicePtr);
		other.devicePtr = nullptr;
		return *this;
	    }

	    /// Destructor.
	    ~Device()
	    {
		if(devicePtr != nullptr) {
		    alcCloseDevice(devicePtr);
		    // LOG("INFO: OpenAL device closed.");
		}
	    }

	    /// Returns the name of the device.
	    std::string GetName() const { return alcGetString(devicePtr, ALC_DEVICE_SPECIFIER); }
	    /// Returns the name of the default device.
	    static std::string GetDefaultDeviceName() {
		return alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
	    }

	    /// Returns the actual device handle.
	    ALCdevice* GetDeviceHandle() const { return devicePtr; }
	private:
	    ALCdevice *devicePtr;
	};
    }
}

#endif
