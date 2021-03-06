/* $Id: slider.h $ */
#ifndef __GUI_SLIDER_H
#define __GUI_SLIDER_H

#include "element.h"

namespace GUI {

class Slider : public Element {
protected:
	sdle::Texture tex_start;
	sdle::Texture tex_mid;
	sdle::Texture tex_end;
	sdle::Texture tex_bar_start;
//	sdle::Texture tex_bar_mid;
	sdle::Texture tex_bar_end;

	virtual bool ParseXmlAttr(const TiXmlAttribute*, CacheManager&);

	int m_value, m_maxvalue;

public:
	Slider(Element* parent, CacheManager& cache);
//	Slider(Element* parent, const sdle::Texture& start, const sdle::Texture& mid, const sdle::Texture& end);
	Slider(Element* parent, const TiXmlElement*, CacheManager&);
//	Slider(Element* parent, const std::string& start, const std::string& mid, const std::string& end, CacheManager&);

	virtual void Draw(unsigned int delay = 0);

	void Click();

	void setValue(int value);
	void setMaxValue(int maxvalue);
	int getValue() const;
	int getMaxValue() const;
};

}

#endif /* __GUI_SLIDER_H */
