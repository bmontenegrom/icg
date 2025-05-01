#include <SDL.h>
#include "SDL_opengl.h"
#include <GL/glu.h>
#include "Constants.h"
#include <cmath> // Para funciones trigonométricas


class Camera {
public:
	Camera(float x, float y, float z);
	Camera(float x, float y, float z, float targetX, float targetY, float targetZ);
	void setPositionAndDirection(float x, float y, float z, float targetX, float targetY, float targetZ);
	void updateMouseMovement(int mouseX, int mouseY);
	void applyView();
	void moveForward(float speed);
	void moveBackward(float speed);
	void moveLeft(float speed);
	void moveRight(float speed);
	void setSensitivity(float newSensitivity);
	void switchCameraMode();
	CameraMode getCameraMode() const;
	void followTarget(float targetX, float targetY, float targetZ, float distance);
private:
	float posX, posY, posZ;
	float yaw, pitch;//rotacion horizontal y vertical
	float sensitivity;
	CameraMode mode;

};