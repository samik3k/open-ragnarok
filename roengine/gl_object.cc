/* $Id$ */
#include "stdafx.h"

#include "gl_object.h"

GLObject::GLObject() {
	xrot=yrot=zrot=0.0f;
	scalex=scaley=scalez=1.0f;
	m_useTexture = false; 
	m_visible = true;
	m_frustum_check = true;
}

GLObject::~GLObject() {}

void GLObject::setVisible(bool v) {
	m_visible = v;
}

void GLObject::AfterDraw() {
	if (m_useTexture)
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

bool GLObject::isVisible() const {
	return(m_visible);
}

void GLObject::BeforeDraw() {
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(yrot, 0, 1, 0);
	glRotatef(zrot, 1, 0, 0);
	glRotatef(xrot, 0, 0, 1);
	glScalef(scalex, scaley, scalez);
	if (m_useTexture) {
		glEnable(GL_TEXTURE_2D);
		m_texture.Activate();
	}
	else {
		glDisable(GL_TEXTURE_2D);
	}
}

void GLObject::Render(long tickdelay) {
	m_tickdelay = tickdelay;
	BeforeDraw();
	Draw();
	AfterDraw();
}

void GLObject::setPos(const float& x, const float& y, const float& z) {
	pos.set(x, y, z);
}

void GLObject::setRot(const float& x, const float& y, const float& z) {
	this->xrot = x;
	this->yrot = y;
	this->zrot = z;
}

void GLObject::setScale(const float& x, const float& y, const float& z) {
	scalex = x;
	scaley = y;
	scalez = z;
}

Vector3f& GLObject::getPos() {
	return(pos);
}

const Vector3f& GLObject::getPos() const {
	return(pos);
}

void GLObject::setRotX(const float& x) {
	xrot = x;
}

void GLObject::setRotY(const float& y) {
	yrot = y;
}

void GLObject::setRotZ(const float& z) {
	zrot = z;
}

void GLObject::setTexture(Texture::Pointer& tex) {
	m_texture = tex;
}

void GLObject::useTexture(bool b) {
	m_useTexture = b;
}

void GLObject::setFrustumCheck(bool b) {
	m_frustum_check = b;
}

bool GLObject::isInFrustum(const Frustum& f) const {
	if (!m_frustum_check)
		return(true);
	return(f.PointVisible(pos[0], pos[1], pos[2]));
}
