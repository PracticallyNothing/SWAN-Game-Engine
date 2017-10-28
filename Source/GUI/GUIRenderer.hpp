#ifndef SWAN_GUIRENDERER_HPP
#define SWAN_GUIRENDERER_HPP

#include "GUIElement.hpp" // For SWAN::GUIElement
#include "GUIPrim.hpp"    // For SWAN::GUIP::*

#include "Physics/Transform.hpp" // For SWAN::Transform
#include "Rendering/Mesh.hpp"    // For SWAN::Mesh
#include "Rendering/Shader.hpp"  // For SWAN::Shader
#include "Utility/Either.hpp"    // For SWAN::Util::Either<T, U>

#include <vector>

namespace SWAN {
struct LayerSorter {
	typedef SWAN::Util::Either<SWAN::GUIP::IElement*,
	                           SWAN::GUIP::IElementContainer*>
	    ElemOrCont;

	bool operator()(ElemOrCont a, ElemOrCont b) {
		if(a.hasFirst() && b.hasFirst())
			return compare(a.getFirst(), b.getFirst());

		else if(a.hasFirst() && !b.hasFirst())
			return compare(a.getFirst(), b.getSecond()->getElements());

		else if(!a.hasFirst() && b.hasFirst())
			return compare(a.getSecond()->getElements(), b.getFirst());

		else
			return compare(a.getSecond()->getElements(),
			               b.getSecond()->getElements());
	}

	inline bool compare(SWAN::GUIP::IElement* a, SWAN::GUIP::IElement* b) {
		return compare(a->layer, a->sublayer, b->layer, b->sublayer);
	}
	inline bool compare(SWAN::GUIP::IElement* a, SWAN::GUIP::ElementGroup& b) {
		return compare(a->layer, a->sublayer, b.layer, b.sublayer);
	}
	inline bool compare(SWAN::GUIP::ElementGroup& a, SWAN::GUIP::IElement* b) {
		return compare(a.layer, a.sublayer, b->layer, b->sublayer);
	}
	inline bool compare(SWAN::GUIP::ElementGroup& a,
	                    SWAN::GUIP::ElementGroup& b) {
		return compare(a.layer, a.sublayer, b.layer, b.sublayer);
	}

	bool compare(int aLayer, int aSublayer, int bLayer, int bSublayer) {
		if(aLayer != bLayer) {
			return aLayer > bLayer;
		} else {
			if(aSublayer == bSublayer) {
				return true;
			} else {
				return aSublayer > bSublayer;
			}
		}
	}
};

class GUIRenderer {
  public:
	using ElementType = SWAN::Util::Either<SWAN::GUIP::IElement*,
	                                       SWAN::GUIP::IElementContainer*>;
	GUIRenderer();
	~GUIRenderer();

	GUIP::IElement* add(GUIP::IElement*);
	GUIP::IElementContainer* add(GUIP::IElementContainer*);

	void render();
	void update();

	const std::vector<ElementType>& getElems() const;

	ElementType& getElem(int index);

	void _exp_render();
	void _exp_update();
	GUIElement* _exp_add(std::unique_ptr<GUIElement>);
	SWAN::Shader* _exp_shad;
	std::vector<std::unique_ptr<GUIElement>> _exp_elems; // experimental elements
  private:
	void renderElement(SWAN::GUIP::IElement*);
	void _exp_renderElement(GUIElement*);
	SWAN::Shader* shad;

	std::vector<ElementType> elems;

	SWAN::Mesh* rect;
	SWAN::Transform transform;

	bool sortedByLayer;
};
}

#endif
