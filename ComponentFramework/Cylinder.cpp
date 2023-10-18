#include "Cylinder.h"
#include <MMath.h>
#include "QuadraticSolver.h"

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

RayIntersectionInfo GEOMETRY::Cylinder::rayIntersectionInfo(const Ray& ray) const
{
    RayIntersectionInfo rayInfo = checkInfiniteCylinder(ray);
    if (rayInfo.isIntersected == false) {
        return rayInfo;
    }
       // are we outside of endCapA
    Vec3 P = ray.currentPosition(rayInfo.t);
    Vec3 AP = P - capCentrePosA;
    Vec3 AB = capCentrePosB - capCentrePosA;
    //Step 1 check if we are outside endCapA
    if (VMath::dot(AB, AP) < 0) {
        // We are outside endCapA
        //Step 2 Is the ray direction towards endCapA
        if (VMath::dot(AB, ray.dir) > 0) {
            // The ray is pointing towards end cap A
            //Step 3 Plane Intersection
            Vec3 planeA_normal = VMath::normalize(-AB);
            float planeA_D = VMath::dot(planeA_normal, capCentrePosA);

        }
        else {
            return RayIntersectionInfo();
        }
    }
    return rayInfo;
}

RayIntersectionInfo GEOMETRY::Cylinder::checkInfiniteCylinder(const Ray& ray) const
{
    Vec3 D = ray.dir;
    Vec3 ABnormalized = VMath::normalize(-capCentrePosA + capCentrePosB);
    Vec3 AS = -capCentrePosA + ray.dir;

    float a = VMath::dot(D, D) - (VMath::dot(D, ABnormalized) * VMath::dot(D, ABnormalized));
    float b = 2 * (VMath::dot(AS, D) - VMath::dot(AS, ABnormalized) * VMath::dot(D, ABnormalized));
    float c = VMath::dot(AS, AS) - (VMath::dot(AS, ABnormalized) * VMath::dot(AS, ABnormalized)) - (radius * radius);


    RayIntersectionInfo rayInfo;

    QuadraticSolver answer = solveQuadratic(a, b, c);
    if (answer.numSolutions == NumSolutions::ZERO) {
        return RayIntersectionInfo();
    }
    else if (answer.numSolutions == NumSolutions::ONE || answer.numSolutions == NumSolutions::TWO) {
        rayInfo.isIntersected = true;
        rayInfo.t = answer.firstSolution;
        rayInfo.intersectoinPoint = ray.currentPosition(rayInfo.t);
        return rayInfo;
    }
    return rayInfo;
}

RayIntersectionInfo GEOMETRY::Cylinder::checkCaps(const Ray& ray) const
{
    return RayIntersectionInfo();
}
