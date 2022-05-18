#ifndef __UTIL_VALUE__
#define __UTIL_VALUE__

#include <define.h>

#include <functional>

namespace simple3deditor {

// 我发现lambda表达式若是直接捕获成员，则是通过this引用访问
// 如果想拷贝其值，新建一个局部变量用作副本也许可行
template <typename T>
class Value {
private:
    std::function<T()> func;

public:
    Value<T>() : func([]{ return T(); }) {}
    Value<T>(T&& x) : func([=]{ return x; }) {}
    Value<T>(std::function<T()>&& f) : func(f) {}
    Value<T>(Value<T>&& v) : func(v.func) {}
    Value<T>(const T& x) : func([=]{ return x; }) {}
    Value<T>(const std::function<T()>& f) : func(f) {}
    Value<T>(const Value<T>& v) : func(v.func) {}
    Value<T>& operator=(T&& x){ func = [=]{ return x; }; return *this; }
    Value<T>& operator=(std::function<T()>&& f){ func = std::move(f); return *this; }
    Value<T>& operator=(Value<T>&& v){ func = std::move(v.func); return *this; }
    Value<T>& operator=(const T& x){ func = [=]{ return x; }; return *this; }
    Value<T>& operator=(const std::function<T()>& f){ func = std::move(f); return *this; }
    Value<T>& operator=(const Value<T>& v){ func = std::move(v.func); return *this; }
    ~Value<T>(){}

    template <typename U>
    auto operator+(Value<U>&& x) -> Value<decltype(func() + x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() + x())>([=]{ return f() + x.func(); });
    }

    template <typename U>
    auto operator+(U&& x) -> Value<decltype(func() + x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() + x)>([=]{ return f() + x; });
    }

    template <typename U>
    auto operator-(Value<U>&& x) -> Value<decltype(func() - x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() - x())>([=]{ return f() - x.func(); });
    }

    template <typename U>
    auto operator-(U&& x) -> Value<decltype(func() - x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() - x)>([=]{ return f() - x; });
    }

    template <typename U>
    auto operator*(Value<U>&& x) -> Value<decltype(func() * x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() * x())>([=]{ return f() * x.func(); });
    }

    template <typename U>
    auto operator*(U&& x) -> Value<decltype(func() * x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() * x)>([=]{ return f() * x; });
    }

    template <typename U>
    auto operator/(Value<U>&& x) -> Value<decltype(func() / x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() / x())>([=]{ return f() / x.func(); });
    }

    template <typename U>
    auto operator/(U&& x) -> Value<decltype(func() / x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() / x)>([=]{ return f() / x; });
    }

    template <typename U>
    auto operator%(Value<U>&& x) -> Value<decltype(func() % x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() % x())>([=]{ return f() % x.func(); });
    }

    template <typename U>
    auto operator%(U&& x) -> Value<decltype(func() % x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() % x)>([=]{ return f() % x; });
    }

    template <typename U>
    auto operator==(Value<U>&& x) -> Value<decltype(func() == x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() == x())>([=]{ return f() == x.func(); });
    }

    template <typename U>
    auto operator==(U&& x) -> Value<decltype(func() == x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() == x)>([=]{ return f() == x; });
    }

    template <typename U>
    auto operator!=(Value<U>&& x) -> Value<decltype(func() != x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() != x())>([=]{ return f() != x.func(); });
    }

    template <typename U>
    auto operator!=(U&& x) -> Value<decltype(func() != x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() != x)>([=]{ return f() != x; });
    }

    template <typename U>
    auto operator>(Value<U>&& x) -> Value<decltype(func() > x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() > x())>([=]{ return f() > x.func(); });
    }

    template <typename U>
    auto operator>(U&& x) -> Value<decltype(func() > x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() > x)>([=]{ return f() > x; });
    }

    template <typename U>
    auto operator<(Value<U>&& x) -> Value<decltype(func() < x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() < x())>([=]{ return f() < x.func(); });
    }

    template <typename U>
    auto operator<(U&& x) -> Value<decltype(func() < x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() < x)>([=]{ return f() < x; });
    }

    template <typename U>
    auto operator>=(Value<U>&& x) -> Value<decltype(func() >= x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() >= x())>([=]{ return f() >= x.func(); });
    }

    template <typename U>
    auto operator>=(U&& x) -> Value<decltype(func() >= x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() >= x)>([=]{ return f() >= x; });
    }

    template <typename U>
    auto operator<=(Value<U>&& x) -> Value<decltype(func() <= x())> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() <= x())>([=]{ return f() <= x.func(); });
    }

    template <typename U>
    auto operator<=(U&& x) -> Value<decltype(func() <= x)> const {
        std::function<T()> f = this->func;
        return Value<decltype(func() <= x)>([=]{ return f() <= x; });
    }

    template <typename U>
    Value<T>& operator+=(U&& x){
        std::function<T()> f = this->func;
        func = [=]{ return f() + x; };
        return *this;
    }

    template <typename U>
    Value<T>& operator-=(U&& x){
        std::function<T()> f = this->func;
        func = [=]{ return f() - x; };
        return *this;
    }

    template <typename U>
    Value<T>& operator*=(U&& x){
        std::function<T()> f = this->func;
        func = [=]{ return f() * x; };
        return *this;
    }

    template <typename U>
    Value<T>& operator/=(U&& x){
        std::function<T()> f = this->func;
        func = [=]{ return f() / x; };
        return *this;
    }

    template <typename U>
    Value<T>& operator%=(U&& x){
        std::function<T()> f = this->func;
        func = [=]{ return f() % x; };
        return *this;
    }

    T operator()() const{ return func(); }
    T Get() const{ return func(); }
};

}

#endif