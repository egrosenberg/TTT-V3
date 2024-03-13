#include "Model.h"
#include "main.h"

Model::Model(const char *fname)
{
	// read json file
	std::string text = readFile(fname);
	m_JSON = json::parse(text);

	// initalize transform matrix
	m_transform = glm::mat4(1.0f);

	// set file name
	m_fname = fname;
	// read bin file
	m_data = GetData();
	
	// init vectors
	m_loadedTexName = new std::vector<std::string>();
	m_loadedTex = new std::vector<Texture*>();
	m_meshes = new std::vector<Mesh*>();
	m_translationsMeshes = new std::vector<glm::vec3>();
	m_rotationsMeshes = new std::vector<glm::quat>();
	m_scalesMeshes = new std::vector<glm::vec3>();
	m_matricesMeshes = new std::vector<glm::mat4>();

	// load meshes
	TraverseNode(0);
}

void Model::Draw(Shader *shader, Camera *camera)
{
	for (unsigned int i = 0; i < m_meshes->size(); ++i)
	{
		(*m_meshes)[i]->Mesh::Draw
		(
			shader,
			camera,
			(*m_matricesMeshes)[i] * m_transform,
			(*m_translationsMeshes)[i],
			(*m_rotationsMeshes)[i],
			(*m_scalesMeshes)[i]
		);
	}
}

void Model::LoadMesh(unsigned int indMesh)
{
	// get accessor indices for position, normal, texture, and indices
	unsigned int posAccInd = m_JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = m_JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = m_JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = m_JSON["meshes"][indMesh]["primitives"][0]["indices"];

	// Get and group positions, normals, and textureUVs
	std::vector<float> posVec = GetFloats(m_JSON["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = GroupFloatsVec3(posVec);
	std::vector<float> normalVec = GetFloats(m_JSON["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = GroupFloatsVec3(normalVec);
	std::vector<float> texVec = GetFloats(m_JSON["accessors"][texAccInd]);
	std::vector<glm::vec2> texUVs = GroupFloatsVec2(texVec);

	// combine vectors into vertexes
	std::vector<Vertex> vertices = AssembleVertices(positions, normals, texUVs);
	// get indices
	std::vector<GLuint> indices = GetIndices(m_JSON["accessors"][indAccInd]);
	// get textures
	std::vector<Texture*> textures = GetTextures();

	// add mesh to meshes vector
	Mesh *mesh = new Mesh(vertices, indices, textures);
	m_meshes->push_back(mesh);
}

// traverse nodes to get transformations
void Model::TraverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	// get current node as a json
	json node = m_JSON["nodes"][nextNode];

	// extract each transformation from node that exists
	// translation
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float tValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); ++i)
		{
			tValues[i] = (node["translation"][i]);
		}
		translation = glm::make_vec3(tValues);
	}
	// rotation
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		// swap index order because of gltf vs glm quat encoding
		float rValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2],
		};
		rotation = glm::make_quat(rValues);
	}
	// scale
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float sValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); ++i)
		{
			sValues[i] = (node["scale"][i]);
		}
		scale = glm::make_vec3(sValues);
	}
	// model
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float mValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); ++i)
		{
			mValues[i] = (node["matrix"][i]);
		}
		matNode = glm::make_mat4(mValues);
	}

	// calculate each transformation matrix
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);
	// combine transformation matrices
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	// check if we have a mesh index
	if (node.find("mesh") != node.end())
	{
		// if we do, push all the transformatinos and load the mesh
		m_translationsMeshes->push_back(translation);
		m_rotationsMeshes->push_back(rotation);
		m_scalesMeshes->push_back(scale);
		m_matricesMeshes->push_back(matNextNode);

		LoadMesh(node["mesh"]);
	}

	// check if the mesh has any children
	if (node.find("children") != node.end())
	{
		// if it does, traverse all of the child nodes 
		// while supplying the current transmatrix
		for (unsigned int i = 0; i < node["children"].size(); ++i)
		{
			TraverseNode(node["children"][i], matNextNode);
		}
	}
}

std::vector <unsigned char> *Model::GetData()
{
	// string to store the bytes as text
	std::string bytesText;
	// find file loc using buffer gives location of binary data
	std::string uri = m_JSON["buffers"][0]["uri"];

	// get file path as string
	std::string fileStr = std::string(m_fname);
	// find file directory by getting substring of file path
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	// read file contents
	bytesText = readFile((fileDirectory + uri).c_str());

	// put file contents in our vector
	return new std::vector<unsigned char>(bytesText.begin(), bytesText.end());
}


std::vector<float> Model::GetFloats(json accessor)
{
	// create vector to store floats
	std::vector<float> floatVec;

	// fetch values from accessor
	// using value() for non-guaranteed keys
	// get index of bufferview
	unsigned int buffViewInd = accessor.value("bufferView", 1);  // not guaranteed
	// get amnt of floats we have to read
	unsigned int count = accessor["count"];
	// get offset of data with respect to bufferview
	unsigned int accByteOffset = accessor.value("byteOffset", 0); // not guaranteed
	// get how we need to group floats
	std::string type = accessor["type"];

	// get bufferview nwo that we have its index
	json bufferView = m_JSON["bufferViews"][buffViewInd];
	// get byte offset for where bufferview starts
	unsigned int byteOffset = bufferView["byteOffset"];

	// set number of floats per vertex based on type
	unsigned int numPerVert = 0;
	if (type == "SCALAR")
	{
		numPerVert = 1;
	}
	else if (type == "VEC2")
	{
		numPerVert = 2;
	}
	else if (type == "VEC3")
	{
		numPerVert = 3;
	}
	else if (type == "VEC4")
	{
		numPerVert = 4;
	}
	else
	{
		// throw error if no match
		throw std::invalid_argument("Vertex type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");
	}

	// calculate data start point, length, and end point of float data
	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	unsigned int endOfData = beginningOfData + lengthOfData;
	for (unsigned int i = beginningOfData; i < endOfData; i)
	{
		// store the bytes in a temporary array
		unsigned char bytes[] = { (*m_data)[i++], (*m_data)[i++], (*m_data)[i++], (*m_data)[i++]};
		// convert bytes to float via memcpy
		float value = 0;
		std::memcpy(&value, bytes, sizeof(float));
		// add value to vector
		floatVec.push_back(value);
	}

	return floatVec;
}


