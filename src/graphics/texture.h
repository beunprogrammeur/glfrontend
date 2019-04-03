#ifndef TEXTURE_H
#define TEXTURE_H

namespace graphics
{
	
class Texture
{
private:
	unsigned int m_textureId;
	bool m_isLoaded;

public:
	Texture();
	Texture(const char* path);
	~Texture();
	
	void load(const char* path);
	void unload();
	void setActive();
	
	inline bool isLoaded() const { return m_isLoaded; }
	inline unsigned int id() const { return m_textureId; }

};
	
	
} /** namespace graphics **/

#endif
