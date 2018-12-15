#ifndef GEOM_GEOMETRY_H
#define GEOM_GEOMETRY_H

#include <iostream>
#include <vector>
#include <set>
#include <map>

namespace Geometry {
    template <class T>
    class Point {
    private:
        using value = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using size_type = size_t;

        value _x, _y;
        size_type _id;
    public:
        Point(const_reference x, const_reference y, size_type id) : _x(x), _y(y), _id(id) {}

        Point() {}

        value getX() const {
            return _x;
        }

        value getY() const {
            return _y;
        }

        size_type getId() const {
            return _id;
        }

        void setId(size_type id) {
            _id = id;
        }

        long double operator^(const Point<value> &p) {
            return _x * p._y - _y * p._x;
        }

        friend std::ostream& operator<<(std::ostream &os, const Point<value> &p) {
            os << p.getX() << ' ' << p.getY();
            return os;
        }

        friend std::istream& operator>>(std::istream &is, Point<value> &p) {
            is >> p._x >> p._y;
            return is;
        }

        bool operator<(const Point& other) const {
            if (_x != other._x) return _x < other._x;
            return _y < other._y;
        }
    };

    template <class T>
    class Segment {
    private:
        using value = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using size_type = size_t;

        size_type _id;
        value _left, _right;
    public:
        Segment(const_reference &a, const_reference &b, size_type id = -1) : _left(a), _right(b), _id(id) {}

        const_reference first() const {
            return _left;
        }

        const_reference second() const {
            return _right;
        }

        const_reference minX() const {
            return (_left.getX() < _right.getX() ? _left : _right);
        }

        const_reference maxX() const {
            return (_left.getX() < _right.getX() ? _right : _left);
        }

        const_reference minY() const {
            return (_left.getY() < _right.getY() ? _left : _right);
        }

        const_reference maxY() const {
            return (_left.getY() < _right.getY() ? _right : _left);
        }


        size_type getId() const {
            return _id;
        }

        double y(double x) const {
            if (minX().getX() == maxX().getX())
                return first().getY();

            return minX().getY() + (double)((maxX().getY() - minX().getY()) * (x - minX().getX()))
                                   / (maxX().getX() - minX().getX());
        }
    };

    enum STATE {
        OUTSIDE,
        INSIDE,
        BORDER,
    };
}


#endif //GEOM_GEOMETRY_H
