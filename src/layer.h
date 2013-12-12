#ifndef LAYER_H
#define LAYER_H
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

template <class T>
class Layer {
    public:
	Layer(){};

	Layer(std::string file, T r_, T g_, T b_){
		r = r_; g = g_; b = b_;
		read_file(file);
	};

	T z(int x, int y) const{
		unsigned int idx = y*dimx+x;
		assert (idx < dimx*dimy);
		return zs[idx];
	};

	void read_file(std::string file){
		dimx = 0; dimy = 0;
		std::string line;
		std::ifstream infile(file.c_str());

		std::getline(infile, line);
		std::istringstream first_line(line);
		first_line >> x;
		first_line >> y;
		first_line >> dx;
		first_line >> dy;

		while (std::getline(infile, line)){
			T n;
			std::istringstream iss(line);

			while (iss >> n)
				zs.push_back(n);

			if (dimx == 0)
				dimx = zs.size();
			dimy++;
		}
	}

    //Layer();
    //Layer(std::string file, T r, T g, T b);
	//~Layer();

    //T z(int x, int y) const;
    //void read_file(std::string file);

	T x, y;
	T dx, dy;
	unsigned int dimx, dimy;
	T r, g, b;

    private:
    std::vector<T> zs;
};
#endif // LAYER_H
