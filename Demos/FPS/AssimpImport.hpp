#ifndef ASSIMP_IMPORT_HPP
#define ASSIMP_IMPORT_HPP

#include "Rendering/Light.hpp"
#include "Rendering/Mesh.hpp"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include <iostream> // For std::cout
#include <memory>   // For std::unique_ptr<T>
#include <string>   // For std::string
#include <vector>   // For std::vector<T>

using std::cout;
using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;

struct Scene {
	bool valid = false;
	vector<unique_ptr<SWAN::Mesh>> meshes;
	vector<SWAN::Light> lights;
};

void logError(string s) {
	cout << "  Importing error: " << s << '\n';
}

static glm::vec3& operator=(glm::vec3& lhs, const aiColor3D& rhs) {
	lhs.x = rhs.r;
	lhs.y = rhs.g;
	lhs.z = rhs.b;
	return lhs;
}

static glm::vec3& operator=(glm::vec3& lhs, const aiVector3D& rhs) {
	lhs.x = rhs.x;
	lhs.y = rhs.y;
	lhs.z = rhs.z;
	return lhs;
}

static unique_ptr<SWAN::Mesh> processMesh(const aiMesh* mesh) {
	vector<SWAN::Vertex> verts;
	vector<unsigned> inds;

	const aiVector2D zero2D(0, 0, 0);
	const aiVector3D zero3D(0, 0, 0);

	for(int i = 0; i < f.mNumVertices; i++) {
		verts.push_back(Vertex(
							   mesh->mVertices[i],
							   mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : zero2D,
							   mesh->hasNormals() ? mesh->mNormals[i] : zero3D));
	}

	for(int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& f = mesh->mFaces[i];
		indices.push_back(f->indices[0]);
		indices.push_back(f->indices[1]);
		indices.push_back(f->indices[2]);
	}

	return make_unique<SWAN::Mesh>(verts, indices);
}

static SWAN::Light processLight(const aiLight* l) {
	SWAN::Light light;
	switch(l->mType) {
	case aiLightSource_DIRECTIONAL:
		light.type                  = L_DIRECTIONAL;
		light.dirLight.direction    = l->mDirection;
		light.dirLight.ambient      = l->mColorAmbient;
		light.dirLight.diffuse      = l->mColorDiffuse;
		light.dirLight.specular     = l->mColorSpecular;
		light.dirLight.constAtt     = l->mAttenuationConstant;
		light.dirLight.linearAtt    = l->mAttenuationLinear;
		light.dirLight.quadraticAtt = l->mAttenuationQuadratic;
		break;
	case aiLightSource_POINT:
		light.type                    = L_POINT;
		light.pointLight.position     = l->mPosition;
		light.pointLight.ambient      = l->mColorAmbient;
		light.pointLight.diffuse      = l->mColorDiffuse;
		light.pointLight.specular     = l->mColorSpecular;
		light.pointLight.constAtt     = l->mAttenuationConstant;
		light.pointLight.linearAtt    = l->mAttenuationLinear;
		light.pointLight.quadraticAtt = l->mAttenuationQuadratic;
		break;
	case aiLightSource_SPOT:
		light.type                   = L_SPOT;
		light.spotLight.position     = l->mPosition;
		light.spotLight.direction    = l->mDirection;
		light.spotLight.ambient      = l->mColorAmbient;
		light.spotLight.diffuse      = l->mColorDiffuse;
		light.spotLight.specular     = l->mColorSpecular;
		light.spotLight.constAtt     = l->mAttenuationConstant;
		light.spotLight.linearAtt    = l->mAttenuationLinear;
		light.spotLight.quadraticAtt = l->mAttenuationQuadratic;
		break;
	}
	return light;
}

static Scene processScene(const aiScene* s) {
	Scene res;
	res.valid = true;
	res.meshes.reserve(s->mNumMeshes);
	res.lights.reserve(s->mNumLights);

	for(int i = 0; i < s->mNumMeshes; i++)
		res.meshes.emplace_back(processMesh(s->mMeshes[i]));

	for(int i = 0; i < s->mNumLights; i++)
		res.lights.push_back(processLight(s->mLights[i]));

	return res;
}

Scene AssimpImport(const std::string& pFile) {
	Assimp::Importer importer;

	const aiScene* scene =
		importer.ReadFile(pFile,
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
