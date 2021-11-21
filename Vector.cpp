#include "Vector.hpp"

using namespace SDK::utils::maths;

vector4::vector4( )
{
}

vector4::vector4( float x, float y, float z, float w )
	: x( x ), y( y ), z( z ), w( w )
{
}

vector4& vector4::operator+=( const vector4& vector )
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
	w += vector.w;

	return *this;
}

vector4& vector4::operator-=( const vector4& vector )
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	w -= vector.w;

	return *this;
}

vector4& vector4::operator*=( float number )
{
	x *= number;
	y *= number;
	z *= number;
	w *= number;

	return *this;
}

vector4& vector4::operator/=( float number )
{
	x /= number;
	y /= number;
	z /= number;
	w /= number;

	return *this;
}