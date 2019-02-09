#ifndef SWAN_UTIL_PTROROBJ_HPP
#define SWAN_UTIL_PTROROBJ_HPP

#include <type_traits> // For std::remove_cv<T>
#include <utility>     // For std::move()

namespace SWAN
{
	namespace Util
	{
		template <typename T>
		class PtrOrObject
		{
			//using Type = typename std::remove_cv<T>::type;
			using Type = T;

		  public:
			PtrOrObject() : hasPtr(true), ptr(nullptr) {}
			PtrOrObject(Type* t) : hasPtr(true), ptr(t) {}
			PtrOrObject(const Type& t) : hasPtr(false), t(t) {}
			PtrOrObject(Type&& t) : hasPtr(false), t(std::move(t)) {}
			PtrOrObject(const PtrOrObject& p) : hasPtr(p.hasPtr)
			{
				if(hasPtr)
					ptr = p.ptr;
				else
					t = p.t;
			}
			PtrOrObject(PtrOrObject&& p) : hasPtr(std::move(p.hasPtr))
			{
				if(hasPtr)
					ptr = std::move(p.ptr);
				else
					t = std::move(p.t);
				p.ptr = nullptr;
			}
			~PtrOrObject() {}

			PtrOrObject& operator=(Type* t)
			{
				hasPtr = true;
				ptr = t;
				return *this;
			}
			PtrOrObject& operator=(const Type& t)
			{
				hasPtr = false;
				this->t = t;
			}
			PtrOrObject& operator=(Type&& t)
			{
				hasPtr = false;
				this->t = std::move(t);
			}

			PtrOrObject& operator=(const PtrOrObject& p)
			{
				hasPtr = p.hasPtr;
				if(hasPtr)
					ptr = p.ptr;
				else
					t = p.t;
				return *this;
			}
			PtrOrObject& operator=(PtrOrObject&& p)
			{
				hasPtr = std::move(p.hasPtr);
				if(hasPtr)
					ptr = std::move(p.ptr);
				else
					t = std::move(p.t);
				p.ptr = nullptr;
				return *this;
			}

			Type& operator*() { return (hasPtr ? *ptr : t); }
			Type* operator->() { return (hasPtr ? ptr : &t); }
			Type* operator&() { return (hasPtr ? ptr : &t); }
			Type& get() { return (hasPtr ? *ptr : t); }
			Type& getT() { return t; }
			Type* getPtr() { return ptr; }

			const Type& operator*() const { return (hasPtr ? *ptr : t); }
			const Type* operator->() const { return (hasPtr ? ptr : &t); }
			const Type* operator&() const { return (hasPtr ? ptr : &t); }
			const Type& get() const { return (hasPtr ? *ptr : t); }
			const Type& getT() const { return t; }
			const Type* getPtr() const { return ptr; }

			bool hasPointer() const { return hasPtr; }

		  private:
			union {
				Type t;
				Type* ptr;
			};
			bool hasPtr;
		};
	} // namespace Util
} // namespace SWAN

#endif
