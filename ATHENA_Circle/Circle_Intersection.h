/* 
 * File:   main.cpp
 * Author: SAMARAS
 *
 * 13/10/13
 */
 
#include <cstdlib>
#include <iostream>
#include <cmath>
 
#ifndef max
  #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
  #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
 
/*
 * Find the intersection point(s) of two Circle_Intersections,
 * when their centers and radiuses are given (2D).
 */
 
class Point2d{
public:
    Point2d() {}
    Point2d(double x, double y)
        : X(x), Y(y) {}
     
    double x() const { return X; }
    double y() const { return Y; }
     
    /**
     * Returns the norm of this vector.
     * @return the norm
    */
    double norm() const {
        return sqrt( X * X + Y * Y );
    }
     
    void setCoords(double x, double y) {
        X = x; Y = y;
    }
     
    // Print point
    friend std::ostream& operator << ( std::ostream& s, const Point2d& p )  {
      s << p.x() << " " << p.y();
      return s;
    }
private:
    double X;
    double Y;
};
 
class Circle_Intersection{
public:
    /**
     * @param R - radius
     * @param C - center
     */
    Circle_Intersection(double R, Point2d& C) 
        : r(R), c(C) {}
         
    /**
     * @param R - radius
     * @param X - center's x coordinate
     * @param Y - center's y coordinate
     */
    Circle_Intersection(double R, double X, double Y) 
        : r(R), c(X, Y) {}    
     
    Point2d getC() const { return c; }
    double getR() const { return r; }
     
    size_t intersect(const Circle_Intersection& C2, Point2d& i1, Point2d& i2) {
        // distance between the centers
        double d = Point2d(c.x() - C2.c.x(), 
                c.y() - C2.c.y()).norm();
         
        // find number of solutions
        if(d > r + C2.r) // Circle_Intersections are too far apart, no solution(s)
        {
            std::cout << "Circle_Intersections are too far apart\n";
            return 0;
        }
        else if(d == 0 && r == C2.r) // Circle_Intersections coincide
        {
            std::cout << "Circle_Intersections coincide\n";
            return 0;
        }
        // one Circle_Intersection contains the other
        else if(d + min(r, C2.r) < max(r, C2.r))
        {
            std::cout << "One Circle_Intersection contains the other\n";
            return 0;
        }
        else
        {
            double a = (r*r - C2.r*C2.r + d*d)/ (2.0*d);
            double h = sqrt(r*r - a*a);
             
            // find p2
            Point2d p2( c.x() + (a * (C2.c.x() - c.x())) / d,
                    c.y() + (a * (C2.c.y() - c.y())) / d);
             
            // find intersection points p3
            i1.setCoords( p2.x() + (h * (C2.c.y() - c.y())/ d),
                    p2.y() - (h * (C2.c.x() - c.x())/ d)
            );
            i2.setCoords( p2.x() - (h * (C2.c.y() - c.y())/ d),
                    p2.y() + (h * (C2.c.x() - c.x())/ d)
            );
             
            if(d == r + C2.r)
                return 1;
            return 2;
        }
    }
     
    // Print Circle_Intersection
    friend std::ostream& operator << ( std::ostream& s, const Circle_Intersection& C )  {
      s << "Center: " << C.getC() << ", r = " << C.getR();
      return s;
    }
private:
    // radius
    double r;
    // center
    Point2d c;
     
};
int main(void)
{
    // radius and center of Circle_Intersections
    Circle_Intersection c1(2, 0, 0);
    Circle_Intersection c2(1, 0, 2);
    Point2d i1, i2;
     
    std::cout << c1 << "\n" << c2 << "\n";
    // intersections point(s)
    size_t i_points = c1.intersect(c2, i1, i2);
     
    std::cout << "Intersection point(s)\n";
    if(i_points == 2)
    {
        std::cout << i1 << "\n";
        std::cout << i2 << "\n";
    }
    else if(i_points)
        std::cout << i1 << "\n";
     
    return 0;
}
