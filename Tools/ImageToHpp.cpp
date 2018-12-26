#define SDL_main_h_

#include <iomanip>
#include <fstream>
#include <algorithm>

#include "SWAN/Core/Display.hpp"
#include "SWAN/Core/Logging.hpp"
#include "SWAN/Rendering/Image.hpp"
#include "SWAN/Rendering/Texture.hpp"
#include "SWAN/Utility/StringUtil.hpp"

using namespace std;
using namespace string_literals;

using namespace SWAN;

int main(int argc, char** argv)
{
    if(argc < 2)
	Log("Must have at least one file to convert.",
		  LogLevel::Fatal);
	
    Display::Init(320, 480, "Converting texture...");
    for(int i = 1; i < argc; i++) {
	string filename = Util::GetFilename(argv[i], true);
	Log("Converting file \""s + filename + "\"...", LogLevel::Info);

	string argName = filename;
	replace(argName.begin(), argName.end(), '.', '_');

	Image img = Image(argv[i]);
	ofstream file; file.open(filename + ".cpp");
	file << "#include <array>\n"
	     << "#include <cstdint>\n"
	     << '\n'
	     << "const unsigned int Width = " << (unsigned int) img.width << ";\n"
	     << "const unsigned int Height = " << (unsigned int) img.height << ";\n"
	     << '\n'
	     << "const std::array<uint8_t, " << img.width * img.height * 4 << "> " << argName << " {\n"
	     << "// R,G,B,A\n"
	    ;

	for(int i = 0; i < img.width * img.height; i+=4)
	    file << (int) img.data[i*4+ 0] << ","
		 << (int) img.data[i*4+ 1] << ","
		 << (int) img.data[i*4+ 2] << ","
		 << (int) img.data[i*4+ 3] << ","
		 << (int) img.data[i*4+ 4] << ","
		 << (int) img.data[i*4+ 5] << ","
		 << (int) img.data[i*4+ 6] << ","
		 << (int) img.data[i*4+ 7] << ","
		 << (int) img.data[i*4+ 8] << ","
		 << (int) img.data[i*4+ 9] << ","
		 << (int) img.data[i*4+10] << ","
		 << (int) img.data[i*4+11] << ","
		 << (int) img.data[i*4+12] << ","
		 << (int) img.data[i*4+13] << ","
		 << (int) img.data[i*4+14] << ","
		 << (int) img.data[i*4+15]
		 << ",\n";

	file << "};\n";
    }

    Display::Close();
}
