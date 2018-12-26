#ifndef ASSIMP_IMPORT_HPP
#define ASSIMP_IMPORT_HPP

#include "SWAN/Rendering/Light.hpp"   // For SWAN::Light
#include "SWAN/Rendering/Mesh.hpp"    // For SWAN::Mesh, SWAN::Vertex
#include "SWAN/Rendering/Texture.hpp" // For SWAN::Texture

#include "SWAN/Physics/Transform.hpp" // For SWAN::Transform

#include "SWAN/Maths/Matrix.hpp" // For SWAN::mat4

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include <algorithm> // For std::find()
#include <iostream>  // For std::cout
#include <memory>    // For std::unique_ptr<T>
#include <string>    // For std::string
#include <vector>    // For std::vector<T>
#include <map>       // For std::map<K, V>

using SWAN::vec2;
using SWAN::vec3;
using SWAN::vec4;
using SWAN::mat4;

using std::cout;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::vector;
using std::find;

/// Renderable object.
struct Actor {
    /// Construct an empty Actor.
    Actor() : tex(nullptr), mesh(nullptr) {}

    /// Construct an Actor with a texture and mesh.
    Actor(const SWAN::Texture* t, const SWAN::Mesh* m)
	: tex(t), mesh(m) {}

    /// Texture to bind for the actor
    const SWAN::Texture* tex = nullptr;

    /// Mesh to use for the actor
    const SWAN::Mesh* mesh = nullptr;
};

/// A node of a scene
struct Node {
    /// Parent node
    const Node* parent = nullptr;

    /// Children of the node
    vector<unique_ptr<Node>> children;
    
    /// Renderable objects of a node
    vector<Actor> actors;

    /// Is the node the root of the scene?
    inline bool isRoot() const { return !parent; }
    /// Does the node have children?
    inline bool hasChildren() const { return !children.empty(); }

    /// Add a child node.
    void addChild(Node* child) {
	// Straight up error - can't adopt null or self
	if(!child || child == this) return;

	// Useless call - argument is already a child
	else if(child->parent == this) return;

	parent = child->parent;
	children.emplace_back(child);
	child->parent = this;
    }

    /// (Internal) Model matrix for the node
    SWAN::mat4 _transform;

    /// Retrieve the proper model matrix for the node.
    SWAN::mat4 getMat() const {
	return (parent
		? parent->getMat()
		: mat4(1))
	    * _transform;
    }
};

/// Description of a scene
struct Scene {
    /// Whether the scene has been correctly imported/created
    bool valid = false;

    /// Every mesh resource in the scene
    vector<unique_ptr<SWAN::Mesh>> meshes;
    /// Every texture resource in the scene
    vector<unique_ptr<SWAN::Texture>> textures;
    /// Every light source in the scene
    vector<SWAN::Light> lights;

    /// Root node of the scene
    unique_ptr<Node> root;
};

/// Report an error in the importing process.
void logError(string s) { cout << "  Importing error: " << s << '\n'; }

static SWAN::vec2 Convert(const SWAN::vec2& rhs) {return rhs;}
static SWAN::vec3 Convert(const SWAN::vec3& rhs) {return rhs;}
static SWAN::vec4 Convert(const SWAN::vec4& rhs) {return rhs;}

/// Convert an aiVector into a SWAN vector. 
static SWAN::vec2 Convert(const aiVector2D& rhs) {return vec2(rhs.x, rhs.y);}
/// Convert an aiVector into a SWAN vector. 
static SWAN::vec3 Convert(const aiVector3D& rhs) {return vec3(rhs.x, rhs.y, rhs.z);}

/// Convert an aiColor into a SWAN vector. 
static SWAN::vec3 Convert(const aiColor3D& rhs) {return vec3(rhs.r, rhs.g, rhs.b);}
/// Convert an aiColor into a SWAN vector. 
static SWAN::vec4 Convert(const aiColor4D& rhs) {return vec4(rhs.r, rhs.g, rhs.b, rhs.a);}

/// Convert an aiMatrix into a SWAN matrix
static SWAN::mat3 Convert(const aiMatrix3x3& m) {
    SWAN::mat3 res;

    for(int y = 0; y < 3; y++)
	for(int x = 0; x < 3; x++)
	    res(x, y) = m[y][x];

    return res;
}
/// Convert an aiMatrix into a SWAN matrix
static SWAN::mat4 Convert(const aiMatrix4x4& m) {
    SWAN::mat4 res;

    for(int y = 0; y < 4; y++)
	for(int x = 0; x < 4; x++)
	    res(x, y) = m[y][x];

    return res;
}

/// Process a mesh.
static unique_ptr<SWAN::Mesh> processMesh(const aiMesh* mesh) {
    vector<SWAN::Vertex> verts;
    vector<unsigned> inds;

    for(int i = 0; i < mesh->mNumVertices; i++) {
	verts.push_back(SWAN::Vertex(Convert(mesh->mVertices[i]),
				     mesh->HasTextureCoords(0) ? Convert(mesh->mTextureCoords[0][i]) : vec2(0),
				     mesh->HasNormals() ? Convert(mesh->mNormals[i]) : vec3(0)));
    }

    for(int i = 0; i < mesh->mNumFaces; i++) {
	const aiFace& f = mesh->mFaces[i];
	inds.push_back(f.mIndices[0]);
	inds.push_back(f.mIndices[1]);
	inds.push_back(f.mIndices[2]);
    }

    return make_unique<SWAN::Mesh>(verts, inds);
}

