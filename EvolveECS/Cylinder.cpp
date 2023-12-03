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


    Vec3 axis = VMath::normalize(capCentrePosB - capCentrePosA);
    Vec3 randomVector(12, 7, 314);
    Vec3 radialVector = VMath::normalize(VMath::cross(axis, randomVector));
    radialVector *= radius ;

    for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta)
    {
        // Build a ring
        Vec3 circle = MMath::rotate(thetaDeg, axis) * radialVector;
        for (int i = 10; i > -1; )
        {
            //a top lid of the capsule
            scaleMatrix = MMath::scale(Vec3(i / deltaRing, i / deltaRing, 0.0f));
            Vec3 scaledCircle = scaleMatrix * circle;
            scaledCircle += capCentrePosB;
            vertices.push_back(scaledCircle);
            normals.push_back(scaledCircle);
            i--;
        }
        //draw subsequent capsule rings
        for (float phiDeg = capCentrePosB.z; phiDeg <= capCentrePosA.z; phiDeg += deltaPhi)
        {
            circle.z = phiDeg;
            vertices.push_back(circle);
            normals.push_back(circle);
        }
        //draw a bottom lid of the capsule
        for (int i = 10; i > -1; )
        {
            scaleMatrix = MMath::scale(Vec3(i / deltaRing, i / deltaRing, 0.0f));
            Vec3 scaledCircle = scaleMatrix * circle;
            scaledCircle += capCentrePosA;
            vertices.push_back(scaledCircle);
            normals.push_back(scaledCircle);
            i--;
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
    Vec3 P = ray.currentPosition(rayInfo.t);
    Vec3 AP = P - capCentrePosA;
    Vec3 AB = capCentrePosB - capCentrePosA;
  
    //Step 1 check if we are outside endCapA
    if (VMath::dot(AB, AP) < 0) {
    //    // We are outside endCap A
    //    //Step 2 Is the ray direction towards endCapA
        if (VMath::dot(AB, ray.dir) > 0) {
            //Step 3 Plane Intersection
            Vec3 planeA_normal = VMath::normalize(-AB);
            float planeA_D = VMath::dot(planeA_normal, capCentrePosA);
            float t = (planeA_D - VMath::dot(planeA_normal, ray.start)) / VMath::dot(planeA_normal, ray.dir);
            Vec3 Q = ray.currentPosition(t);
            // step 4 is q inside the circle
            if (VMath::distance(capCentrePosA, Q) <= radius) {
                return rayInfo;
            }
            else {
                return RayIntersectionInfo();
            }
        }
        else {
            return RayIntersectionInfo();
        }
    }
    else if (VMath::dot(VMath::normalize(AB), AP) > VMath::mag(AB)) {
        if (VMath::dot(-AB, ray.dir) > 0) {
            //Step 3 Plane Intersection
            Vec3 planeB_normal = VMath::normalize(AB);
            float planeB_D = VMath::dot(planeB_normal, capCentrePosB);
            float t = (planeB_D - VMath::dot(planeB_normal, ray.start)) / VMath::dot(planeB_normal, ray.dir);
            Vec3 Q = ray.currentPosition(t);
            // step 4 is q inside the circle
            if (VMath::distance(capCentrePosB, Q) <= radius) {
                return rayInfo;
            }
            else {
                return RayIntersectionInfo();
            }
        }
        else {
            return RayIntersectionInfo();
        }
    }
    else {
        return rayInfo;
    }
}

RayIntersectionInfo GEOMETRY::Cylinder::checkInfiniteCylinder(const Ray& ray) const
{
    Vec3 D = ray.dir;
    Vec3 ABnormalized = VMath::normalize(-capCentrePosA + capCentrePosB);
    Vec3 AS = -capCentrePosA + ray.start;

    float a = VMath::dot(D, D) - (VMath::dot(D, ABnormalized) * VMath::dot(D, ABnormalized));
    float b = 2 * (VMath::dot(AS, D) - VMath::dot(AS, ABnormalized) * VMath::dot(D, ABnormalized));
    float c = VMath::dot(AS, AS) - (VMath::dot(AS, ABnormalized) * VMath::dot(AS, ABnormalized)) - (radius * radius);

     
    RayIntersectionInfo rayInfo;

    QuadraticSolver answer = solveQuadratic(a, b, c);
    if (answer.numSolutions == NumSolutions::ZERO) {
        rayInfo = RayIntersectionInfo();
    }
    else if (answer.numSolutions == NumSolutions::ONE || answer.numSolutions == NumSolutions::TWO) {
        rayInfo.isIntersected = true;
        rayInfo.t = answer.firstSolution;
        rayInfo.intersectoinPoint = ray.currentPosition(rayInfo.t);
    } 
    return rayInfo;
}

RayIntersectionInfo GEOMETRY::Cylinder::checkCaps(const Ray& ray) const
{
    return RayIntersectionInfo();
}
