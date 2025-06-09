/**
 * @file Quad.cpp
 * @brief Implementación de la clase Quad para cuadriláteros/rectángulos axis-aligned
 * 
 * Esta implementación sigue el enfoque de axis-aligned rectangles como se describe
 * en las referencias web de raytracing para Cornell Box.
 * 
 * @author Valentin Dutra
 * @date 08/06/2025
 */

#include "Quad.h"
#include "Material.h"
#include "HitRecord.h"
#include <cmath>

/**
 * @brief Constructor de la clase Quad
 * @param minPoint Punto mínimo que define una esquina del cuadrilátero
 * @param maxPoint Punto máximo que define la esquina opuesta del cuadrilátero
 * @param axis Eje en el que el cuadrilátero está alineado (0=X, 1=Y, 2=Z)
 * @param value Valor fijo en el eje alineado
 */
Quad::Quad(const Vec3& minPoint, const Vec3& maxPoint, int axis, double value)
    : minPoint(minPoint), maxPoint(maxPoint), fixedAxis(axis), fixedValue(value) {
}

/**
 * @brief Determina si un rayo intersecta con el cuadrilátero
 * @param ray Rayo a verificar
 * @param ray_t Intervalo de tiempo válido para la intersección
 * @param rec Registro donde se almacena la información de la intersección
 * @return true si hay intersección, false en caso contrario
 */
bool Quad::hit(const Ray& ray, Interval ray_t, HitRecord& rec) const {
    // Obtener los componentes del rayo
    Vec3 origin = ray.getOrigin();
    Vec3 direction = ray.getDirection();
    
    // Calcular t basado en el eje fijo
    double t;
    if (fixedAxis == 0) { // Pared en X fijo
        if (std::abs(direction.getX()) < 1e-8) return false; // Rayo paralelo
        t = (fixedValue - origin.getX()) / direction.getX();
    } else if (fixedAxis == 1) { // Piso/techo en Y fijo
        if (std::abs(direction.getY()) < 1e-8) return false; // Rayo paralelo
        t = (fixedValue - origin.getY()) / direction.getY();
    } else { // Pared trasera en Z fijo
        if (std::abs(direction.getZ()) < 1e-8) return false; // Rayo paralelo
        t = (fixedValue - origin.getZ()) / direction.getZ();
    }
    
    // Verificar si t está en el rango válido
    if (!ray_t.contains(t)) {
        return false;
    }
    
    // Calcular el punto de intersección
    Vec3 hitPoint = ray.pointAtParameter(t);
    
    // Verificar si el punto está dentro del rectángulo
    bool inBounds = false;
    Vec3 normal;
    
    if (fixedAxis == 0) { // Pared en X
        inBounds = (hitPoint.getY() >= minPoint.getY() && hitPoint.getY() <= maxPoint.getY() &&
                   hitPoint.getZ() >= minPoint.getZ() && hitPoint.getZ() <= maxPoint.getZ());
        normal = Vec3(direction.getX() > 0 ? -1.0 : 1.0, 0, 0); // Normal apunta hacia la cámara
    } else if (fixedAxis == 1) { // Piso/techo en Y
        inBounds = (hitPoint.getX() >= minPoint.getX() && hitPoint.getX() <= maxPoint.getX() &&
                   hitPoint.getZ() >= minPoint.getZ() && hitPoint.getZ() <= maxPoint.getZ());
        normal = Vec3(0, direction.getY() > 0 ? -1.0 : 1.0, 0); // Normal apunta hacia la cámara
    } else { // Pared trasera en Z
        inBounds = (hitPoint.getX() >= minPoint.getX() && hitPoint.getX() <= maxPoint.getX() &&
                   hitPoint.getY() >= minPoint.getY() && hitPoint.getY() <= maxPoint.getY());
        normal = Vec3(0, 0, direction.getZ() > 0 ? -1.0 : 1.0); // Normal apunta hacia la cámara
    }
    
    if (!inBounds) {
        return false;
    }
    
    // Llenar el registro de intersección
    rec.t = t;
    //rec.point = hitPoint;
    rec.point = hitPoint;
    rec.normal = normal;
    rec.frontFace = dotProduct(direction, normal) < 0;
    //rec.mat = material_ptr;
    rec.material_ptr = material_ptr;
    
    return true;
}

/**
 * @brief Establece el material del cuadrilátero
 * @param material Puntero compartido al material a asignar
 */
void Quad::setMaterial(std::shared_ptr<Material> material) {
    material_ptr = material;
} 