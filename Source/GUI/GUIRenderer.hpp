#ifndef SWAN_GUIRENDERER_HPP
#define SWAN_GUIRENDERER_HPP

#include "GUIPrim.hpp" // For SWAN::GUIP::*

#include "Physics/Transform.hpp" // For SWAN::Transform
#include "Rendering/Mesh.hpp"    // For SWAN::Mesh
#include "Rendering/Shader.hpp"  // For SWAN::Shader
#include "Utility/Either.hpp"    // For SWAN::Util::Either<T, U>

#include <vector>

namespace SWAN {
struct LayerSorter {
	typedef SWAN::Util::Either<SWAN::GUIP::IElement*, SWAN::GUIP::IElementContainer*> ElemOrCont;

	bool operator() (ElemOrCont a, ElemOrCont b) {
		if (a.hasFirst () && b.hasFirst ())
			return compare (a.getFirst (), b.getFirst ());

		else if (a.hasFirst () && !b.hasFirst ())
			return compare (a.getFirst (), b.getSecond ()->getElements ());

		else if (!a.hasFirst () && b.hasFirst ())
			return compare (a.getSecond ()->getElements (), b.getFirst ());

		else
			return compare (a.getSecond ()->getElements (), b.getSecond ()->getElements ());
	}

	inline bool compare (SWAN::GUIP::IElement* a, SWAN::GUIP::IElement* b) {
		return compare (a->layer, a->sublayer, b->layer, b->sublayer);
	}
	inline bool compare (SWAN::GUIP::IElement* a, SWAN::GUIP::ElementGroup& b) {
		return compare (a->layer, a->sublayer, b.layer, b.sublayer);
	}
	inline bool compare (SWAN::GUIP::ElementGroup& a, SWAN::GUIP::IElement* b) {
		return compare (a.layer, a.sublayer, b->layer, b->sublayer);
	}
	inline bool compare (SWAN::GUIP::ElementGroup& a, SWAN::GUIP::ElementGroup& b) {
		return compare (a.layer, a.sublayer, b.layer, b.sublayer);
	}

	bool compare (int aLayer, int aSublayer, int bLayer, int bSublayer) {
		if (aLayer != bLayer) {
			return aLayer > bLayer;
		} else {
			if (aSublayer == bSublayer) {
				return true;
			} else {
				return aSublayer > bSublayer;
			}
		}
	}
};

class GUIRenderer {
  public:
	using ElementType = SWAN::Util::Either<SWAN::GUIP::IElement*, SWAN::GUIP::IElementContainer*>;
	GUIRenderer ();
	~GUIRenderer ();

	SWAN::GUIP::IElement*          add (SWAN::GUIP::IElement*);
	SWAN::GUIP::IElementContainer* add (SWAN::GUIP::IElementContainer*);
	void                           render ();
	void                           update ();

	const std::vector<ElementType>& getElems () const;

	ElementType& getElem (int index);

  private:
	void          renderElement (SWAN::GUIP::IElement*);
	SWAN::Shader* shad;

	std::vector<ElementType> elems;

	SWAN::Mesh*     rect;
	SWAN::Transform transform;

	bool sortedByLayer;
};
}

#endif
