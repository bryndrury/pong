#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include <vector>

class vec3 {
public:
    // Constructors
    vec3() : _x(0), _y(0), _z(0) {};
    vec3(double x, double y, double z) : _x(x), _y(y), _z(z) {};
    vec3(const vec3& u) : _x(u._x), _y(u._y), _z(u._z) {};
    vec3(double i) : _x(i), _y(i), _z(i) {};

    // Destructor
    ~vec3() {};

    // Getters
    double x() const { return _x; };
    double y() const { return _y; };
    double z() const { return _z; };
    std::vector<double> getValues() const { return { _x, _y, _z }; }

    // Setters
    void setX(double x) { _x = x; };
    void setY(double y) { _y = y; };
    void setZ(double z) { _z = z; };
    void setValues(double x, double y, double z) { _x = x; _y = y; _z = z; };

    // Operators - Setters
    vec3& operator=(const double i);
    vec3& operator=(const vec3 &u);
    vec3& operator=(const std::vector<double> &u);
    vec3& operator=(const int *i);

    // Return - Operators - Arithmetic
    vec3 operator+(const vec3 &u) const;
    vec3 operator-(const vec3 &u) const;
    vec3 operator*(const vec3 &u) const;
    vec3 operator*(const double i) const;
    vec3 operator/(const double i) const;

    // Assign - Operators - Arithmetic
    vec3& operator+=(const vec3 &u);
    vec3& operator-=(const vec3 &u);
    vec3& operator*=(const vec3 &u);
    vec3& operator*=(const double i);
    vec3& operator/=(const double i);

    // Methods
    double len() const;
    double lenSqrt() const;
    vec3 norm() const;
    vec3 fnorm() const;
    vec3 cross(const vec3 &u) const;
    double dot(const vec3 &u) const;
    double ang(const vec3 &u) const;
    double dist(const vec3 &u) const;

    void out() const;
    int size() const;

private:
    double _x, _y, _z;
};


// Fast inverse square root algorithm from Quake III Arena
inline float q_rsqrt(float number)
{
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y  = number;
  i  = * ( long * ) &y;                       // evil floating point bit level hacking
  i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
  y  = * ( float * ) &i;
  y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
  // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

  return y;
}

// Setter - Operators
vec3& vec3::operator=(const double i)
{
    _x = i;
    _y = i;
    _z = i;
    return *this;
}
vec3& vec3::operator=(const vec3 &u)
{
    _x = u._x;
    _y = u._y;
    _z = u._z;
    return *this;
}
vec3& vec3::operator=(const std::vector<double> &u)
{
    _x = u[0];
    _y = u[1];
    _z = u[2];
    return *this;
}
vec3& vec3::operator=(const int *i)
{
    _x = i[0];
    _y = i[1];
    _z = i[2];
    return *this;
}

// Return - Operators - Arithmetic
vec3 vec3::operator+(const vec3 &u) const
{
    vec3 result;
    result._x = _x + u._x;
    result._y = _y + u._y;
    result._z = _z + u._z;
    return result;
}
vec3 vec3::operator-(const vec3 &u) const
{
    vec3 result;
    result._x = _x - u._x;
    result._y = _y - u._y;
    result._z = _z - u._z;
    return result;
}
vec3 vec3::operator*(const vec3 &u) const
{
    vec3 result;
    result._x = _x * u._x;
    result._y = _y * u._y;
    result._z = _z * u._z;
    return result;
}
vec3 vec3::operator*(const double i) const
{
    vec3 result;
    result._x = _x * i;
    result._y = _y * i;
    result._z = _z * i;
    return result;
}
vec3 vec3::operator/(const double i) const
{
    vec3 result;
    result._x = _x / i;
    result._y = _y / i;
    result._z = _z / i;
    return result;
}

// Assign - Operators - Arithmetic
vec3& vec3::operator+=(const vec3 &u)
{
    _x += u._x;
    _y += u._y;
    _z += u._z;
    return *this;
}
vec3& vec3::operator-=(const vec3 &u)
{
    _x -= u._x;
    _y -= u._y;
    _z -= u._z;
    return *this;
}
vec3& vec3::operator*=(const vec3 &u)
{
    _x *= u._x;
    _y *= u._y;
    _z *= u._z;
    return *this;
}
vec3& vec3::operator*=(const double i)
{
    _x *= i;
    _y *= i;
    _z *= i;
    return *this;
}
vec3& vec3::operator/=(const double i)
{
    _x /= i;
    _y /= i;
    _z /= i;
    return *this;
}

// Methods
double vec3::len() const
{
    return sqrt(lenSqrt());
}
double vec3::lenSqrt() const
{
    return dot(*this);
}
vec3 vec3::norm() const
{
    return *this / len();
}
vec3 vec3::fnorm() const
{
    // Uses the fast inverse square root algorithm from Quake III Arena
    return *this * q_rsqrt(lenSqrt());
}
vec3 vec3::cross(const vec3 &u) const
{
    vec3 result;
    result._x = _y * u._z - _z * u._y;
    result._y = _z * u._x - _x * u._z;
    result._z = _x * u._y - _y * u._x;
    return result;
}
double vec3::dot(const vec3 &u) const
{
    return (_x * u._x + _y * u._y + _z * u._z);
}
double vec3::ang(const vec3 &u) const
{
    return acos(dot(u) / (len() * u.len()));
}
double vec3::dist(const vec3 &u) const
{
    vec3 r = *this - u;
    return r.len();
}
void vec3::out() const
{
    std::cout << "(" << _x << ", " << _y << ", " << _z << ")" << std::endl;
}
int vec3::size() const
{
    return 3;
}

#endif // VEC3_H