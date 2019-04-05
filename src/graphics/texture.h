#ifndef TEXTURE_H
#define TEXTURE_H

namespace graphics
{
	
class Texture
{
private:
	unsigned int m_textureId;
	bool m_isLoaded;
	int m_width;
	int m_height;

public:
	Texture();
	Texture(const char* path);
	~Texture();
	
	void load(const char* path);
	void unload();
	void setActive();
	
	inline unsigned int id() const { return m_textureId; }
	inline bool isLoaded()   const { return m_isLoaded;  }
	inline int height()      const { return m_height;    }
	inline int width()       const { return m_width;     }

};
	
	
} /** namespace graphics **/

#endif
