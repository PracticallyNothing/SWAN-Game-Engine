#define SDL_MAIN_HANDLED

#include <chrono>
#include <cmath>
#include <random>
#include <set>
#include <unordered_set>

#include <bullet/btBulletDynamicsCommon.h>

#include "SWAN/Core/Defs.hpp"
#include "SWAN/Core/Format.hpp"

// ----- Resource Loading ----- //
#include "SWAN/Core/Resources.hpp"

// ----- Video and Audio ----- //
#include "SWAN/Core/Display.hpp"       // For SWAN::Display::*
#include "SWAN/OpenAL/SoundSystem.hpp" // For SWAN::SoundSystem

// ----- Rendering stuff ----- //
#include "SWAN/Rendering/Camera.hpp"      // For SWAN::Camera
#include "SWAN/Rendering/DebugRender.hpp" // For SWAN::Render()

// ----- GUI ----- //
#include "SWAN/GUI/GUIManager.hpp"
#include "SWAN/GUI/IGUIElement.hpp"

// ----- OpenGL ----- //
#include "SWAN/OpenGL/OnGLInit.hpp" // For SWAN::OnGLInit()

// ----- Input ----- //
#include "SWAN/Input/Event.hpp"      // For SWAN::UpdateInputEvents();
#include "SWAN/Input/InputFrame.hpp" // For SWAN::InputFrame

using fms = std::chrono::duration<double, std::milli>;
using FloatSeconds = std::chrono::duration<double>;

bool GameRunning = false;
void Stop() { GameRunning = false; }

SWAN::vec3 ToSWANVector(btVector3 v) { return { v.getX(), v.getY(), v.getZ() }; }
SWAN::vec4 ToSWANVector(btVector4 v) { return { v.getX(), v.getY(), v.getZ(), v.getW() }; }
btVector3 ToBulletVector(SWAN::vec3 v) { return btVector3(v.x, v.y, v.z); }
btVector4 ToBulletVector(SWAN::vec4 v) { return btVector4(v.x, v.y, v.z, v.w); }

class InputHandler : public SWAN::InputFrame
{
  public:
	InputHandler(SWAN::Camera& c) : cam(c) {}

	void OnKeyPress(SWAN::Key key) override
	{
		if(key.Code == SWAN::Escape)
			Stop();
		if(key.Code == SWAN::J && body) {
			body->activate(true);
			body->applyCentralImpulse({ 0, 2, 0 });
			// body->applyImpulse({0, 20000000, 0}, {0,-1,0});
			SWAN::Log(SWAN::Format("{}", ToSWANVector(body->getLinearVelocity())));
		}
	}

	void OnKeyRepeat(SWAN::Key key) override {}
	void OnKeyRelease(SWAN::Key key) override {}

	void OnKeyHold(SWAN::Key key) override
	{
		if(key.Code == SWAN::W)
			cam.moveForw(speed);
		if(key.Code == SWAN::S)
			cam.moveForw(-speed);

		if(key.Code == SWAN::A)
			cam.moveRight(-speed);
		if(key.Code == SWAN::D)
			cam.moveRight(speed);

		SWAN::GetKeyName()

		if(key.Code == SWAN::Spacebar)
			cam.moveUp(speed);
		if(key.Code == SWAN::LShift)
			cam.moveUp(-speed);
	}

	void OnMouseMove(SWAN::MouseMove move) override
	{
		if(SWAN::GetCurrentInputFrame() == this) {
			cam.rotateByY(
			    SWAN::Util::Radians::FromDegrees(
			        move.RelativeX * sensitivityX));

			cam.rotateByX(
			    SWAN::Util::Radians::FromDegrees(
			        move.RelativeY * sensitivityY));

			SWAN::WarpMouseTo(
			    SWAN::Display::GetWidth() / 2,
			    SWAN::Display::GetHeight() / 2);
		}
	}

	void OnWindowResize(SWAN::WindowResize wr) override {}

	void OnWindowExit() override { Stop(); }

