#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <SDL2/SDL.h>
#include <string>

typedef unsigned int uint;

/*
namespace Display {
    namespace detail {
        SDL_Window* window;
        SDL_GLContext glContext;

        int width, height;
    }

    bool Init();
    void Clear();
    bool Close();


}
*/

class Display{
    public:
        Display(int w, int h, const std::string& title);
        ~Display();

        void focus();
        void resize(int w, int h);

        void clear();
        static void setClearColor(float r, float g, float b, float a);

        inline int getW() const { return w; }
        inline int getH() const { return h; }
        inline float getAspect() const { return (float)w/h; }
        inline static uint getNumDisplays() { return Display::numDisplays; }
    private:
        int w,h;
        SDL_Window* win;

        static SDL_GLContext glContext;
        static uint numDisplays;
};
#endif // !DISPLAY_HPP
