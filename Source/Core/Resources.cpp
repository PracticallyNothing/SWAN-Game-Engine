#include "Resources.hpp"

#include "Utility/Debug.hpp" // For SWAN_DEBUG_OUT()
// For:
//    - SWAN::Util::GetDirectory()
//    - SWAN::Util::IsAbsolutePath()
//    - SWAN::Util::Trim()
#include "Utility/StringUtil.hpp"
#include "Utility/XML.hpp" // For SWAN::Util::XML, SWAN::Util::ReadXML()

#include "Rendering/OBJ-Import.hpp" // For Import::OBJ()

#include <iostream> // For std::cout

namespace SWAN {
namespace Res {
	namespace detail {
		Map<String, Pointer<Mesh>> meshes;
		Map<String, Pointer<Texture>> textures;
		Map<String, Pointer<BitmapFont>> bitmapFonts;
		Map<String, Pointer<Shader>> shaders;
	}

	const Mesh* GetMesh(const std::string& name) {
		auto it = detail::meshes.find(name);

		if(it != detail::meshes.end())
			return it->second.get();
		else
			return nullptr;
	}

	const Texture* GetTexture(const std::string& name) {
		auto it = detail::textures.find(name);

		if(it != detail::textures.end())
			return it->second.get();
		else
			return nullptr;
	}

	const BitmapFont* GetBitmapFont(const std::string& name) {
		auto it = detail::bitmapFonts.find(name);

		if(it != detail::bitmapFonts.end())
			return it->second.get();
		else
			return nullptr;
	}

	Shader* GetShader(const std::string& name) {
		auto it = detail::shaders.find(name);

		if(it != detail::shaders.end())
			return it->second.get();
		else
			return nullptr;
	}

	bool FileExists(CStrRef filename) {
		if(FILE* file = fopen(filename.c_str(), "r")) {
			fclose(file);
			return true;
		} else {
			return false;
		}
	}

	LoadStatus LoadMesh(CStrRef file, CStrRef name) {
		if(!FileExists(file))
			return L_FILEMISSING;

		if(detail::meshes.find(name) != detail::meshes.end())
			return L_NAMETAKEN;

		detail::meshes.emplace(
		    (name.length() ? name : file),
		    Import::OBJ(file,
		                Import::Settings{ true, false, ColWrapper::COL_AABB }));

		return L_OK;
	}

	LoadStatus LoadTexture(CStrRef file, CStrRef name, bool pixelated) {
		if(!FileExists(file))
			return L_FILEMISSING;

		if(detail::textures.find(name) != detail::textures.end())
			return L_NAMETAKEN;

		detail::textures.emplace((name.length() ? name : file),
		                         std::make_unique<Texture>(file, pixelated));

		return L_OK;
	}

	LoadStatus LoadBitmapFont(CStrRef file, CStrRef name) {
		if(!FileExists(file))
			return L_FILEMISSING;

		if(detail::bitmapFonts.find(name) != detail::bitmapFonts.end())
			return L_NAMETAKEN;

		detail::bitmapFonts.emplace((name.length() ? name : file),
		                            std::make_unique<BitmapFont>(file));

		return L_OK;
	}

	bool LoadFromFile(const std::string& filename) {
		Util::XML res = SWAN::Util::ReadXML(filename);
		if(res.root.name != "Resources") {
			std::cout << "ERROR: Attempted to load resources from file \""
			          << filename << "\", but file has incorrect root tag.\n"
			          << "       (Required: \"Resources\", Got: \""
			          << res.root.name << "\")\n";
			return false;
		}

		std::string dir = SWAN::Util::GetDirectory(filename);
		SWAN_DEBUG_OUT(dir);

		SWAN_DEBUG_PRINT("Loading meshes...");
		auto meshes = res.findTagsWithName("Mesh");
		for(auto tag : meshes) {
			auto fileIt = tag->attribs.find("file");

			// TODO: (1)
			//       Very bad way to report errors.
			//       Function should return a flag with the error
			//       and let a logger do the job properly.
			if(fileIt == tag->attribs.end()) {
				std::cout << "ERROR: <Mesh> tag has no file attribute, it will "
				             "be skipped.\n";
				continue;
			}

			auto nameIt = tag->attribs.find("name");
			// TODO: (1) applies here
			if(nameIt == tag->attribs.end() || !nameIt->second.size()) {
				std::cout << "Warning: <Mesh> tag with empty or nonexistant "
				             "name attribute.\n"
				          << "         The filename (\"" << dir + fileIt->second
				          << "\") will be used as the name.\n";
			}

			String name = Util::Trim(nameIt->second);
			String file = Util::Trim(dir + fileIt->second);
			LoadMesh(file, name);
		}

		auto textures = res.findTagsWithName("Texture");
		for(auto tag : textures) {
			auto fileIt = tag->attribs.find("file");

			// TODO: (1) applies here
			if(fileIt == tag->attribs.end()) {
				std::cout
				    << "ERROR: <Texture> tag has no file attribute, it will be "
				       "skipped.\n";
				continue;
			}

			auto nameIt = tag->attribs.find("name");
			// TODO: (1) applies here
			if(nameIt == tag->attribs.end() || !nameIt->second.size()) {
				std::cout
				    << "Warning: <Texture> tag with empty or nonexistant name "
				       "attribute.\n"
				    << "         The filename (\"" << dir + fileIt->second
				    << "\") will be used as the name.\n";
			}

			std::string name = Util::Trim(nameIt->second);
			std::string file = Util::Trim(dir + fileIt->second);

			LoadTexture(file, name, false);
		}

		auto bitmapFonts = res.findTagsWithName("BitmapFont");
		for(auto tag : bitmapFonts) {
			auto fileIt = tag->attribs.find("file");

			// TODO: (1) applies here
			if(fileIt == tag->attribs.end()) {
				std::cout << "ERROR: <BitmapFont> tag has no file attribute, "
				             "it will be "
				             "skipped.\n";
				continue;
			}

			auto nameIt = tag->attribs.find("name");
			// TODO: (1) applies here
			if(nameIt == tag->attribs.end() || !nameIt->second.size()) {
				std::cout
				    << "Warning: <BitmapFont> tag with empty or nonexistant "
				       "name "
				       "attribute.\n"
				    << "         The filename will be used as the name.\n";
			}

			std::string name = Util::Trim(nameIt->second);
			std::string file = Util::Trim(dir + fileIt->second);

			detail::bitmapFonts.emplace((name.length() ? name : file),
			                            std::make_unique<BitmapFont>(file));
		}

		auto shaderTags = res.findTagsWithName("Shader");
		for(auto tag : shaderTags) {
			auto res = std::make_unique<Shader>();

			res->compileShaders(dir + tag->getAttrib("vertex"),
			                    dir + tag->getAttrib("fragment"));

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
						for(int i = 0; i < std::stoi(tag->getAttrib("size"));
						    i++)
							res->addUniform(tag->getAttrib("name") + "[" +
							                std::to_string(i) + "]");
				}
				{ // -------- <StructArrayUniform> --------
					auto tags = tag->findTagsWithName("StructArrayUniform");
					for(auto tag : tags)
						for(int i = 0; i < std::stoi(tag->getAttrib("size"));
						    i++) {
							std::string s = tag->getAttrib("name") + "[" +
							                std::to_string(i) + "].";

							for(auto x : tag->children)
								res->addUniform(s + x->getAttrib("name"));
						}
				}
			}
			res->unuse();

			detail::shaders.emplace(tag->getAttrib("name"), std::move(res));
		}

		return true;
	}
}
}
