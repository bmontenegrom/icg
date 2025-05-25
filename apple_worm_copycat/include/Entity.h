#pragma once
#include <string>
#include "ObjLoader.h"
#include "Constants.h"


enum class EntityType;

class Entity {
public:
	Entity(double x, double y, double z, double width, double height, double depth);
	Entity(double x, double y, double z);
	Entity(double x, double y, double z, double width, double height, double depth, std::string path);
	virtual ~Entity();
	virtual void render(bool texture) = 0;


	void setPosition(double x, double y, double z);
	void setX(double x);
	void setY(double y);
	void setZ(double z);
	void setWidth(double width);
	void setHeight(double height);
	void setDepth(double depth);
	static void setInterpolation(float interp);

	
	double getX() const;
	double getY() const;
	double getZ() const;
	double getWidth() const;
	double getHeight() const;
	double getDepth() const;
	virtual EntityType getType() const = 0;
	void updatePreviousPosition();
	double getPrevX() const;
	double getPrevY() const;
	double getPrevZ() const;
	float getInterpolation();
	double getInterpolatedX() const;
	double getInterpolatedY() const;
	double getInterpolatedZ() const;

	
	bool isColliding(const Entity* other) const;

protected:
	static float interpolation;

private:
	double x;
	double y;
	double z;
	double width;
	double height;
	double depth;
	double prevX, prevY, prevZ;

	std::vector<ObjVertex> vertices;
};