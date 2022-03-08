#include "animatedObject.hpp"

void JointAnim::update(float animationTime)
{
	glm::mat4 position = interpolatePosition(animationTime);
	glm::mat4 rotation = interpolateRotation(animationTime);
	glm::mat4 scale = interpolateScaling(animationTime);
	localTransform = position * rotation * scale;
}

float JointAnim::getScaleFactor(float lastTimestamp, float nextTimestamp, float t)
{
	float midway = t - lastTimestamp;
	float framesDiff = nextTimestamp - lastTimestamp;
	return midway / framesDiff;
}

int JointAnim::getPositionIndex(float t)
{
	for(int i{0}; i < pKeys.size() - 1; ++i)
	{
		if(t < pKeys[i + 1].timestamp)
			return i;
	}
	return -1;
}

int JointAnim::getRotationIndex(float t)
{
	for(int i{0}; i < rKeys.size() - 1; ++i)
	{
		if(t < rKeys[i + 1].timestamp)
			return i;
	}
	return -1;
}

int JointAnim::getScaleIndex(float t)
{
	for(int i{0}; i < sKeys.size() - 1; ++i)
	{
		if(t < sKeys[i + 1].timestamp)
			return i;
	}
	return -1;
}

glm::mat4 JointAnim::interpolatePosition(float t)
{
	if(pKeys.size() == 1)
	{
		return glm::translate(glm::mat4(1.0f), pKeys[0].position);
	}
	int index_left = getPositionIndex(t);
	int index_right = index_left + 1;
	float scale = getScaleFactor(pKeys[index_left].timestamp, pKeys[index_right].timestamp, t);
	glm::vec3 finalPos = glm::mix(pKeys[index_left].position, pKeys[index_right].position, scale);
	return glm::translate(glm::mat4(1.0f), finalPos);
}

