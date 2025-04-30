#include "Camera.h"


Camera::Camera(float x, float y, float z) : posX(x), posY(y), posZ(z), yaw(0.0f), pitch(0.0f), sensitivity(0.1f)
{
	
}

Camera::Camera(float x, float y, float z, float targetX, float targetY, float targetZ)
	: posX(x), posY(y), posZ(z), yaw(0.0f), pitch(0.0f), sensitivity(0.05f) {
	// Calcular yaw y pitch en base al objetivo
	float deltaX = targetX - x;
	float deltaY = targetY - y;
	float deltaZ = targetZ - z;

	yaw = atan2(deltaZ, deltaX) * 180.0f / M_PI;
	pitch = atan2(deltaY, sqrt(deltaX * deltaX + deltaZ * deltaZ)) * 180.0f / M_PI;
}


void Camera::setPosition(float x, float y, float z)
{
	posX = x;
	posY = y;
	posZ = z;
}

void Camera::updateMouseMovement(int mouseX, int mouseY)
{
	yaw += mouseX * sensitivity;
	pitch -= mouseY * sensitivity;
	
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	
	if (pitch < -89.0f) {
		pitch = -89.0f; 
	}
	if (yaw < 0.0f) {
		yaw += 360.0f;
	}
	if (yaw >= 360.0f) {
		yaw -= 360.0f;
	}

}

void Camera::applyView()
{
	// Convertir los ángulos a radianes
	float yawRad = yaw * M_PI / 180.0f;
	float pitchRad = pitch * M_PI / 180.0f;

	// Calcular el vector de dirección
	float dirX = cos(yawRad) * cos(pitchRad);
	float dirY = sin(pitchRad);
	float dirZ = sin(yawRad) * cos(pitchRad);

	gluLookAt(posX, posY, posZ,       // Posición de la cámara
		posX + dirX, posY + dirY, posZ + dirZ, // Objetivo de la cámara
		0.0, 1.0, 0.0);  // Vector "arriba"*/



}

void Camera::moveForward(float speed)
{
	posX += speed * cos(yaw * M_PI / 180.0f);
	posZ += speed * sin(yaw * M_PI / 180.0f);

}

void Camera::moveBackward(float speed)
{
	posX -= speed * cos(yaw * M_PI / 180.0f);
	posZ -= speed * sin(yaw * M_PI / 180.0f);

}

void Camera::moveLeft(float speed)
{
	posX -= speed * sin(yaw * M_PI / 180.0f);
	posZ += speed * cos(yaw * M_PI / 180.0f);

}

void Camera::moveRight(float speed)
{
	posX += speed * sin(yaw * M_PI / 180.0f);
	posZ -= speed * cos(yaw * M_PI / 180.0f);
}

void Camera::setSensitivity(float newSensitivity)
{
	sensitivity = newSensitivity;
}
