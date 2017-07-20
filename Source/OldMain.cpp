#include <iostream>
#include <utility>

#include <cmath>
#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "include/GUI.hpp"

// Display stuff
#define SDL_MAIN_HANDLED
#include "include/Display.hpp"

// OpenGL stuff
#include "include/Mesh.hpp"
#include "include/Shader.hpp"
#include "include/texture.h"

// Movement stuff
#include "include/Camera.hpp"
#include "include/Transform.hpp"

// World + Physics stuff
#include "include/Entity.hpp"
#include "include/HitBox.hpp"

// Misc. stuff
#include "include/Import.hpp"
#include "include/ResourceManager.hpp"
#include "include/Util.hpp"
#include "include/input.hpp"

#define _SEP "---------------------------------------"
#define ON_LETTER_KEY(x, action) \
    if (Input.Keyboard.letterKeys[x - 'a']) action

Mesh genRect2D() {
    Vertex v1(glm::vec3(-0.5, -0.5, 0), glm::vec2(0, 0),
              glm::normalize(glm::vec3(-0.5, -0.5, 0.5)));
    Vertex v2(glm::vec3(0.5, 0.5, 0), glm::vec2(1, 1),
              glm::normalize(glm::vec3(0.5, 0.5, 0.5)));
    Vertex v3(glm::vec3(0.5, -0.5, 0), glm::vec2(1, 0),
              glm::normalize(glm::vec3(0.5, -0.5, 0.5)));
    Vertex v4(glm::vec3(-0.5, 0.5, 0), glm::vec2(0, 1),
              glm::normalize(glm::vec3(-0.5, 0.5, 0.5)));

    return Mesh({v1, v2, v3, v4}, {0, 1, 2, 0, 3, 1});
}
Mesh *genRect2D_ptr() {
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

Mesh genCube3D() {
    Vertex v0(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0, 0),
              glm::normalize(glm::vec3(-0.5, -0.5, 0.5)));
    Vertex v1(glm::vec3(0.5, 0.5, 0.5), glm::vec2(1, 1),
              glm::normalize(glm::vec3(0.5, 0.5, 0.5)));
    Vertex v2(glm::vec3(0.5, -0.5, 0.5), glm::vec2(1, 0),
              glm::normalize(glm::vec3(0.5, -0.5, 0.5)));
    Vertex v3(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0, 1),
              glm::normalize(glm::vec3(-0.5, 0.5, 0.5)));

    Vertex v4(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0),
              glm::normalize(glm::vec3(-0.5, -0.5, -0.5)));
    Vertex v5(glm::vec3(0.5, 0.5, -0.5), glm::vec2(1, 1),
              glm::normalize(glm::vec3(0.5, 0.5, -0.5)));
    Vertex v6(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1, 0),
              glm::normalize(glm::vec3(0.5, -0.5, -0.5)));
    Vertex v7(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0, 1),
              glm::normalize(glm::vec3(-0.5, 0.5, -0.5)));

    return Mesh({v0, v1, v2, v3, v4, v5, v6, v7},

                {0, 2, 1, 3, 0, 1, 7, 4, 3, 4, 0, 3, 1, 2, 5, 2, 6, 5,
                 4, 6, 5, 7, 4, 5, 3, 1, 5, 3, 5, 7, 0, 2, 6, 0, 6, 4});
}
Mesh *genCube3D_ptr() {
    Vertex v0(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0, 0),
              glm::normalize(glm::vec3(-0.5, -0.5, 0.5)));
    Vertex v1(glm::vec3(0.5, 0.5, 0.5), glm::vec2(1, 1),
              glm::normalize(glm::vec3(0.5, 0.5, 0.5)));
    Vertex v2(glm::vec3(0.5, -0.5, 0.5), glm::vec2(1, 0),
              glm::normalize(glm::vec3(0.5, -0.5, 0.5)));
    Vertex v3(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0, 1),
              glm::normalize(glm::vec3(-0.5, 0.5, 0.5)));

    Vertex v4(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, 0),
              glm::normalize(glm::vec3(-0.5, -0.5, -0.5)));
    Vertex v5(glm::vec3(0.5, 0.5, -0.5), glm::vec2(1, 1),
              glm::normalize(glm::vec3(0.5, 0.5, -0.5)));
    Vertex v6(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1, 0),
              glm::normalize(glm::vec3(0.5, -0.5, -0.5)));
    Vertex v7(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0, 1),
              glm::normalize(glm::vec3(-0.5, 0.5, -0.5)));

    return new Mesh({v0, v1, v2, v3, v4, v5, v6, v7},
                    {0, 2, 1, 3, 0, 1, 7, 4, 3, 4, 0, 3, 1, 2, 5, 2, 6, 5,
                     4, 6, 5, 7, 4, 5, 3, 1, 5, 3, 5, 7, 0, 2, 6, 0, 6, 4});
}