	btRigidBody* body = nullptr;
	SWAN::Camera& cam;
	double speed = 0.001;
	double sensitivityX = 0.125,
	       sensitivityY = sensitivityX * SWAN::Display::GetAspectRatio();
};

const char* const unlitFrag = R"ddd(
#version 130

out vec4 fCol;

void main() { 
    fCol = vec4(0.3, 0.3, 0.3, 1.0); 
}
)ddd";

const char* const unlitVert = R"ddd(
#version 130
in vec3 pos;

uniform mat4 perspective;
uniform mat4 view;
uniform mat4 transform;

void main() {
    gl_Position = perspective * view * transform * vec4(pos, 1);
    gl_PointSize = 20;
}
)ddd";

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
	}

	void render(const SWAN::Camera& cam)
	{
		shader.use();
		shader.SetMat4("transform", SWAN::Transform().getModel());
		shader.SetMat4("perspective", cam.getPerspective());
		shader.SetMat4("view", cam.getView());
		vao.draw(numLines * numLines * 4, GL_LINES);
	}
};

SWAN::Pointer<btRigidBody> CreatePlane()
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin({ 0, 0, 0 });

	btScalar mass = 0;
	btVector3 localInertia;

	btCollisionShape* groundShape = new btBoxShape({ 1000, 0, 1000 });
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(transform);

	groundShape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo groundRBInfo(mass, groundMotionState, groundShape, localInertia);
	return std::make_unique<btRigidBody>(groundRBInfo);
}

SWAN::Pointer<btRigidBody> CreateCube(SWAN::vec3 pos, SWAN::Cube* user)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(ToBulletVector(pos));
	btQuaternion rotation;
	rotation.setEulerZYX(0, 0, 0);
	transform.setRotation(rotation);

	btScalar mass = 0.1;
	btVector3 localInertia;

	btCollisionShape* shape = new btBoxShape({ 1, 1, 1 });
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);

	shape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo
	    RBInfo(mass, motionState, shape, localInertia);
	RBInfo.m_restitution = 1;
	RBInfo.m_friction = 0.5;

	btRigidBody* body = new btRigidBody(RBInfo);
	body->setUserPointer(user);

	return std::unique_ptr<btRigidBody>(body);
}

struct PhysicsCube {
	PhysicsCube(btDynamicsWorld* world, SWAN::vec3 pos = { 0, 0, 0 })
	{
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(ToBulletVector(pos));

		btQuaternion rotation;
		rotation.setEulerZYX(0, 0, 0);
		transform.setRotation(rotation);

		btScalar mass = 1;
		btVector3 localInertia;

		btCollisionShape* shape = new btBoxShape({ 1, 1, 1 });
		btDefaultMotionState* motionState = new btDefaultMotionState(transform);

		shape->calculateLocalInertia(mass, localInertia);

		btRigidBody::btRigidBodyConstructionInfo
		    RBInfo(mass, motionState, shape, localInertia);
		RBInfo.m_restitution = 0.5;
		RBInfo.m_friction = 0.5;

		RigidBody = new btRigidBody(RBInfo);
		RigidBody->setUserPointer(&Model);

		World = world;
		World->addRigidBody(RigidBody);
	}

	~PhysicsCube()
	{
		World->removeRigidBody(RigidBody);
		delete RigidBody;
	}

	void Render(const SWAN::Camera& cam)
	{
		btTransform tr;
		RigidBody->getMotionState()->getWorldTransform(tr);

		btQuaternion orientation = tr.getRotation();
		SWAN::Cube* user = &Model;
		user->transform.pos = ToSWANVector(tr.getOrigin());
		SWAN::fvec3 v;
		orientation.getEulerZYX(v.z, v.y, v.x);
		user->transform.rot = v;

		SWAN::Render(cam, Model);
	}

	SWAN::Cube Model;
	btRigidBody* RigidBody;
	btDynamicsWorld* World;
};

void GenerateCubes(btDynamicsWorld* world, SWAN::Vector<PhysicsCube>& vec, int numCubes = 5)
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<double> dist(125, 150);

	vec.reserve(numCubes);

	for(int i = 0; i < numCubes; i++)
		vec.emplace_back(world, SWAN::vec3(dist(eng), dist(eng), dist(eng)));
}

