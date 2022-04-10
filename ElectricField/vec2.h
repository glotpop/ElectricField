#pragma once

#include <cmath>
#include <iostream>

struct vec2 {
private:
    double x_, y_;
public:
    vec2(double x, double y) : x_(x), y_(y) {}

    double x() const { return x_; }
    double y() const { return y_; }

    double length() const {
        return std::sqrt(length2());
    }

    double length2() const {
        return x_ * x_ + y_ * y_;
    }

    inline vec2 operator+ (const vec2& v) const {
        return vec2(x_ + v.x(), y_ + v.y());
    }

    inline vec2 operator- () const  {
        return vec2(-x_, -y_);
    }

    inline vec2 operator- (const vec2& v) const {
        return vec2(x_ - v.x(), y_ - v.y());
    }

    inline vec2 operator* (double m) const {
        return vec2(x_ * m, y_ * m);
    }

    inline vec2 operator/ (double m) const {
        return vec2(x_ / m, y_ / m);
    }

    const vec2& operator+= (const vec2& v) {
        x_ += v.x();
        y_ += v.y();
        return *this;
    }
    
    const vec2& operator-= (const vec2& v) {
        x_ -= v.x();
        y_ -= v.y();
        return *this;
    }

    const vec2& operator*= (double m) {
        x_ *= m;
        y_ *= m;
        return *this;
    }

    const vec2& operator/= (double m) {
        x_ /= m;
        y_ /= m;
        return *this;
    }

    const vec2 normalized() const {
        return *this / length();
    }
};

inline std::ostream& operator<< (std::ostream& out, const vec2& v) {
    return out << "vec2(x=" << v.x() << ",y=" << v.y() << ")";
}