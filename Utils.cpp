#include "utils.h"

//-----------------------------------------------------------------------------
int random_int(int min, int max)
{
	int range = max - min + 1;
	return (rand() % range) + min;
}
float random_between(const float & min, const float & max)
{
	return (max - min) * ((float)rand() / (float)RAND_MAX) + min;
}
double random_between(const double & min, const double & max)
{
	return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

long sq_distance(const Tpoint<int> & a, const Tpoint<int> & b)
{
	return (long)((a.m_x - b.m_x) * (long)(a.m_x - b.m_x))
		+ (long)((a.m_y - b.m_y) * (long)(a.m_y - b.m_y));
}
long long sq_distance(const Tpoint <long> & a, const Tpoint<long> & b)
{
	return (long)((a.m_x - b.m_x) * (long)(a.m_x - b.m_x))
		+ (long)((a.m_y - b.m_y) * (long)(a.m_y - b.m_y));
}
double sq_distance(const Tpoint<float> & a, const Tpoint<float> & b)
{
	return (double)((a.m_x - b.m_x) * (double)(a.m_x - b.m_x))
		+ (double)((a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}
double sq_distance(const Tpoint<double> & a, const Tpoint<double> & b)
{
	return (double)((a.m_x - b.m_x) * (double)(a.m_x - b.m_x))
		+ (double)((a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool equal(double a, double b)
{
	double epsilon = 1e-5;
	return (std::abs(a - b) < epsilon) ? true : false;
}
vector<double> rgb_to_hsv(const Color & c)
{
	double r = c.GetR();
	double g = c.GetG();
	double b = c.GetB();

	double h, s, v = std::max({ r,g,b });

	double diff = v - std::min({ r,g,b });

	if (equal(diff, 0))
		h = s = 0;
	else
	{
		double rr = (v - r) / 6 / diff + 1 / 2.0;
		double gg = (v - g) / 6 / diff + 1 / 2.0;
		double bb = (v - b) / 6 / diff + 1 / 2.0;

		s = diff / v;
		if (equal(r, v))
		{
			h = bb - gg;
		}
		else if (equal(g, v))
		{
			h = (1 / 3.0) + rr - bb;
		}
		else if (equal(b, v))
		{
			h = (2 / 3.0) + gg - rr;
		}
		if (h < 0)
		{
			h += 1.0;
		}
		else if (h > 1)
		{
			h -= 1.0;
		}
	}
	//if (v * 100.0 >= 100)
	v = 1.0;

	return vector<double>({ std::round(h * 360), std::round(s * 100), std::round(v * 100) });
}

Color hsv_to_rgb(const vector<double> & hsv)
{
	double r, g, b;

	double h = std::round(hsv[0]);
	double s = std::round(hsv[1] * 255 / 100);
	double v = std::round(hsv[2] * 255 / 100);
	if (equal(s, 0))
	{
		r = v;
		g = v;
		b = v;
	}
	else
	{
		double t1 = v;
		double t2 = (255 - s) * v / 255.0;
		double t3 = (t1 - t2) * ((int)h % 60) / 60.0;
		if (equal(h, 360)) h = 0.0;
		if (h < 60) { r = t1; b = t2; g = t2 + t3; }
		else if (h < 120) { g = t1; b = t2; r = t1 - t3; }
		else if (h < 180) { g = t1; r = t2; b = t2 + t3; }
		else if (h < 240) { b = t1; r = t2; g = t1 - t3; }
		else if (h < 300) { b = t1; g = t2; r = t2 + t3; }
		else if (h < 360) { r = t1; g = t2; b = t1 - t3; }
		else { r = g = b = 0; }
	}
	return Colors::MakeRGB((unsigned int)std::round(r), (unsigned int)std::round(g), (unsigned int)std::round(b));
}

Color interpolation(const Color & a, const Color & b, double factor)
{
	vector<double> hsvA = rgb_to_hsv(a);
	vector<double> hsvB = rgb_to_hsv(b);
	vector<double> hsvcomb =
	{
		(hsvA[0] * (1 - factor) + hsvB[0] * factor),
		(hsvA[1] * (1 - factor) + hsvB[1] * factor),
		(hsvA[2] * (1 - factor) + hsvB[2] * factor)
	};
	return hsv_to_rgb(hsvcomb);
}

vector<Color> generate_colors(const Color & start, const Color & end, int number_of_colors)
{
	vector<Color> res;
	for (int i = 0; i < number_of_colors; i++)
	{
		Color rgb = interpolation(start, end, (double)i / (std::max(number_of_colors, 2) - 1));
		res.push_back(rgb);
	}
	return res;
}
//-----------------------------------------------------------------------------