class PhysicsSystem
{
  public:
	PhysicsSystem()
	{
		Broadphase = new btDbvtBroadphase();
		CollisionConfiguration = new btDefaultCollisionConfiguration();
		Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
		Solver = new btSequentialImpulseConstraintSolver();
		World = new btDiscreteDynamicsWorld(
		    Dispatcher,
		    Broadphase,
		    Solver,
		    CollisionConfiguration);
		World->setGravity(btVector3(0, -9.8, 0));
	}

	btBroadphaseInterface* Broadphase;
	btDefaultCollisionConfiguration* CollisionConfiguration;
	btCollisionDispatcher* Dispatcher;
	btSequentialImpulseConstraintSolver* Solver;
	btDiscreteDynamicsWorld* World;
};

class FPSCounter : public SWAN::IGUIElement
{
  public:
	FPSCounter(const SWAN::BitmapFont* font)
	    : Font(font)
	{
	}

	SWAN::Rect2D GetRect() const override
	{
		return SWAN::Rect2D::FromMinMax(
		    { 0, 0 },
		    { Font->getGlyphWidth() * 5, Font->getGlyphHeight() });
	}

	void OnRender(SWAN::GUIManager& man) override
	{
		man.RenderText(0, 0, Font, std::to_string(ShowFPS ? 1.0 / FrameTime_ms.count() : FrameTime_ms.count()));
	}

	void SetFrameTime(fms ms) { FrameTime_ms = ms; }
	void ToggleShowFPSorFrameTime() { ShowFPS = !ShowFPS; }

  private:
	bool ShowFPS = true;
	fms FrameTime_ms = fms{ 0 };
	const SWAN::BitmapFont* Font;
};

class Timer
{
	using Clock = std::chrono::steady_clock;
	using FloatSeconds = std::chrono::duration<double>;

  public:
	Timer() {}

	template <typename Func>
	Timer(FloatSeconds dur, Func&& f, bool repeat = false)
	    : started(Clock::now()), duration(dur), func(f), repeat(repeat)
	{
	}

	void Update()
	{
		if(!expired && TimeSinceStart() >= duration) {
			func();
			if(repeat)
				started = Clock::now();
			else
				expired = true;
		}
	}

	void Restart()
	{
		started = Clock::now();
		expired = false;
	}

	FloatSeconds TimeSinceStart() const { return Clock::now() - started; }

  private:
	std::function<void(void)> func;
	Clock::time_point started;
	FloatSeconds duration;
	bool repeat = false;
	bool expired = false;
};

static SWAN::Vector<Timer> Timers;

template <typename Func>
void Every(FloatSeconds seconds, Func&& f)
{
	Timers.emplace_back(seconds, std::forward<Func&&>(f), true);
}

template <typename Func>
void After(FloatSeconds seconds, Func&& f)
{
	Timers.emplace_back(seconds, std::forward<Func&&>(f), false);
}

void UpdateTimers()
{
	for(auto& t : Timers)
		t.Update();
}

class VoxelLevelEditor : public SWAN::InputFrame
{
	using ivec3 = SWAN::ivec3;
	using uvec3 = SWAN::uvec3;
	using vec3 = SWAN::vec3;

  public:
	VoxelLevelEditor()
	{
		Voxels.reserve(128);
	}

#define MAPKEY(keycode, codeblock) \
	do {                           \
		if(key.Code == keycode)    \
			codeblock;             \
	} while(0)

#define MAPKEYEX(keycode, NoCtrl, OnCtrlHeld) \
	do {                                      \
		if(key.Code == keycode) {             \
			if(key.LCtrlHeld)                 \
				OnCtrlHeld;                   \
			else                              \
				NoCtrl;                       \
		}                                     \
	} while(0)

