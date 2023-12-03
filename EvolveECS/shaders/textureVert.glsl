#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

// What if we took all the camera data into a uniform
// buffer object and shader looks at it whenver we update it
// And also why bother sending camera data for every mesh?
// No longer going to use the simple uniforms
// we'll use the standard 140 layout detailed in the UBO_Padding.h file
// remember the bindingPoint = 0 for the camera
layout(std140, binding = 0) uniform CameraMatrices{ // this is not a struct
    mat4 projection; // remember we filled in the projection matrix first into the UBO
    mat4 view;
};

layout(std140, binding = 1) uniform LightData{ 
    vec3 lightPos; // remember we filled in the position first
    vec4 colour;
};

layout(location = 2) uniform mat4 modelMatrix;

layout(location = 0) out vec3 vertNormal; // technically the next stage is the tesselation shader, then geometry shader
layout(location = 1) out vec3 lightDir;
layout(location = 2) out vec3 eyeDir; 
layout(location = 3) out vec2 texCoord; 


void main() { // this will be called for every vertex/normal/UV. Flushes after every three vertices (primitive is triangle)
    texCoord = uvCoord;
    texCoord.y *= -1.0;
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    vertNormal = normalize(normalMatrix * vNormal); /// Rotate the normal to the correct orientation 
    vec3 vertPos = vec3(modelMatrix * vVertex); // should vertPos care about the camera's view matrix? 
    vec3 vertDir = normalize(vertPos);
    eyeDir = -vertDir;
    lightDir = normalize(lightPos - vertPos); 
    // read the bottom from right to left for model. But would be left to right for camera (which doesn't exist)
    // to get the modelMatrix work for a hammer parented to mario
    gl_Position = projection * view * modelMatrix * vVertex; // position will be in 3D. It grabs the w component. Hardware divides x, y, z by w to normalize
}