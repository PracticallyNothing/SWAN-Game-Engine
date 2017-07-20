#ifndef RES_MANAGER_HPP
#define RES_MANAGER_HPP

#include <map>    // For std::map<T,U>
#include <string> // For std::string

// TODO: Useless wrapper around std::map, 
// 		 replace with something useful
template<typename T>
class ResourceManager{
    public:
        ~ResourceManager(){
            for(auto& res : resources)
                delete res.second;	
        }

        void add(std::string name, T* resource){ 
			resources[name] = resource; 
		}

		void remove(std::string name) { 
			delete resources[name]; 
		}
		
		T* get(std::string name) { 
			return resources[name]; 
		}
		
		bool has(std::string name) { 
			return resources[name] == NULL; 
		}
    private:
        std::map<std::string, T*> resources;
};

#endif
