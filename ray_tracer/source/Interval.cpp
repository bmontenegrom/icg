/**
 * @file Interval.cpp
 * @brief Implementación de la clase Interval para el sistema de ray tracing
 * 
 * Esta clase se utiliza para definir rangos de valores, como el rango de parámetros
 * para la intersección de rayos con entidades.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#include "Interval.h"

/**
 * @brief Constructor que inicializa el intervalo con valores mínimo y máximo
 * @param min_val Valor mínimo del intervalo
 * @param max_val Valor máximo del intervalo
 */
Interval::Interval(double min_val, double max_val) : min(min_val), max(max_val) {}

/**
 * @brief Constructor por defecto
 */
Interval::Interval() : min(infinity), max(-infinity) {}

/**
 * @brief Obtiene el valor mínimo del intervalo
 * @return Valor mínimo
 */
double Interval::getMin() const {
	return min;
}

/**
 * @brief Obtiene el valor máximo del intervalo
 * @return Valor máximo
 */
double Interval::getMax() const {
	return max;
}

/**
 * @brief Establece el valor mínimo del intervalo
 * @param min_val Nuevo valor mínimo
 */
void Interval::setMin(double min_val) {
	min = min_val;
}

/**
 * @brief Establece el valor máximo del intervalo
 * @param max_val Nuevo valor máximo
 */
void Interval::setMax(double max_val) {
	max = max_val;
}

/**
 * @brief Calcula el tamaño del intervalo
 * @return Tamaño del intervalo
 */
double Interval::size() const {
	return max - min;
}

/**
 * @brief Verifica si un valor está contenido en el intervalo
 * @param value Valor a verificar
 * @return true si el valor está contenido, false en caso contrario
 */
bool Interval::contains(double value) const {
	return value >= min && value <= max;
}

/**
 * @brief Verifica si un valor está rodeado por el intervalo
 * @param value Valor a verificar
 * @return true si el valor está rodeado, false en caso contrario
 */
bool Interval::surrounds(double value) const {
	return value > min && value < max;
}

/**
 * @brief Ajusta un valor al intervalo
 * @param value Valor a ajustar
 * @return Valor ajustado
 */
double Interval::clamp(double value) const {
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}
	return value;
	
}