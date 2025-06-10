/**
 * @file Color.cpp
 * @brief Implementación de la clase Color para el sistema de ray tracing
 * 
 * Esta clase representa un color en el espacio RGB y proporciona métodos para
 * manipular y operar con colores.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#include "Color.h"

/**
 * @brief Constructor por defecto
 */
Color::Color() : r(0), g(0), b(0) {}

/**
 * @brief Constructor que inicializa el color con valores RGB
 * @param r Componente rojo
 * @param g Componente verde
 * @param b Componente azul
 */
Color::Color(double r, double g, double b) : r(r), g(g), b(b) {}

/**
 * @brief Obtiene la componente roja del color
 * @return Componente roja
 */
double Color::getR() const {
	return r;
}

/**
 * @brief Obtiene la componente verde del color
 * @return Componente verde
 */
double Color::getG() const {
	return g;
}

/**
 * @brief Obtiene la componente azul del color
 * @return Componente azul
 */
double Color::getB() const {
	return b;
}

/**
 * @brief Establece la componente roja del color
 * @param r Nueva componente roja
 */
void Color::setR(double r) {
	this->r = r;
}

/**
 * @brief Establece la componente verde del color
 * @param g Nueva componente verde
 */
void Color::setG(double g) {
	this->g = g;
}

/**
 * @brief Establece la componente azul del color
 * @param b Nueva componente azul
 */
void Color::setB(double b) {
	this->b = b;
}

/**
 * @brief Obtiene la componente roja del color en formato byte
 * @return Componente roja en formato byte
 */
int Color::getRbyte() const {
	double temp = r; // Usar una copia para evitar modificar el original
	if (r > 1.0) {
		temp = 1.0; // Asegurar que no exceda el rango
	}
	else if (r < 0.0) {
		temp = 0.0; // Asegurar que no sea negativo
	}
	return static_cast<int>(255.999 * temp);
}

/**
 * @brief Obtiene la componente verde del color en formato byte
 * @return Componente verde en formato byte
 */
int Color::getGbyte() const {
	double temp = g; // Usar una copia para evitar modificar el original
	if (g > 1.0) {
		temp = 1.0; // Asegurar que no exceda el rango
	}
	else if (g < 0.0) {
		temp = 0.0; // Asegurar que no sea negativo
	}
	return static_cast<int>(255.999 * temp);
}

/**
 * @brief Obtiene la componente azul del color en formato byte
 * @return Componente azul en formato byte
 */
int Color::getBbyte() const {
	double temp = this->b; // Usar una copia para evitar modificar el original
	if (b > 1.0) {
		temp = 1.0; // Asegurar que no exceda el rango
	}
	else if (b < 0.0) {
		temp = 0.0; // Asegurar que no sea negativo
	}

	return static_cast<int>(255.999 * temp);
}

/**
 * @brief Operador de suma y asignación
 * @param other Color a sumar
 * @return Referencia al color resultante
 */
Color& Color::operator+=(const Color& other) {
	r += other.r;
	g += other.g;
	b += other.b;
	return *this;
}

Color& Color::operator*=(double t)
{
	return *this = Color(r * t, g * t, b * t);
}

Color& Color::operator*=(const Color& other)
{
	r *= other.r;
	g *= other.g;
	b *= other.b;
	return *this;
}

bool Color::nearZero() const {
	return (r < 1e-8 && g < 1e-8 && b < 1e-8);
}

/**
 * @brief Operador de salida para imprimir el color
 * @param os Flujo de salida
 * @param color Color a imprimir
 * @return Referencia al flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const Color& color) {
	os << color.getRbyte() << " "
		<< color.getBbyte() << " "
		<< color.getGbyte();
	return os;
}

/**
 * @brief Operador de suma
 * @param a Primer color
 * @param b Segundo color
 * @return Color resultante
 */
Color operator+(const Color& a, const Color& b) {
	return Color(a.getR() + b.getR(), a.getG() + b.getG(), a.getB() + b.getB());
}

/**
 * @brief Operador de resta
 * @param a Primer color
 * @param b Segundo color
 * @return Color resultante
 */
Color operator-(const Color& a, const Color& b) {
	return Color(a.getR() - b.getR(), a.getG() - b.getG(), a.getB() - b.getB());
}

/**
 * @brief Operador de multiplicación por escalar
 * @param a Color
 * @param t Escalar
 * @return Color resultante
 */
Color operator*(const Color& a, double t) {
	return Color(a.getR() * t, a.getG() * t, a.getB() * t);
}

/**
 * @brief Operador de multiplicación por escalar
 * @param t Escalar
 * @param a Color
 * @return Color resultante
 */
Color operator*(double t, const Color& a) {
	return Color(a.getR() * t, a.getG() * t, a.getB() * t);
}

/**
 * @brief Operador de multiplicación entre dos colores
 * @param a Primer color
 * @param b Segundo color
 * @return Color resultante
 */
Color operator*(const Color& a, const Color& b) {
	return Color(a.getR() * b.getR(), a.getG() * b.getG(), a.getB() * b.getB());
}

/**
 * @brief Operador de división entre un color y un número
 * @param a Color
 * @param b Escalar
 * @return Color resultante
 */
Color operator/(const Color& a, double b) {
	return Color(a.getR() / b, a.getG() / b, a.getB() / b);
}

