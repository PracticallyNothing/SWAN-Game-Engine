#ifndef UTIL_EITHER_HPP
#define UTIL_EITHER_HPP

#include <utility>  // For std::move

namespace Util {

/// Lousy implementation of std::variant<T1, T2>
template <typename T1, typename T2>
class Either {
   public:
    typedef T1 Type1;
    typedef T2 Type2;

    Either(T1 t1) : _hasFirst(true) { first = t1; }
    Either(T2 t2) : _hasFirst(false) { second = t2; }

    Either(const Either<T1, T2>& other) : _hasFirst(other._hasFirst) {
        if (_hasFirst) {
            first = other.first;
        } else {
            second = other.second;
        }
    }

    Either(Either<T1, T2>&& other) : _hasFirst(std::move(other._hasFirst)) {
        if (_hasFirst) {
            first = std::move(other.first);
        } else {
            second = std::move(other.second);
        }
    }

    Either<T1, T2>& operator=(Either<T1, T2>&& other) {
        _hasFirst = std::move(other._hasFirst);

        if (_hasFirst) {
            this->first = std::move(other.first);
        } else {
            this->second = std::move(other.second);
        }
        return *this;
    }

    T1& getFirst() { return first; }
    T2& getSecond() { return second; }
    inline bool hasFirst() { return _hasFirst; }

   private:
    bool _hasFirst;
    union {
        T1 first;
        T2 second;
    };
};
}

#endif
