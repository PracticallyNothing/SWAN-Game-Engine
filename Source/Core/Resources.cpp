#include "Resources.hpp"

#include "../Utility/XML.hpp"        // For Util::XML, Util::ReadXML()
#include "../Utility/StringUtil.hpp" // For Util::GetDirectory(), Util::IsAbsolutePath()
#include "../Utility/Debug.hpp"      // For DEBUG_OUT()

#include "../Rendering/OBJ-Import.hpp" // For Import::OBJ()

#include <iostream> // For std::cout

namespace Resources {

	namespace detail{
		Map<String, Pointer<Mesh>>       meshes;
		Map<String, Pointer<Texture>>    textures;
		Map<String, Pointer<BitmapFont>> bitmapFonts;
		Map<String, Pointer<Shader>>     shaders;
	}

	const Mesh* GetMesh(const std::string& name){
		auto it = detail::meshes.find(name);

		if(it != detail::meshes.end())
			return it->second.get();
		else
			return nullptr;
	}

	const Texture* GetTexture(const std::string& name){
		auto it = detail::textures.find(name);

		if(it != detail::textures.end())
			return it->second.get();
		else
			return nullptr;
	}

	const BitmapFont* GetBitmapFont(const std::string& name){
		auto it = detail::bitmapFonts.find(name);

		if(it != detail::bitmapFonts.end())
			return it->second.get();
		else
			return nullptr;
	}

	Shader* GetShader(const std::string& name){
		auto it = detail::shaders.find(name);

		if(it != detail::shaders.end())
			return it->second.get();
		else
			return nullptr;
	}

	bool LoadFromFile(const std::string& filename){
		Util::XML res = Util::ReadXML(filename);
		if(res.root.name != "Resources"){
			std::cout << "ERROR: Attempted to load resources from file \"" << filename << "\", but file has incorrect root tag.\n"
				<< "       (Required: \"Resources\", Got: \"" << res.root.name << "\")\n";
			return false;
		}

		std::string dir = Util::GetDirectory(filename);
		DEBUG_OUT(dir);

		auto meshes = res.findTagsWithName("Mesh");
		for(auto tag : meshes){
			auto fileIt = tag->attribs.find("file");

			if(fileIt == tag->attribs.end()){
				std::cout << "ERROR: <Mesh> tag has no file attribute, it will be skipped.\n";
				continue;
			}

			auto nameIt = tag->attribs.find("name");
			if(nameIt == tag->attribs.end() || !nameIt->second.size()){
				std::cout << "Warning: <Mesh> tag with empty or nonexistant name attribute.\n"
					<< "         The filename (\"" << dir+ fileIt->second << "\") will be used as the name.\n";
			}

			std::string name = nameIt->second;
			std::string file = dir + fileIt->second;

			detail::meshes.emplace(
					(name.length() ? name : file),
					Import::OBJ(file)
					);
		}

		auto textures = res.findTagsWithName("Texture");
		for(auto tag : textures){
			auto fileIt = tag->attribs.find("file");

			if(fileIt == tag->attribs.end()){
				std::cout << "ERROR: <Texture> tag has no file attribute, it will be skipped.\n";
				continue;
			}

			auto nameIt = tag->attribs.find("name");
			if(nameIt == tag->attribs.end() || !nameIt->second.size()){
				std::cout << "Warning: <Texture> tag with empty or nonexistant name attribute.\n"
					<< "         The filename (\"" << dir+ fileIt->second << "\") will be used as the name.\n";
			}

			std::string name = nameIt->second;
			std::string file = dir + fileIt->second;

			detail::textures.emplace(
					(name.length() ? name : file),
					std::make_unique<Texture>(file)
					);
		}

		auto bitmapFonts = res.findTagsWithName("BitmapFont");
		for(auto tag : bitmapFonts){
			auto fileIt = tag->attribs.find("file");

			if(fileIt == tag->attribs.end()){
				std::cout << "ERROR: <BitmapFont> tag has no file attribute, it will be skipped.\n";
				continue;
			}

			auto nameIt = tag->attribs.find("name");
			if(nameIt == tag->attribs.end() || !nameIt->second.size()){
				std::cout << "Warning: <BitmapFont> tag with empty or nonexistant name attribute.\n"
					<< "         The filename will be used as the name.\n";
			}

			std::string name = nameIt->second;
			std::string file = dir + fileIt->second;

			detail::bitmapFonts.emplace(
					(name.length() ? name : file),
					std::make_unique<BitmapFont>(file)
					);
		}

		auto shaderTags = res.findTagsWithName("Shader");
		for(auto tag : shaderTags){
			auto res = std::make_unique<Shader>();
			res->compileShaders(
				dir + tag->getAttrib("vertex"),
				dir + tag->getAttrib("fragment")
			);

			{ // ----------- <Attribute/> -----------
				auto tags = tag->findTagsWithName("Attribute");
				for(auto tag : tags)
					res->addAttrib(tag->getAttrib("name"));
			}
			res->linkShaders();

			res->use();
			{
				{ // ----------- <Uniform/> -----------
					auto tags = tag->findTagsWithName("Uniform");
					for(auto tag : tags)
						res->addUniform(tag->getAttrib("name"));
				}
				{ // -------- <ArrayUniform/> --------
					auto tags = tag->findTagsWithName("ArrayUniform");
					for(auto tag : tags)
						for(int i = 0; i < std::stoi(tag->getAttrib("size")); i++)
							res->addUniform(tag->getAttrib("name") + "[" + std::to_string(i) + "]");
				}
				{ // -------- <StructArrayUniform> --------
					auto tags = tag->findTagsWithName("StructArrayUniform");
					for(auto tag : tags)
						for(int i = 0; i < std::stoi(tag->getAttrib("size")); i++){
							std::string s = tag->getAttrib("name") + "[" + std::to_string(i) + "].";

							for(auto x : tag->children)
								res->addUniform(s + x->getAttrib("name"));
						}
				}
			}
			res->unuse();

			detail::shaders.emplace(
				tag->getAttrib("name"),
				std::move(res)
			);
		}

		return true;
	}
}
