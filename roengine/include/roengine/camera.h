#ifndef __CAMERA_H
#define __CAMERA_H

#include "vector.h"
#include "ro/constants.h"

/**
 * A simple camera with many functionalities.
 * Provides an way to setup and move around the world.
 */
class Camera {
protected:
	Vector3f eye;
	Vector3f dest;

	float fx, vx, fy, vy;
	float zoom, zoom_a;
public:
	Camera();
	~Camera();

	void Update();
	void RotateY(float f);
	void RotateX(float f);
	void Translate(const Vector3f&);
	void TranslateX(const float&);
	void TranslateY(const float&);
	void TranslateZ(const float&);

	/**
	 * Translates the Destination vector to a given point (absolute. not relative), taking the eye vector with it.
	 */
	void TranslateDestTo(const Vector3f&);

	void setEye(const Vector3f&);
	void setDest(const Vector3f&);

	Vector3f& getEye();
	Vector3f& getDest();
	const Vector3f& getEye() const;
	const Vector3f& getDest() const;

	void ZoomIn(float);
	void ZoomOut(float);

	void Look() const;

	ro::CDir getDirection() const;
};

#endif /* __CAMERA_H */

