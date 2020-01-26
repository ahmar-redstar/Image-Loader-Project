#include "Image.h"
#include <fstream>
#include <string>




void ImageLoader::skipWhitespace(ifstream &file)
{
	int ch;

	for (; isWhitespace(file.peek());)
		ch = file.get();
}

void ImageLoader::skipComments(ifstream &file)
{
	int ch = file.peek();

	if (ch == '#')
	{
		ch = file.get();

		for (;;)
		{
			ch = file.get();
			if (ch == '\n' || ch == '\r' || ch == char_traits<char>::eof())
				break;
		}

		skipWhitespace(file);
	}
}

bool ImageLoader::readWord(ifstream &file, string &word, bool skip_ws, bool skip_comments)
{
	string temp;
	
	for (;;) // ;; page 188 apparently breaks the statement kind of looks cleaner
	{
		int ch = file.peek();

		if (isWhitespace(ch) || ch == char_traits<char>::eof())
			break;
		else
		{
			ch = file.get();
			temp += ch;
		}
	}

	if (skip_ws)
		skipWhitespace(file);

	if (skip_comments)
		skipComments(file);

	if (temp.empty())
		return false;

	word = temp;
	return true;
}

bool ImageLoader::readInt(ifstream &file, int &value, bool skip_ws, bool skip_comments)
{
	string temp;

	value = 0;

	if (!readWord(file, temp, skip_ws, skip_comments))
		return false;

	if (::sscanf(temp.c_str(), "%d", &value) != 1)
		return false;

	return true;
}


P3Loader *P3Loader::semaphore = NULL;

P3Loader::P3Loader()
{
	semaphore = this;
}

P3Loader *P3Loader::getsemaphore()
{
	if (semaphore == NULL)
	{
		semaphore = new P3Loader();
		cout << "[Info] P3Loader :D DONE! \n";
	}

	return semaphore;
}

Color** P3Loader::loadImage(string filename)
{
	ifstream file(filename.c_str(), ios::in | ios::binary);
	int row, col;

	readWord(file, datatype, true, true);

	if (datatype != "P3")
		return NULL;

	if (!readInt(file, width, true, true))
		return NULL;

	if (!readInt(file, height, true, true))
		return NULL;

	if (!readInt(file, (int &)maxValue, false, false))
		return NULL;

	int ch = file.get();

	if (!isWhitespace(ch))
	{
		cout << "#Error: Need whitespace \n";
		return NULL;
	}

	Color **darray = new Color*[height];

	for (row = 0; row < height; row++)
	{
		darray[row] = new Color[width];
	}

	for (row = 0; row < height; row++)
	{
		for (col = 0; col < width; col++)
		{
			int r, g, b;

			readInt(file, r, true, false);
			readInt(file, g, true, false);
			readInt(file, b, true, false);

			darray[row][col].r = r;
			darray[row][col].g = g;
			darray[row][col].b = b;
		}
	}

	return darray;
}

void P3Loader::saveImage(string filename, Color** img, int w, int h)
{
	ofstream outFile(filename.c_str(), ios::out | ios::binary);

	outFile << "P3" << endl;
	outFile << "#Ahmar Cis17A" << endl; 
	outFile << w << " " << h << endl;
	outFile << maxValue << endl;

	for (int r = 0; r < height; ++r)
	{
		/*
		for (int c = 0; c < width; c++) //  c++ protected? find out why not working
		{
			outFile << img[r][c].r << " " << img[r][c].g << " " << img[r][c].b << endl;
		}*/
		for (int c = 0; c < width; ++c) 
		{
			outFile << img[r][c].r << " " << img[r][c].g << " " << img[r][c].b << endl;
		}
	}
}



P6Loader *P6Loader::semaphore = NULL;

P6Loader::P6Loader()
{
	semaphore = this;
}

P6Loader *P6Loader::getsemaphore()
{
	if (semaphore == NULL)
	{
		semaphore = new P6Loader();
		cout << "[Info] P6Loader semaphore please work \n";
	}

	return semaphore;
}

int P6Loader::loadColorComponent(ifstream &file)
{
	int result;

	if (maxValue <= 255)
	{
		result = file.get();
	}
	else if (maxValue <= 65535)
	{
		unsigned int b1 = file.get();
		unsigned int b2 = file.get();

		result = 256 * b1 + b2;
	}

	return result;
}

void P6Loader::saveColorComponent(ofstream &file, unsigned int value)
{
	if (maxValue <= 255)
	{
		file.put(value);
	}
	else if (maxValue <= 65535)
	{
		unsigned char c1 = value / 256;
		unsigned char c2 = value & 0xFF;

		file.put(c1);
		file.put(c2);
	}
}


