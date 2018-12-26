#define SDL_main_h_ // SDL_Main might be very useful, but I find it to be pesky.

#include "AssimpImport.hpp" // For AssimpImport()

#include "SWAN/Core/Display.hpp"       // For SWAN::Display
#include "SWAN/Core/EventListener.hpp" // For SWAN::EventListener
#include "SWAN/Core/Input.hpp"         // For SWAN_Input
#include "SWAN/Core/Resources.hpp"     // For SWAN::Res::LoadFromFile(), SWAN::Res::Get*()

#include "SWAN/Physics/Transform.hpp" // For SWAN::Transform

#include "SWAN/Rendering/Camera.hpp"  // For SWAN::Camera
#include "SWAN/Rendering/Mesh.hpp"    // For SWAN::Mesh
#include "SWAN/Rendering/Shader.hpp"  // For SWAN::Shader
#include "SWAN/Rendering/Text.hpp"    // For SWAN::Text
#include "SWAN/Rendering/Texture.hpp" // For SWAN::Texture

#include "SWAN/Maths/Vector.hpp"

#include "SWAN/Utility/AngleUnits.hpp" // For SWAN::Radians, SWAN::Degrees
#include "SWAN/Utility/Debug.hpp"      // For SWAN_DEBUG_DO(), SWAN_DEBUG_VAR
#include "SWAN/Utility/StreamOps.hpp"      // For SWAN_DEBUG_DO(), SWAN_DEBUG_VAR
#include "SWAN/Utility/Math.hpp"       // For SWAN::Util::PixelToGLCoord()

#include <cassert>   // For std::assert()
#include <algorithm> // For std::generate()
#include <chrono>    // For std::chrono::*
#include <iostream>  // For std::cout
#include <map>       // For std::map<K, V>
#include <random>    // For std::random_device
#include <sstream>   // For std::stringstream
#include <string>    // For std::string
#include <vector>    // For std::vector<T>

using namespace std::chrono;
using std::cout;
using std::map;
using std::vector;

using SWAN::Util::Radians;
using namespace SWAN::Util::StreamOps;

/// Typedef for floating point miliseconds
using fms = std::chrono::duration<float, milliseconds::period>;

std::random_device rd;

/// Generate a random transformation.
SWAN::Transform getRandTransform() {
    std::uniform_real_distribution<double> d(-75, 75);
    std::uniform_real_distribution<double> d2(1, 7.5);

    return SWAN::Transform(SWAN::vec3(d(rd), d(rd), d(rd)),
			   SWAN::vec3(d(rd), d(rd), d(rd)));
}

#if 1
#define GLERR(func)				\
    do {					\
	(func);					\
	_CheckGLError(SWAN_STRINGIFY(func));	\
    } while(0)
#else
#define GLERR(func) (func)
#endif

/// Checks and reports an OpenGL error.
void _CheckGLError(std::string funcCall) {
    cout << "    Checking GL errors...\n";
    map<GLenum, std::string> errors = {
	{ 0x0500, "GL_INVALID_ENUM" },
	{ 0x0501, "GL_INVALID_VALUE" },
	{ 0x0502, "GL_INVALID_OPERATION" },
	{ 0x0503, "GL_STACK_OVERFLOW" },
	{ 0x0504, "GL_STACK_UNDERFLOW" },
	{ 0x0505, "GL_OUT_OF_MEMORY" },
	{ 0x0506, "GL_INVALID_FRAMEBUFFER_OPERATION" },
	{ 0x0507, "GL_CONTEXT_LOST" }
    };

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
	cout << "OpenGL error:\n"
	     << "    type: " << errors[err] << "(" << err << ")\n"
	     << "    func: " << funcCall << '\n';
    }
}

/// Initialize a display, input, OpenGL and resources.
void Init() {
    SWAN::Display::Init(1280, 720, "SWAN-FPS");
    SWAN::Display::SetClearColor(0.5f, 0.3f, 0.25f, 0.0f);
    SWAN_Input_Init();

    GLERR(glEnable(GL_PROGRAM_POINT_SIZE));
    GLERR(glEnable(GL_DEPTH_TEST));
    GLERR(glEnable(GL_CULL_FACE));
    GLERR(glCullFace(GL_FRONT));

    GLERR(glEnable(GL_BLEND));
    GLERR(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    SWAN::Res::LoadFromFile("Resources/res.xml");
}

/// Track inputs for camera control.
void CamControl(SWAN::Camera& cam, double speed = 0.001, double sensitivity = 2.0) {
    if(SWAN_Input.Keyboard.letterKeys['w' - 'a'])
	cam.moveForw(speed);
    else if(SWAN_Input.Keyboard.letterKeys['s' - 'a'])
	cam.moveForw(-speed);

    if(SWAN_Input.Keyboard.letterKeys['a' - 'a'])
	cam.moveRight(speed);
    else if(SWAN_Input.Keyboard.letterKeys['d' - 'a'])
	cam.moveRight(-speed);

    if(SWAN_Input.Keyboard.spaceKey)
	cam.moveUp(-speed);
    else if(SWAN_Input.Keyboard.LShiftKey)
	cam.moveUp(speed);

    auto mouseX     = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetCurrMouseState().x);
    auto prevMouseX = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetPrevMouseState().x);
    cam.rotateByY(-SWAN::Util::Radians((mouseX - prevMouseX) * sensitivity));

    auto mouseY     = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetCurrMouseState().y);
    auto prevMouseY = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetPrevMouseState().y);
    cam.rotateByX(SWAN::Util::Radians((mouseY - prevMouseY) * sensitivity));
}

