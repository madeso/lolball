#ifndef RESOLUTION_HPP
#define RESOLUTION_HPP

struct Resolution {
	Resolution(int pWidth, int pHeight) : width(pWidth), height(pHeight) {
	}

	bool operator==(const Resolution& other) const {
		return width == other.width && height == other.height;
	}
	int width;
	int height;
};

#endif