Color** P6Loader::loadImage(string filename)
{
	ifstream file(filename.c_str(), ios::in | ios::binary);
	int row, col;

	readWord(file, datatype, true, true);

	if (datatype != "P6")
		return NULL;

	if (!readInt(file, width, true, true))
		return NULL;

	if (!readInt(file, height, true, true))
		return NULL;

	if (!readInt(file, (int &)maxValue, false, false))
		return NULL;

	int ch = file.get();

	if (!isWhitespace(ch))
	{
		cout << "#Error: Y U NO WHITESPACE \n";
		return NULL;
	}

	Color **darray = new Color*[height];

	for (row = 0; row < height; row++)
	{
		//darray[row] = new Color[with]; 
		darray[row] = new Color[width]; 	}

	for (row = 0; row < height; row++)
	{
		for (col = 0; col < width; col++)
		{
			darray[row][col].r = loadColorComponent(file);
			darray[row][col].g = loadColorComponent(file);
			darray[row][col].b = loadColorComponent(file);
		}
	}

	return darray;
}

void P6Loader::saveImage(string filename, Color** img, int w, int h)
{
	ofstream outFile(filename.c_str(), ios::out | ios::binary);

	outFile << "P6" << endl;
	outFile << "#Ahmar Cis17A" << endl;
	outFile << w << " " << h << endl;
	outFile << maxValue << endl;

	for (int r = 0; r < height; ++r)
	{
		for (int c = 0; c < width; ++c)
		{
			saveColorComponent(outFile, img[r][c].r);
			saveColorComponent(outFile, img[r][c].g);
			saveColorComponent(outFile, img[r][c].b);
		}
	}
}


Image::Image()
	: pixels(NULL)
	, width(0)
	, height(0)
	, loader(NULL)
{
}

Image::Image(const Image& copy)
	: pixels(NULL)
	, width(0)
	, height(0)
	, loader(NULL)
{
	loader = copy.loader;

	resize(copy.width, copy.height);



	for (int r = 0; r < height; r++) 
	{
		for (int cm = 0; cm < width; cm++)
		{
			pixels[r][cm] = copy.pixels[r][cm]; 
		}
	}
}

Image::~Image()
{
	free();
}

void Image::free()
{
	if (pixels != NULL)
	{
		for (int r = 0; r < height; r++)
		{
			delete[] pixels[r];
		}

		delete[] pixels;
		pixels = NULL;
		width = 0;
		height = 0;
	}
}

void Image::resize(int w, int h)
{
	free();

	width = w;
	height = h;

	pixels = new Color*[height];
	for (int r = 0; r < height; ++r)
		pixels[r] = new Color[width];
}

void Image::loadImage(string filename)
{
	ifstream file;
	string dataType;
	string comment;

	file.open(filename.c_str(), ios::in);

	if (!file)
	{
		cout << "#Error: Opening the file\n";
		exit(1);
	}

	if (!ImageLoader::readWord(file, dataType, false, false))
	{
		file.close();
		cout << "#Error: Reading the magic \n"; 

		exit(1);
	}

	file.close();
	loader = NULL;

	if (dataType == "P6")
	{
		loader = P6Loader::getsemaphore();
	}
	else if (dataType == "P3")
	{
		loader = P3Loader::getsemaphore();
	}

	if (loader == NULL)
	{
		cout << "#Error: Could not find a loader\n";
		exit(1);
	}

	free();

	pixels = loader->loadImage(filename);

	if (pixels != NULL)
	{
		width = loader->width;
		height = loader->height;

		//cout << filename.c_str() << "\n";
		

		cout << "[Info] Loaded a " << width << "x" << height <<
			" image from file " << filename.c_str() << "\n";
	}
}

void Image::saveImage(string filename)
{
	if (loader != NULL && pixels != NULL)
	{
		loader->saveImage(filename, pixels, width, height);

		cout << "[Info] YOUR DONE! :D FINALLY!!!!  " << filename.c_str() << "\n";
	}
}


Image Image::superimpose(const Image& ontop, Color mask)
{
	Image temp = *this;

	if (temp.width != ontop.width || temp.height != ontop.height)
	{
		cout << "#Error: Trying to superimpose size\n";
		return temp;
	}

	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			Color ontop_color = ontop.pixels[r][c];

			if (!(ontop_color == mask))
				temp.pixels[r][c] = ontop.pixels[r][c];
		}
	}

	cout << "[Info] Superimpose complete.\n"; 

	return temp;
}


const Image &Image::operator= (const Image& copy)
{
	if (this == &copy)
		return *this;

	loader = copy.loader;

	resize(copy.width, copy.height);

	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			pixels[r][c] = copy.pixels[r][c];
		}
	}

	return *this;
}
