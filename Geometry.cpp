#include "geometry.h"

template<typename T>
Vector2<T>::Vector2(T ix, T iy) : x(ix), y(iy) {}
template<typename T>
Vector2<T>::Vector2(T ia) : x(ia), y(ia) {}
template<typename T>
Vector2<T>::Vector2() : x(0), y(0) {}

template<typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T> &right) const {
	Vector2<T> temp(*this);
	temp += right;
	return temp;
}
template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T> &right) const {
	Vector2<T> temp(*this);
	temp -= right;
	return temp;
}
template<typename T>
Vector2<T> Vector2<T>::operator*(const Vector2<T> &right) const {
	Vector2<T> temp(*this);
	temp *= right;
	return temp;
}
template<typename T>
Vector2<T> Vector2<T>::operator/(const Vector2<T> &right) const {
	Vector2<T> temp(*this);
	temp /= right;
	return temp;
}
template<typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2<T> &right) {
	x += right.x;
	y += right.y;
	return *this;
}
template<typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2<T> &right) {
	x -= right.x;
	y -= right.y;
	return *this;
}
template<typename T>
Vector2<T>& Vector2<T>::operator*=(const Vector2<T> &right) {
	x *= right.x;
	y *= right.y;
	return *this;
}
template<typename T>
Vector2<T>& Vector2<T>::operator/=(const Vector2<T> &right) {
	if (right.x != 0 && right.y != 0) {
		x /= right.x;
		y /= right.y;
	}
	return *this;
}
template<typename T>
void Vector2<T>::normalize() {
	if (x != 0 || y != 0) {
		T lenght = std::sqrt(x * x + y * y);
		x /= lenght;
		y /= lenght;
	}
}
template<typename T>
void Vector2<T>::rotate(T rotation) {
	T x_1 = x;
	x = (x *   std::cos(rotation)) - (y * std::sin(rotation));
	y = (x_1 * std::sin(rotation)) + (y * std::cos(rotation));
}
template<typename T>
Vector2<T> Vector2<T>::rotated(T rotation) const {
	return Vector2(
		(x * std::cos(rotation)) - (y * std::sin(rotation)),
		(x * std::sin(rotation)) + (y * std::cos(rotation))
	);
}
template<typename T>
Vector2<T> Vector2<T>::normalized() const {
	if (x != 0 || y != 0) {
		T lenght = std::sqrt(x * x + y * y);
		return Vector2<T>(
			x / lenght,
			y / lenght);
	}
	else {
		return Vector2<T>(0, 0);
	}
}
template<typename T>
T Vector2<T>::absolute() const {
	return std::sqrt(x * x + y * y);
}
template<typename T>
T Vector2<T>::dotProduct(const Vector2<T> &left, const Vector2<T> &right) {
	return left.x * right.x + left.y * right.y;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vector2) {
	os << '{' << vector2.x << ',' << vector2.y << '}';
	return os;
}

//compile the following classes and function using the templates

//Compile for double
template class Vector2<double>;
template std::ostream& operator<<(std::ostream& os, const Vector2<double>& vector2);
//Compile for float
template class Vector2<float>;
template std::ostream& operator<<(std::ostream& os, const Vector2<float>& vector2);