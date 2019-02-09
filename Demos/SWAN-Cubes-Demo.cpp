#define SDL_main_h_ // SDL_Main might be very useful, but I find it to be pesky.

#include "SWAN/Core/Defs.hpp"
#include "SWAN/Core/Display.hpp"       // For SWAN::Display
#include "SWAN/Core/EventListener.hpp" // For SWAN::EventListener
#include "SWAN/Core/Format.hpp"
#include "SWAN/Core/Logging.hpp"
#include "SWAN/Core/Resources.hpp" // For SWAN::Res::LoadFromFile(), SWAN::Res::Get*()

#include "SWAN/Physics/Basic.hpp"
#include "SWAN/Physics/Transform.hpp" // For SWAN::Transform

#include "SWAN/Input/Event.hpp"
#include "SWAN/Input/InputFrame.hpp"

#include "SWAN/Rendering/Camera.hpp"      // For SWAN::Camera
#include "SWAN/Rendering/DebugRender.hpp" // For SWAN::Render(...)
#include "SWAN/Rendering/Mesh.hpp"        // For SWAN::Mesh
#include "SWAN/Rendering/Shader.hpp"      // For SWAN::Shader
#include "SWAN/Rendering/SpriteSheet.hpp" // For SWAN::SpriteSheet
#include "SWAN/Rendering/Text.hpp"        // For SWAN::Text
#include "SWAN/Rendering/Texture.hpp"     // For SWAN::Texture

#include "SWAN/GUI/GUIManager.hpp"
#include "SWAN/GUI/List.hpp"
#include "SWAN/GUI/TextBox.hpp"
#include "SWAN/GUI/VisualDebugger.hpp"

#include "SWAN/OpenGL/OnGLInit.hpp" // For SWAN::GL::OnGLInit

#include "SWAN/OpenAL/SoundSystem.hpp" // For SWAN::SoundSystem

#include "SWAN/Utility/AngleUnits.hpp" // For SWAN::Radians, SWAN::Degrees
#include "SWAN/Utility/Debug.hpp"      // For SWAN_DEBUG_DO(), SWAN_DEBUG_VAR
#include "SWAN/Utility/Math.hpp"       // For SWAN::Util::PixelToGLCoord()
#include "SWAN/Utility/StreamOps.hpp"

#include <algorithm> // For std::generate()
#include <cassert>   // For assert()
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

typedef duration<float, milliseconds::period> fms;

std::random_device rd;
std::uniform_real_distribution<double> d(0, 1);
SWAN::vec4 RandomColor() { return SWAN::vec4(d(rd), d(rd), d(rd), 1.0); }

void _CheckGLError(std::string funcCall)
{
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
		     << "    type: " << errors[err] << '\n'
		     << "    func: " << funcCall << '\n';
	}
}

void Init()
{
	SWAN::Display::Init(1280, 720, "SWAN-FPS");
	SWAN::Display::SetClearColor(0.5f, 0.3f, 0.25f, 0.0f);

	//glFrontFace(GL_CW);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SWAN::Res::LoadFromFile("Resources/res.xml");
}

void PrintMatrices(SWAN::Transform transf)
{
	SWAN::LogVar("Matrix|Translation", "Translation matrix", transf.getPosMat(), SWAN::LogLevel::Debug);
	SWAN::LogVar("Matrix|Rotation", "Rotation matrix", transf.getRotMat(), SWAN::LogLevel::Debug);
	SWAN::LogVar("Matrix|Scale", "Scale matrix ", transf.getScaleMat(), SWAN::LogLevel::Debug);
	SWAN::LogVar("Matrix|Model", "Model matrix", transf.getModel(), SWAN::LogLevel::Debug);
}

struct CameraController : SWAN::InputFrame {
	CameraController(SWAN::Camera& c, bool& r) : cam(c), running(r) {}

	void OnKeyPress(SWAN::Key key) override
	{
		if(key.Code == SWAN::Escape)
			running = false;

		if(key.Code == SWAN::G)
			SWAN::Display::Resize(640, 480);
		else if(key.Code == SWAN::H)
			SWAN::Display::Resize(1280, 720);
		else if(key.Code == SWAN::Y)
			SWAN::Display::Resize(320, 240);
	}

