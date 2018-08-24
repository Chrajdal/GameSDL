#pragma once

#include <SDL_image.h>
#include <string>

class Surface
{
public:
	Surface(void);
	~Surface(void);

	void LoadData(const std::string & file_name);
	SDL_Surface * GetData(void)const;
private:
	SDL_Surface * m_data;
};
