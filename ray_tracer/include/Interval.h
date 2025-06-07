/**
 * @file Interval.h
 * @brief Clase que representa un intervalo en el sistema de ray tracing
 * 
 * Esta clase se utiliza para definir rangos de valores, como el rango de parámetros
 * para la intersección de rayos con entidades.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#pragma once
#include "Constants.h"

class Interval {
public:
	
	/**
	 * @brief Constructor que inicializa el intervalo con valores mínimo y máximo
	 * @param min_val Valor mínimo del intervalo
	 * @param max_val Valor máximo del intervalo
	 */
	Interval(double min_val, double max_val);
	
	/**
	 * @brief Constructor por defecto
	 */
	Interval();
	
	/**
	 * @brief Obtiene el valor mínimo del intervalo
	 * @return Valor mínimo
	 */
	double getMin() const;
	
	/**
	 * @brief Obtiene el valor máximo del intervalo
	 * @return Valor máximo
	 */
	double getMax() const;
	
	/**
	 * @brief Establece el valor mínimo del intervalo
	 * @param min_val Nuevo valor mínimo
	 */
	void setMin(double min_val);
	
	/**
	 * @brief Establece el valor máximo del intervalo
	 * @param max_val Nuevo valor máximo
	 */
	void setMax(double max_val);	

	/**
	 * @brief Calcula el tamaño del intervalo
	 * @return Tamaño del intervalo
	 */
	double size() const;
	
	/**
	 * @brief Verifica si un valor está contenido en el intervalo
	 * @param value Valor a verificar
	 * @return true si el valor está contenido, false en caso contrario
	 */
	bool contains(double value) const;
	
	/**
	 * @brief Verifica si un valor está rodeado por el intervalo
	 * @param value Valor a verificar
	 * @return true si el valor está rodeado, false en caso contrario
	 */
	bool surrounds(double value) const;
	
	/**
	 * @brief Ajusta un valor al intervalo
	 * @param value Valor a ajustar
	 * @return Valor ajustado
	 */
	double clamp(double value) const;
	
private:
	double min;
	double max;

};