	void OnKeyRepeat(SWAN::Key key) override
	{
	}

	void OnKeyRelease(SWAN::Key key) override
	{
		// SWAN::Log("KeyRelease", SWAN::GetKeyName(key));
	}

	void OnKeyHold(SWAN::Key key) override
	{
		// SWAN::Log("KeyHold", SWAN::GetKeyName(key));
		if(key.Code == SWAN::W)
			cam.moveForw(speed);
		if(key.Code == SWAN::S)
			cam.moveForw(-speed);
		if(key.Code == SWAN::A)
			cam.moveRight(-speed);
		if(key.Code == SWAN::D)
			cam.moveRight(speed);
		if(key.Code == SWAN::Spacebar)
			cam.moveUp(speed);
		if(key.Code == SWAN::LShift)
			cam.moveUp(-speed);
	}

	void OnMouseMove(SWAN::MouseMove move) override
	{
		// if(move.RelativeX) SWAN::LogVar<int>("X", move.RelativeX);
		// if(move.RelativeY) SWAN::LogVar<int>("Y", move.RelativeY);

		if(SWAN::GetCurrentInputFrame() == this) {
			cam.rotateByY(SWAN::Util::Radians::FromDegrees(move.RelativeX * sensitivityX));
			cam.rotateByX(SWAN::Util::Radians::FromDegrees(move.RelativeY * sensitivityY));

			SWAN::WarpMouseTo(SWAN::Display::GetWidth() / 2, SWAN::Display::GetHeight() / 2);
		}
	}

	void OnWindowResize(SWAN::WindowResize wr) override
	{
		//InputFrame::OnWindowResize(wr);
	}

	SWAN::Camera& cam;
	bool& running;
	double speed = 0.001;
	double sensitivityX = 0.25, sensitivityY = sensitivityX * SWAN::Display::GetAspectRatio();
};

void RenderString(std::string s, int x, int y, const SWAN::SpriteSheet& ss)
{
	using SWAN::ivec2;

	ivec2 cursor = ivec2(x, y);
	ivec2 dim = ss[0].dimensions;
	const unsigned tabWidth = 4;

	for(char c : s) {
		if(c == '\n') {
			cursor.y += dim.y;
			cursor.x = x;
		} else if(c == '\t') {
			cursor.x += dim.x * tabWidth;
		} else {
			if(std::isprint(c) && c != ' ')
				ss.at(c - '!').renderOverride({ cursor.x, cursor.y }, { -1, -1 }, { 0.3, 1, 0.3 });
			cursor.x += dim.x;
		}
	}
}

void DrawXYZ(const SWAN::Camera& cam, SWAN::vec3 pos, float s = 1)
{
	SWAN::Render(cam, SWAN::DrawnLine(pos, pos + SWAN::vec3(s, 0, 0), SWAN::vec4(1, 0, 0, 1))); // Draw X
	SWAN::Render(cam, SWAN::DrawnLine(pos, pos + SWAN::vec3(0, s, 0), SWAN::vec4(0, 1, 0, 1))); // Draw Y
	SWAN::Render(cam, SWAN::DrawnLine(pos, pos + SWAN::vec3(0, 0, s), SWAN::vec4(0, 0, 1, 1))); // Draw Z
}

const char* const unlitFrag = "#version 130"
                              "\n"
                              ""
                              "out vec4 fCol;"
                              "\n"
                              ""
                              "void main() { fCol = vec4(0.3, 0.3, 0.3, 1.0); }";

const char* const unlitVert = "#version 130"
                              "\n"
                              ""
                              "in vec3 pos;"
                              "\n"
                              ""
                              "uniform mat4 perspective;"
                              "\n"
                              "uniform mat4 view;"
                              "\n"
                              "uniform mat4 transform;"
                              "\n"
                              ""
                              "void main() {"
                              "\n"
                              "    gl_Position = perspective * view * transform * vec4(pos, 1);"
                              "\n"
                              "    gl_PointSize = 20;"
                              "\n"
                              "}";

struct Grid {
	Grid()
	{
		SWAN::OnGLInit _ = {
			[this] {
			    shader.compileShadersFromSrc(unlitVert, unlitFrag);
			    shader.addAttrib("pos");
			    shader.linkShaders();

			    shader.use();
			    shader.addUniform("perspective");
			    shader.addUniform("view");
			    shader.addUniform("transform");
			    shader.unuse();
			}
		};
	}

