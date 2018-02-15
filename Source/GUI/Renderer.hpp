#ifndef SWAN_GUIRENDERER_HPP
#define SWAN_GUIRENDERER_HPP

#include "Element.hpp" // For SWAN::GUIElement

#include "OpenGL/VAO.hpp"        // For SWAN::GL::VAO
#include "Physics/Transform.hpp" // For SWAN::Transform
#include "Rendering/Mesh.hpp"    // For SWAN::Mesh
#include "Rendering/Shader.hpp"  // For SWAN::Shader

#include <memory> // For std::unique_ptr<T>
#include <vector> // For std::vector<T>

namespace SWAN {
namespace GUI {
	struct LayerSorter {
		bool operator()(Element* a, Element* b) {
			if(a->layer != b->layer)
				return a->layer > b->layer;
			else
				return a->sublayer > b->sublayer;
		}
	};

	class Renderer {
		template <typename T>
		using Vector = std::vector<T>;

		template <typename T>
		using UniqPtr = std::unique_ptr<T>;

	  public:
		/// Standard constructor
		Renderer();

		/// Constructor that sets the rendering shader to s
		Renderer(Shader* s);

		/// Store a GUI element
		Element* add(UniqPtr<Element>);

		/**
		 * Sets the shader used for rendering GUI elements.
		 *
		 * To fully be able to render the standard elements, the Shader needs to
		 * have the following uniforms:
		 *     - "type"      - Type of the element, see GUIElement::Type
		 *     - "transform" - transformation of the item on the screen
		 *     - "color1"    - Color / Gradient top / Gradient left
		 *     - "color2"    - Gradient bottom / Gradient right
		 *     - a sampler2D - texture
		 *
		 * Optional support may be provided for the following uniforms:
		 *     - "topLeft"     - position of the top left vertex of the GUI element
		 *     - "bottomRight" - position of the bottom right vertex of the GUI element
		 */
		inline void setShader(Shader* s) { shad = s; }

		/// Returns the current shader
		inline const Shader* getShader() { return shad; }

		/// Render all stored elements
		void render();
		/// Update all stored elements
		void update();

	  private:
		/// Shader used for rendering
		Shader* shad = nullptr;

		void initMesh();

		/// Storage for all elements
		Vector<UniqPtr<Element>> elems;

		/// Function that handles the rendering of a single GUI element
		void renderElement(const Element*);

		GL::VAO vao;

		bool sortedByLayer;
	};
} // namespace GUI
} // namespace SWAN

#endif
