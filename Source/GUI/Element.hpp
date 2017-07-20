#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <vector>         // For std::vector<T>
#include "IComponent.hpp" // For IComponent

class Element {
public:
    Element(){}
    Element(Element* _parent) : parent(_parent){}
    
    ~Element(){}

    void addComponent(IComponent& comp){
        components.push_back(comp);
    }
	
	void render() {
		for(auto& c : components) 
			c.render();
	}

	void update() {
		for(auto& c : components) 
			c.update();
	}

	void input() {
		for(auto& c : components) 
			c.input();
	}
private:
	Element* parent = nullptr;
    std::vector<IComponent&> components;
};

#endif