/// Pointer to a texture that indicates a lack of a proper texture
const SWAN::Texture* emptyTex = nullptr;

/// Render a node's actors.
void RenderNode(SWAN::Shader* shader, const Node* node) {
    assert(shader);
    assert(node);
    shader->setUniform({ "transform", node->getMat() });

    // Render actors
    for(const auto& actor : node->actors) {
	if(actor.tex)
	    actor.tex->bind();
	else
	    emptyTex->bind();
	shader->renderMesh(*actor.mesh);
    }

    // Render children
    for(const auto& pc : node->children)
	RenderNode(shader, pc.get());
}

/// Render a Scene.
void Render(SWAN::Shader* shader, const Scene& scene) {
    assert(shader);
    assert(scene.root.get());
    RenderNode(shader, scene.root.get());
}

int main() {
    Init();

    Scene scene = AssimpImport("Resources/Scene.dae");

    SWAN::Shader* unlitShader = SWAN::Res::GetShader("Unlit");
    SWAN::Shader* fogShader = SWAN::Res::GetShader("Fog");
    SWAN::Shader* shader = fogShader;

    emptyTex = SWAN::Res::GetTexture("Error");

    const SWAN::Mesh* planeMesh = SWAN::Res::GetMesh("Plane");
    const SWAN::Texture* planeTex  = SWAN::Res::GetTexture("Place");

    const SWAN::Mesh* cubeMesh = SWAN::Res::GetMesh("1x1 Cube");
    const SWAN::Texture* placeTex = SWAN::Res::GetTexture("Place");

    SWAN::Camera cam (SWAN::Display::GetAspectRatio(),
		      SWAN::vec3(0.0f, 0.0f, 0.0f),
		      SWAN::Util::Radians::FromDegrees(90.0f),
		      0.001, 1000.0);

    bool running = true;
    SWAN::EventListener exitEvent([]() {return SWAN_Input.Keyboard.escapeKey || SWAN_Input.Window.exitRequest;},
				  [&running]() { running = false; }, false);

    auto prevTime = steady_clock::now();

    // Hacky way to set type to duration
    auto frameTime = prevTime - prevTime;

    int nCubes = 100;
    std::vector<SWAN::Transform> transforms(nCubes);
    std::generate(transforms.begin(), transforms.end(), getRandTransform);

    while(running) {
	exitEvent();
	SWAN_Input.handleEvents();

	CamControl(cam);

	auto now = steady_clock::now();

	SWAN::Text text("", SWAN::Res::GetBitmapFont("Monospace 12"));
	if(now - prevTime >= 16ms) {
	    std::vector<SWAN::ShaderUniform> unis = {
		{ "view", cam.getView() },
		{ "perspective", cam.getPerspective() },
		{ "fogColor", (SWAN::vec3) SWAN::Display::GetClearColor() },
		{ "fogZ", 0.85f * cam.zFar }
	    };
	    shader->setUniforms(unis);

	    placeTex->bind();
	    Render(shader, scene);

	    std::stringstream info;
	    info << "Frame time: " << duration_cast<fms>(frameTime).count() << " ms\n"
		 << "FPS: " << 1000 / duration_cast<fms>(frameTime).count() << '\n'
		//<< "# rendered cubes: " << nRenderedCubes << " out of " << nCubes << '\n'
		 << "Camera: " << cam.pos() << '\n'
		 << "     View: " << SWAN::Transpose(cam.getView()) << '\n'
		;

	    text.text = info.str();
	    text.updateVAO();

	    GLERR(glClear(GL_DEPTH_BUFFER_BIT));
	    text.render(SWAN::Res::GetShader("Text"), 0, 0, SWAN::vec4(1, 1, 1, 1));

	    GLERR(glClear(GL_DEPTH_BUFFER_BIT));
	    SWAN::Display::Clear();

	    now       = steady_clock::now();
	    frameTime = now - prevTime;
	    prevTime  = now;
	    SWAN::SetMousePos(SWAN::Display::detail::width / 2, SWAN::Display::detail::height / 2);
	}
    }

    SWAN::Display::Close();

    return 0;
}
