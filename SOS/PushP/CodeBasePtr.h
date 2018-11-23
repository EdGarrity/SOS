#pragma once
namespace Push
{
	namespace detail
	{

		template <class T> class ExecBase_ptr;

		template <class T>
		class CodeBase_prt
		{
		private:
			T* ptr;

			friend class ExecBase_ptr<T>;

		public:

			CodeBase_prt() : ptr(0) {}
			~CodeBase_prt() {}
			CodeBase_prt(const CodeBase_prt<T> & o) : ptr(o.ptr) {}
			CodeBase_prt(T* p) : ptr(p) {}
			explicit CodeBase_prt(const ExecBase_ptr<T> & w) : ptr(w.ptr) {}

			CodeBase_prt<T> & operator=(const CodeBase_prt<T> & o)
			{
				if (ptr == o.ptr) 
					return *this;

				ptr = o.ptr;
				return *this;
			}

			T* get()
			{
				return ptr;
			}

			T* operator->()
			{
				return ptr;
			}

			T & operator*()
			{
				return *ptr;
			}

			const T* get() const
			{
				return ptr;
			}
			
			const T* operator->() const
			{
				return ptr;
			}
			
			const T & operator*() const
			{
				return *ptr;
			}

			bool operator==(const CodeBase_prt<T> & o) const
			{
				return ptr == o.ptr;
			}
			
			bool operator!=(const CodeBase_prt<T> & o) const
			{
				return ptr != o.ptr;
			}
			
			bool operator<(const CodeBase_prt<T> & o) const
			{
				return ptr < o.ptr;
			}
		};

		template <class T>
		class ExecBase_ptr
		{
			T* ptr;

			friend class CodeBase_prt<T>;

		public:

			ExecBase_ptr() : ptr(0) {} 
			~ExecBase_ptr() {}
			ExecBase_ptr(const ExecBase_ptr<T> & o) : ptr(o.ptr) {} 
			ExecBase_ptr(T* p) : ptr(p) {} 

			explicit ExecBase_ptr(const CodeBase_prt<T> & s) : ptr(s.ptr) {} 

			CodeBase_prt<T> to_CodeBase() const
			{
				return CodeBase_prt<T>(*this);
			}

			ExecBase_ptr<T> & operator=(const ExecBase_ptr<T> & o) 
			{
				if (ptr == o.ptr)
					return *this;

				ptr = o.ptr;
				return *this;
			}

			T* get()
			{
				return ptr;
			}

			T* operator->()
			{
				return ptr;
			}

			T & operator*()
			{
				return *ptr;
			}

			const T* get() const
			{
				return ptr;
			}

			const T* operator->() const
			{
				return ptr;
			}

			const T & operator*() const
			{
				return *ptr;
			}

			bool operator==(const ExecBase_ptr<T> & o) const
			{
				return ptr == o.ptr;
			}

			bool operator!=(const ExecBase_ptr<T> & o) const
			{
				return ptr != o.ptr;
			}

			bool operator<(const ExecBase_ptr<T> & o) const
			{
				return ptr < o.ptr;
			}
		};
	} // namespace detail
} // namespace push
