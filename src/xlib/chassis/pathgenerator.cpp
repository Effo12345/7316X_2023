#include "xlib/chassis/pathgenerator.hpp"

namespace xlib {

	/**
	 * Treat each set of points in the waypoints input as its own vector and
	 * inject points with an even spacing along that vector. Increases the
	 * resolution of the finished path
	 *
	 * @param waypoints Input points to form the path
	 * @param spacing Distance between points to be injected (in inches)
	 */
    void PathGenerator::injectPoints(QPath& waypoints, const float spacing) {
		QPath path;

		for(int i = 0; i < waypoints.points.size() - 1; i++) {
			QPoint vector;

			vector = waypoints.points[i + 1] - waypoints.points[i];

			float magnitude = vector.getMagnitude();

			int num_QPoints_that_fit = ceil((magnitude / spacing));

			vector = (vector / magnitude) * spacing;

			QPath tmp;
			tmp.points.resize(num_QPoints_that_fit);

			for(int j = 0; j < num_QPoints_that_fit; j++) {
				tmp.points[j] = waypoints.points[i] + (vector * j);
			}
				
			path.points.insert(std::end(path.points), std::begin(tmp.points), std::end(tmp.points));
		}
		path.points.insert(std::end(path.points), waypoints.points[waypoints.points.size() - 1]);

		waypoints = path;
	}

	/**
	 * Uses the gradient descent algorithm to smooth the waypoints into a curved
	 * path. Finds the curved position of the current point based on the
	 * positions of the previous and the next point as the algorithm iterates
	 * through all points on the path
	 *
	 * @param path Input points to be processed
	 * @param weight_smooth Determines the cuviness of the path
	 * @param Tolerance The maximum permissible slope between two points
	 */
    void PathGenerator::optimizePath(QPath& path, const float weight_smooth, const float tolerance) {
		const float weight_data = 1 - weight_smooth;
		QPath curvedPath = path;

		float change = tolerance;
		while(change >= tolerance) {
			change = 0.0f;
			for(int i = 1; i < path.points.size() - 1; i++) {
				QPoint aux = curvedPath.points[i];
				curvedPath.points[i] += (path.points[i] - curvedPath.points[i]) * weight_data  + (curvedPath.points[i-1] + curvedPath.points[i+1] - ( curvedPath.points[i] * 2.0f)) * weight_smooth;
				change += std::abs(aux.x - curvedPath.points[i].x);
				change += std::abs(aux.y - curvedPath.points[i].y);
			}
		}

		path = curvedPath;
	}

	/**
	 * Calculate the curvature of the path at each point. Curvature is defined
	 * as the reciprocal of the radius of a circle that passes through the 
	 * current point, the next point, and the previous point. All math for this
	 * function is derived from the equation of a circle. This likewise 
	 * iterates through each point along the path
	 *
	 * @param path Points to be processed
	 */
    void PathGenerator::calculateCurvature(QPath& path) {
		//Reserve the memory for the vector in advance for increased performance
		path.curvature.reserve(path.points.size());

		 for(int i = 1; i < path.points.size() - 1; i++) {
			path.points[i].x += 0.001;

			float k1 = 0.5 * (pow(path.points[i].x, 2) + pow(path.points[i].y, 2) - pow(path.points[i - 1].x, 2) - pow(path.points[i - 1].y, 2)) / (path.points[i].x - path.points[i - 1].x);
			float k2 = (path.points[i].y - path.points[i - 1].y) / (path.points[i].x - path.points[i - 1].x);
			float b = 0.5 * (pow(path.points[i - 1].x, 2) - 2 * path.points[i - 1].x  * k1 + pow(path.points[i - 1].y, 2) - pow(path.points[i + 1].x, 2) + 2 * path.points[i + 1].x * k1 - pow(path.points[i + 1].y, 2)) / (path.points[i + 1].x * k2 - path.points[i + 1].y + path.points[i - 1].y - path.points[i - 1].x * k2);
			float a = k1 - k2 * b;
			float curvature = 1 / path.points[i].distanceTo({a, b});

			if(std::isnan(curvature))
				curvature = 0;

			path.curvature[i] = curvature;
		}
		path.curvature[0] = 0.0f;
		path.curvature[path.points.size() - 1] = 0.0f;
	}

	/**
	 * Computes the target velocity for each point based on the curvature of the
	 * path at that point and the maximum acceleration. The robot drives slower 
	 * through curves to increase accuracy. Velocity capped by maxVelocity
	 *
	 * @param path Input points to be processed
	 * @param maxVelocity Maximum speed the robot can follow the path with
	 * @param k How fast the robot moves around curves
	 * @param maxAcceleration Limits the robot's maximum acceleration (rpm/s/s)
	 */
    void PathGenerator::targetVelocity(QPath& path, const float maxVelocity, const float k, const float maxAcceleration) {
		//Reserve the memory for the vector in advance for increased performance
		path.velocity.reserve(path.points.size());
		path.velocity[path.points.size()-1] = 0.0f;

		for(int i = (path.points.size() - 2); i >= 0; i--) {
			float velocityLimit = std::min(maxVelocity, (k / path.curvature[i]));

			float distance = path.points[i].distanceTo(path.points[i+1]);
			float accelerationLimit = sqrt(pow(path.velocity[i+1], 2) + (2 * maxAcceleration * distance));

			path.velocity[i] = std::min(velocityLimit, accelerationLimit);
		}

	}

	/**
	 * Runs the above helper functions to process the path passed as a parameter
	 * Accesses the settings struct to provide the necessary constants. 
	 * Modifies the path parameter by reference.
	 *
	 * @param path Reference to path object to be processed
	 * @param settings Holds tuning constants
	 */
    void PathGenerator::processWaypoints(QPath& path, QPath::Settings& settings) {
		injectPoints(path, settings.pointSpacing);
		optimizePath(path, settings.weight_smooth, settings.tolerance);
		calculateCurvature(path);
		targetVelocity(path, settings.maxVelocity, settings.k, settings.maxAcceleration);
	}

}