#include "Cylinder.h"
#include <MMath.h>

using namespace GEOMETRY;
using namespace MATH;

void GEOMETRY::Cylinder::generateVerticesAndNormals()
{
    // We need to fill the vertices and normals arrays with the correct data for a sphere
    // deltaTheta governs how close each ring will be around our sphere. Try messing with it
    const float deltaTheta = 2.5f;
    // deltaPhi governs how close each point will be per ring. Try messing with it
    const float deltaPhi = 0.5f;
    const float deltaRing = 10.0f;
    Matrix3 scaleMatrix;
    for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta)
    {
        // Build a ring
        Vec3 circle(radius * sin(thetaDeg * DEGREES_TO_RADIANS), radius * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f);
        for (int i = 10; i > 1; )
        {
            //a top lid of the capsule
            scaleMatrix = MMath::scale(Vec3(i / deltaRing, i / deltaRing, 0.0f));
            Vec3 scaledCircle = scaleMatrix * circle;
            vertices.push_back(scaledCircle);
            normals.push_back(scaledCircle);
            i--;
        }
        //draw subsequent capsule rings
        for (float phiDeg = deltaPhi; phiDeg <= 3; phiDeg += deltaPhi)
        {
            circle.z += deltaPhi;
            vertices.push_back(circle);
            normals.push_back(circle);

            //draw a bottom lid of the capsule
            if (phiDeg == 3)
            {
                for (int i = 9; i > 1; )
                {
                    scaleMatrix = MMath::scale(Vec3(i / deltaRing, i / deltaRing, 0.0f));
                    circle = scaleMatrix * circle;
                    circle.z += phiDeg;
                    vertices.push_back(circle);
                    normals.push_back(circle);
                    i--;
                }
            }
        }
    }

    // Once we are all done filling the vertices and normals, use the base class method to store the data in the GPU
    StoreMeshData(GL_POINTS);
}
