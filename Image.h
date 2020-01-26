

#ifndef _IMAGE_H
#define	_IMAGE_H

#include <iostream>
#include <fstream>
#include "color.h"
#include <string>

using namespace std;

class ImageLoader
{
public:
	virtual Color** loadImage(string filename) = 0;
	virtual void saveImage(string filename, Color** img, int w, int h) = 0;

	static bool isWhitespace(int ch) 
	{
		return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
	}

	static void skipWhitespace(ifstream &file); 
	static void skipComments(ifstream &file); 
	static bool readWord(ifstream &file, string &word, bool skip_ws, bool skip_comments); 
	static bool readInt(ifstream &file, int &value, bool skip_ws, bool skip_comments); 

	int	width;
	int	height;
	string datatype;
	unsigned int maxValue;
};

class Image
{
public:
	Image();
	Image(const Image& copy);
	~Image();

	void loadImage(string filename);
	void saveImage(string filename);
	Image superimpose(const Image& ontop, Color mask);

	int getWidth() { return width; }
	int getHeight() { return height; }
	const Image &operator=		(const Image&);
	void resize(int w, int h); 
	void free(); 

protected:
	Color**			pixels;
	int				width;
	int				height;
	ImageLoader*	loader;

};

class P3Loader : public ImageLoader
{
public:
	P3Loader();

	virtual Color** loadImage(string filename);
	virtual void saveImage(string filename, Color** img, int w, int h);

	static P3Loader *getsemaphore();

private:
	static P3Loader *semaphore;
};

class P6Loader : public ImageLoader
{
public:
	P6Loader();

	virtual Color** loadImage(string filename);
	virtual void saveImage(string filename, Color** img, int w, int h);

	static P6Loader *getsemaphore();

private:
	int loadColorComponent(ifstream &file); 
	void saveColorComponent(ofstream &file, unsigned int value); 

private:
	static P6Loader *semaphore;
};


#endif