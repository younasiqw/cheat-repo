#pragma once

#include "../interfaces/interfaces.h"
#include <string>
struct color2 {
	inline bool operator!=(const color2& src) const {
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.r != r) || (src.g != g) || (src.b != b);
	}

	unsigned char r, g, b, a;
};
class color {
public:

	color( ) = default;

	color(int red, int green, int blue, int alpha = 255) {
		r = std::clamp<int>(red, 0, 255);
		g = std::clamp<int>(green, 0, 255);
		b = std::clamp<int>(blue, 0, 255);
		a = std::clamp<int>(alpha, 0, 255);
	}

	color(color color, int alpha) {
		r = std::clamp<int>(color.r, 0, 255);
		g = std::clamp<int>(color.g, 0, 255);
		b = std::clamp<int>(color.b, 0, 255);
		a = std::clamp<int>(alpha, 0, 255);
	}
	color(color2 color) {
		r = std::clamp<int>(color.r, 0, 255);
		g = std::clamp<int>(color.g, 0, 255);
		b = std::clamp<int>(color.b, 0, 255);
		a = std::clamp<int>(color.a, 0, 255);
	}

	inline bool operator!=(const color& src) const {
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.r != r) || (src.g != g) || (src.b != b);
	}

	static color hsb_to_rgb(float hue, float saturation, float brightness, int alpha = 255) {

		hue = std::clamp(hue, 0.f, 1.f);
		saturation = std::clamp(saturation, 0.f, 1.f);
		brightness = std::clamp(brightness, 0.f, 1.f);

		float h = (hue == 1.f) ? 0.f : (hue * 6.f);
		float f = h - static_cast<int>(h);
		float p = brightness * (1.f - saturation);
		float q = brightness * (1.f - saturation * f);
		float t = brightness * (1.f - (saturation * (1.f - f)));

		if (h < 1)
			return color(brightness * 255.f, t * 255.f, p * 255.f, alpha);

		else if (h < 2)
			return color(q * 255.f, brightness * 255.f, p * 255.f, alpha);

		else if (h < 3)
			return color(p * 255.f, brightness * 255.f, t * 255.f, alpha);

		else if (h < 4)
			return color(p * 255.f, q * 255.f, brightness * 255.f, alpha);

		else if (h < 5)
			return color(t * 255.f, p * 255.f, brightness * 255.f, alpha);

		else
			return color(brightness * 255.f, p * 255.f, q * 255.f, alpha);

	}

	static float get_hue(color color) {

		float r = (std::clamp<int>(color.r, 0, 255) / 255.f);
		float g = (std::clamp<int>(color.g, 0, 255) / 255.f);
		float b = (std::clamp<int>(color.b, 0, 255) / 255.f);
		float max = std::fmaxf(std::fmaxf(r, g), b);
		float min = std::fminf(std::fminf(r, g), b);
		float delta = max - min;

		if (delta != 0) {

			float hue;

			if (r == max)
				hue = (g - b) / delta;

			else {

				if (g == max)
					hue = 2 + (b - r) / delta;
				else
					hue = 4 + (r - g) / delta;
			}

			hue *= 60;

			if (hue < 0)
				hue += 360;

			return hue / 360;
		}

		else
			return 0;
	}

	static color interpolate(color color1, color color2, float ttt) {

		float r = (std::clamp<int>(color1.r, 0, 255) * (1.f - ttt) + std::clamp<int>(color2.r, 0, 255) * ttt);
		float g = (std::clamp<int>(color1.g, 0, 255) * (1.f - ttt) + std::clamp<int>(color2.g, 0, 255) * ttt);
		float b = (std::clamp<int>(color1.b, 0, 255) * (1.f - ttt) + std::clamp<int>(color2.b, 0, 255) * ttt);
		float a = (std::clamp<int>(color1.a, 0, 255) * (1.f - ttt) + std::clamp<int>(color2.a, 0, 255) * ttt);

		return color(r, g, b, a);
	}
	constexpr color& from_hsv(float h, float s, float v) {
		float colOut[3]{ };
		if (s == 0.0f)
		{
			r = g = b = static_cast<int>(v * 255);
			return *this;
		}

		h = std::fmodf(h, 1.0f) / (60.0f / 360.0f);
		int   i = static_cast<int>(h);
		float f = h - static_cast<float>(i);
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i)
		{
		case 0:
			colOut[0] = v;
			colOut[1] = t;
			colOut[2] = p;
			break;
		case 1:
			colOut[0] = q;
			colOut[1] = v;
			colOut[2] = p;
			break;
		case 2:
			colOut[0] = p;
			colOut[1] = v;
			colOut[2] = t;
			break;
		case 3:
			colOut[0] = p;
			colOut[1] = q;
			colOut[2] = v;
			break;
		case 4:
			colOut[0] = t;
			colOut[1] = p;
			colOut[2] = v;
			break;
		case 5: default:
			colOut[0] = v;
			colOut[1] = p;
			colOut[2] = q;
			break;
		}

		r = static_cast<int>(colOut[0] * 255);
		g = static_cast<int>(colOut[1] * 255);
		b = static_cast<int>(colOut[2] * 255);
		return *this;
	}

	unsigned char r, g, b, a;
};

namespace Render {
	namespace Fonts {
		extern HFont main;
		extern HFont menu;
		extern HFont visuals;
		extern HFont tab;
		extern HFont title;
		extern HFont slider;
		extern HFont preview;
		extern HFont WeaponIcon;
		extern HFont WeaponConfig;
		extern HFont WeaponText;

		bool initialize(void);
	}

	extern int screen_w, screen_h;

	void FilledRect(const int x, const int y, const int w, const int h, color col);
	void OutlinedRect(const int x, const int y, const int w, const int h, color col);
	void line(const int x, const int y, const int x2, const int y2, color col);
	void Text(const HFont& font, const std::string Text, const int x, const int y, const bool centered, const int r, const int g, const int b, const int a = 255);
	void draw_circle(int x, int y, int r, int s, int red, int green, int blue);
	void faded_rect(const int x, const int y, const int w, const int h, color top, color bottom, bool horizontal);
	void RoundedRect(int x, int y, int width, int height, float radius, color col);
	void PulseRect( int x, int y, int w, int h, int FadeWidth, color color );
	void draw_circle_3d( Vector3D position );
	bool initialize(void);
}