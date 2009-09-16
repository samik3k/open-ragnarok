#ifndef __ROGL_SPR_TEXTURE_H
#define __ROGL_SPR_TEXTURE_H

#include "sdle/texture.h"
#include "ro/types/spr.h"
#include "ro/types/act.h"
#include "globject.h"

namespace rogl {

typedef struct sprInfo {
	// Start U and Start V
	float su, sv;
	// End U and End V
	float eu, ev;
	// Width and Height
	int w, h;
} _sprInfo;

/**
 * Utility class to properly load and create a SPR texture.
 */
class ROGL_DLLAPI SprGL : public GLObject {
protected:
	sdle::Texture m_texture;
	unsigned int m_framecount;
	sprInfo* m_info;

public:
	SprGL();
	SprGL(const SprGL&);
	virtual ~SprGL();

	bool open(const RO::SPR*);

	void Draw() const;
	void Draw(unsigned int idx) const;
	void Draw(const RO::ACT::Pat& cpat, unsigned int sprno, float& x, float& y, bool v_mirror, bool ext) const;

	// Deletes all info
	void release();

	unsigned int getFrameCount() const;

	SprGL& operator = (const SprGL&);
	sdle::Texture& getTexture();
	const sdle::Texture& getTexture() const;
};

}

#endif /* __ROGL_SPR_TEXTURE_H */