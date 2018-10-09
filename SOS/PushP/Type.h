#pragma once

#include <vector>
#include <algorithm>

namespace push
{
	class Type;
	class Env;
	std::ostream &operator<<(std::ostream &os, const Type &tp);

	class Type
	{
		std::vector<int> type;
		unsigned _start;

	public:

		Type(size_t len = 0) : type(len)
		{
		}
		Type(size_t len, size_t which, int value = 1) : type(len)
		{
			type[which] = value;
		}

		typedef std::vector<int> TypeVec;

		const TypeVec &get() const
		{
			return type;
		}

		Type &operator+=(const Type &org)
		{
			Type other = make_equal_length(org);

			for (unsigned i = 0; i < type.size(); ++i)
				type[i] += other.type[i];

			shorten();
			return *this;
		}

		Type &operator-=(const Type &org)
		{
			Type other = make_equal_length(org);

			for (unsigned i = 0; i < type.size(); ++i)
				type[i] -= other.type[i];

			shorten();
			return *this;
		}

		bool operator==(const Type &other) const;
		bool can_pop_from(/*const Env &env*/) const;

		int operator[](unsigned i) const
		{
			return i < type.size() ? type[i] : 0;
		}

		void abs();

		Type &operator*=(int val)
		{
			for (unsigned i = 0; i < type.size(); ++i)
				type[i] *= val;

			return *this;
		}

		bool is_positive() const
		{
			for (unsigned i = 0; i < type.size(); ++i)
			{
				if (type[i] < 0) return false;
			}

			return true;
		}

	private:
		Type make_equal_length(const Type &org)
		{
			if (type.size() == org.type.size()) return org;

			//            size_t len = std::max( type.size(), org.type.size() );
			size_t len = (type.size() > org.type.size()) ? (type.size()) : (org.type.size());
			type.resize(len, 0);

			if (org.type.size() != len)
			{
				Type org2 = org;
				org2.type.resize(len, 0);
				return org2;
			}

			return org;
		}

		void shorten()
		{
			while (type.size() && type.back() == 0) type.pop_back();

//			postc();
		}
		//void postc()
		//{
		//	for (_start = 0; _start < type.size(); ++_start)
		//	{
		//		if (type[_start] > 0) break;
		//	}
		//}
	};

	inline Type operator+(const Type &a, const Type &b)
	{
		return Type(a) += b;
	}
	inline Type operator-(const Type &a, const Type &b)
	{
		return Type(a) -= b;
	}
	inline Type abs(const Type &a)
	{
		Type retval(a);
		retval.abs();
		return retval;
	}

	inline Type operator*(const Type &a, int b)
	{
		return Type(a) *= b;
	}
	inline Type operator*(int b, const Type &a)
	{
		return Type(a) *= b;
	}
}