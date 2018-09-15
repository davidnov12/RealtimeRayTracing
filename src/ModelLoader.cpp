#include "ModelLoader.h"


ModelLoader::ModelLoader(std::string path){

	minCoord = glm::vec3(std::numeric_limits<float>::max());
	maxCoord = glm::vec3(std::numeric_limits<float>::min());

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals/* | aiProcess_CalcTangentSpace*/);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		printf("fail\n");
		return;
	}
	
	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);

#ifdef TRIANGLE_SET
	printf("%f %f %f\n", geom[1].vertex_a.x, geom[1].vertex_a.y, geom[1].vertex_a.z);
	printf("%f %f %f\n", geom[1].vertex_b.x, geom[1].vertex_b.y, geom[1].vertex_b.z);
	printf("%f %f %f\n", geom[1].vertex_c.x, geom[1].vertex_c.y, geom[1].vertex_c.z);
#endif
	std::cout << "DONE\n";

}

ModelLoader::~ModelLoader(){
	geom.clear();
	mats.clear();
}

void ModelLoader::processNode(aiNode * node, const aiScene * scene){
	
	//std::cout << "NODE " << node->mName.C_Str() << std::endl;

	for (unsigned int i = 0; i < node->mNumMeshes; i++){	
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++){
		processNode(node->mChildren[i], scene);
	}
}

void ModelLoader::processMesh(aiMesh * mesh, const aiScene * scene){

	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> uvs;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

		vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		minCoord.x = std::min(minCoord.x, (float)mesh->mVertices[i].x);
		minCoord.y = std::min(minCoord.y, (float)mesh->mVertices[i].y);
		minCoord.z = std::min(minCoord.z, (float)mesh->mVertices[i].z);

		maxCoord.x = std::max(maxCoord.x, (float)mesh->mVertices[i].x);
		maxCoord.y = std::max(maxCoord.y, (float)mesh->mVertices[i].y);
		maxCoord.z = std::max(maxCoord.z, (float)mesh->mVertices[i].z);
		
		if (mesh->mNormals != NULL)
			normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));

		if (mesh->mTextureCoords[0])
			uvs.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
			
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {

#ifdef TRIANGLE_SET
		triangle t;
		
		aiFace face = mesh->mFaces[i];
			
		t.vertex_a = glm::vec4(vertices[face.mIndices[0]], 1.0f);
		t.vertex_b = glm::vec4(vertices[face.mIndices[1]], 1.0f);
		t.vertex_c = glm::vec4(vertices[face.mIndices[2]], 1.0f);

		t.normal_a = glm::vec4(normals[face.mIndices[0]], 1.0f);
		t.normal_b = glm::vec4(normals[face.mIndices[1]], 1.0f);
		t.normal_c = glm::vec4(normals[face.mIndices[2]], 1.0f);

		if (uvs.size()) {
			t.uv_a = uvs[face.mIndices[0]];
			t.uv_b = uvs[face.mIndices[1]];
			t.uv_c = uvs[face.mIndices[2]];
		}

		geom.push_back(t);

#else

#ifdef VERTEX_SET
		vertex v[3];

		aiFace face = mesh->mFaces[i];

		v[0].coord = vertices[face.mIndices[0]];
		v[1].coord = vertices[face.mIndices[1]];
		v[2].coord = vertices[face.mIndices[2]];
		
		if(normals.size()){
			v[0].normal = normals[face.mIndices[0]];
			v[1].normal = normals[face.mIndices[1]];
			v[2].normal = normals[face.mIndices[2]];
		}

		if (uvs.size()){
			v[0].uv = uvs[face.mIndices[0]];
			v[1].uv = uvs[face.mIndices[1]];
			v[2].uv = uvs[face.mIndices[2]];
		}

		geom.push_back(v[0]);
		geom.push_back(v[1]);
		geom.push_back(v[2]);

#endif
#endif
	}

}

std::vector<Mesh> ModelLoader::getMeshes(){
	return meshes;
}

void* ModelLoader::getGeometry(){
	return geom.data();
}

std::vector<material> ModelLoader::getMaterials(){
	return mats;
}

unsigned ModelLoader::getVertices(){
#ifdef TRIANGLE_SET
	return geom.size() * 3;
#else
#ifdef VERTEX_SET
	return geom.size();
#endif
#endif	
}

unsigned ModelLoader::getTriangles(){
#ifdef TRIANGLE_SET
	return geom.size();
#else
#ifdef VERTEX_SET
	return geom.size() / 3;
#endif
#endif	
}

std::vector<triangle> ModelLoader::verticesToTriangles(){

#ifdef TRIANGLE_SET
	return geom;
#else
#ifdef VERTEX_SET

	std::vector<triangle> tng;
	for (unsigned int i = 0; i < geom.size(); i += 3) {
		triangle t;

		t.vertex_a = glm::vec4(geom[i].coord, 1.0f);
		t.normal_a = glm::vec4(geom[i].normal, 1.0f);
		t.uv_a = geom[i].uv;

		t.vertex_b = glm::vec4(geom[i + 1].coord, 1.0f);
		t.normal_b = glm::vec4(geom[i + 1].normal, 1.0f);
		t.uv_b = geom[i + 1].uv;

		t.vertex_c = glm::vec4(geom[i + 2].coord, 1.0f);
		t.normal_c = glm::vec4(geom[i + 2].normal, 1.0f);
		t.uv_c = geom[i + 2].uv;

		tng.push_back(t);
	}
	return tng;

#endif
#endif
}

glm::vec3 ModelLoader::getMinCoord(){
	return minCoord;
}

glm::vec3 ModelLoader::getMaxCoord(){
	return maxCoord;
}