glm::vec2 getMousePos(Display *d) {
    return glm::vec2(Util::pixelToGLCoord(d->getW(), Input.Mouse.x),
                     Util::pixelToGLCoord(d->getH(), Input.Mouse.y));
}

void loadMeshes(ResourceManager<Mesh> *rmMesh) {
    rmMesh->add("rect", genRect2D_ptr());
    rmMesh->add("cube", genCube3D_ptr());

#define MESH_LOC "res/meshes/untitled.obj"
    rmMesh->add("mesh", ImportMesh(MESH_LOC));
#undef MESH_LOC
}

void loadTextures(ResourceManager<Texture> *rmTex) {
    rmTex->add("red", new Texture("res/textures/placeholder/Red.png"));
    rmTex->add("green", new Texture("res/textures/placeholder/Green.png"));
    rmTex->add("blue", new Texture("res/textures/placeholder/Blue.png"));
    rmTex->add("gray", new Texture("res/textures/placeholder/Gray.png"));

    rmTex->add("test", new Texture("res/textures/Repair.png"));
    rmTex->add("barrier", new Texture("res/textures/Barrier.png"));
}

int main() {
    // Display d (1280,720,"[Game Title]");
    Display d2(1280, 720, "[[== DEBUGGING ==]]");

    Input_init();
    Display::setClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    bool running = true;

    ResourceManager<Mesh> rmMesh;
    ResourceManager<Texture> rmTex;

    loadMeshes(&rmMesh);
    loadTextures(&rmTex);

    GUI::Renderer guiRen;

    GUI::Clickable *button = new GUI::Clickable();
    button->x = 10;
    button->y = 20;
    button->w = 200;
    button->h = 50;
    button->active = rmTex.get("red");
    button->inactive = rmTex.get("test");
    button->pressed = rmTex.get("barrier");

    GUI::Clickable *button2 = new GUI::Clickable(*button);
    button2->x = 1000;

    GUI::Draggable *draggable =
        new GUI::Draggable(GUI::Image(rmTex.get("test")));
    draggable->x = 500;
    draggable->y = 500;

    guiRen.add(button);
    guiRen.add(button2);
    guiRen.add(draggable);

    Entity character(rmMesh.get("rect"), rmTex.get("red"));
    HitBox hb_char;
    hb_char.applyTransform(character.getTransform_ptr());

    Entity test(rmMesh.get("rect"), rmTex.get("green"));
    test.getTransform_ref().setParent(&character.getTransform_ref());
    test.getTransform_ref().getPos().y += 1.0f;
    test.getTransform_ref().getPos().x += 1.0f;

    Entity cube(rmMesh.get("cube"), rmTex.get("barrier"));
    HitBox hb_cube;
    hb_cube.applyTransform(character.getTransform_ptr());
    cube.getTransform_ref().getPos().z += 10;

    Entity bg(rmMesh.get("rect"), rmTex.get("gray"));
    HitBox hb_bg;
    hb_bg.applyTransform(bg.getTransform_ptr());

    Entity mouseFollower(rmMesh.get("cube"), rmTex.get("red"));
    mouseFollower.scaleToX(0.2f);
    mouseFollower.scaleToY(0.2f);
    mouseFollower.scaleToZ(0.2f);

    Shader s;
    s.compileShaders("res/shaders/BasicShader.vs",
                     "res/shaders/BasicShader.fs");
    s.addAttrib("pos");
    s.addAttrib("UV");
    s.addAttrib("norm");
    s.linkShaders();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    Camera cam(d2.getAspect());
    cam.moveByZ(-2.0f);

    glm::mat4 buff;

    s.use();
    buff = cam.getPerspective();
    s.addUniform("perspective");
    s.setUniformData("perspective", buff);

    buff = cam.getView();
    s.addUniform("view");
    s.setUniformData("view", buff);

    s.addUniform("booltest");

    s.addUniform("sunDir");
    s.setUniformData("sunDir", glm::vec3(0.0f, 0.0f, -1.0f));

    s.addUniform("sunCol");
    s.setUniformData("sunCol", glm::vec3(0.1f, 1.0f, 1.0f));

    s.addUniform("transform");
    s.unuse();

    // std::cout << d2.getAspect() << std::endl;

    Uint32 time;

    character.scaleToX(0.2f);
    character.scaleToY(0.5f);

    bg.scaleToX(10.0f);
    bg.scaleToY(0.2f);

    std::cout << std::boolalpha;

    std::clock_t prevTime = std::clock();
    std::clock_t now;

    glm::vec3 camVel;
    cube.setCurrAngVelX(1.0f);

    while (running) {
        now = std::clock();
        time = SDL_GetTicks();

        Input.handleEvents();
        if (Input.Window.exitRequest || Input.Keyboard.escapeKey) {
            // std::cout << "Quit request!" << std::endl;
            running = false;
        }

        ON_LETTER_KEY('a',
                      cam.moveByX(camVel.x = Interpolate(
                                      camVel.x, 0.1f, (now - prevTime) * 5)));
        else ON_LETTER_KEY(
            'd',
            cam.moveByX(
                camVel.x = Interpolate(camVel.x, -0.1f, (now - prevTime) * 5)));
        else cam.moveByX(camVel.x =
                             Interpolate(camVel.x, 0.0f, (now - prevTime) * 5));

        ON_LETTER_KEY('w',
                      cam.moveByY(camVel.y = Interpolate(
                                      camVel.y, 0.1f, (now - prevTime) * 5)));
        else ON_LETTER_KEY(
            's',
            cam.moveByY(
                camVel.y = Interpolate(camVel.y, -0.1f, (now - prevTime) * 5)));
        else cam.moveByY(camVel.y =
                             Interpolate(camVel.y, 0.0f, (now - prevTime) * 5));

        ON_LETTER_KEY('q', cam.rotateByZ(-0.1f));
        else ON_LETTER_KEY('e', cam.rotateByZ(0.1f));

        if (Input.Keyboard.LArrowKey)
            character.setTargetVelX(1.0f);
        else if (Input.Keyboard.RArrowKey)
            character.setTargetVelX(-1.0f);
        else
            character.setTargetVelX(0.0f);

        if (Input.Keyboard.UArrowKey)
            character.setTargetVelY(1.0f);
        else if (Input.Keyboard.DArrowKey)
            character.setTargetVelY(-1.0f);
        else
            character.setTargetVelY(0.0f);

        std::cout << std::boolalpha;

        bool intersects =
            hb_char.intersectsWith(hb_bg) || hb_bg.intersectsWith(hb_char);
        std::cout << intersects << "\n";

        mouseFollower.getTransform_ref().getPos() =
            cam.getPos() + cam.getForw() -
            glm::vec3(getMousePos(&d2), 0.0f) * d2.getAspect();

        d2.focus();

        guiRen.render(d2);
        guiRen.update();

        s.use();
        buff = cam.getView();
        s.setUniformData("view", buff);

        character.useTransform(&s);
        character.render();

        cube.useTransform(&s);
        cube.render();

        bg.useTransform(&s);
        bg.render();

        test.useTransform(&s);
        test.render();

        mouseFollower.useTransform(&s);
        mouseFollower.render();
        s.unuse();
        d2.clear();

        glClear(GL_DEPTH_BUFFER_BIT);

        if (20 > (SDL_GetTicks() - time)) {
            SDL_Delay(20 - (SDL_GetTicks() - time));  // SDL_Delay pauses the
            // execution.
        }

        character.update((now - prevTime) * 15);
        cube.update((now - prevTime) * 15);

        prevTime = now;
    }

    return 0;
}
