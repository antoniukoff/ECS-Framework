#pragma once
#include <iostream>
namespace GEOMETRY
{
	enum class NumSolutions {
		ZERO = 0,
		ONE,
		TWO
	};

	struct QuadraticSolver
	{
		QuadraticSolver() = default;
		NumSolutions numSolutions{ NumSolutions::ZERO };
		float firstSolution = 0.0f;
		float secondSolution = 0.0f;
		void print() const {
			std::cout << "Number of solutions: " << static_cast<int>(numSolutions) << "\n";
			std::cout << "First solution: " << firstSolution << "\n";
			std::cout << "Second solution: " << secondSolution << "\n";
		}
	};
	QuadraticSolver solveQuadratic(float a, float b, float c);
}

