#pragma once

#include <SDL_image.h>
#include <string>

class Image
{
public:
	Image(void);
	~Image(void);

	void LoadData(const std::string & file_name);
	SDL_Texture * GetData(void)const;
private:
	SDL_Texture * m_data;
};