glm::mat4 JointAnim::interpolateRotation(float t)
{
	if(rKeys.size() == 1)
	{
		auto rotation = glm::normalize(rKeys[0].rotation);
		return glm::toMat4(rotation);
	}
	int index_left = getRotationIndex(t);
	int index_right = index_left + 1;
	float scale = getScaleFactor(rKeys[index_left].timestamp, rKeys[index_right].timestamp, t);
	glm::quat finalRotation = glm::slerp(rKeys[index_left].rotation, rKeys[index_right].rotation, scale);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 JointAnim::interpolateScaling(float t)
{
	if(sKeys.size() == 1)
	{
		return glm::scale(glm::mat4(1.0f), sKeys[0].scale);
	}
	int index_left = getScaleIndex(t);
	int index_right = index_left + 1;
	float scale = getScaleFactor(sKeys[index_left].timestamp, sKeys[index_right].timestamp, t);
	glm::vec3 finalScale = glm::mix(sKeys[index_left].scale, sKeys[index_right].scale, scale);
	return glm::scale(glm::mat4(1.0f), finalScale);
}

// ############################################################
// ############################################################
// ############################################################

Animator::Animator() :
	currentTime(0.0f)
{
	for(int i{0}; i < 50; ++i)
		finalJointTransform[i] = glm::mat4(1.0f);
}

Animator::Animator(std::shared_ptr<Animation> animation) :
	currentTime(0.0f),
	currentAnimation(animation)
{
	for(int i{0}; i < 50; ++i)
		finalJointTransform[i] = glm::mat4(1.0f);
}

void Animator::updateAnimation(float delta)
{
	if(currentAnimation)
	{
		currentTime += currentAnimation->ticksPerSecond * delta;
		float moduloCurrentTime = fmod(currentTime, currentAnimation->duration);
		if(moduloCurrentTime < currentTime && currentAnimation->name == "Jump_game")
		{
			stopAnimation();
		}
		else
		{
			currentTime = moduloCurrentTime;
			calculateJointTransform(currentAnimation->rootJoint, glm::mat4(1.0f));
		}
	}
}

void Animator::playAnimation(std::shared_ptr<Animation> & animation)
{
	if(currentAnimation != animation)
	{
		currentAnimation = animation;
		currentTime = 0.0f;
	}
}

void Animator::stopAnimation()
{
	if(currentAnimation)
	{
		currentAnimation.reset();
		currentTime = 0.0f;
	}
}

void Animator::calculateJointTransform(std::shared_ptr<Joint> joint, glm::mat4 parentTransform)
{
	glm::mat4 jointTransform;
	std::shared_ptr<struct JointAnim> jointAnim;

	for(int i{0}; i < currentAnimation->jointAnim.size(); ++i)
	{
		if(currentAnimation->jointAnim[i]->joint->getId() == joint->getId())
		{
			jointAnim = currentAnimation->jointAnim[i];
			break;
		}
	}

	if(jointAnim)
	{
		jointAnim->update(currentTime);
		jointTransform = jointAnim->localTransform;

		glm::mat4 globalTransformation = parentTransform * jointTransform;

		int index = jointAnim->joint->getId();
		glm::mat4 offset = jointAnim->joint->getOffsetMatrix();
		finalJointTransform[index] = currentAnimation->globalInverseTransform * globalTransformation * offset;

		for(int i{0}; i < joint->getChildren().size(); ++i)
		{
			calculateJointTransform(joint->getChildren()[i], globalTransformation);
		}
	}
}

std::array<glm::mat4, 50> & Animator::getFinalJointTransform()
{
	return finalJointTransform;
}

std::shared_ptr<Animation> & Animator::getCurrentAnimation()
{
	return currentAnimation;
}

// ############################################################
// ############################################################
// ############################################################

AnimatedObject::AnimatedObject(const std::string & path, glm::mat4 p_model) :
	Object(p_model),
	animator(std::make_unique<Animator>())
{
	load(path);
}

AnimatedObject::~AnimatedObject(){}

std::vector<std::shared_ptr<Animation>> & AnimatedObject::getAnimations()
{
	return animations;
}

std::unique_ptr<Animator> & AnimatedObject::getAnimator()
{
	return animator;
}

void AnimatedObject::draw(Shader& shader, std::array<glm::mat4, 50> & finalJointTransform, struct IBL_DATA * iblData, DRAWING_MODE mode)
{
	shader.use();
	shader.setInt("animated", 1);
	shader.setMatrix("model", model);

	for(int i{0}; i < finalJointTransform.size(); ++i)
	{
		std::string boneMatrixStr{"bonesMatrices["};
		boneMatrixStr += std::to_string(i);
		boneMatrixStr += std::string("]");
		shader.setMatrix(boneMatrixStr, finalJointTransform[i]);
	}

	int meshCount = meshes.size();

	for(int i{0}; i < meshCount; ++i)
	{
		meshes[i]->draw(shader, iblData, instancing, instanceModel.size(), mode);
	}
}

void AnimatedObject::load(const std::string & path)
{
	/*
	// prepare export to assbin
	Assimp::Exporter exporter;
	auto numExtensions = exporter.GetExportFormatCount();
	int extensionIndex{-1};
	for(int i{0}; i < numExtensions; ++i)
	{
		const aiExportFormatDesc * format = exporter.GetExportFormatDescription(i);
		if(std::string(format->id) == "assbin")
		{
			extensionIndex = i;
			break;
		}
	}
	const aiExportFormatDesc * format = exporter.GetExportFormatDescription(extensionIndex);
	*/
	// regular import
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || ! scene->mRootNode)
	{
		std::cerr << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	name = scene->mRootNode->mName.C_Str();
	int dotIndex = name.find_first_of(".");
	name = name.substr(0, dotIndex);

	if(scene->HasAnimations())
	{
		loadJointHierarchy(scene);
		loadAnimations(scene->mAnimations, scene->mNumAnimations);
		for(int i{0}; i < animations.size(); ++i)
		{
			animations[i]->globalInverseTransform = assimpMat4_to_glmMat4(scene->mRootNode->mTransformation.Inverse());
		}
	}

	exploreNode(scene->mRootNode, scene);
	computeAABB();
	/*
	// export to assbin
	std::string fileName{path.substr(0, path.find_last_of('.')) + ".assbin"};
	exporter.Export(scene, format->id, fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	*/
}

void AnimatedObject::loadJointHierarchy(const aiScene * scene)
{
	// compute list of all bones' name
	std::vector<std::string> bonesName;

	for(int m{0}; m < scene->mNumMeshes; ++m)
	{
		aiMesh * currentMesh = scene->mMeshes[m];
		for(int b{0}; b < currentMesh->mNumBones; ++b)
		{
			aiBone* currentBone = currentMesh->mBones[b];
			std::string bName = currentBone->mName.C_Str();
			if(std::find(bonesName.begin(), bonesName.end(), bName) == bonesName.end())
			{
				bonesName.push_back(bName);
			}
		}
	}

	// get root bone of skeleton
	aiNode * rootBone = getRootBone(scene->mRootNode, bonesName);
	rootJoint = std::make_shared<Joint>(1, rootBone->mName.C_Str());

	// compute hierarchy
	computeHierarchy(rootBone, rootJoint, bonesName);

	// assign offset matrices to each joint
	for(int m{0}; m < scene->mNumMeshes; ++m)
	{
		aiMesh * currentMesh = scene->mMeshes[m];
		for(int b{0}; b < currentMesh->mNumBones; ++b)
		{
			aiBone* currentBone = currentMesh->mBones[b];
			std::string bName = currentBone->mName.C_Str();
			if(nameJoint[bName] != nullptr)
			{
				aiMatrix4x4 offsetMatrix = currentBone->mOffsetMatrix;
				nameJoint[bName]->setOffsetMatrix(assimpMat4_to_glmMat4(offsetMatrix));
			}
		}
	}
}

void AnimatedObject::computeHierarchy(aiNode* node, std::shared_ptr<Joint> joint, std::vector<std::string> & bNames)
{
	static int ID{1};

	nameJoint[joint->getName()] = joint;
	finalJointTransform[joint->getName()] = glm::mat4(1.0f);
	for(int i{0}; i < node->mNumChildren; ++i)
	{
		aiNode* child = node->mChildren[i];
		if(std::find(bNames.begin(), bNames.end(), child->mName.C_Str()) != bNames.end())
		{
			ID++;
			std::shared_ptr<Joint> j = std::make_shared<Joint>(ID, child->mName.C_Str());
			joint->addChild(j);
			computeHierarchy(child, j, bNames);
		}
	}
}

aiNode* AnimatedObject::getRootBone(aiNode * node, std::vector<std::string> & bNames)
{
	aiNode* rootBone = nullptr;

	if(std::find(bNames.begin(), bNames.end(), node->mName.C_Str()) != bNames.end())
	{
		aiNode* parentNode = node->mParent;
		if(std::find(bNames.begin(), bNames.end(), parentNode->mName.C_Str()) == bNames.end())
		{
			rootBone = node;
			return rootBone;
		}
	}

	for(int i{0}; i < node->mNumChildren; ++i)
	{
		rootBone = getRootBone(node->mChildren[i], bNames);
		if(rootBone != nullptr)
			break;
	}

	return rootBone;
}

void AnimatedObject::loadAnimations(aiAnimation** anims, int animCount)
{
	aiString name;
	double duration;
	double ticksPerSecond;
	for(int i{0}; i < animCount; ++i)
	{
		aiAnimation* current = anims[i];
		name = current->mName;
		duration = current->mDuration;
#ifdef __unix__
		ticksPerSecond = current->mTicksPerSecond;
#elif defined(_WIN32) || defined(WIN32)
		ticksPerSecond = 1000.0f;
#endif

		std::shared_ptr<Animation> animation = std::make_shared<Animation>();
		animation->name = name.C_Str();
		animation->duration = static_cast<float>(duration);
		animation->ticksPerSecond = static_cast<float>(ticksPerSecond);
		animation->rootJoint = rootJoint;

		for(int n{0}; n < current->mNumChannels; ++n) // number of bones involved
		{
			aiNodeAnim* nodeAnim = current->mChannels[n];
			std::shared_ptr<struct JointAnim> jointAnim = std::make_shared<struct JointAnim>();
			jointAnim->joint = nameJoint[nodeAnim->mNodeName.C_Str()];
			jointAnim->localTransform = glm::mat4(1.0f);

			for(int k{0}; k < nodeAnim->mNumPositionKeys; ++k)
			{
				aiVectorKey positionVectorKey = nodeAnim->mPositionKeys[k];
				aiVector3D position = positionVectorKey.mValue;

				struct PositionKey pKey;
				pKey.timestamp = static_cast<float>(positionVectorKey.mTime);
				pKey.position = glm::vec3(position.x, position.y, position.z);
				
				jointAnim->pKeys.push_back(pKey);
			}
			for(int k{0}; k < nodeAnim->mNumRotationKeys; ++k)
			{
				aiQuatKey rotationQuatKey = nodeAnim->mRotationKeys[k];
				aiQuaternion rotation = rotationQuatKey.mValue;

				struct RotationKey rKey;
				rKey.timestamp = static_cast<float>(rotationQuatKey.mTime);
				rKey.rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);

				jointAnim->rKeys.push_back(rKey);
			}
			for(int k{0}; k < nodeAnim->mNumScalingKeys; ++k)
			{
				aiVectorKey scalingVectorKey = nodeAnim->mScalingKeys[k];
				aiVector3D scaling = scalingVectorKey.mValue;

				struct ScalingKey sKey;
				sKey.timestamp = static_cast<float>(scalingVectorKey.mTime);
				sKey.scale = glm::vec3(scaling.x, scaling.y, scaling.z);
				
				jointAnim->sKeys.push_back(sKey);
			}
			animation->jointAnim.push_back(jointAnim);
		}

		animations.push_back(animation);
	}
}

