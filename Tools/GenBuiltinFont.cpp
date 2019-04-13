#include <fstream>
#include <string>

#include "SWAN/Rendering/Image.hpp"

using namespace SWAN;
using namespace std;

std::string PackImage(const Image& img)
{
	size_t bitIndex = 0;
	std::string res;

	auto PushBit = [&](int value) {
		if(bitIndex == 8) {
			res += '\0';
			bitIndex = 0;
		}
		res.back() |= (value ? 0x80 : 0) >> bitIndex;
		++bitIndex;
	};

	for(size_t y = 0; y < img.height; y++)
		for(size_t x = 0; x < img.width; x++)
			PushBit(img.pixelAt(x, y) == Color::Black ? 1 : 0);

	return res;
}

Image UnpackImage(const std::string& str)
{
	Image img = Image(210, 60);
	auto GetBit = [&](size_t pos) -> bool {
		return (str[pos / 8]) & (0x80 >> (pos % 8));
	};

	for(size_t y = 0; y < img.height; y++)
		for(size_t x = 0; x < img.width; x++)
			img.setPixelAt(x, y, GetBit(x + y * img.width) ? Color::Black : Color::White);
	return img;
}

const std::string UnpackAlgorithm = R"ddd(
SWAN::Image UnpackImage(const std::string& str)
{
	SWAN::Image img = SWAN::Image(210, 60);
	auto GetBit = [&](size_t pos) -> bool {
		return (str[pos / 8]) & (0x80 >> (pos % 8));
	};

	for(size_t y = 0; y < img.height; y++)
		for(size_t x = 0; x < img.width; x++)
			img.setPixelAt(x, y, GetBit(x + y * img.width) ? SWAN::Color::Black : SWAN::Color::White);
	return img;
}
)ddd";

int main()
{
	Image* img = new Image("builtin_font.png");
	std::string res = PackImage(*img);

	ofstream file;
	file.open("BuiltinFont.cpp");
	file << "#include <string>\n"
	        "#include \"SWAN/Rendering/Image.hpp\"\n\n";
	file << "std::string fontData = R\"builtin(" << res << ")builtin\";\n\n";
	file << UnpackAlgorithm;

	delete img;
	return 0;
}
