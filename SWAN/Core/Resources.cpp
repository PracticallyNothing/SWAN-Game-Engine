#include "Resources.hpp"

#include "Utility/Debug.hpp" // For SWAN_DEBUG_OUT()
// For:
//    - SWAN::Util::GetDirectory()
//    - SWAN::Util::IsAbsolutePath()
//    - SWAN::Util::Trim()
#include "Utility/StringUtil.hpp"
#include "Utility/StreamOps.hpp"
#include "Utility/XML.hpp" // For SWAN::Util::XML, SWAN::Util::ReadXML()
#include "Utility/INI.hpp" // For SWAN::Util::INI::Config

#include "Rendering/OBJ-Import.hpp" // For Import::OBJ()

#include <iostream> // For std::cout

namespace SWAN {
    namespace Res {
	using namespace SWAN::Util::StreamOps;
	namespace detail {
	    Map<String, Pointer<Mesh>> meshes;
	    Map<String, Pointer<Texture>> textures;
	    Map<String, Pointer<BitmapFont>> bitmapFonts;
	    Map<String, Pointer<Shader>> shaders;
	} // namespace detail

	static void ReportLoad(LoadInfo li, bool reportOK = true) {
	    String type;

	    switch(li.resType) {
	    case RT_UNKNOWN: type = "Res|Unknown"; break;
	    case RT_MESH   : type = "Res|Mesh"; break;
	    case RT_TEX    : type = "Res|Texture"; break;
	    case RT_SHAD   : type = "Res|Shader"; break;
	    case RT_FONT   : type = "Res|Font"; break;
	    }

	    switch(li.status) {
	    case LS_UNKNOWN:
		Log(type, std::string("Unknown status for \"") + " " + li.name + "\".", LogLevel::Warning);
		break;
	    case LS_FILEMISSING:
		Log(type, std::string("No such file \"") + li.file + ".", LogLevel::Error);
		break;
	    case LS_NAMETAKEN:
		Log(type, std::string("Name \"") + li.name + "\" is already taken.", LogLevel::Error);
		break;
	    case LS_OK:
		if(reportOK)
		    Log(type, std::string("Resource \"") + li.name + "\" loaded.", LogLevel::Success);
		break;
	    }
	}

	const Mesh* GetMesh(const String& name) {
	    auto it = detail::meshes.find(name);

	    if(it != detail::meshes.end())
		return it->second.get();
	    else
		return nullptr;
	}

	const Texture* GetTexture(const String& name) {
	    auto it = detail::textures.find(name);

	    if(it != detail::textures.end())
		return it->second.get();
	    else
		return nullptr;
	}

	const BitmapFont* GetBitmapFont(const String& name) {
	    auto it = detail::bitmapFonts.find(name);

	    if(it != detail::bitmapFonts.end())
		return it->second.get();
	    else
		return nullptr;
	}

