#pragma once

class Color
{
public:
	uint32_t dword= 0;
public:
	constexpr Color() : dword(0) {}
	constexpr Color(const Color& col)
		:
		dword(col.dword)
	{}
	constexpr Color(uint32_t dw = 0)
		:
		dword(dw)
	{}
	constexpr Color(uint8_t x, uint8_t r, uint8_t g, uint8_t b)
		:
		dword((x << 24u) | (r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(uint8_t r, uint8_t g, uint8_t b)
		:
		dword((r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(Color col, uint8_t x)
		:
		Color((x << 24u) | col.dword)
	{}
	Color& operator =(Color color)
	{
		dword = color.dword;
		return *this;
	}
	constexpr uint8_t GetX() const
	{
		return dword & 0xFFu;
	}
	constexpr uint8_t GetA() const
	{
		return GetX();
	}
	constexpr uint8_t GetR() const
	{
		return (dword >> 24u) & 0xFFu;
	}
	constexpr uint8_t GetG() const
	{
		return (dword >> 16u) & 0xFFu;
	}
	constexpr uint8_t GetB() const
	{
		return (dword >> 8u) & 0xFFu;
	}
	void SetR(uint8_t r)
	{
		dword = (dword & 0xFFFFFFu) | (r << 24u);
	}
	void SetA(uint8_t a)
	{
		SetX(a);
	}
	void SetG(uint8_t g)
	{
		dword = (dword & 0xFF00FFFFu) | (g << 16u);
	}
	void SetB(uint8_t b)
	{
		dword = (dword & 0xFFFF00FFu) | (b << 8u);
	}
	void SetX(uint8_t x)
	{
		dword = (dword & 0xFFFFFF00u) | x;
	}


	operator uint32_t() const
	{
		return dword;
	}
};

namespace Colors
{
	inline static constexpr Color MakeRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
	{
		return (r << 24) | (g << 16) | (b << 8) | a;
	}

	inline static constexpr Color MakeRGB(uint8_t r, uint8_t g, uint8_t b) noexcept
	{
		return MakeRGBA(r, g, b, (uint8_t)0xff);
	}

	static constexpr Color White = MakeRGB(255u, 255u, 255u);
	static constexpr Color Black = MakeRGB(0u, 0u, 0u);
	static constexpr Color Gray = MakeRGB(0x80u, 0x80u, 0x80u);
	static constexpr Color LightGray = MakeRGB(0xD3u, 0xD3u, 0xD3u);
	static constexpr Color Red = MakeRGB(255u, 0u, 0u);
	static constexpr Color Green = MakeRGB(0u, 255u, 0u);
	static constexpr Color Blue = MakeRGB(0u, 0u, 255u);
	static constexpr Color Yellow = MakeRGB(255u, 255u, 0u);
	static constexpr Color Cyan = MakeRGB(0u, 255u, 255u);
	static constexpr Color Magenta = MakeRGB(255u, 0u, 255u);
}
