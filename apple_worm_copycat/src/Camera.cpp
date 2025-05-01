#include "Camera.h"
#include <iostream>


Camera::Camera(float x, float y, float z) : posX(x), posY(y), posZ(z), yaw(0.0f), pitch(0.0f), sensitivity(0.1f), mode(CameraMode::ISOMETRIC)
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


void Camera::setPositionAndDirection(float x, float y, float z, float targetX, float targetY, float targetZ)
{
	posX = x;
	posY = y;
	posZ = z;
	float deltaX = targetX - x;
	float deltaY = targetY - y;
	float deltaZ = targetZ - z;

	yaw = atan2(deltaZ, deltaX) * 180.0f / M_PI;
	pitch = atan2(deltaY, sqrt(deltaX * deltaX + deltaZ * deltaZ)) * 180.0f / M_PI;
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
	if (mode == CameraMode::ISOMETRIC) {
		this->setPositionAndDirection(0.0, 0.0, 1.5, 0.5, 0.5, 0.0);
	}
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

void Camera::switchCameraMode()
{
	this->mode = static_cast<CameraMode>((static_cast<unsigned int>(mode) + 4) % 3);
	if (this->mode == CameraMode::FREE_CAMERA) {
		this->setPositionAndDirection(0.0, 0.0, 1.5, 0.5, 0.5, 0.0);
	}
	
}

CameraMode Camera::getCameraMode() const
{
	return this->mode;
}

void Camera::followTarget(float targetX, float targetY, float targetZ, float distance)
{
	this->setPositionAndDirection(targetX + distance, targetY + distance, targetZ, targetX, targetY, targetZ);



}
