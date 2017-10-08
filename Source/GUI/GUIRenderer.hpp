#ifndef GUIRENDERER_HPP
#define GUIRENDERER_HPP

#include "GUIPrim.hpp"             // For GUIPrim::*

#include "Rendering/Mesh.hpp"   // For Mesh
#include "Rendering/Shader.hpp" // For Shader
#include "Physics/Transform.hpp" // For Transform
#include "Utility/Either.hpp"   // For Util::Either<T, U>

#include <vector>

struct LayerSorter {
	typedef Util::Either<GUIPrim::IElement*, GUIPrim::IElementContainer*> ElemOrCont;

	bool operator()(ElemOrCont a, ElemOrCont b){
		if(a.hasFirst() && b.hasFirst())
			return compare(a.getFirst(), b.getFirst());

		else if(a.hasFirst() && !b.hasFirst())
			return compare(a.getFirst(), b.getSecond()->getElements());

		else if(!a.hasFirst() && b.hasFirst())
			return compare(a.getSecond()->getElements(), b.getFirst());

		else
			return compare(a.getSecond()->getElements(), b.getSecond()->getElements());
	}

	inline bool compare(GUIPrim::IElement* a, GUIPrim::IElement* b){
		return compare(a->layer, a->sublayer,
		               b->layer, b->sublayer);
	}
	inline bool compare(GUIPrim::IElement* a, GUIPrim::ElementGroup& b){
		return compare(a->layer, a->sublayer,
		               b.layer, b.sublayer);
	}
	inline bool compare(GUIPrim::ElementGroup& a, GUIPrim::IElement* b){
		return compare(a.layer, a.sublayer,
		               b->layer, b->sublayer);
	}
	inline bool compare(GUIPrim::ElementGroup& a, GUIPrim::ElementGroup& b){
		return compare(a.layer, a.sublayer,
		               b.layer, b.sublayer);
	}

	bool compare(int aLayer, int aSublayer, int bLayer, int bSublayer){
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
		using ElementType = Util::Either<GUIPrim::IElement*, GUIPrim::IElementContainer*>;
		GUIRenderer();
		~GUIRenderer();

		GUIPrim::IElement*          add(GUIPrim::IElement*);
		GUIPrim::IElementContainer* add(GUIPrim::IElementContainer*);
		void render();
		void update();

		const std::vector<ElementType>& getElems() const;

		ElementType& getElem(int index);
	private:
		void renderElement(GUIPrim::IElement*);
		Shader* shad;

		std::vector<ElementType> elems;

		Mesh* rect;
		Transform transform;

		bool sortedByLayer;
};

#endif
