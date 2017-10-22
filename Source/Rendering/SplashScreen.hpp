#ifndef SWAN_SPLASHSCREEN_HPP
#define SWAN_SPLASHSCREEN_HPP

#include "Shader.hpp"
#include "Texture.hpp"
#include "Utility/OnExit.hpp"

namespace SWAN {
	Mesh* __genRect2D_ptr() {
		Vertex v0(glm::vec3(-1,  1, 0), glm::vec2(0, 1), glm::normalize(glm::vec3(-0.5, -0.5, 0.5)));
		Vertex v1(glm::vec3( 1,  1, 0), glm::vec2(1, 1), glm::normalize(glm::vec3( 0.5,  0.5, 0.5)));
		Vertex v2(glm::vec3(-1, -1, 0), glm::vec2(0, 0), glm::normalize(glm::vec3( 0.5, -0.5, 0.5)));
		Vertex v3(glm::vec3( 1, -1, 0), glm::vec2(1, 0), glm::normalize(glm::vec3(-0.5,  0.5, 0.5)));

		return new Mesh({v0, v1, v2, v3}, {0, 2, 1, 1, 2, 3});
	}

	struct SplashScreen {
		const Texture* tex;

		void render(Shader* shader, float fade) {
			static const Mesh* m = __genRect2D_ptr();
			static Util::OnExit([&m] { delete m; });

			shader->setUniform(ShaderUniform("fade", fade));
			shader->renderMesh(m);
		}
	};
}

#endif
