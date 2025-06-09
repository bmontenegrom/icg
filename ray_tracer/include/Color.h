/**
 * @file Color.h
 * @brief Definición de la clase Color para el sistema de ray tracing
 * 
 * Esta clase representa un color en el espacio RGB y proporciona métodos para
 * manipular y operar con colores.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#pragma once
#include <iostream>

class Color {
public:
	/**
	 * @brief Constructor por defecto que inicializa el color a negro
	 */
	Color();

	/**
	 * @brief Constructor que inicializa el color con valores RGB específicos
	 * @param r Componente rojo (0-1)
	 * @param g Componente verde (0-1)
	 * @param b Componente azul (0-1)
	 */
	Color(double r, double g, double b);
	~Color() = default;

	/**
	 * @brief Obtiene el componente rojo del color
	 * @return Valor del componente rojo
	 */
	double getR() const;

	/**
	 * @brief Obtiene el componente verde del color
	 * @return Valor del componente verde
	 */
	double getG() const;

	/**
	 * @brief Obtiene el componente azul del color
	 * @return Valor del componente azul
	 */
	double getB() const;

	/**
	 * @brief Establece el componente rojo del color
	 * @param r Nuevo valor del componente rojo
	 */
	void setR(double r);

	/**
	 * @brief Establece el componente verde del color
	 * @param g Nuevo valor del componente verde
	 */
	void setG(double g);

	/**
	 * @brief Establece el componente azul del color
	 * @param b Nuevo valor del componente azul
	 */
	void setB(double b);

	/**
	 * @brief Obtiene el componente rojo en formato byte (0-255)
	 * @return Valor del componente rojo en byte
	 */
	int getRbyte() const;

	/**
	 * @brief Obtiene el componente verde en formato byte (0-255)
	 * @return Valor del componente verde en byte
	 */
	int getGbyte() const;

	/**
	 * @brief Obtiene el componente azul en formato byte (0-255)
	 * @return Valor del componente azul en byte
	 */
	int getBbyte() const;

	/**
	 * @brief Operador de suma y asignación
	 * @param other Color a sumar
	 * @return Referencia al color resultante
	 */
	Color& operator+=(const Color& other);

	/**
	 * @brief Operador de multiplicación por escalar y asignación
	 * @param t Escalar
	 * @return Referencia al color resultante
	 */
	Color& operator*=(double t);

	Color& operator *=(const Color& other);

	bool nearZero() const;

private:
	double r, g, b; ///< Componentes RGB del color
};

/**
 * @brief Operador de salida para imprimir un color
 * @param os Stream de salida
 * @param color Color a imprimir
 * @return Stream de salida
 */
std::ostream& operator<<(std::ostream& os, const Color& color);

/**
 * @brief Operador de suma de colores
 * @param a Primer color
 * @param b Segundo color
 * @return Color resultante
 */
Color operator+(const Color& a, const Color& b);

/**
 * @brief Operador de resta de colores
 * @param a Primer color
 * @param b Segundo color
 * @return Color resultante
 */
Color operator-(const Color& a, const Color& b);

/**
 * @brief Operador de multiplicación de color por escalar
 * @param a Color
 * @param t Escalar
 * @return Color resultante
 */
Color operator*(const Color& a, double t);

/**
 * @brief Operador de multiplicación de escalar por color
 * @param t Escalar
 * @param a Color
 * @return Color resultante
 */
Color operator*(double t, const Color& a);

/**
 * @brief Operador de multiplicación componente a componente de colores
 * @param a Primer color
 * @param b Segundo color
 * @return Color resultante
 */
Color operator*(const Color& a, const Color& b);

/**
 * @brief Operador de división de color por escalar
 * @param a Color
 * @param t Escalar
 * @return Color resultante
 */
Color operator/(const Color& a, double t);


