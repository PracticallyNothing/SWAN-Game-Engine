#define SDL_MAIN_HANDLED

#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <random>
#include <set>
#include <unordered_set>

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

class VoxelWorld
{
  public:
	VoxelWorld(unsigned size) : Size(size), VoxelData(new bool[size * size * size]) {}
	~VoxelWorld() { delete[] VoxelData; }

	bool& Get(int x, int y, int z) { return VoxelData[x + y * Size + z * Size * Size]; }
	bool& Get(SWAN::ivec3 v) { return Get(v.x, v.y, v.z); }

	inline bool& operator()(int x, int y, int z) { return Get(x, y, z); }
	inline bool& operator()(SWAN::ivec3 v) { return Get(v.x, v.y, v.z); }

	const bool& Get(int x, int y, int z) const { return VoxelData[x + y * Size + z * Size * Size]; }
	const bool& Get(SWAN::ivec3 v) const { return Get(v.x, v.y, v.z); }

	const bool& operator()(int x, int y, int z) const { return VoxelData[x + y * Size + z * Size * Size]; }
	inline const bool& operator()(SWAN::ivec3 v) const { return (*this)(v.x, v.y, v.z); }

	uint32_t GetSize() const { return Size; }

	void Resize(uint32_t NewSize)
	{
		bool* newData = new bool[NewSize * NewSize * NewSize];
		for(int z = 0; z < NewSize; z++)
			for(int y = 0; y < NewSize; y++)
				for(int x = 0; x < NewSize; x++)
					newData[x + y * NewSize + z * NewSize * NewSize] = Get(x, y, z);
		delete[] VoxelData;
		VoxelData = newData;
	}

  private:
	bool* VoxelData;
	unsigned Size;
};

class VoxelLevelEditor : public SWAN::InputFrame
{
	using ivec3 = SWAN::ivec3;
	using uvec3 = SWAN::uvec3;
	using vec3 = SWAN::vec3;

  public:
	VoxelLevelEditor() { Voxels.reserve(128); }

	void OnWindowExit() override { Stop(); }

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
		MAPKEY(SWAN::Spacebar, ToggleVoxelAtCursor());
		//MAPKEY(SWAN::Backspace, RemoveVoxelAtCursor());
		MAPKEY(SWAN::LeftArrow, CursorLeft());
		MAPKEY(SWAN::RightArrow, CursorRight());
		MAPKEYEX(SWAN::UpArrow, CursorForward(), CursorUp());
		MAPKEYEX(SWAN::DownArrow, CursorBackward(), CursorDown());
		MAPKEY(SWAN::Z, ToggleWireframe());
		MAPKEY(SWAN::F5, Save());
		MAPKEY(SWAN::F6, Load());

		MAPKEY(SWAN::Escape, Stop());
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

	void ToggleWireframe() { wire = !wire; }
	void OnKeyRepeat(SWAN::Key key) override { OnKeyPress(key); }

