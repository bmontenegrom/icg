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
}

void Camera::updateMouseMovement(int x, int y) {
	if (mode == CameraMode::FREE_CAMERA || mode == CameraMode::FIRST_PERSON) {
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
	if (mode == CameraMode::FIRST_PERSON) {
		// FOV normal para first person
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(35.0, 800.0 / 600.0, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
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
		case CameraMode::FIRST_PERSON:
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

void Camera::moveForward(float speed) {
	posX += speed * cos(yaw * M_PI / 180.0f);
	posZ += speed * sin(yaw * M_PI / 180.0f);
}

void Camera::moveBackward(float speed) {
	posX -= speed * cos(yaw * M_PI / 180.0f);
	posZ -= speed * sin(yaw * M_PI / 180.0f);
}

void Camera::moveLeft(float speed) {
	posX -= speed * sin(yaw * M_PI / 180.0f);
	posZ += speed * cos(yaw * M_PI / 180.0f);
}

void Camera::moveRight(float speed) {
	posX += speed * sin(yaw * M_PI / 180.0f);
	posZ -= speed * cos(yaw * M_PI / 180.0f);
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

void Camera::followTarget(float tx, float ty, float tz, float distance, float dirX, float dirY, float dirZ) {
	if (mode == CameraMode::THIRD_PERSON) {
		float upOffset = 0.15f;
		float backOffset = distance;
		float len = sqrt(dirX*dirX + dirY*dirY + dirZ*dirZ);
		float nx = (len > 0) ? dirX / len : 0.0f;
		float ny = (len > 0) ? dirY / len : 0.0f;
		float nz = (len > 0) ? dirZ / len : 1.0f;
		posX = tx - nx * backOffset;
		posY = ty + upOffset;
		posZ = tz - nz * backOffset;
		targetX = tx;
		targetY = ty + upOffset * 0.5f;
		targetZ = tz;
	}
	else if (mode == CameraMode::FIRST_PERSON) {
		// Cámara exactamente en la cabeza mirando hacia adelante, sin offset
		posX = tx;
		posY = ty;
		posZ = tz;
		float baseYaw = atan2(dirZ, dirX);
		float totalYaw = baseYaw + yaw * M_PI / 180.0f;
		float totalPitch = pitch * M_PI / 180.0f;
		float lookX = cos(totalPitch) * cos(totalYaw);
		float lookY = sin(totalPitch);
		float lookZ = cos(totalPitch) * sin(totalYaw);
		targetX = posX + lookX * 0.5f;
		targetY = posY + lookY * 0.5f;
		targetZ = posZ + lookZ * 0.5f;
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
}
