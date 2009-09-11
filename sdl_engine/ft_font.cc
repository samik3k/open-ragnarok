/* $Id$
 * font.cc
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#include "stdafx.h"
#include "sdle/ft_font.h"
#include "sdle/sdl_engine.h"

#include <stdexcept>

namespace sdle {

FT_Library FTFont::library = NULL;
unsigned int FTFont::m_count = 0;

void FTFont::initLibrary() {
	if (FT_Init_FreeType(&library) != 0) {
		throw std::runtime_error("Could not initialize FreeType2 library.");
	}
}

void FTFont::killLibrary() {
	FT_Done_FreeType(library);
}

FTFont::FTFont() : Font() {
	if (library == NULL) {
		initLibrary();
	}
	m_valid = false;
	m_widths = NULL;
	m_count++;

	m_startchar = 0;
	m_charcount = 0;
}

FTFont::~FTFont() {
	m_count--;
	if (m_count == 0) {
		killLibrary();
	}
}

void FTFont::release() {
	if (m_widths != NULL)
		delete[] m_widths;

	if (glIsList(m_gllists))
		glDeleteLists(m_gllists, m_charcount);

	m_valid = false;
	m_startchar = 0;
	m_charcount = 0;
	m_texture = NULL;
}

bool FTFont::open(const char* fn, unsigned int size, unsigned int startchar, unsigned int charcount) {
	const static size_t MARGIN = 0;

	if (valid())
		release();

	if (m_widths != NULL)
		delete[] m_widths;
	m_widths = new unsigned int[charcount];

	m_startchar = startchar;
	m_charcount = charcount;

	// Step 1: Open the font using FreeType //
	FT_Face face;

	if (FT_New_Face(library, fn, 0, &face) != 0)
		throw std::runtime_error("Could not load font file.");

	// Abort if this is not a scalable font.
	if (!(face->face_flags & FT_FACE_FLAG_SCALABLE) || !(face->face_flags
			& FT_FACE_FLAG_HORIZONTAL))
		throw std::runtime_error("Invalid font: Error setting font size.");

	// Set the font size
	FT_Set_Pixel_Sizes(face, size, 0);

	// Step 2: Find maxAscent/Descent to calculate imageHeight //
	size_t imageHeight = 0;
	size_t imageWidth = 256;
	int maxDescent = 0;
	int maxAscent = 0;
	size_t lineSpace = imageWidth - MARGIN;
	size_t lines = 1;
	size_t charIndex;

	for (unsigned int ch = 0; ch < charcount; ++ch) {
		// Look up the character in the font file.
		charIndex = FT_Get_Char_Index(face, ch + startchar);

		// Render the current glyph.
		FT_Load_Glyph(face, charIndex, FT_LOAD_RENDER);

		m_widths[ch] = (face->glyph->metrics.horiAdvance >> 6) + MARGIN;
		// If the line is full go to the next line
		if (m_widths[ch] > lineSpace) {
			lineSpace = imageWidth - MARGIN;
			++lines;
		}
		lineSpace -= m_widths[ch];

#ifndef _MSC_VER
		maxAscent = std::max(face->glyph->bitmap_top, maxAscent);
		maxDescent = std::max(face->glyph->bitmap.rows - face->glyph->bitmap_top, maxDescent);
#else
		maxAscent = max(face->glyph->bitmap_top, maxAscent);
		maxDescent = max(face->glyph->bitmap.rows - face->glyph->bitmap_top, maxDescent);
#endif
	}

	m_height = maxAscent + maxDescent; // calculate height_ for text

	// Compute how high the texture has to be.
	size_t neededHeight = (maxAscent + maxDescent + MARGIN) * lines + MARGIN;
    // Get the first power of two in which it will fit
    imageHeight = 16;
    while(imageHeight < neededHeight)
    	imageHeight <<= 1;

    // Step 3: Generation of the actual texture //
	Image image(NULL, imageWidth, imageHeight, 8);

    // These are the position at which to draw the next glyph
    size_t x = MARGIN;
    size_t y = MARGIN + maxAscent;
    float texX1, texX2, texY1, texY2;   // used for display list

    m_gllists = glGenLists(charcount);  // generate the lists for filling

    // Drawing loop
    for(unsigned int ch = 0; ch < charcount; ++ch) {
        charIndex = FT_Get_Char_Index(face, ch+startchar);

        // Render the glyph
        FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        // See whether the character fits on the current line
        if(m_widths[ch] > imageWidth - x) {
            x = MARGIN;
            y += (maxAscent + maxDescent + MARGIN);
        }

        // calculate texture coordinates of the character
        texX1 = static_cast<float>(x) / imageWidth;
        texX2 = static_cast<float>(x+m_widths[ch]) / imageWidth;
        texY1 = static_cast<float>(y - maxAscent) / imageHeight;
        texY2 = texY1 + static_cast<float>(m_height) / imageHeight;

        // generate the character's display list
        glNewList(m_gllists + ch, GL_COMPILE);
        glBegin(GL_QUADS);
        glTexCoord2f(texX1,texY1);  glVertex2i(0,0);
        glTexCoord2f(texX2,texY1);  glVertex2i(m_widths[ch],0);
        glTexCoord2f(texX2,texY2);  glVertex2i(m_widths[ch],m_height);
        glTexCoord2f(texX1,texY2);  glVertex2i(0,m_height);
        glEnd();
        glTranslatef((float)m_widths[ch], 0.0f, 0.0f);  // translate forward
        glEndList();

        // copy image generated by FreeType to the texture
        for(int row = 0; row < face->glyph->bitmap.rows; ++row) {
            for(int pixel = 0; pixel < face->glyph->bitmap.width; ++pixel) {
                // set pixel at position to intensity (0-255) at the position
                image[(x + face->glyph->bitmap_left + pixel) +
                    (y - face->glyph->bitmap_top + row) * imageWidth] =
                        face->glyph->bitmap.buffer[pixel +
                            row * face->glyph->bitmap.pitch];
            }
        }

        x += m_widths[ch];
    }

    // generate the OpenGL texture from the byte array
    Texture::Root* root = new Texture::Root();
    if (!root->Create(&image)) {
    	release();
    	return(false);
    }
    m_texture = root;

    FT_Done_Face(face); // free the face data

    m_valid = true;
	return (true);
}

void FTFont::draw(const Rect& rect, const char* buf) const {
    if(!valid()) {
        throw std::logic_error("Invalid Font::drawText call.");
    }

    m_texture.Activate();

	float used_w = 0;

    glPushMatrix();
    glTranslated(rect.x, rect.y, 0);
    for(unsigned int i=0; i < strlen(buf); ++i) {
        unsigned char ch(buf[i] - m_startchar);     // ch - m_startchar = DisplayList offset
        // replace characters outside the valid range with undrawable
        if(ch > m_charcount) {
            ch = m_charcount-1;   // last character is 'undrawable'
        }

		if (rect.w > 0) {
			if ((used_w + m_widths[ch]) > rect.w) {
				// We've reached our maximum width.
				break;
			}
		}
        glCallList(m_gllists + ch);    // calculate list to call
		used_w += m_widths[ch]; // Increment our used width.
    }

    // Alternative, ignores undrawables (no noticable speed difference)
    //glListBase(listBase_-32);
    //glCallLists(static_cast<int>(std::strlen(buf)), GL_UNSIGNED_BYTE, buf);

    glPopMatrix();
}


float FTFont::getWidth(const char *str, ...) const {
    if(!valid()) {
        throw std::logic_error("Invalid Font::drawText call.");
    }

    std::va_list args;
    char buf[1024];

    va_start(args,str);
#ifndef _MSC_VER
	std::vsnprintf(buf, 1024, str, args);   // avoid buffer overflow
#else
	vsnprintf(buf, 1024, str, args);   // avoid buffer overflow
#endif
    va_end(args);

	float used_w = 0;

    for(unsigned int i=0; i < strlen(buf); ++i) {
        unsigned char ch(buf[i] - m_startchar);     // ch - m_startchar = DisplayList offset
        // replace characters outside the valid range with undrawable
        if(ch > m_charcount) {
            ch = m_charcount-1;   // last character is 'undrawable'
        }
		used_w += m_widths[ch]; // Increment our used width.
    }

	return(used_w);
}


}

