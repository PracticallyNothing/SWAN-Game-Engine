#ifndef SWAN_OPENAL_LISTENER_HPP
#define SWAN_OPENAL_LISTENER_HPP

#include <AL/al.h>
#include <array>
#include "Maths/Vector.hpp"

namespace SWAN {
    namespace AL {
	/// Structure for OpenAL listener properties.
	struct ListenerProps {
	    /// The position of the listener in 3D space.
	    vec3 Position;
	    /// The velocity of the listener.
	    vec3 Velocity;
	    /// A normalized vector pointing towards where the lisener is looking.
	    vec3 Forward;
	    /// A normalized vector pointing upwards of the listener.
	    vec3 Up;
	};

	/// An OpenAL listener structure.
	struct Listener {
	    /// Constructs a listener.
	    Listener(vec3 pos = {0,0,0},
			   vec3 vel = {0,0,0},
			   vec3 forw = {0,0,1},
			   vec3 up = {0,1,0}) {
		SetPosition(pos);
		SetVelocity(vel);
		SetOrientation(forw, up);
	    }

	    /// Constructs a listener from the given properties.
	    Listener(ListenerProps props) { SetProperties(props); }

	    /// Sets the properties of the listener to the values in the given props structure.
	    void SetProperties(ListenerProps props) {
		SetPosition(props.Position);
		SetVelocity(props.Velocity);
		SetOrientation(props.Forward, props.Up);
	    }

	    /// Sets where forward is for the listener.
	    inline void SetOrientationForward(vec3 forward) { SetOrientation(forward, props.Up); }
	    /// Sets where up is for the listener.
	    inline void SetOrientationUp(vec3 up) { SetOrientation(props.Forward, up); }

	    /// Sets where forwards and up are for the listener.
	    void SetOrientation(vec3 forw, vec3 up) {
		std::array<fvec3, 2> arr = { Normalized(forw), Normalized(up) };
		alListenerfv(AL_ORIENTATION, (float*)arr.data());

		props.Forward = forw;
		props.Up = up;
	    }

	    /// Sets the position of the listener in 3D space.
	    void SetPosition(vec3 pos) {alListener3f(AL_POSITION, pos.x, pos.y, pos.z); props.Position = pos;}
	    /// Sets the listener's velocity.
	    void SetVelocity(vec3 vel) {alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z); props.Velocity = vel;}

	    /// Returns the properties of the listener.
	    ListenerProps GetProperties() const { return props; }
	private:
	    /// A structure with the properties of the listener.
	    /// All values should be modified with the Set*() functions.
	    ListenerProps props;
	};
    }
}

#endif