	void OnKeyPress(SWAN::Key key) override
	{
		MAPKEY(SWAN::Spacebar, AddVoxelAtCursor());
		MAPKEY(SWAN::Backspace, RemoveVoxelAtCursor());
		MAPKEY(SWAN::LeftArrow, CursorLeft());
		MAPKEY(SWAN::RightArrow, CursorRight());
		MAPKEYEX(SWAN::UpArrow, CursorForward(), CursorUp());
		MAPKEYEX(SWAN::DownArrow, CursorBackward(), CursorDown());
		MAPKEY(SWAN::Z, ToggleWireframe());
	}

	void OnKeyHold(SWAN::Key key) override
	{
		double speed = 0.001;
		if(key.Code == SWAN::W)
			cam->moveForw(speed);
		if(key.Code == SWAN::S)
			cam->moveForw(-speed);

		if(key.Code == SWAN::A)
			cam->moveRight(-speed);
		if(key.Code == SWAN::D)
			cam->moveRight(speed);

		if(key.Code == SWAN::Tab)
			cam->moveUp(speed);
		if(key.Code == SWAN::LShift)
			cam->moveUp(-speed);
	}

	void ToggleWireframe()
	{
		wire = !wire;
	}

	void OnKeyRepeat(SWAN::Key key) override
	{
		OnKeyPress(key);
	}

	void Render()
	{
		for(const auto& cube : Voxels) {
			SWAN::Render(*cam, cube, SWAN::DefaultFramebuffer, wire);
		}
		glClear(GL_DEPTH_BUFFER_BIT);
		SWAN::Render(*cam, SWAN::Cube(SWAN::Transform(CursorPos, {}, { 0.6, 0.6, 0.6 }), { 0.3, 0.3, 1.0, 0.4 }));
	}

	void OnMouseMove(SWAN::MouseMove move) override
	{
		double sensitivityX = 0.125,
		       sensitivityY = sensitivityX * SWAN::Display::GetAspectRatio();
		if(SWAN::GetCurrentInputFrame() == this) {
			cam->rotateByY(
			    SWAN::Util::Radians::FromDegrees(
			        move.RelativeX * sensitivityX));

			cam->rotateByX(
			    SWAN::Util::Radians::FromDegrees(
			        move.RelativeY * sensitivityY));

			SWAN::WarpMouseTo(
			    SWAN::Display::GetWidth() / 2,
			    SWAN::Display::GetHeight() / 2);
		}
	}

	void SetCamera(SWAN::Camera* cam) { this->cam = cam; }

#undef MAPKEY
#undef MAPKEYEX

  private:
	void AddVoxelAtCursor()
	{
		auto it = std::find_if(
		    Voxels.begin(),
		    Voxels.end(),
		    [this](const SWAN::Cube& c) { return c.transform.pos == CursorPos; });

		if(it == Voxels.end())
			Voxels.emplace_back(SWAN::Transform(CursorPos, {}, { 0.5, 0.5, 0.5 }));
	}

	void RemoveVoxelAtCursor()
	{
		auto it = std::find_if(
		    Voxels.begin(),
		    Voxels.end(),
		    [this](const SWAN::Cube& c) { return c.transform.pos == CursorPos; });

		if(it != Voxels.end())
			Voxels.erase(it);
	}

	void CursorLeft() { CursorPos += GetRight(); }
	void CursorRight() { CursorPos -= GetRight(); }

	void CursorUp() { CursorPos += GetUp(); }
	void CursorDown() { CursorPos -= GetUp(); }

	void CursorForward() { CursorPos += GetForward(); }
	void CursorBackward() { CursorPos -= GetForward(); }

	struct Axes {
		SWAN::vec3 forw, up, right;
	};

	vec3 GetUp()
	{
		if(!cam)
			return DefaultAxes.up;

		auto up = cam->up();
		if(abs(up.x) >= abs(up.y)) {
			if(abs(up.x) >= abs(up.z))
				return (up.x >= 0 ? vec3(1, 0, 0) : vec3(-1, 0, 0));
			else
				return (up.z >= 0 ? vec3(0, 0, 1) : vec3(0, 0, -1));
		} else {
			if(abs(up.y) >= abs(up.z))
				return (up.y >= 0 ? vec3(0, 1, 0) : vec3(0, -1, 0));
			else
				return (up.z >= 0 ? vec3(0, 0, 1) : vec3(0, 0, -1));
		}
	}

