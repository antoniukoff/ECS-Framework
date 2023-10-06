#include "QuadraticSolver.h"

using namespace GEOMETRY;

QuadraticSolver solveQuadratic(float a, float b, float c)
{
    QuadraticSolver answer;
    float discriminant = b * b - 4.0f * a * c;

    // TODO for assignment 2; if discriminant is less than 0 than there is no solution
    // if it is 0 there is only one solution 
    // set both solutions to -b/2a
    // and if it is greater than 0 there are 2
    // set first solution to (-b-sqrt(siscriminant) / 2a)
    // set first solution to (-b+sqrt(siscriminant) / 2a)






    return QuadraticSolver();
}

QuadraticSolver GEOMETRY::solveQuadratic(float a, float b, float c)
{
    return QuadraticSolver();
}
