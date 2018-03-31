#include "ColWrapper.hpp"

#include <utility> // For std::move()

using std::move;

namespace SWAN {
ColWrapper::ColWrapper(const ColWrapper& c) : type(c.type) {
	switch(type) {
		case ColWrapper::COL_NONE: break;

		case ColWrapper::COL_POINT: point = c.point; break;

		case ColWrapper::COL_RAY: ray = c.ray; break;

		case ColWrapper::COL_PLANE: plane = c.plane; break;

		case ColWrapper::COL_AABB:
			aabb = c.aabb;
			break;
			/*
	  case ColWrapper::COL_SPHERE:
	  sphere = c.sphere;
	  break;
	  */
	}
}

ColWrapper::ColWrapper(ColWrapper&& c) : type(move(c.type)) {
	switch(type) {
		case ColWrapper::COL_NONE: break;
		case ColWrapper::COL_POINT: point = move(c.point); break;
		case ColWrapper::COL_PLANE: plane = move(c.plane); break;
		case ColWrapper::COL_RAY: ray     = move(c.ray); break;
		case ColWrapper::COL_AABB:
			aabb = move(c.aabb);
			break;
			// case ColWrapper::COL_SPHERE: sphere = move(c.sphere); break;
	}
}

ColWrapper& ColWrapper::operator=(const ColWrapper& c) {
	type = c.type;

	switch(type) {
		case ColWrapper::COL_NONE: break;

		case ColWrapper::COL_POINT: point = c.point; break;

		case ColWrapper::COL_RAY: ray = c.ray; break;

		case ColWrapper::COL_PLANE: plane = c.plane; break;

		case ColWrapper::COL_AABB:
			aabb = c.aabb;
			break;
			/*
	  case ColWrapper::COL_SPHERE:
	  sphere = c.sphere;
	  break;
	  */
	}

	return *this;
}

ColWrapper& ColWrapper::operator=(ColWrapper&& c) {
	type = move(c.type);

	switch(type) {
		case ColWrapper::COL_NONE: break;
		case ColWrapper::COL_POINT: point = move(c.point); break;
		case ColWrapper::COL_PLANE: plane = move(c.plane); break;
		case ColWrapper::COL_RAY: ray     = move(c.ray); break;
		case ColWrapper::COL_AABB:
			aabb = move(c.aabb);
			break;
			// case ColWrapper::COL_SPHERE: sphere = move(c.sphere); break;
	}

	return *this;
}

Collision ColWrapper::checkCollision(const ColWrapper& other) {
	// if(type == ColWrapper::COL_NONE || other.type == ColWrapper::COL_NONE){
	return Collision{ false, 0 };
	//}

	/*
  switch(type) {
	    case COL_POINT:
	            switch(other.type){
	                    case COL_POINT:  return Collision { point ==
  other.point; Length(point - other.point) };
	                    case COL_LINE:   return Collision {};
	                    case COL_RAY:    return Collision {};
	                    case COL_AABB:   return CheckCollision(point,
  other.aabb);
	                    //case COL_SPHERE: return
  CheckCollision(point, other.sphere);
	            }
	            break;
	    case COL_RAY:
	            switch(other.type){
	                    case COL_POINT:  return Collision {};
	                    case COL_LINE:   return Collision {};
	                    case COL_RAY:    return Collision {};
	                    case COL_AABB:   return CheckCollision(line,
  other.aabb);
	                    //case COL_SPHERE: return CheckCollision(line,
  other.sphere);
	            }
	            break;
  }
  */
}
}
