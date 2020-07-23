#pragma once

#include <cmath>
#include <algorithm>
#define CHECK_VALID( _v ) 0
#define FastSqrt(x)	(sqrt)(x)

#ifdef NDEBUG
#define assert( _exp ) ((void)0)
#else
#define assert(x)
#endif

#include <iostream>
inline unsigned long& FloatBits(float& f)
{
	return *reinterpret_cast<unsigned long*>(&f);
}
inline bool IsFinite(float f)
{
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
}
typedef float vec_t;

struct Vector3D {
	float x, y, z;

	Vector3D( ) = default;

	Vector3D( const float _x, const float _y, const float _z ) {
		x = _x, y = _y, z = _z;
	}

	inline void init(float ix, float iy, float iz)
	{
		x = ix; y = iy; z = iz;
		CHECK_VALID(*this);
	}

	Vector3D operator+( const Vector3D& point ) {
		return Vector3D( x + point.x, y + point.y, z + point.z );
	}

	Vector3D operator-( const Vector3D& point ) {
		return Vector3D( x - point.x, y - point.y, z - point.z );
	}

	Vector3D	operator-(const Vector3D& point) const {
		return Vector3D(x - point.x, y - point.y, z - point.z);
	}

	Vector3D operator*( const float point ) {
		return Vector3D( x * point, y * point, z * point );
	}
	inline bool is_valid() const
	{
		return IsFinite(x) && IsFinite(y) && !(x == 0 && y == 0 && z == 0);
	}
	float distance( const Vector3D& v ) const {
		return ( ( *this - v ).Length( ) );
	}
	float Dot(const Vector3D& vOther) const
	{
		return (x * vOther.x + y * vOther.y + z * vOther.z);
	}
	inline float NormalizeInPlace() {
		float radius = FastSqrt(x * x + y * y + z * z);
		float iradius = 1.f / (radius + FLT_EPSILON);

		x *= iradius;
		y *= iradius;
		z *= iradius;

		return radius;
	}

	Vector3D& operator=( const float array[ 3 ] ) {
		x = array[ 0 ];
		y = array[ 1 ];
		z = array[ 2 ];

		return *this;
	}

	Vector3D& operator+=( const Vector3D& point ) {
		x += point.x;
		y += point.y;
		z += point.z;

		return *this;
	}

	Vector3D& operator-=( const Vector3D& point ) {
		x -= point.x;
		y -= point.y;
		z -= point.z;

		return *this;
	}
	inline bool operator!=(const Vector3D& src) const {
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.x != x) || (src.y != y) || (src.z != z);
	}
	inline bool operator==(const Vector3D& src) const {
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.x == x) || (src.y == y) || (src.z == z);
	}
	Vector3D& operator/=( const float division ) {
		x /= division;
		y /= division;
		z /= division;

		return *this;
	}
	float& operator[](int i) {
		return ((float*)this)[i];
	}
	float operator[](int i) const {
		return ((float*)this)[i];
	}
	float dot( const Vector3D& point ) {
		return ( x * point.x + y * point.y + z * point.z );
	}

	float Length( void ) {
		return std::sqrtf( x * x + y * y + z * z );
	}

	float Length(void) const {
		return std::sqrtf(x * x + y * y + z * z);
	}

	float length_sqr( void ) {
		return ( x * x + y * y + z * z );
	}

	float Length2D( void ) {
		return std::sqrt( x * x + y * y );
	}

	Vector3D average( Vector3D vec_two ) {
		float avg_x = fabsf( this->x + vec_two.x ) / 2.f;
		float avg_y = fabsf( this->y + vec_two.y ) / 2.f;
		float avg_z = fabsf( this->z + vec_two.z ) / 2.f;

		return Vector3D( avg_x, avg_y, avg_z );
	}

	void zero( void ) {
		x = y = z = 0.0f;
	}

	void normalize( void ) {

		x = std::isfinite( x ) ? std::remainderf( x, 360.0f ) : 0.0f;
		y = std::isfinite( y ) ? std::remainderf( y, 360.0f ) : 0.0f;
		z = 0.0f;
	}

	Vector3D r_normalize(  ) {
		
		this->x = std::isfinite( x ) ? std::remainderf( x, 360.0f ) : 0.0f;
		this->y = std::isfinite( y ) ? std::remainderf( y, 360.0f ) : 0.0f;
		this->z = 0.0f;

		return *this;
	}

	void clamp( void ) {
		normalize( );

		x = std::clamp( x, -89.0f, 89.0f );
		y = std::clamp( y, -180.0f, 180.0f );
		z = 0.0f;
	}
	inline Vector3D vect_clamp()
	{
		CHECK_VALID(*this);

		if (x < -89.0f)
			x = -89.0f;

		if (x > 89.0f)
			x = 89.0f;

		while (y < -180.0f)
			y += 360.0f;

		while (y > 180.0f)
			y -= 360.0f;

		z = 0.0f;

		return *this;
	}

	inline Vector3D cross_product(const Vector3D& a, const Vector3D& b)
	{
		return Vector3D(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}
	//===============================================
	inline Vector3D cross(const Vector3D& vOther)
	{
		return cross_product(*this, vOther);
	}
};
#define ALIGN16 __declspec(align(16))
class ALIGN16 Vector3Daligned : public Vector3D
{
public:
	inline Vector3Daligned(void) {};
	inline Vector3Daligned(float X, float Y, float Z)
	{
		init(X, Y, Z);
	}

#ifdef VECTOR_NO_SLOW_OPERATIONS

private:
	// No copy constructors allowed if we're in optimal mode
	Vector3Daligned(const VectorAligned& vOther);
	Vector3Daligned(const Vector& vOther);

#else
public:
	explicit Vector3Daligned(const Vector3D& vOther)
	{
		init(vOther.x, vOther.y, vOther.z);
	}

