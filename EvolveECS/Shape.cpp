#include "Shape.h"

void GEOMETRY::Shape::debugDraw()
{
    glBindVertexArray(vao);
    glDrawArrays(drawmode, 0, dataLength);
    glBindVertexArray(0);
}

void GEOMETRY::Shape::StoreMeshData(GLenum drawmode_)
{
    drawmode = drawmode_;
    /// These just make the code easier for me to read
    // working out the size of these buffers
    // for example each float is 4 bytes, so a Vec3 is 4*3 = 12 bytes
#define VERTEX_LENGTH 	(vertices.size() * (sizeof(MATH::Vec3)))
#define NORMAL_LENGTH 	(normals.size() * (sizeof(MATH::Vec3)))

    // these are the channels
    const int verticiesLayoutLocation = 0;
    const int normalsLayoutLocation = 1;

    /// create and bind the VAO
    glGenVertexArrays(1, &vao);
    // Bind means, hey! Im talking to you!
    glBindVertexArray(vao);
    /// Create and initialize vertex buffer object VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Generate memory for the VRAM of the GPU buffer
    // one giant array
    glBufferData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH, nullptr, GL_STATIC_DRAW);

    /// assigns the addr of "points" to be the beginning of the array buffer "sizeof(points)" in length
    // vertex stuff will start at zero and go VERTEX_LENGTH
    glBufferSubData(GL_ARRAY_BUFFER, 0, VERTEX_LENGTH, &vertices[0]);
    /// assigns the addr of "normals" to be "sizeof(points)" offset from the beginning and "sizeof(normals)" in length  
    // start where the vertex list ended
    glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH, NORMAL_LENGTH, &normals[0]);

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

    dataLength = vertices.size();

    /// give back the memory used in these vectors. The data is safely stored in the GPU now
    vertices.clear();
    normals.clear();

    /// Don't need these defines sticking around anymore
#undef VERTEX_LENGTH
#undef NORMAL_LENGTH
}
