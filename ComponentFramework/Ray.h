#pragma once
#include<Vector.h>
#include<VMath.h>

namespace GEOMETRY
{
	struct RayIntersectionInfo
	{
		bool isIntersected = false;
		MATH::Vec3 intersectoinPoint;
		float t = 0.0f;
	};

	struct Ray {
		MATH::Vec3 start;
		MATH::Vec3 dir;
		Ray() {
			start = MATH::Vec3(0.0f, 0.0f, 0.0f);
			dir = MATH::VMath::normalize(MATH::Vec3(0.0f, 0.0f, 1.0f));
		}
		Ray(const MATH::Vec3& start_, const MATH::Vec3& dir_) {
			set(start_, dir_);
		}
		void set(const MATH::Vec3& start_, const MATH::Vec3& dir_) {
			start = start_;
			dir = dir_;
		}
		MATH::Vec3 currentPosition(float t) const {
			return start + (dir * t);
		}// returns start + t * dir
	};
}

