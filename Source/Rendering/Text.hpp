#ifndef SWAN_TEXT_HPP
#define SWAN_TEXT_HPP

#include "BitmapFont.hpp" // For BitmapFont
#include "Mesh.hpp"       // For Mesh
#include "Shader.hpp"     // For Shader
#include "Texture.hpp"    // For Texture

#include "Physics/Transform.hpp"

#include "Utility/Debug.hpp" // For SWAN_DEBUG_DO(), SWAN_DEBUG_VAR

#include <string> // For std::string
namespace SWAN {
struct TextConfig {
	std::string text;
	const BitmapFont* font;
	bool bold       = false;
	bool italics    = false;
	glm::vec3 color = glm::vec3(1, 1, 1);
};

struct Character {
	SWAN_DEBUG_VAR(char, c);
	bool isSpace = false;

	const Texture* tex = nullptr;
	const Mesh* mesh   = nullptr;
	glm::vec3 color    = glm::vec3(0, 0, 0);
	Transform transform;
};

class Text {
  public:
	Text() { initCharMesh(); }

	Text(TextConfig tc) {
		set(tc);
		initCharMesh();
	}

	~Text() { delete charMesh; }

	void set(TextConfig tc, bool append = false);

	void render(Shader* shad) const;

  private:
	const BitmapFont* font;
	std::vector<Character> chars;
	int lastX = 0, lastY = 0;

	void initCharMesh();

	Mesh* charMesh;
};

void RenderText(int x, int y, std::string text, Shader* s, const BitmapFont* f);
}

#endif