	Shader* GetShader(const String& name) {
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

	LoadInfo LoadMesh(CStrRef file, CStrRef name) {
	    LoadInfo res(LS_UNKNOWN, RT_MESH, name, file);

	    if(!FileExists(file))
		return res.withStatus(LS_FILEMISSING);

	    if(detail::meshes.find(name) != detail::meshes.end())
		return res.withStatus(LS_NAMETAKEN);

	    detail::meshes.emplace (
		(name.length() ? name : file),
		Import::OBJ(file, Import::Settings{ true })
	    );

	    return res.withStatus(LS_OK);
	}

	LoadInfo LoadTexture(CStrRef file, CStrRef name, bool pixelated) {
	    LoadInfo res(LS_UNKNOWN, RT_TEX, name, file);

	    if(!FileExists(file))
		return res.withStatus(LS_FILEMISSING);

	    if(detail::textures.find(name) != detail::textures.end())
		return res.withStatus(LS_NAMETAKEN);

	    detail::textures.emplace((name.length() ? name : file),
				     std::make_unique<Texture>(file, pixelated));

	    return res.withStatus(LS_OK);
	}

	LoadInfo LoadBitmapFont(CStrRef file, CStrRef name) {
	    LoadInfo res(LS_UNKNOWN, RT_FONT, name, file);

	    if(!FileExists(file))
		return res.withStatus(LS_FILEMISSING);

	    if(detail::bitmapFonts.find(name) != detail::bitmapFonts.end())
		return res.withStatus(LS_NAMETAKEN);

	    detail::bitmapFonts.emplace((name.length() ? name : file),
					std::make_unique<BitmapFont>(file));

	    return res.withStatus(LS_OK);
	}

	bool LoadFromFile(const String& filename) {
	    Util::XML res = SWAN::Util::ReadXML(filename);
	    if(res.root.name != "Resources") {
		std::cout << "ERROR: Attempted to load resources from file \""
			  << filename << "\", but file has incorrect root tag.\n"
			  << "       (Required: \"Resources\", Got: \""
			  << res.root.name << "\")\n";
		return false;
	    }

	    String dir = SWAN::Util::GetDirectory(filename);
	    // SWAN_DEBUG_OUT_CONTEXT(dir);

	    Log("Loading meshes...", LogLevel::Info);
	    auto meshes = res.findTagsWithName("Mesh");
	    for(auto tag : meshes) {
		auto fileIt = tag->attribs.find("file");

		// TODO: (1)
		//       Very bad way to report errors.
		//       Function should return a flag with the error
		//       and let a logger do the job properly.
		if(fileIt == tag->attribs.end()) {
		    Log("Res|Mesh|XML", "<Mesh> tag has no file attribute, it will be skipped.", LogLevel::Error);
		    continue;
		}

		auto nameIt = tag->attribs.find("name");
		// TODO: (1) applies here
		if(nameIt == tag->attribs.end() || !nameIt->second.size()) {
		    Log("Res|Mesh|XML",
			std::string("<Mesh> tag with empty or nonexistant name attribute. ") +
			"The filename (\"" + dir + fileIt->second + "\") will be used as the name.\n",
			LogLevel::Warning
		    );
		}

		String name = Util::Trim(nameIt->second);
		String file = Util::Trim(dir + fileIt->second);
		ReportLoad(LoadMesh(file, name));
	    }

	    Log("Loading textures...", LogLevel::Info);
	    auto textures = res.findTagsWithName("Texture");
	    for(auto tag : textures) {
		auto fileIt = tag->attribs.find("file");

		// TODO: (1) applies here
		if(fileIt == tag->attribs.end()) {
		    std::cout << "ERROR: <Texture> tag has no file attribute, it will be skipped.\n";
		    continue;
		}

		auto nameIt = tag->attribs.find("name");
		// TODO: (1) applies here
		if(nameIt == tag->attribs.end() || !nameIt->second.size()) {
		    std::cout << "Warning: <Texture> tag with empty or nonexistant name attribute.\n"
			      << "         The filename (\"" << dir + fileIt->second << "\") will be used as the name.\n";
		}

		String name = Util::Trim(nameIt->second);
		String file = Util::Trim(dir + fileIt->second);

		ReportLoad(LoadTexture(file, name, false));
	    }

	    Log("Loading fonts...", LogLevel::Info);
	    auto bitmapFonts = res.findTagsWithName("BitmapFont");
	    for(auto tag : bitmapFonts) {
		auto fileIt = tag->attribs.find("file");

		// TODO: (1) applies here
		if(fileIt == tag->attribs.end()) {
		    std::cout << "ERROR: <BitmapFont> tag has no file attribute, it will be skipped.\n";
		    continue;
		}

		String name;
		String file = Util::Trim(dir + fileIt->second);

		auto conf = Util::INI::ParseFile(file);
		if(!conf.hasVar("name")) {
		    std::cout << "Warning: <BitmapFont> tag with empty or nonexistant name attribute.\n"
			      << "         The filename will be used as the name.\n";
		    name = file;
		} else {
		    name = Util::Trim(conf["global"]["name"]);
		}
		
		ReportLoad(LoadBitmapFont(file, name));
	    }

	    Log("Loading shaders...", LogLevel::Info);
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
			    for(int i = 0; i < std::stoi(tag->getAttrib("size")); i++)
				res->addUniform(tag->getAttrib("name") + "[" + std::to_string(i) + "]");
		    }
		    { // -------- <StructArrayUniform> --------
			auto tags = tag->findTagsWithName("StructArrayUniform");
			for(auto tag : tags)
			    for(int i = 0; i < std::stoi(tag->getAttrib("size")); i++) {
				String s = tag->getAttrib("name") + "[" + std::to_string(i) + "].";

				for(auto x : tag->children)
				    res->addUniform(s + x->getAttrib("name"));
			    }
		    }
		}
		res->unuse();

		Log("Res|Shader", std::string("Resource \"") + tag->getAttrib("name") + "\" loaded.", LogLevel::Success);
		detail::shaders.emplace(tag->getAttrib("name"), std::move(res));
	    }

	    return true;
	}
    } // namespace Res
} // namespace SWAN
