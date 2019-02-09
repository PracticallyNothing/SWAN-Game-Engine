#include "Resources.hpp"

#include "Core/Format.hpp"
#include "Core/Logging.hpp"

// For:
//    - SWAN::Util::GetDirectory()
//    - SWAN::Util::IsAbsolutePath()
//    - SWAN::Util::Trim()
#include "Importing/INI.hpp" // For SWAN::Util::INI::Config
#include "Importing/XML.hpp" // For SWAN::Util::XML, SWAN::Util::ReadXML()
#include "Utility/StreamOps.hpp"
#include "Utility/StringUtil.hpp"

#include "Rendering/OBJ-Import.hpp" // For Import::OBJ()

namespace SWAN
{
	namespace Res
	{
		using namespace SWAN::Util::StreamOps;
		namespace detail
		{
			Map<String, Pointer<Mesh>> meshes;
			Map<String, Pointer<Texture>> textures;
			Map<String, Pointer<BitmapFont>> bitmapFonts;
			Map<String, Pointer<Shader>> shaders;
		} // namespace detail

		void ReportLoad(LoadInfo li, bool reportOK)
		{
			String type;

			switch(li.resType) {
				case RT_UNKNOWN: type = "Res|Unknown"; break;
				case RT_MESH: type = "Res|Mesh"; break;
				case RT_TEX: type = "Res|Texture"; break;
				case RT_SHAD: type = "Res|Shader"; break;
				case RT_FONT: type = "Res|Font"; break;
			}

			switch(li.status) {
				case LS_UNKNOWN:
					Log(type, Format("Unknown status for \"{}\".", li.name), LogLevel::Warning);
					break;
				case LS_ERR:
					Log(type, Format("An error occured while loading \"{}\".", li.name), LogLevel::Error);
					break;
				case LS_FILEMISSING:
					Log(type, Format("No such file \"{}\".", li.file), LogLevel::Error);
					break;
				case LS_NAMETAKEN:
					Log(type, Format("Name \"{}\" is already taken.", li.name), LogLevel::Error);
					break;
				case LS_OK:
					if(reportOK)
						Log(type, Format("Resource \"{}\" loaded.", li.name), LogLevel::Success);
					break;
			}
		}

		const Mesh* GetMesh(const String& name)
		{
			auto it = detail::meshes.find(name);

			if(it != detail::meshes.end())
				return it->second.get();
			else
				return nullptr;
		}

		const Texture* GetTexture(const String& name)
		{
			auto it = detail::textures.find(name);

			if(it != detail::textures.end())
				return it->second.get();
			else
				return nullptr;
		}

		const BitmapFont* GetBitmapFont(const String& name)
		{
			auto it = detail::bitmapFonts.find(name);

			if(it != detail::bitmapFonts.end())
				return it->second.get();
			else
				return nullptr;
		}

		Shader* GetShader(const String& name)
		{
			auto it = detail::shaders.find(name);

			if(it != detail::shaders.end())
				return it->second.get();
			else
				return nullptr;
		}

		bool FileExists(const String& filename)
		{
			if(FILE* file = fopen(filename.c_str(), "r")) {
				fclose(file);
				return true;
			} else {
				return false;
			}
		}

		LoadInfo LoadMesh(const String& file, const String& name)
		{
			LoadInfo res(LS_UNKNOWN, RT_MESH, name, file);

			if(!FileExists(file))
				return res.withStatus(LS_FILEMISSING);

			if(detail::meshes.find(name) != detail::meshes.end())
				return res.withStatus(LS_NAMETAKEN);

			detail::meshes.emplace(
			    (name.length() ? name : file),
			    Import::OBJ(file, Import::Settings{ true }));

			return res.withStatus(LS_OK);
		}

		LoadInfo LoadTexture(const String& file, const String& name, bool pixelated)
		{
			LoadInfo res(LS_UNKNOWN, RT_TEX, name, file);

			if(!FileExists(file))
				return res.withStatus(LS_FILEMISSING);

			if(detail::textures.find(name) != detail::textures.end())
				return res.withStatus(LS_NAMETAKEN);

			detail::textures.emplace((name.length() ? name : file),
			                         std::make_unique<Texture>(file, pixelated));

			return res.withStatus(LS_OK);
		}

		LoadInfo LoadBitmapFont(const String& file, const String& name)
		{
			LoadInfo res(LS_UNKNOWN, RT_FONT, name, file);

			if(!FileExists(file))
				return res.withStatus(LS_FILEMISSING);

			if(detail::bitmapFonts.find(name) != detail::bitmapFonts.end())
				return res.withStatus(LS_NAMETAKEN);

			detail::bitmapFonts.emplace((name.length() ? name : file), ImportBitmapFromINI(file));
			return res.withStatus(LS_OK);
		}

		bool LoadFromFile(const String& filename)
		{
			XML res = ReadXML(filename);
			if(res.root.name != "Resources") {
				Log(Format("Attempted to load resources from file \"{}\", but file has incorrect root tag. "
				           "(Required: \"Resources\", Got: \"{}\")",
				           filename, res.root.name),
				    LogLevel::Error);
				return false;
			}

			String dir = SWAN::Util::GetDirectory(filename);

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
					    Format("<Mesh> tag with empty or nonexistant name attribute. "
					           "The filename (\"{}\") will be used as the name.",
					           dir + fileIt->second),
					    LogLevel::Warning);
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
					Log("<Texture> tag has no file attribute, it will be skipped.", LogLevel::Error);
					continue;
				}

				auto nameIt = tag->attribs.find("name");
				// TODO: (1) applies here
				if(nameIt == tag->attribs.end() || !nameIt->second.size()) {
					Log(Format("<Texture> tag with empty or nonexistant name attribute. "
					           "The filename (\"{}\") will be used as the name.",
					           dir + fileIt->second),
					    LogLevel::Warning);
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
					Log("<BitmapFont> tag has no file attribute, it will be skipped.", LogLevel::Error);
					continue;
				}

				String name;
				String file = Util::Trim(dir + fileIt->second);

				auto conf = INI::ParseFile(file);
				if(!conf.hasVar("name")) {
					Log("<BitmapFont> tag with empty or nonexistant name attribute. "
					    "The filename will be used as the name.",
					    LogLevel::Warning);
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
