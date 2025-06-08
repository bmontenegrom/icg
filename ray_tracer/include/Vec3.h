/**
 * @file Vec3.h
 * @brief Clase que representa un vector tridimensional en el sistema de ray tracing
 * 
 * Esta clase proporciona funcionalidades para operaciones vectoriales, como suma, resta,
 * multiplicación, y cálculo de longitud.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#pragma once
#include <iostream>
class Vec3 {
public:
	/**
	 * @brief Constructor por defecto
	 */
	Vec3();
	
	/**
	 * @brief Constructor que inicializa el vector con coordenadas x, y, z
	 * @param x Coordenada x
	 * @param y Coordenada y
	 * @param z Coordenada z
	 */
	Vec3(double x, double y, double z);
	
	/**
	 * @brief Destructor por defecto
	 */
	~Vec3() = default;
	
	/**
	 * @brief Obtiene la coordenada x del vector
	 * @return Coordenada x
	 */
	double getX() const;
	
	/**
	 * @brief Obtiene la coordenada y del vector
	 * @return Coordenada y
	 */
	double getY() const;
	
	/**
	 * @brief Obtiene la coordenada z del vector
	 * @return Coordenada z
	 */
	double getZ() const;
	
	/**
	 * @brief Establece la coordenada x del vector
	 * @param x Nueva coordenada x
	 */
	void setX(double x);
	
	/**
	 * @brief Establece la coordenada y del vector
	 * @param y Nueva coordenada y
	 */
	void setY(double y);
	
	/**
	 * @brief Establece la coordenada z del vector
	 * @param z Nueva coordenada z
	 */
	void setZ(double z);

	/**
	 * @brief Operador de negación
	 * @return Vector negado
	 */
	Vec3 operator-() const;
	
	/**
	 * @brief Operador de suma y asignación
	 * @param other Vector a sumar
	 * @return Referencia al vector resultante
	 */
	Vec3& operator+=(const Vec3& other);
	
	/**
	 * @brief Operador de multiplicación y asignación
	 * @param t Escalar a multiplicar
	 * @return Referencia al vector resultante
	 */
	Vec3& operator*=(double t);
	
	/**
	 * @brief Operador de división y asignación
	 * @param t Escalar a dividir
	 * @return Referencia al vector resultante
	 */
	Vec3& operator/=(double t);

	/**
	 * @brief Calcula la longitud del vector
	 * @return Longitud del vector
	 */
	double length() const;
	
	/**
	 * @brief Calcula la longitud al cuadrado del vector
	 * @return Longitud al cuadrado del vector
	 */
	double lengthSquared() const;

private:
	double x, y, z; ///< Coordenadas x, y, z
};


std::ostream& operator<<(std::ostream& os, const Vec3& vec); ///< Operador de salida

Vec3 operator+(const Vec3& a, const Vec3& b); ///< Suma
Vec3 operator-(const Vec3& a, const Vec3& b); ///< Resta
Vec3 operator*(const Vec3& a, const Vec3& b); ///< Multiplicación
Vec3 operator*(const Vec3& a, double t); ///< Multiplicación por escalar
Vec3 operator*(double t, const Vec3& a); ///< Multiplicación por escalar
Vec3 operator/(const Vec3& a, double t); ///< División por escalar

double dotProduct(const Vec3& a, const Vec3& b); ///< Producto punto
Vec3 crossProduct(const Vec3& a, const Vec3& b); ///< Producto cruz

Vec3 unitVector(const Vec3& v); ///< Vector unitario