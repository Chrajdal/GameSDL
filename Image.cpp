#include "Image.h"

Image::Image(void)
	: m_data(NULL)
{}
Image::~Image(void)
{
	if (m_data != NULL)
		SDL_DestroyTexture(m_data);
}

void Image::LoadData(const std::string & file_name)
{
	SDL_Surface * tmp = IMG_Load(file_name.c_str());
	if (tmp == NULL)
		SDL_Log("IMG_Load failed: %s", SDL_GetError());
	//m_data = SDL_CreateTextureFromSurface(ren, img.GetData());
}

SDL_Texture * Image::GetData(void)const
{
	return m_data;
}