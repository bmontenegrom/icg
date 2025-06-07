/**
 * @file Constants.h
 * @brief Constantes y funciones de utilidad para el sistema de ray tracing
 * 
 * Este archivo contiene constantes matemáticas, dimensiones de la imagen
 * y funciones de utilidad utilizadas en el sistema de ray tracing.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#pragma once
#include <limits>
#include <cstdlib>

const double PI = 3.14159265358979323846; ///< Valor de pi
const int WIDTH = 800; ///< Ancho predeterminado de la imagen
const int HEIGHT = 600; ///< Alto predeterminado de la imagen

const double infinity = std::numeric_limits<double>::infinity(); ///< Valor de infinito

//utility functions

/**
 * @brief Convierte grados a radianes
 * @param degrees Ángulo en grados
 * @return Ángulo en radianes
 */
inline double degToRad(double degrees) {
	return degrees * (PI / 180.0);
}

/**
 * @brief Genera un número aleatorio entre 0 y 1
 * @return Número aleatorio en el rango [0,1)
 */
inline double random_double() {
	return std::rand() / (RAND_MAX + 1.0);
}

/**
 * @brief Genera un número aleatorio en un rango específico
 * @param min Valor mínimo del rango
 * @param max Valor máximo del rango
 * @return Número aleatorio en el rango [min,max)
 */
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}