#pragma once

/** Instead of messing with multidemensional arrays to store lists of coordinates, 
	ill just use this to make single dimensional arrays and easily store 2d points.  **/
class Point
{
	public:
		Point(double, double);
		double x, y,z;
};

