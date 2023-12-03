#include "QuadraticSolver.h"

using namespace GEOMETRY;

QuadraticSolver GEOMETRY::solveQuadratic(float a, float b, float c)
{
    QuadraticSolver answer;
    float discriminant = (b * b) - (4.0f * a * c);

    // TODO for assignment 2; if discriminant is less than 0 than there is no solution
    // if it is 0 there is only one solution 
    // set both solutions to -b/2a
    // and if it is greater than 0 there are 2
    // set first solution to (-b-sqrt(siscriminant) / 2a)
    // set first solution to (-b+sqrt(siscriminant) / 2a)
    if (discriminant < 0) {
        answer.numSolutions = NumSolutions::ZERO;
        return QuadraticSolver();
    }
    else if (discriminant == 0) {
        answer.numSolutions = NumSolutions::ONE;
        answer.firstSolution = -b / (2 * a);
        answer.secondSolution = -b / (2 * a);
        return answer;
    }
    else if (discriminant > 0) {
        answer.numSolutions = NumSolutions::TWO;
        answer.firstSolution = std::min((-b - sqrt(discriminant)) / (2 * a), ((-b + sqrt(discriminant)) / (2 * a)));
        answer.secondSolution = std::max((-b - sqrt(discriminant)) / (2 * a), ((-b + sqrt(discriminant)) / (2 * a)));

        return answer; 
    }
    return QuadraticSolver();
}