std::shared_ptr<Mesh> AnimatedObject::getMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	struct Material material;
    glm::vec3 center = glm::vec3(0.0f);
	std::string meshName(mesh->mName.C_Str());
	
	// vertices
	int nb_vertices = mesh->mNumVertices;
	
	glm::vec3 v_pos;
	glm::vec3 v_norm;
	glm::vec2 v_tex_coords;
	glm::vec3 v_tangent;
	glm::vec3 v_bTangent;
	glm::ivec4 v_bonesID = glm::ivec4(0);
	glm::vec4 v_bonesWeights = glm::vec4(0.0f);

	aiVector3D vertex_pos;
	aiVector3D vertex_norm;
	aiVector3D vertex_tangent;
	aiVector3D vertex_bTangent;

	for(int i{0}; i < nb_vertices; ++i)
	{
		vertex_pos = mesh->mVertices[i];
		vertex_norm = mesh->mNormals[i];
		if(mesh->HasNormals())
		{
			vertex_tangent = mesh->mTangents[i];
			vertex_bTangent = mesh->mBitangents[i];
		}
		else
		{
			vertex_tangent = aiVector3D(0.0f, 0.0f, 0.0f);
			vertex_bTangent = aiVector3D(0.0f, 0.0f, 0.0f);
		}

		v_pos = glm::vec3(vertex_pos.x, vertex_pos.y, vertex_pos.z);
		v_norm = glm::vec3(vertex_norm.x, vertex_norm.y, vertex_norm.z);
		v_tangent = glm::vec3(vertex_tangent.x, vertex_tangent.y, vertex_tangent.z);
		v_bTangent = glm::vec3(vertex_bTangent.x, vertex_bTangent.y, vertex_bTangent.z);
		
		if(mesh->mTextureCoords[0])
		{
			v_tex_coords.x = mesh->mTextureCoords[0][i].x;
			v_tex_coords.y = mesh->mTextureCoords[0][i].y;
		}
		else
			v_tex_coords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(Vertex(v_pos, v_norm, v_tex_coords, v_tangent, v_bTangent));
        center += v_pos;
	}
    
    center /= static_cast<float>(nb_vertices);

	// #################### SET BONE DATA ####################
	for(int j{0}; j < mesh->mNumBones; ++j)
	{
		aiBone * bone = mesh->mBones[j];
		for(int v{0}; v < bone->mNumWeights; ++v) // vertices influenced by current bone
		{
			// get bone ID
			int bone_ID = nameJoint[bone->mName.C_Str()]->getId();

			// get bone weight on current vertex
			float weight = bone->mWeights[v].mWeight;

			// end
			int vertexIndex = bone->mWeights[v].mVertexId;
			for(int i{0}; i < 4; ++i)
			{
				if(vertices[vertexIndex].weights[i] == 0.0f)
				{
					vertices[vertexIndex].bonesID[i] = bone_ID;
					vertices[vertexIndex].weights[i] = weight;
					break;
				}
			}
		}
	}
	// #######################################################

	// indices
	int nb_faces = mesh->mNumFaces;
	int nb_indices_face = 0;

	for(int i{0}; i < nb_faces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		nb_indices_face = face.mNumIndices;

		for(int j{0}; j < nb_indices_face; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// material
	aiMaterial* mesh_material = scene->mMaterials[mesh->mMaterialIndex];
    
	aiString material_name{"material_name"};
	aiColor4D color_diffuse{0.0f, 0.0f, 0.0f, 1.0f};
	aiColor3D color_specular{0.0f, 0.0f, 0.0f};
	aiColor3D color_ambient{0.0f, 0.0f, 0.0f};
	aiColor3D color_emissive{0.0f, 0.0f, 0.0f};
	int opaque{1};
    float opacity{1.0f};
	float shininess{1.0f};
	float roughness{0.5f};
	float metallic{0.0f};
	float emission_intensity{0.0f};
	std::vector<Texture> textures;

	std::vector<Texture> diffuse = loadMaterialTextures(scene, mesh_material, aiTextureType_DIFFUSE, TEXTURE_TYPE::DIFFUSE);
	textures.insert(textures.end(), diffuse.begin(), diffuse.end());
	
	std::vector<Texture> specular = loadMaterialTextures(scene, mesh_material, aiTextureType_SPECULAR, TEXTURE_TYPE::SPECULAR);
	textures.insert(textures.end(), specular.begin(), specular.end());
	
	std::vector<Texture> normal = loadMaterialTextures(scene, mesh_material, aiTextureType_NORMALS, TEXTURE_TYPE::NORMAL);
	if(normal.size() == 0)
		normal = loadMaterialTextures(scene, mesh_material, aiTextureType_HEIGHT, TEXTURE_TYPE::NORMAL);
	textures.insert(textures.end(), normal.begin(), normal.end());

	std::vector<Texture> metallicRoughMaps = loadMaterialTextures(scene, mesh_material, aiTextureType_METALNESS, TEXTURE_TYPE::METALLIC_ROUGHNESS);
	if(metallicRoughMaps.size() == 0)
		metallicRoughMaps = loadMaterialTextures(scene, mesh_material, aiTextureType_UNKNOWN, TEXTURE_TYPE::METALLIC_ROUGHNESS);
	textures.insert(textures.end(), metallicRoughMaps.begin(), metallicRoughMaps.end());
	
	std::vector<Texture> emission = loadMaterialTextures(scene, mesh_material, aiTextureType_EMISSIVE, TEXTURE_TYPE::EMISSIVE);
	textures.insert(textures.end(), emission.begin(), emission.end());

	mesh_material->Get(AI_MATKEY_NAME, material_name);
	mesh_material->Get(AI_MATKEY_COLOR_DIFFUSE, color_diffuse);
	mesh_material->Get(AI_MATKEY_COLOR_SPECULAR, color_specular);
	mesh_material->Get(AI_MATKEY_COLOR_AMBIENT, color_ambient);
	mesh_material->Get(AI_MATKEY_COLOR_EMISSIVE, color_emissive);
	mesh_material->Get(AI_MATKEY_OPACITY, opacity);
	mesh_material->Get(AI_MATKEY_SHININESS, shininess);
	mesh_material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, roughness);
	mesh_material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallic);
	
	//########## START GET EMISSION INTENSITY & OPAQUE DATA ##########
	std::string file{fullPath.substr(0, fullPath.find_last_of('.')) + ".xml"};
	std::ifstream metadata{file};
	if(!metadata.fail())
	{
		std::vector<char> buffer((std::istreambuf_iterator<char>(metadata)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');

		rapidxml::xml_document<> doc;
		doc.parse<0>(&buffer[0]);
		rapidxml::xml_node<> * root_node = doc.first_node("Collection");

		for(rapidxml::xml_node<> * mesh_node = root_node->first_node("Material"); mesh_node; mesh_node = mesh_node->next_sibling())
		{
			rapidxml::xml_attribute<> * attr = mesh_node->first_attribute("name");
			if(strcmp(attr->value(), material_name.C_Str()) == 0)
			{
				attr = attr->next_attribute();
				emission_intensity = atof(attr->value());
				attr = attr->next_attribute();
				opaque = atoi(attr->value());
			}
		}
	}
	//########## END GET EMISSION INTENSITY & OPAQUE DATA ##########

	material.textures = textures;
    material.color_diffuse = glm::vec3(color_diffuse.r, color_diffuse.g, color_diffuse.b);
    material.color_specular = glm::vec3(color_specular.r, color_specular.g, color_specular.b);
    material.color_ambient = glm::vec3(color_ambient.r, color_ambient.g, color_ambient.b);
    material.color_emissive = glm::vec3(color_emissive.r, color_emissive.g, color_emissive.b);
	material.opaque = opaque;
	material.opacity = opacity;
	material.shininess = shininess;
	material.roughness = roughness;
	material.metallic = metallic;
	material.emission_intensity = emission_intensity;

	// pack everything
    return std::make_shared<Mesh>(vertices, indices, material, meshName, center);
}
