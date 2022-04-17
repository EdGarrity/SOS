#pragma once
//#define NOMINMAX
//#include <algorithm>
#include <vector>

namespace Plush
{

	class Type;
	class Environment;

	class Type
	{
		std::vector<size_t> type;
		unsigned _start;
	public:

		Type(size_t len = 0) : type(len)
		{
			postc();
		}
		Type(size_t len, size_t which, size_t value = 1) : type(len)
		{
			type[which] = value;
			postc();
		}
		Type(const Type& other) : type(other.type), _start(other._start)
		{		
			postc();
		}

		typedef std::vector<size_t> TypeVec;

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

		bool operator==(/*const*/ Type &other); // const;
		bool can_pop_from(/*const*/ Environment &env); // const;
		bool can_push_to(/*const*/ Environment &env); // const;

		size_t operator[](unsigned i) const
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

		Type make_equal_length(const Type &org);

	private:
		void shorten()
		{
			while (type.size() && type.back() == 0) type.pop_back();

			postc();
		}
		void postc()
		{
			for (_start = 0; _start < type.size(); ++_start)
			{
				if (type[_start] > 0) break;
			}
		}
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

	extern Type execType; //(EXEC_STACK+1,EXEC_STACK,1);
	extern Type integerType; //(INTEGER_STACK+1,INTEGER_STACK,1);
	extern Type codeType; //(CODE_STACK+1, CODE_STACK,1);
	extern Type boolType; //(BOOL_STACK+1, BOOL_STACK,1);
	extern Type floatType; //(FLOAT_STACK+1, FLOAT_STACK,1);
	extern Type nullType;

}
