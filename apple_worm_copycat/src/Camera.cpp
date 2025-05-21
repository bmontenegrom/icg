#include "Camera.h"
#include <iostream>

Camera::Camera() {
	mode = CameraMode::DEFAULT;
	sensitivity = 0.1f;
	radius = 2.0f;
	yaw = 0.0f;
	pitch = 0.0f;

	// Posición y objetivo por defecto
	defaultPosX = 0.5f;
	defaultPosY = 1.0f;
	defaultPosZ = 1.5f;
	defaultTargetX = 0.5f;
	defaultTargetY = 0.25f;
	defaultTargetZ = 0.0f;

	// Inicializar posición y objetivo actual
	posX = defaultPosX;
	posY = defaultPosY;
	posZ = defaultPosZ;
	targetX = defaultTargetX;
	targetY = defaultTargetY;
	targetZ = defaultTargetZ;

	// Inicializar dirección
	dirX = 0.0f;
	dirY = 0.0f;
	dirZ = 1.0f;
}

void Camera::updateMouseMovement(int x, int y) {
	if (mode == CameraMode::FREE_CAMERA) {
		// Convertir el movimiento del mouse a ángulos
		yaw += x * sensitivity;
		pitch -= y * sensitivity;

		// Limitar el pitch para evitar volteos
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}
	if (mode == CameraMode::FREE_CAMERA) {
		// Calcular la nueva posición de la cámara
		float yawRad = yaw * M_PI / 180.0f;
		float pitchRad = pitch * M_PI / 180.0f;
		posX = targetX + radius * cos(pitchRad) * sin(yawRad);
		posY = targetY + radius * sin(pitchRad);
		posZ = targetZ + radius * cos(pitchRad) * cos(yawRad);
	}
}

void Camera::applyView() {
	switch (mode) {
		case CameraMode::DEFAULT:
			gluLookAt(defaultPosX, defaultPosY, defaultPosZ,
					 defaultTargetX, defaultTargetY, defaultTargetZ,
					 0.0f, 1.0f, 0.0f);
			break;
		case CameraMode::THIRD_PERSON:
			gluLookAt(posX, posY, posZ,
					 targetX, targetY, targetZ,
					 0.0f, 1.0f, 0.0f);
			break;
		case CameraMode::FREE_CAMERA:
			gluLookAt(posX, posY, posZ,
					 targetX, targetY, targetZ,
					 0.0f, 1.0f, 0.0f);
			break;
	}
}


void Camera::setSensitivity(float newSensitivity) {
	sensitivity = newSensitivity;
}

void Camera::switchCameraMode() {
	switch (mode) {
	case CameraMode::DEFAULT:
		mode = CameraMode::THIRD_PERSON;
		break;
	case CameraMode::THIRD_PERSON:
		mode = CameraMode::FREE_CAMERA;
		yaw = 0.0f;
		pitch = 0.0f;
		break;
	case CameraMode::FREE_CAMERA:
		mode = CameraMode::DEFAULT;
		resetToDefault();
		break;
	}
}

CameraMode Camera::getCameraMode() const {
	return mode;
}

void Camera::followTarget(float tx, float ty, float tz) {
	if (mode == CameraMode::THIRD_PERSON) {
		posX = tx - 0.3;
		posY = ty + 0.3;
		posZ = tz + 0.3;
		targetX = tx;
		targetY = ty;
		targetZ = tz;
	}
	
}

void Camera::updatePosition(float x, float y, float z) {
	posX = x;
	posY = y;
	posZ = z;
}

void Camera::updateTarget(float x, float y, float z) {
	targetX = x;
	targetY = y;
	targetZ = z;
}

void Camera::resetToDefault() {
	posX = defaultPosX;
	posY = defaultPosY;
	posZ = defaultPosZ;
	targetX = defaultTargetX;
	targetY = defaultTargetY;
	targetZ = defaultTargetZ;
	yaw = 0.0f;
	pitch = 0.0f;
	mode = CameraMode::DEFAULT;
}