	Vector3Daligned& operator=(const Vector3D& vOther)
	{
		init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

#endif
	float w = 0.f;	// this space is used anyway
};

struct vector2d_t {
	float x, y;

	vector2d_t() = default;

	vector2d_t(const float _x, const float _y) {
		x = _x, y = _y;
	}

	vector2d_t operator+(const vector2d_t& point) {
		return vector2d_t(x + point.x, y + point.y);
	}

	vector2d_t operator-(const vector2d_t& point) {
		return vector2d_t(x - point.x, y - point.y);
	}

	vector2d_t operator*(const float point) {
		return vector2d_t(x * point, y * point);
	}

	bool is_valid( ) const {
		return IsFinite( x ) && IsFinite( y ) && !( x == 0 && y == 0);
	}

	vector2d_t& operator=(const float array[2]) {
		x = array[0];
		y = array[1];

		return *this;
	}

	vector2d_t& operator+=(const vector2d_t& point) {
		x += point.x;
		y += point.y;

		return *this;
	}

	vector2d_t& operator-=(const vector2d_t& point) {
		x -= point.x;
		y -= point.y;

		return *this;
	}
	inline bool operator!=(const vector2d_t& src) const {
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.x != x) || (src.y != y);
	}
	vector2d_t& operator/(const float division) {
		x /= division;
		y /= division;

		return *this;
	}

	float& operator[](int i) {
		return ((float*)this)[i];
	}
	float operator[](int i) const {
		return ((float*)this)[i];
	}
	float dot(const vector2d_t& point) {
		return (x * point.x + y * point.y);
	}

	float Length(void) {
		return std::sqrtf(x * x + y * y);
	}

	float Length(void) const {
		return std::sqrtf(x * x + y * y);
	}

	float length_sqr(void) {
		return (x * x + y * y);
	}

	float Length2D(void) {
		return std::sqrt(x * x + y * y);
	}

	void zero(void) {
		x = y = 0.0f;
	}

	void normalize(void) {
		x = std::isfinite(x) ? std::remainderf(x, 360.0f) : 0.0f;
		y = std::isfinite(y) ? std::remainderf(y, 360.0f) : 0.0f;
	}

	void clamp(void) {
		normalize();

		x = std::clamp(x, -89.0f, 89.0f);
		y = std::clamp(y, -180.0f, 180.0f);
	}
};