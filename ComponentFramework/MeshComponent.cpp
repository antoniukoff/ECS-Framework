#include "MeshComponent.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

MeshComponent::MeshComponent(Component* parent_, const char* filename_) : Component(parent_) {
    filename = filename_;
}
MeshComponent::~MeshComponent() {
    OnDestroy();
}

bool MeshComponent::OnCreate() {
    if (isCreated) return true;
    // tinyObj throws an exception if it's having a bad day
    // blame management system Scott calls it
    // we'll do something with this later
    LoadModel(filename);
    StoreMeshData(GL_TRIANGLES);

    isCreated = true;
    return true;
}

void MeshComponent::LoadModel(const char* filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    vertices.clear();
    normals.clear();
    uvCoords.clear();

    // we don't have a catch, so it will just bubble up the stack looking and then die
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename)) {
        throw std::runtime_error(warn + err);
    }
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vec3 vertex{};
            vertex.x = attrib.vertices[3 * index.vertex_index + 0];
            vertex.y = attrib.vertices[3 * index.vertex_index + 1];
            vertex.z = attrib.vertices[3 * index.vertex_index + 2];

            Vec3 normal{};
            normal.x = attrib.normals[3 * index.normal_index + 0];
            normal.y = attrib.normals[3 * index.normal_index + 1];
            normal.z = attrib.normals[3 * index.normal_index + 2];

            Vec2 uvCoord{};
            uvCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
            uvCoord.y = attrib.texcoords[2 * index.texcoord_index + 1];

            vertices.push_back(vertex);
            normals.push_back(normal);
            uvCoords.push_back(uvCoord);
        }
    }
}

void MeshComponent::StoreMeshData(GLenum drawmode_) {
    drawmode = drawmode_;
    /// These just make the code easier for me to read
    // working out the size of these buffers
    // for example each float is 4 bytes, so a Vec3 is 4*3 = 12 bytes
#define VERTEX_LENGTH 	(vertices.size() * (sizeof(Vec3)))
#define NORMAL_LENGTH 	(normals.size() * (sizeof(Vec3)))
#define TEXCOORD_LENGTH (uvCoords.size() * (sizeof(Vec2)))

    // these are the channels
    const int verticiesLayoutLocation = 0;
    const int normalsLayoutLocation = 1;
    const int uvCoordsLayoutLocation = 2;

    /// create and bind the VAO
    glGenVertexArrays(1, &vao);
    // Bind means, hey! Im talking to you!
    glBindVertexArray(vao);
    /// Create and initialize vertex buffer object VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Generate memory for the VRAM of the GPU buffer
    // one giant array
    glBufferData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH + TEXCOORD_LENGTH, nullptr, GL_STATIC_DRAW);

    /// assigns the addr of "points" to be the beginning of the array buffer "sizeof(points)" in length
    // vertex stuff will start at zero and go VERTEX_LENGTH
    glBufferSubData(GL_ARRAY_BUFFER, 0, VERTEX_LENGTH, &vertices[0]);
    /// assigns the addr of "normals" to be "sizeof(points)" offset from the beginning and "sizeof(normals)" in length  
    // start where the vertex list ended
    glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH, NORMAL_LENGTH, &normals[0]);
    /// assigns the addr of "texCoords" to be "sizeof(points) + sizeof(normals)" offset from the beginning and "sizeof(texCoords)" in length  
    // start where the normal list ended
    glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH, TEXCOORD_LENGTH, &uvCoords[0]);

    // the attributes are the per-vertex stuff like vertices, normals, UVs
    glEnableVertexAttribArray(verticiesLayoutLocation);
    // 3 of them is the stride
    // floating point long using GL_FLOAT. So striding by 12 bytes
    // the last argument is a void*, but where does it begin?
    // Its an old C trick to declare a "I don't care" pointer
    // a void * is an integer. A 4 byte unsigned integer
    // and we start at the beginning
    // need a very brutal reinterpret_cast means Dammit!
    glVertexAttribPointer(verticiesLayoutLocation, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(normalsLayoutLocation);
    glVertexAttribPointer(normalsLayoutLocation, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(VERTEX_LENGTH));
    glEnableVertexAttribArray(uvCoordsLayoutLocation);
    // we stride by 2 for UV as its a Vec2
    glVertexAttribPointer(uvCoordsLayoutLocation, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(VERTEX_LENGTH + NORMAL_LENGTH));

    
    dataLength = vertices.size();

    /// give back the memory used in these vectors. The data is safely stored in the GPU now
    vertices.clear();
    normals.clear();
    uvCoords.clear();

    /// Don't need these defines sticking around anymore
#undef VERTEX_LENGTH
#undef NORMAL_LENGTH
#undef TEXCOORD_LENGTH

}

void MeshComponent::Render() const {
    glBindVertexArray(vao);
    glDrawArrays(drawmode, 0, dataLength);
    glBindVertexArray(0); // Unbind the VAO
}

void MeshComponent::Render(GLenum drawmode_) const {
    glBindVertexArray(vao);
    glDrawArrays(drawmode_, 0, dataLength);
    glBindVertexArray(0); // Unbind the VAO
}

void MeshComponent::OnDestroy() {
    // kill all the memory in the VRAM
    glDeleteBuffers(1, &vbo);
    // the vao holds the vbo so do that last
    glDeleteVertexArrays(1, &vao);
    isCreated = false;
}

void MeshComponent::Update(const float deltaTime) {}