std::vector<GLuint> Model::GetIndices(json accessor)
{
	// create vector to store indices
	std::vector<GLuint> indices;

	// fetch values from accessor
	// using value() for non-guaranteed keys
	// get index of bufferview
	unsigned int buffViewInd = accessor.value("bufferView", 1);  // not guaranteed
	// get amnt of floats we have to read
	unsigned int count = accessor["count"];
	// get offset of data with respect to bufferview
	unsigned int accByteOffset = accessor.value("byteOffset", 0); // not guaranteed
	// get how we need to group floats
	unsigned int componentType = accessor["componentType"];


	// get bufferview nwo that we have its index
	json bufferView = m_JSON["bufferViews"][buffViewInd];
	// get byte offset for where bufferview starts
	unsigned int byteOffset = bufferView.value("byteOffset", 0);
	if (byteOffset == 0)
	{
		std::cout << "ERROR: MODEL LOADER FAILED TO FIND BYTE OFFSET" << std::endl;
	}

	// calculate data start point
	unsigned int beginningOfData = byteOffset + accByteOffset;

	// read data based on component type
	// unsigned int
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < beginningOfData + (count * 4); i)
		{
			// store the bytes in a temporary array
			unsigned char bytes[] = { (*m_data)[i++], (*m_data)[i++], (*m_data)[i++], (*m_data)[i++] };
			// convert bytes to uint via memcpy
			unsigned int value = 0;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			// add value to vector
			indices.push_back((GLuint)value);
		}
	}
	// unsigned short
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < beginningOfData + (count * 2); i)
		{
			// store the bytes in a temporary array
			unsigned char bytes[] = { (*m_data)[i++], (*m_data)[i++] };
			// convert bytes to ushort via memcpy
			unsigned short value = 0;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			// add value to vector
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < beginningOfData + (count * 2); i)
		{
			// store the bytes in a temporary array
			unsigned char bytes[] = { (*m_data)[i++], (*m_data)[i++] };
			// convert bytes to short via memcpy
			short value = 0;
			std::memcpy(&value, bytes, sizeof(short));
			// add value to vector
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

std::vector<Texture*> Model::GetTextures()
{
	// create vector to store textures
	std::vector<Texture*> textures;

	// get file path as string
	std::string fileStr = std::string(m_fname);
	// find file directory by getting substring of file path
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// loop through images key keeping track of each texture
	for (unsigned int i = 0; i < m_JSON["images"].size(); ++i)
	{
		// get uri for current texture
		std::string texPath = m_JSON["images"][i]["uri"];

		// avoid loading duplicate textures
		bool skip = false;
		for (unsigned int j = 0; j < m_loadedTexName->size(); ++i)
		{
			if ((*m_loadedTexName)[j] == texPath)
			{
				textures.push_back((*m_loadedTex)[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			// check name of tex
			// diffuse texture
			if (texPath.find("baseColor") != std::string::npos)
			{
				Texture *diffuse = new Texture((fileDirectory + texPath).c_str(), "diffuse", m_loadedTex->size());
				textures.push_back(diffuse);
				m_loadedTex->push_back(diffuse);
				m_loadedTexName->push_back(texPath);
			}
			// aproximate specular with roughness (as we are not doing any PBR)
			else if (texPath.find("metallicRoughness") != std::string::npos)
			{
				Texture *specular = new Texture((fileDirectory + texPath).c_str(), "specular", m_loadedTex->size());
				textures.push_back(specular);
				m_loadedTex->push_back(specular);
				m_loadedTexName->push_back(texPath);
			}
		}
	}

	return textures;
}

// combines c++vector of glmvectors and combines them into a vector of vertexes
std::vector<Vertex> Model::AssembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	// create vector to store vertices
	std::vector<Vertex> vertices;
	// iterate through input vectors and add to output vector
	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f), // use default value as many models dont include color
				texUVs[i]
			}
		);
	}
	
	return vertices;
}

std::vector<glm::vec2> Model::GroupFloatsVec2(std::vector<float> floatVec)
{
	// create vector to store vecs in
	std::vector<glm::vec2> vectors;
	// iterate through float vector and grab groups of floats
	for (unsigned int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}
	// return pointer to new vector
	return vectors;
}

std::vector<glm::vec3> Model::GroupFloatsVec3(std::vector<float> floatVec)
{
	// create vector store vecs in
	std::vector<glm::vec3> vectors;
	// iterate through float vector and grab groups of floats
	for (unsigned int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	// return pointer to new vector
	return vectors;
}

std::vector<glm::vec4> Model::GroupFloatsVec4(std::vector<float> floatVec)
{
	// create vector to store vecs in
	std::vector<glm::vec4> vectors;
	// iterate through float vector and grab groups of floats
	for (unsigned int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	// return pointer to new vector
	return vectors;
}

Model::~Model()
{
	delete m_data;
	delete m_loadedTexName;
	delete m_loadedTex;

	delete m_meshes;
	delete m_translationsMeshes;
	delete m_rotationsMeshes;
	delete m_scalesMeshes;
	delete m_matricesMeshes;
}