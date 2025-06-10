/**
 * @file HitRecord.h
 * @brief Definición de la clase HitRecord para almacenar información de intersecciones
 * 
 * Esta clase almacena información sobre la intersección de un rayo con una superficie,
 * incluyendo el punto de intersección, la normal, y el material asociado.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#pragma once
#include "Vec3.h"
#include "Ray.h"
#include <memory>

// Forward declaration
class Material;

/**
 * @brief Almacena información sobre la intersección de un rayo con una superficie
 * 
 * Esta estructura contiene todos los datos necesarios sobre una intersección
 * entre un rayo y un objeto geométrico, incluyendo el punto de intersección,
 * la normal de la superficie, el parámetro t del rayo, y el material asociado.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 * @version 1.0
 * @author Valentin Dutra
 * @date 08/06/2025
 * @version 1.1
 */
class HitRecord {
public:
                                
	Vec3 point;                              ///< Punto de intersección en el espacio 3D
	Vec3 normal;                             ///< Vector normal unitario en el punto de intersección
	double t;                                ///< Valor del parámetro t en la ecuación del rayo
	double u, v;                             ///< Coordenadas de textura (u,v)
	bool frontFace;                          ///< true si el rayo golpea la cara frontal, false si es la trasera
	std::shared_ptr<Material> material_ptr;  ///< Puntero al material del objeto intersectado
	//std::shared_ptr<Material> mat;           ///< Alias para material_ptr
	
	/**
	 * @brief Constructor parametrizado
	 * @param p Punto de intersección
	 * @param n Normal en el punto de intersección
	 * @param t_val Valor del parámetro t
	 */
	HitRecord(const Vec3& p, const Vec3& n, double t_val);
	
	/**
	 * @brief Constructor por defecto
	 */
	HitRecord() : point(Vec3()), normal(Vec3()), t(0), u(0), v(0), frontFace(true) {}

	/**
	 * @brief Determina si la intersección es en la cara frontal o trasera
	 * 
	 * Calcula automáticamente si el rayo intersectó la cara frontal o trasera
	 * del objeto basándose en la dirección del rayo y la normal externa.
	 * Ajusta la normal para que siempre apunte hacia el lado del rayo incidente.
	 * 
	 * @param ray Rayo que causó la intersección
	 * @param outwardNormal Normal externa de la superficie (apuntando hacia afuera)
	 */
	void setFaceNormal(const Ray& ray, const Vec3& outwardNormal);
};