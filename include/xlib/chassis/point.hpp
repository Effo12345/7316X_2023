#pragma once
#include "api.h"

namespace xlib {

	struct QPoint {
		float x;
		float y;

		//Overload output operator
		friend std::ostream& operator<<(std::ostream& out, const QPoint& p);

		//Overload math operators to interact with other point objects and with
		//scalar values
		QPoint operator+(const QPoint& p) {
			return {x + p.x, y + p.y};
		} 

		QPoint operator+(const float f) {
			return {x + f, y + f};
		}

		QPoint operator-(const QPoint& p) {
			return {x - p.x, y - p.y};
		} 

		QPoint operator-(const float f) {
			return {x - f, y - f};
		}

		QPoint operator*(const float f) {
			return {x * f, y * f};
		}

		QPoint operator/(const float f) {
			return {x / f, y / f};
		}


		void operator+=(const QPoint& p) {
			x += p.x;
			y += p.y;
		}

		void operator-=(const float f) {
			x += f;
			y += f;
		}


		/**
		 * Treats the x and y values as the components of a vector and returns
		 * its magnitude using the pythagorean theorem
		 */
		float getMagnitude() {
			return sqrt(pow(x, 2) + pow(y, 2));
		}

		/**
		 * Returns the distance between the current point and another point 
		 * using the distance formula
		 */
		float distanceTo(QPoint p) {
			return sqrt(pow((x - p.x), 2) + pow((y - p.y), 2));
		}

		/**
		 * Returns the dot product of this point and the parameter p
		 */
		float dotProduct(QPoint p) {
			return pow(x * p.x, 2) + pow(y * p.y, 2);
		}
	};

}