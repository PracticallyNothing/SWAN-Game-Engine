#include "Text.hpp"

#include "../Utility/Math.hpp"  // For Util::GLToPixelCoord()
#include "../Utility/Debug.hpp" // For DEBUG_DO()

Mesh* genRect2D_ptr() {
    Vertex v1(glm::vec3(-0.5, -0.5, 0), glm::vec2(0, 0),
              glm::normalize(glm::vec3(-0.5, -0.5, 0.5)));
    Vertex v2(glm::vec3(0.5, 0.5, 0), glm::vec2(1, 1),
              glm::normalize(glm::vec3(0.5, 0.5, 0.5)));
    Vertex v3(glm::vec3(0.5, -0.5, 0), glm::vec2(1, 0),
              glm::normalize(glm::vec3(0.5, -0.5, 0.5)));
    Vertex v4(glm::vec3(-0.5, 0.5, 0), glm::vec2(0, 1),
              glm::normalize(glm::vec3(-0.5, 0.5, 0.5)));

    return new Mesh({v1, v2, v3, v4}, {0, 1, 2, 0, 3, 1});
}

void Text::initCharMesh(){
	charMesh = genRect2D_ptr();
}

// TODO: WAY TOO MANY ARGUMENTS
void Text::set(const Display& d, TextConfig tc, bool append){
	this->font = tc.font;

	if(!append)
		chars.clear();
	
	int currLine = 0;
	int ii = 0;

	int x = 0;
	int y = 0;

	for(unsigned i = 0; i < tc.text.length(); i++){
		char c = tc.text[i];

		x = (append ? lastX : 0) + font->getGlyphWidth(c) * (i - ii);
		y = (append ? lastY : 0) + font->getGlyphHeight() * currLine;
		
		if(c == ' '){
			Character res;
			res.isSpace = true;
			chars.push_back(res);
			continue;
		} else if (c == '\n') {
			lastX = 0;
			currLine++;
			ii = i+1;
			continue;
		}

		Character res;

		res.tex = font->getGlyphTexture(c, tc.bold, tc.italics);
		res.mesh = charMesh;
		res.color = tc.color;
		res.transform = Transform(glm::vec3(
							      Util::pixelToGLCoord(d.getW(), x + font->getGlyphWidth(c) / 2.0), 
								  Util::pixelToGLCoord(d.getH(), d.getH() - y - font->getGlyphHeight() / 2.0), 
								  0), 
								  glm::vec3(0,0,0),
								  glm::vec3((double) font->getGlyphWidth(c) / d.getW(),
									 		(double) font->getGlyphHeight() / d.getH(),	0));
		
		chars.push_back(res);

	}
	if(chars.size()){
		lastX = x;
		lastY = y;
	}
}

void Text::render(Shader* shad) const {
	shad->use();
		
	for(unsigned i = 0; i < chars.size(); i++){
		if(chars[i].isSpace)
			continue;
		
		shad->setUniformData("model", chars[i].transform.getModel());
		shad->setUniformData("color", chars[i].color);

		chars[i].tex->Bind();
		chars[i].mesh->render();
	}

	shad->unuse();
}
