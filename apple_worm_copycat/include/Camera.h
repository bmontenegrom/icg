#pragma once
#include <SDL.h>
#include "SDL_opengl.h"
#include <GL/glu.h>
#include "Constants.h"
#include <cmath> // Para funciones trigonométricas

enum CameraMode {
	DEFAULT,        // Vista inicial configurada
	THIRD_PERSON,   // Sigue al gusano
	FREE_CAMERA     // Cámara libre controlable
};

class Camera {
public:
	Camera();
	void switchCameraMode();
	CameraMode getCameraMode() const;
	void applyView();
	void updateMouseMovement(int x, int y);
	void updatePosition(float x, float y, float z);
	void updateTarget(float x, float y, float z);
	void resetToDefault();
	void moveForward(float speed);
	void moveBackward(float speed);
	void moveLeft(float speed);
	void moveRight(float speed);
	void setSensitivity(float newSensitivity);
	void followTarget(float targetX, float targetY, float targetZ);

private:
	CameraMode mode;
	float posX, posY, posZ;
	float targetX, targetY, targetZ;
	float defaultPosX, defaultPosY, defaultPosZ;
	float defaultTargetX, defaultTargetY, defaultTargetZ;
	float yaw, pitch;
	float sensitivity;
	float radius;
	
	// Vectores de dirección
	float dirX, dirY, dirZ;
};