#pragma once

#include <valarray>

namespace Utilities
{
	class ThreeDimensionalArray
	{
		std::valarray<double> data;
		size_t x, y, z;

	public:
		ThreeDimensionalArray() : data(1), x(1), y(1), z(1) {}
		ThreeDimensionalArray(size_t x, size_t y, size_t z) : data(x* y* z), x(x), y(y), z(z) {}
		void resize(size_t x, size_t y, size_t z) { data.resize(x * y * z, 0); this->x = x; this->y = y; this->z = z; }
		double& operator()(size_t i, size_t j, size_t k) { return data[i + x * (j + y * k)]; }
		double operator()(size_t i, size_t j, size_t k) const { return data[i + x * (j + y * k)]; }
	};
}