/// Process a light source.
static SWAN::Light processLight(const aiLight* l) {
    SWAN::Light light;
    switch(l->mType) {
    case aiLightSource_DIRECTIONAL:
	light.type                  = SWAN::L_DIRECTIONAL;
	light.dirLight.direction    = Convert(l->mDirection);
	light.dirLight.ambient      = Convert(l->mColorAmbient);
	light.dirLight.diffuse      = Convert(l->mColorDiffuse);
	light.dirLight.specular     = Convert(l->mColorSpecular);
	light.dirLight.constAtt     = l->mAttenuationConstant ;
	light.dirLight.linearAtt    = l->mAttenuationLinear   ;
	light.dirLight.quadraticAtt = l->mAttenuationQuadratic;
	break;
    case aiLightSource_POINT:
	light.type                    = SWAN::L_POINT;
	light.pointLight.position     = Convert(l->mPosition);
	light.pointLight.ambient      = Convert(l->mColorAmbient);
	light.pointLight.diffuse      = Convert(l->mColorDiffuse);
	light.pointLight.specular     = Convert(l->mColorSpecular);
	light.pointLight.constAtt     = l->mAttenuationConstant;
	light.pointLight.linearAtt    = l->mAttenuationLinear;
	light.pointLight.quadraticAtt = l->mAttenuationQuadratic;
	break;
    case aiLightSource_SPOT:
	light.type                   = SWAN::L_SPOT;
	light.spotLight.position     = Convert(l->mPosition);
	light.spotLight.direction    = Convert(l->mDirection);
	light.spotLight.ambient      = Convert(l->mColorAmbient);
	light.spotLight.diffuse      = Convert(l->mColorDiffuse);
	light.spotLight.specular     = Convert(l->mColorSpecular);
	light.spotLight.constAtt     = l->mAttenuationConstant;
	light.spotLight.linearAtt    = l->mAttenuationLinear;
	light.spotLight.quadraticAtt = l->mAttenuationQuadratic;
	break;
    default: break;
    }
    return light;
}

/// Load a texture from file.
SWAN::Texture* processTexture(const char* path) { return new SWAN::Texture(path); }
/// Load a texture from file.
SWAN::Texture* processTexture(const std::string& path) { return new SWAN::Texture(path); }

/// Process an embedded texture.
SWAN::Texture* processTexture(const aiTexture* tex) {
    SWAN::Image img(tex->mWidth, tex->mHeight);
    for(int y = 0; y < img.height; y++){
	for(int x = 0; x < img.width; x++) {
	    aiTexel t = tex->pcData[x + y * tex->mWidth];
	    img.setPixelAt(x, y, SWAN::Color (t.r, t.g, t.b, t.a));
	}
    }
    return new SWAN::Texture(img);
}

/// Add a node and its children to the scene.
Node* processNode(const aiNode* n, const aiScene* as, Scene& s) {
    Node* res = new Node();
    for(int i = 0; i < n->mNumChildren; i++) {
	res->addChild(processNode(n->mChildren[i], as, s));
    }

    res->_transform = Convert(n->mTransformation);
    for(int i = 0; i < n->mNumMeshes; i++) {
	const aiMesh* am = as->mMeshes[n->mMeshes[i]];
	const aiMaterial* mat = as->mMaterials[am->mMaterialIndex];

	aiString texPath; 
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);

	s.textures.emplace_back(processTexture(texPath.C_Str()));
	const SWAN::Mesh* m = s.meshes[n->mMeshes[i]].get();
	const SWAN::Texture* t = s.textures.back().get();
	res->actors.emplace_back(t, m);
    }
    return res;
}

/// Populate a scene with resources and nodes.
static Scene processScene(const aiScene* s) {
    Scene res;
    res.valid = true;
    res.meshes.reserve(s->mNumMeshes);
    res.lights.reserve(s->mNumLights);

    // NOTE: This only loads embedded textures.
    //       The actual textures are loaded in processNode().
    for(int i = 0; i < s->mNumTextures; i++)
	res.textures.emplace_back(processTexture(s->mTextures[i]));

    for(int i = 0; i < s->mNumMeshes; i++)
	res.meshes.emplace_back(processMesh(s->mMeshes[i]));

    for(int i = 0; i < s->mNumLights; i++)
	res.lights.push_back(processLight(s->mLights[i]));

    res.root = std::unique_ptr<Node>(processNode(s->mRootNode, s, res));

    return res;
}

/** 
 * @brief Import a scene through Assimp.
 *
 * @param filename The name of the file where the scene is kept on disk. 
 *
 * @return A Scene object. 
 *         If an error occurs, the scene's valid variable is set to false.
 */
Scene AssimpImport(const std::string& filename) {
    Assimp::Importer importer;

    const aiScene* scene =
	importer.ReadFile(filename,
			  aiProcess_CalcTangentSpace |
			  aiProcess_Triangulate |
			  aiProcess_JoinIdenticalVertices |
			  aiProcess_SortByPType);

    if(!scene) {
	logError(importer.GetErrorString());
	return Scene();
    }

    return processScene(scene);
}

#endif
