#ifndef I_COMPONENT_HPP
#define I_COMPONENT_HPP

struct IComponent {
    virtual ~IComponent(){}
    virtual void update(){}
    virtual void render(){}
    virtual void input(){}
};

#endif
