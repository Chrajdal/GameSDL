#include "Surface.h"

Surface::Surface(void)
	: m_data(NULL)
{}
Surface::~Surface(void)
{
	if (m_data != NULL)
		SDL_FreeSurface(m_data);
}

void Surface::LoadData(const std::string & file_name)
{
	m_data = IMG_Load(file_name.c_str());
	if (m_data == NULL)
		SDL_Log("IMG_Load failed: %s", SDL_GetError());
}

SDL_Surface * Surface::GetData(void)const
{
	return m_data;
}