#pragma once
#include "xlib/chassis/path.hpp"

namespace xlib {

	class PathGenerator {
		void injectPoints(QPath& waypoints, const float spacing);

		void optimizePath(QPath& path, const float weight_smooth, const float tolerance);

		void calculateCurvature(QPath& path);

		void targetVelocity(QPath& path, const float maxVelocity, const float k, const float maxAcceleration);

	public:
		void processWaypoints(QPath& path, QPath::Settings& settings);
	};

}