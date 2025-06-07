/**
 * @file Vec3.cpp
 * @brief Implementación de la clase Vec3 para el sistema de ray tracing
 * 
 * Esta clase representa un vector tridimensional y proporciona métodos para
 * operaciones vectoriales básicas y avanzadas.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#include "Vec3.h"

/**
 * @brief Constructor por defecto que inicializa el vector a (0,0,0)
 */
Vec3::Vec3() : x(0), y(0), z(0) {}

/**
 * @brief Constructor que inicializa el vector con coordenadas específicas
 * @param x Coordenada x
 * @param y Coordenada y
 * @param z Coordenada z
 */
Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z){}

/**
 * @brief Obtiene la coordenada x del vector
 * @return Valor de la coordenada x
 */
double Vec3::getX() const
{
	return this->x;
}

/**
 * @brief Obtiene la coordenada y del vector
 * @return Valor de la coordenada y
 */
double Vec3::getY() const
{
	return this->y;
}

/**
 * @brief Obtiene la coordenada z del vector
 * @return Valor de la coordenada z
 */
double Vec3::getZ() const
{
	return this->z;
}

/**
 * @brief Establece la coordenada x del vector
 * @param x Nuevo valor de la coordenada x
 */
void Vec3::setX(double x)
{
	this->x = x;
}

/**
 * @brief Establece la coordenada y del vector
 * @param y Nuevo valor de la coordenada y
 */
void Vec3::setY(double y)
{
	this->y = y;
}

/**
 * @brief Establece la coordenada z del vector
 * @param z Nuevo valor de la coordenada z
 */
void Vec3::setZ(double z)
{
	this->z = z;
}

/**
 * @brief Operador de negación
 * @return Vector con signos invertidos
 */
Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

/**
 * @brief Operador de suma y asignación
 * @param other Vector a sumar
 * @return Referencia al vector resultante
 */
Vec3& Vec3::operator+=(const Vec3& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

/**
 * @brief Operador de multiplicación por escalar y asignación
 * @param t Escalar
 * @return Referencia al vector resultante
 */
Vec3& Vec3::operator*=(double t)
{
	this->x *= t;
	this->y *= t;
	this->z *= t;
	return *this;
}

/**
 * @brief Operador de división por escalar y asignación
 * @param t Escalar
 * @return Referencia al vector resultante
 */
Vec3& Vec3::operator/=(double t)
{
	this->x /= t;
	this->y /= t;
	this->z /= t;
	return *this;
}

/**
 * @brief Calcula la longitud del vector
 * @return Longitud del vector
 */
double Vec3::length() const
{
	return std::sqrt(lengthSquared());
}

/**
 * @brief Calcula el cuadrado de la longitud del vector
 * @return Cuadrado de la longitud del vector
 */
double Vec3::lengthSquared() const
{
	return x * x + y * y + z * z;
}

/**
 * @brief Operador de salida para imprimir un vector
 * @param os Stream de salida
 * @param vec Vector a imprimir
 * @return Stream de salida
 */
std::ostream& operator<<(std::ostream& os, const Vec3& vec)
{
	os << "Vec3(" << vec.getX() << ", " << vec.getY() << ", " << vec.getZ() << ")";
	return os;
}

/**
 * @brief Operador de suma de vectores
 * @param a Primer vector
 * @param b Segundo vector
 * @return Vector resultante
 */
Vec3 operator+(const Vec3& a, const Vec3& b)
{
	return Vec3(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
}

/**
 * @brief Operador de resta de vectores
 * @param a Primer vector
 * @param b Segundo vector
 * @return Vector resultante
 */
Vec3 operator-(const Vec3& a, const Vec3& b)
{
	return Vec3(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
}

/**
 * @brief Operador de multiplicación componente a componente
 * @param a Primer vector
 * @param b Segundo vector
 * @return Vector resultante
 */
Vec3 operator*(const Vec3& a, const Vec3& b)
{
	return Vec3(a.getX() * b.getX(), a.getY() * b.getY(), a.getZ() * b.getZ());
}

/**
 * @brief Operador de multiplicación de vector por escalar
 * @param a Vector
 * @param t Escalar
 * @return Vector resultante
 */
Vec3 operator*(const Vec3& a, double t)
{
	return Vec3(a.getX() * t, a.getY() * t, a.getZ() * t);
}

/**
 * @brief Operador de multiplicación de escalar por vector
 * @param t Escalar
 * @param a Vector
 * @return Vector resultante
 */
Vec3 operator*(double t, const Vec3& a)
{
	return Vec3(a.getX() * t, a.getY() * t, a.getZ() * t);
}

/**
 * @brief Operador de división de vector por escalar
 * @param a Vector
 * @param t Escalar
 * @return Vector resultante
 */
Vec3 operator/(const Vec3& a, double t)
{
	return Vec3(a.getX() / t, a.getY() / t, a.getZ() / t);
}

/**
 * @brief Calcula el producto punto entre dos vectores
 * @param a Primer vector
 * @param b Segundo vector
 * @return Producto punto
 */
double dotProduct(const Vec3& a, const Vec3& b)
{
	return a.getX() * b.getX() + a.getY() * b.getY() + a.getZ() * b.getZ();
}

/**
 * @brief Calcula el producto cruz entre dos vectores
 * @param a Primer vector
 * @param b Segundo vector
 * @return Producto cruz
 */
Vec3 crossProduct(const Vec3& a, const Vec3& b)
{
	return Vec3(
		a.getY() * b.getZ() - a.getZ() * b.getY(),
		a.getZ() * b.getX() - a.getX() * b.getZ(),
		a.getX() * b.getY() - a.getY() * b.getX()
	);
}

/**
 * @brief Calcula el vector unitario
 * @param v Vector de entrada
 * @return Vector unitario
 */
Vec3 unitVector(const Vec3& v)
{
	return v / v.length();
}