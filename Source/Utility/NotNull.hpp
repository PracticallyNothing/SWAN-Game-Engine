#ifndef UTIL_NOT_NULL_HPP
#define UTIL_NOT_NULL_HPP

#include <stdexcept>  //For std::invalid_argument, std::logic_error

namespace Util {
    template <typename T>
    class NotNull;

    template <typename T>
    class NotNull<T*> {
        public:
            NotNull(T* ptr) {
                if (!ptr) throw std::invalid_argument("NotNull::NotNull() - Argument is NULL pointer!");
                m_ptr = ptr;
            }
            ~NotNull() {}

            T& get() {
                if (!m_ptr) throw std::logic_error("NotNull::get() - Dereference of NULL pointer!");
                return *m_ptr;
            }
            T& operator*() {
                if (!m_ptr) throw std::logic_error("NotNull::operator*() - Dereference of NULL pointer!");
                return *m_ptr;
            }
            T* operator->() {
                if (!m_ptr) throw std::logic_error("NotNull::operator->() - Return NULL pointer!");
                return m_ptr;
            }

            inline bool checkValidity() { return !m_ptr; }

        private:
            T* m_ptr;
    };
}

#endif