	void Render()
	{
		for(const auto& cube : Voxels) {
			SWAN::Render(*cam, cube);
		}
		if(wire) {
			SWAN::Cube wireCube;
			for(const auto& cube : Voxels) {
				wireCube = cube;
				wireCube.transform.scale = { 0.501, 0.501, 0.501 };
				wireCube.color = { 0, 0, 0, 1 };
				SWAN::Render(*cam, wireCube, SWAN::DefaultFramebuffer, true);
			}
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
#undef MAPKEY
#undef MAPKEYEX

	void SetCamera(SWAN::Camera* cam)
	{
		this->cam = cam;
	}

  private:
	void ToggleVoxelAtCursor()
	{
		auto it = std::find_if(
		    Voxels.begin(),
		    Voxels.end(),
		    [this](const SWAN::Cube& c) { return c.transform.pos == CursorPos; });

		if(it == Voxels.end())
			Voxels.emplace_back(SWAN::Transform(CursorPos, {}, { 0.5, 0.5, 0.5 }));
		else
			Voxels.erase(it);
	}

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
		return (up.y >= 0 ? vec3(0, 1, 0) : vec3(0, -1, 0));
	}

	vec3 GetForward()
	{
		if(!cam)
			return DefaultAxes.forw;

		auto forw = cam->forw();
		if(abs(forw.x) >= abs(forw.z))
			return (forw.x >= 0 ? SWAN::vec3(1, 0, 0) : SWAN::vec3(-1, 0, 0));
		else
			return (forw.z >= 0 ? SWAN::vec3(0, 0, 1) : SWAN::vec3(0, 0, -1));
	}

	vec3 GetRight()
	{
		if(!cam)
			return DefaultAxes.right;

		return SWAN::Cross(GetForward(), GetUp());
	}

	void Save()
	{
		FILE* file = std::fopen("voxedit.save", "wb");

		auto numVoxels = Voxels.size();
		std::fwrite(&numVoxels, sizeof(numVoxels), 1, file);
		std::fwrite(Voxels.data(), sizeof(SWAN::Cube), numVoxels, file);

		std::fclose(file);

		SWAN::Log(SWAN::Format("Saved {} voxels.", numVoxels));
	}

	void Load()
	{
		FILE* file = std::fopen("voxedit.save", "rb");
		if(!file) return;

		auto numVoxels = Voxels.size();
		std::fread(&numVoxels, sizeof(numVoxels), 1, file);
		SWAN::Log(SWAN::Format("Loaded {} voxels.", numVoxels));

		Voxels.resize(numVoxels);
		std::fread(Voxels.data(), sizeof(SWAN::Cube), numVoxels, file);

		std::fclose(file);
	}

	/*
	void BuildVAO()
	{
		using SWAN::fvec3;
		using SWAN::Vector;

		Vector<fvec3> points;
		Vector<unsigned> indices;

		vao.storeAttribData(0, 3, (float*) points.data(), sizeof(fvec3) * points.size());
		vao.storeIndices(indices.data(), indices.size());
	}
	*/

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

	SWAN::GL::VAO vao;

	SWAN::vec3 CursorPos;
	std::vector<SWAN::Cube> Voxels;
};

int main()
{
	// ----- Video initilazitaion ----- //
	SWAN::Display::Init(1280, 720, "Hello, world!");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ----- Camera ----- //
	SWAN::Camera cam;
	cam.moveByZ(-1);
	cam.zNear = 0.01;
	cam.zFar = 10000.0;

	// ----- Voxel Level Editor ----- //
	VoxelLevelEditor vle;
	vle.SetCamera(&cam);
	SWAN::SetCurrentInputFrame(&vle);

	// ----- GUI ----- //
	SWAN::Res::ReportLoad(SWAN::Res::LoadBitmapFont("Resources/Fonts/Font2.toml", "font"));
	SWAN::GUIManager gui;

	auto prevTime = std::chrono::steady_clock::now();
	GameRunning = true;

	while(GameRunning) {
		auto now = std::chrono::steady_clock::now();
		SWAN::UpdateInputEvents();

		if(now - prevTime >= std::chrono::milliseconds{ 16 }) {
			vle.Render();
			gui.RenderText(0, 0, SWAN::Res::GetBitmapFont("font"),
			               SWAN::Format("FPS: {}\n{}", fms(now - prevTime).count(),
			                            "+------------+\n"
			                            "| Esc - Exit |\n"
			                            "+------------+\n"
			                            "| F5 - Save  |\n"
			                            "| F6 - Load  |\n"
			                            "-------------+---------+\n"
			                            "| Space - Toggle voxel |\n"
			                            "-----------------------+------------+\n"
			                            "| W/S - Move cam forwards/backwards |\n"
			                            "| A/D - Move cam left/right         |\n"
			                            "| Tab/Shift - fly up/down           |\n"
			                            "------------------------------------+\n"));

			glClear(GL_DEPTH_BUFFER_BIT);
			SWAN::Display::Clear();

			prevTime = std::chrono::steady_clock::now();
		}
	}

	SWAN::Display::Close();
}
