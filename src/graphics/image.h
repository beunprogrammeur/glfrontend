#ifndef IMAGE_H
#define IMAGE_H

#include "texture.h" 

namespace graphics
{


class Image 
{
public:
	enum class Layer
	{
		Layer0,
		Layer1,
		Layer2,
		Layer3
	};

private:
	Texture m_texture;
	unsigned int m_vbo;
	unsigned int m_vao;
	unsigned int m_ebo;
public:
	Image();
	Image(const char* path);	
	~Image();

	void load(const char* path);
	void activate(Image::Layer layer);
	inline unsigned int id() const { return m_texture.id(); } 
};
	
} /** namespace graphics **/



#endif
