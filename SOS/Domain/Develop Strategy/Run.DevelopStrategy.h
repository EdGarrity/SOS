#pragma once

namespace domain
{
	namespace develop_strategy
	{
		class ThreeDimensionalArray
		{
			std::valarray<double> data;
			size_t x, y, z;

		public:
			class ThreeDimensionalArray(size_t x, size_t y, size_t z) : data(x * y * z), x(x), y(y), z(z) {}
			double& operator()(size_t i, size_t j, size_t k) { return data[i + x * (j + y * k)]; }
			double operator()(size_t i, size_t j, size_t k) const { return data[i + x * (j + y * k)]; }
		};


		int run();
	}
}
