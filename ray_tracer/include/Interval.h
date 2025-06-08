/**
 * @file Interval.h
 * @brief Clase que representa un intervalo en el sistema de ray tracing
 * 
 * Esta clase se utiliza para definir rangos de valores, como el rango de parámetros
 * para la intersección de rayos con entidades. Proporciona funcionalidades para
 * manipular y consultar intervalos numéricos, incluyendo operaciones como:
 * - Verificación de contención de valores
 * - Ajuste de valores al intervalo (clamp)
 * - Cálculo del tamaño del intervalo
 * - Verificación de valores rodeados
 * 
 * Es especialmente útil para:
 * - Definir rangos válidos para el parámetro t en intersecciones rayo-objeto
 * - Establecer límites para valores como coordenadas de texturas
 * - Acotar rangos de valores en cálculos de iluminación
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#pragma once
#include "Constants.h"

/**
 * @brief Clase que encapsula un intervalo numérico [min, max]
 * 
 * Proporciona una interfaz orientada a objetos para manipular intervalos
 * numéricos cerrados. Incluye operaciones comunes como verificación de
 * pertenencia, ajuste de valores y consultas sobre el intervalo.
 */
class Interval {
public:
	
	/**
	 * @brief Constructor que inicializa el intervalo con valores mínimo y máximo
	 * @param min_val Valor mínimo del intervalo
	 * @param max_val Valor máximo del intervalo
	 * @pre min_val <= max_val
	 */
	Interval(double min_val, double max_val);
	
	/**
	 * @brief Constructor por defecto
	 * 
	 * Inicializa un intervalo vacío o inválido. Útil cuando se necesita
	 * declarar un intervalo que será configurado posteriormente.
	 */
	Interval();
	
	/**
	 * @brief Obtiene el valor mínimo del intervalo
	 * @return Valor mínimo que define el límite inferior del intervalo
	 */
	double getMin() const;
	
	/**
	 * @brief Obtiene el valor máximo del intervalo
	 * @return Valor máximo que define el límite superior del intervalo
	 */
	double getMax() const;
	
	/**
	 * @brief Establece el valor mínimo del intervalo
	 * @param min_val Nuevo valor mínimo
	 * @note Si min_val > max, el intervalo podría quedar en estado inválido
	 */
	void setMin(double min_val);
	
	/**
	 * @brief Establece el valor máximo del intervalo
	 * @param max_val Nuevo valor máximo
	 * @note Si max_val < min, el intervalo podría quedar en estado inválido
	 */
	void setMax(double max_val);	

	/**
	 * @brief Calcula el tamaño del intervalo
	 * @return Diferencia entre el valor máximo y mínimo (max - min)
	 */
	double size() const;
	
	/**
	 * @brief Verifica si un valor está contenido en el intervalo
	 * @param value Valor a verificar
	 * @return true si min <= value <= max, false en caso contrario
	 */
	bool contains(double value) const;
	
	/**
	 * @brief Verifica si un valor está rodeado por el intervalo
	 * @param value Valor a verificar
	 * @return true si min < value < max, false en caso contrario
	 * @note Difiere de contains() en que los límites no se consideran incluidos
	 */
	bool surrounds(double value) const;
	
	/**
	 * @brief Ajusta un valor al intervalo
	 * @param value Valor a ajustar
	 * @return value limitado al rango [min, max]
	 * @note Si value < min retorna min, si value > max retorna max
	 */
	double clamp(double value) const;
	
private:
	double min;    ///< Límite inferior del intervalo
	double max;    ///< Límite superior del intervalo

};
