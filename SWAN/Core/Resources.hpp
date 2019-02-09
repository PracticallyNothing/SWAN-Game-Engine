#ifndef SWAN_RESOURCES_HPP
#define SWAN_RESOURCES_HPP

#include "Core/Defs.hpp"

#include "Rendering/BitmapFont.hpp" // For BitmapFont
#include "Rendering/Mesh.hpp"       // For Mesh
#include "Rendering/Shader.hpp"     // For Shader
#include "Rendering/Texture.hpp"    // For Texture

namespace SWAN
{
	namespace Res
	{
		namespace detail
		{
			/// (Internal) A dictionary with every mesh that has been loaded.
			extern Map<String, Pointer<Mesh>> meshes;

			/// (Internal) A dictionary with every texture that has been loaded.
			extern Map<String, Pointer<Texture>> textures;

			/// (Internal) A dictionary with every bitmap font that has been loaded.
			extern Map<String, Pointer<BitmapFont>> bitmapFonts;

			/// (Internal) A dictionary with every shader that has been loaded.
			extern Map<String, Pointer<Shader>> shaders;

		} // namespace detail

		/// Information about how the loading of the resource went.
		enum LoadStatus {
			/// Resource was loaded correctly.
			LS_OK,

			/// Resource couldn't be loaded, because the name has already been taken by another resource.
			LS_NAMETAKEN,

			/// Resource couldn't be loaded, because there file to load it from is incorrect.
			LS_FILEMISSING,

			/// Resource couldn't be loaded, because of some resource specific or unspecified error.
			LS_ERR,

			/// Whether the resource was loaded or not is unknown.
			LS_UNKNOWN,
		};

		enum ResourceType {
			// The resource's type is unknown.
			RT_UNKNOWN,

			/// The resource is a SWAN::Mesh.
			RT_MESH,

			/// The resource is a SWAN::Texture.
			RT_TEX,

			/// The resource is a SWAN::Shader.
			RT_SHAD,

			/// The resource is a SWAN::BitmapFont.
			RT_FONT,
		};

		/// Structure with information concerning whether the resource loaded correctly and what resource it was.
		struct LoadInfo {
			/// Status of the resource. See LoadStatus.
			LoadStatus status = LS_UNKNOWN;

			/// Resource type. See ResourceType.
			ResourceType resType = RT_UNKNOWN;

			/// Name with which the resource will be accessed.
			String name = "";

			/// File from which the resource was (or was supposed to be) loaded.
			String file = "";

			/**
			 * @brief Construct a LoadInfo instance with parameters.
			 * 
			 * @param ls LoadStatus of the resource.
			 * @param rt Type of resource.
			 * @param name Name of the resource. 
			 * @param file Resource source.
			 */
			LoadInfo(LoadStatus ls = LS_UNKNOWN,
			         ResourceType rt = RT_UNKNOWN,
			         String name = "",
			         String file = "")
			    : status(ls),
			      resType(rt),
			      name(name),
			      file(file) {}

			/**
			 * @brief Set a LoadInfo's LoadStatus.
			 * @param ls LoadStatus of the resource.
			 */
			LoadInfo& withStatus(LoadStatus ls)
			{
				status = ls;
				return *this;
			}

			/**
			 * @brief Set a LoadInfo's ResourceType.
			 * @param rt Type of resource.
			 */
			LoadInfo& withResType(ResourceType rt)
			{
				resType = rt;
				return *this;
			}

			/**
			 * @brief Set a LoadInfo's name.
			 * @param str Name of the resource.
			 */
			LoadInfo& withName(String str)
			{
				name = str;
				return *this;
			}

			/**
			 * @brief Set a LoadInfo's file.
			 * @param str Resource source.
			 */
			LoadInfo& withFile(String str)
			{
				file = str;
				return *this;
			}
		};

		/**
		 * @brief Load an individual shader.
		 *
		 * @param filename File of the font.
		 * @param name Name with which the font will be recalled.
		 * @param attribs List of all the attributes in the shader.
		 * @param uniforms List of all the uniforms in the shader.
		 *
		 * @return Information about the load process.
		 */
		extern LoadInfo LoadShader(const String& filename, const String& name,
		                           Vector<String> attribs,
		                           Vector<String> uniforms);

		/**
		 * @brief Load an individual bitmap font.
		 *
		 * @param filename File of the font.
		 * @param name Name with which the font will be recalled.
		 *
		 * @return Information about the load process.
		 */
		extern LoadInfo LoadBitmapFont(const String& filename, const String& name);

		/**
		 * @brief Load an individual mesh.
		 *
		 * @param filename File of the mesh.
		 * @param name Name with which the mesh will be recalled.
		 *
		 * @return Information about the load process.
		 */
		extern LoadInfo LoadMesh(const String& filename, const String& name);

		/**
		 * @brief Load an individual texture.
		 *
		 * @param filename File of the texture.
		 * @param name Name with which the texture will be recalled.
		 *
		 * @return Information about the load process.
		 */
		extern LoadInfo LoadTexture(const String& filename, const String& name, bool pixelated = false);

		/**
		 * @brief Load resources from an XML file.
		 *
		 * Loaded resources can be accessed with the GetMesh(), 
		 * GetTexture(), GetBitmapFont() and GetShader() functions.
		 *
		 * @param filename XML file to read from.
		 * @return Whether the file could be read successfully.
		 */
		bool LoadFromFile(const String& filename);

		/**
		 * @brief Retrieve a pointer to a loaded mesh.
		 * @return The pointer to a SWAN::Mesh, nullptr otherwise.
		 */
		const Mesh* GetMesh(const String& name);

		/**
		 * @brief Retrieve a pointer to a loaded texture.
		 * @return The pointer to a SWAN::Texture, nullptr otherwise.
		 */
		const Texture* GetTexture(const String& name);

		/**
		 * @brief Retrieve a pointer to a loaded font.
		 * @return The pointer to a SWAN::BitmapFont, nullptr otherwise.
		 */
		const BitmapFont* GetBitmapFont(const String& name);

		/**
		 * @brief Retrieve a pointer to a loaded shader. 
		 * @return The pointer to a SWAN::Shader, nullptr otherwise.
		 */
		Shader* GetShader(const String& name);

		void ReportLoad(LoadInfo li, bool reportOK = true);
	} // namespace Res
} // namespace SWAN

#endif