	SWAN::GL::VAO vao;
	SWAN::Shader shader;

	double margin = 1.0;
	int numLines = 20;

	void generate()
	{
		std::vector<SWAN::fvec3> p;

		for(int z = 0; z < numLines; z++) {
			for(int x = 0; x < numLines; x++) {
				SWAN::vec3 start(margin * x, 0.0, margin * z);
				p.push_back(start - SWAN::vec3(1000, 0, 0));
				p.push_back(start + SWAN::vec3(1000, 0, 0));

				p.push_back(start - SWAN::vec3(0, 0, 1000));
				p.push_back(start + SWAN::vec3(0, 0, 1000));
			}
		}
		vao.storeAttribData(0, 3, (float*) p.data(), p.size() * sizeof(SWAN::fvec3));

		shader.use();
		shader.SetMat4("transform", SWAN::Transform().getModel());
	}

	void render(const SWAN::Camera& cam)
	{
		shader.use();
		shader.SetMat4("perspective", cam.getPerspective());
		shader.SetMat4("view", cam.getView());
		vao.draw(numLines * numLines * 4, GL_LINES);
		shader.unuse();
	}
};

int main()
{
	SWAN::SetLogLevel(SWAN::LogLevel::Debug);

	Init();

	SWAN::SoundSystem sys;
	auto src = sys.AddSourceWithBuffer("Resources/Sound/music.wav");
	src->SetRelative(true);

	SWAN::Plane plane(SWAN::vec3(0, 1, 0));

	SWAN::Res::LoadTexture("Resources/Fonts/Monospace_16.png", "TestSpriteSheet");

	assert(SWAN::Res::GetTexture("TestSpriteSheet") != nullptr);
	SWAN::SpriteSheet ss;
	ss.genSprites(SWAN::Res::GetTexture("TestSpriteSheet"), 15, 29);
	bool lockMouse = true;

	SWAN::Camera cam(SWAN::Display::GetAspectRatio(),
	                 SWAN::vec3(0.0f, 00.0f, 0.0f),
	                 SWAN::Util::Radians::FromDegrees(90.0f),
	                 0.01f, 10000.0f);

	bool running = true;

	CameraController camContr(cam, running);
	SWAN::SetCurrentInputFrame(&camContr);

	unsigned i = 0;
	auto prevTime = steady_clock::now();

	// Hacky way to set type to duration
	auto frameTime = prevTime - prevTime;

	unsigned long count = 0;

	Grid grid;
	grid.margin = 5;
	grid.numLines = 50;
	grid.generate();

	SWAN::Text
	    text("", SWAN::Res::GetBitmapFont("Terminus 10")),
	    text2 = text;

	SWAN::Transform cubeTransform({ 2, -2, 5 });
	SWAN::SoundSystem s2;

	// SWAN::Pointer<SWAN::TextBox> tb = std::make_unique<SWAN::TextBox>(
	// 	SWAN::Rect2D(10, 10, 1000, 100, {0.3, 0.3, 0.3, 0.5}),
	// 	SWAN::Res::GetBitmapFont("Monospace 16"),
	// 	""
	// );

	// SWAN::Pointer<SWAN::List> list = std::make_unique<SWAN::List>(
	// 	SWAN::Res::GetBitmapFont("Monospace 16"),
	// 	300, 10
	// );

	// list->AddLine("  Play  ");
	// list->AddLine("Settings");
	// list->AddLine("  Exit  ");

	//SWAN::SetCurrentInputFrame(&camContr);

	SWAN::GUIManager renderer;
	SWAN::Pointer<SWAN::VisualDebugger> debugger
	    = std::make_unique<SWAN::VisualDebugger>(SWAN::Res::GetBitmapFont("Monospace 12"));
	SWAN::SetCurrentInputFrame(&renderer);
	// renderer.SetExtraInputFrame(&camContr);
	// renderer.AddElement(tb.get());
	// renderer.AddElement(list.get());
	renderer.AddElement(debugger.get());

	double time = 0;

	while(running) {
		SWAN::UpdateInputEvents();

		auto now = steady_clock::now();

		if(now - prevTime >= 16ms) {
			time += 0.01;
			count++;

			grid.render(cam);

			std::stringstream info;
			info << "Frame time: " << duration_cast<fms>(frameTime).count() << " ms\n"
			     << "FPS: " << 1000 / duration_cast<fms>(frameTime).count() << '\n'
			     << "LMB - spawn a ray\n"
			     << "RMB - clear all rays.\n"
			     << "Camera looking at: " << -cam.forw() << '\n'
			     << "i: " << i << '\n';

			SWAN::Intersection inters = SWAN::FindIntersection(plane, SWAN::Ray(cam.pos(), cam.forw()));
			if(inters)
				info << "Pointing at: " << inters.point << '\n';
			else
				info << "Not pointing at anything.\n";

			glClear(GL_DEPTH_BUFFER_BIT);

			auto cube = SWAN::Cube(cubeTransform);
			cube.color = { 0.3, 1, 0.3, 1 };
			// cube.transform.rot.x = sin(time) * M_PI;
			//cube.transform.setParent(&tf);

			auto cube2 = SWAN::Cube(SWAN::Transform(SWAN::vec3(0, 0, 3)));
			cube2.color = SWAN::vec4(1, 0, 0, 1);
			cube2.transform.setParent(&cube.transform);
			cube2.transform.rot.x = sin(time) * M_PI_2;

			auto cube3 = SWAN::Cube(SWAN::Transform(SWAN::vec3(0, 0, 3)));
			cube3.color = SWAN::vec4(0, 0, 1, 1);
			cube3.transform.setParent(&cube2.transform);
			cube3.transform.rot.x = sin(time) * M_PI_2;

			auto cube4 = SWAN::Cube(SWAN::Transform(SWAN::vec3(0, 0, 3)));
			cube4.color = SWAN::vec4(1, 0, 1, 1);
			cube4.transform.setParent(&cube3.transform);

			cube.transform.setParent(&cam.transform);

			SWAN::Render(cam, cube, SWAN::DefaultFramebuffer, false);
			SWAN::Render(cam, cube2, SWAN::DefaultFramebuffer, false);
			SWAN::Render(cam, cube3, SWAN::DefaultFramebuffer, false);
			SWAN::Render(cam, cube4, SWAN::DefaultFramebuffer, false);

			SWAN::DrawnLine dl(
			    cube.transform.pos,
			    cube.transform.pos + cube.transform.getForw() * 5,
			    SWAN::vec4(1, 0, 0, 1));
			SWAN::DrawnLine dl2(
			    cube.transform.pos,
			    cube.transform.pos + cube.transform.getUp() * 5,
			    SWAN::vec4(0, 1, 0, 1));
			SWAN::Render(cam, dl);
			SWAN::Render(cam, dl2);
			// console.Render();

			auto* font = SWAN::Res::GetBitmapFont("Monospace 16");
			// renderer.RenderText(0, 0, font, SWAN::Format("width: {}, height: {}", font->getGlyphWidth(), font->getGlyphHeight()));
			// renderer.RenderRect2D(SWAN::Rect2D(6, font->getGlyphHeight(), 2, font->getGlyphHeight(), {1,1,1,1}));
			renderer.RenderElements();

			//text = info.str();
			//text.render(0, 0);

			glClear(GL_DEPTH_BUFFER_BIT);
			SWAN::Display::Clear();

			now = steady_clock::now();
			frameTime = now - prevTime;
			prevTime = now;
			// if(lockMouse)
			// 	SWAN::WarpMouseTo(SWAN::Display::detail::width / 2, SWAN::Display::detail::height / 2);
		}
	}

	SWAN::Log("Number of frames rendered: " + std::to_string(count), SWAN::LogLevel::Info);
	SWAN::Log("Number of OpenGL VAO initializations: " + std::to_string(SWAN::GL::detail::numInits));
	SWAN::Log("Number of OpenGL VAO deletions: " + std::to_string(SWAN::GL::detail::numDeletions));
	SWAN::Log("Number of OpenGL VAO bind calls: " + std::to_string(SWAN::GL::detail::numBinds));
	SWAN::Log("Number of OpenGL VAO draw calls: " + std::to_string(SWAN::GL::detail::numDrawCalls));

	SWAN::Display::Close();

	return 0;
}