	vec3 GetForward()
	{
		if(!cam)
			return DefaultAxes.forw;

		auto forw = cam->forw();
		if(abs(forw.x) >= abs(forw.y)) {
			if(abs(forw.x) >= abs(forw.z))
				return (forw.x >= 0 ? SWAN::vec3(1, 0, 0) : SWAN::vec3(-1, 0, 0));
			else
				return (forw.z >= 0 ? SWAN::vec3(0, 0, 1) : SWAN::vec3(0, 0, -1));
		} else {
			if(abs(forw.y) >= abs(forw.z))
				return (forw.y >= 0 ? SWAN::vec3(0, 1, 0) : SWAN::vec3(0, -1, 0));
			else
				return (forw.z >= 0 ? SWAN::vec3(0, 0, 1) : SWAN::vec3(0, 0, -1));
		}
	}

	vec3 GetRight()
	{
		if(!cam)
			return DefaultAxes.right;

		return SWAN::Cross(GetForward(), GetUp());
	}

	Axes GetAxesFromCam()
	{
		if(!cam)
			return DefaultAxes;

		return { GetForward(), GetUp(), GetRight() };
	}

	const Axes DefaultAxes{
		{ 1, 0, 0 }, // Forward
		{ 0, 1, 0 }, // Up
		{ 0, 0, 1 }  // Right
	};

	SWAN::Camera* cam;

	bool wire = false;

	SWAN::vec3 CursorPos;
	std::vector<SWAN::Cube> Voxels;
};

int main()
{
	Timers.reserve(20);

	PhysicsSystem sys;

	auto Ground = CreatePlane();
	sys.World->addRigidBody(Ground.get());

	SWAN::Vector<PhysicsCube> Cubes;
	GenerateCubes(sys.World, Cubes, 100);

	// ----- Video initilazitaion ----- //
	SWAN::Display::Init(1280, 720, "Hello, world!");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ----- Audio initialization ----- //
	SWAN::SoundSystem SoundSys;

	// ----- Camera ----- //
	SWAN::Camera cam;
	cam.moveByZ(-1);
	cam.zNear = 0.01;
	cam.zFar = 10000.0;
	// cam.transform.setParent(&CubeUser.transform);

	// ----- Voxel Level Editor ----- //
	VoxelLevelEditor vle;
	vle.SetCamera(&cam);

	// ----- Grid ----- //
	Grid grid;
	grid.margin = 5;
	grid.numLines = 50;
	grid.generate();

	// ----- Input initialization ----- //
	// InputHandler Handler(cam);
	SWAN::SetCurrentInputFrame(&vle);

	// ----- GUI ----- //
	SWAN::Res::ReportLoad(SWAN::Res::LoadBitmapFont("Resources/Fonts/Terminus.toml", "font"));
	FPSCounter fps(SWAN::Res::GetBitmapFont("font"));
	SWAN::GUIManager gui;
	gui.AddElement(&fps);

	double PosValue = 0;
	auto prevTime = std::chrono::steady_clock::now();
	FloatSeconds dt{ 0 };

	//Every(FloatSeconds(0.008), [&sys] { sys.World->stepSimulation(0.064); });

	GameRunning = true;
	while(GameRunning) {
		auto now = std::chrono::steady_clock::now();
		SWAN::UpdateInputEvents();
		UpdateTimers();
		//sys.World->stepSimulation(FloatSeconds{ dt }.count());

		if(now - prevTime >= std::chrono::milliseconds{ 16 }) {
			fps.SetFrameTime(now - prevTime);
			gui.RenderElements();
			grid.render(cam);
			vle.Render();

			// for(auto& c : Cubes)
			// c.Render(cam);

			glClear(GL_DEPTH_BUFFER_BIT);
			SWAN::Display::Clear();

			prevTime = std::chrono::steady_clock::now();
		}

		dt = std::chrono::duration_cast<FloatSeconds>(std::chrono::steady_clock::now() - now);
	}

	SWAN::Display::Close();
}
