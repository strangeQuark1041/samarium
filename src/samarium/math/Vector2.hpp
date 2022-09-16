/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2022 Jai Bellare
 * See <https://opensource.org/licenses/MIT/> or LICENSE.md
 * Project homepage: https://github.com/strangeQuark1041/samarium
 */

#pragma once

#include <utility>

#include "samarium/core/concepts.hpp"
#include "samarium/core/types.hpp"

#include "Extents.hpp"
#include "math.hpp"

namespace sm
{

/**
 * Represents a canonical Euclidean vector, ie an arrow in space
 * @tparam T type of x and y, required to be integral or floating point
 *
 * @code
 * auto vec = Vector2{.x = 4.2, .y = -1.4};
 * @endcode
 */
template <typename T> struct Vector2_t
{
    using value_type = T;

    T x{};
    T y{};

    [[nodiscard]] constexpr auto length() const noexcept { return std::sqrt(x * x + y * y); }

    struct Polar
    {
        T length{};
        T angle{};
    };

    [[nodiscard]] constexpr auto length_sq() const noexcept { return x * x + y * y; }

    [[nodiscard]] constexpr auto angle() const noexcept { return std::atan2(y, x); }

    [[nodiscard]] constexpr auto slope() const noexcept { return y / x; }

    [[nodiscard]] static constexpr auto combine(T value)
    {
        return Vector2_t{.x = value, .y = value};
    }

    [[nodiscard]] static constexpr auto
    from_polar(Polar polar) noexcept requires concepts::FloatingPoint<T>
    {
        return Vector2_t<T>{polar.length * std::cos(polar.angle),
                            polar.length * std::sin(polar.angle)};
    }

    [[nodiscard]] constexpr auto to_polar() const noexcept
    {
        return Polar{this->length(), this->angle()};
    }

    constexpr auto normalize() noexcept
    {
        const auto length_ = this->length();
        x /= length_;
        y /= length_;
    }

    [[nodiscard]] constexpr auto normalized() const noexcept
    {
        auto vec = *this;
        vec.normalize();
        return vec;
    }

    constexpr auto set_length(f64 new_length) noexcept
    {
        const auto factor = new_length / this->length();
        x *= factor;
        y *= factor;
    }

    constexpr auto set_angle(f64 new_angle) noexcept
    {
        *this = from_polar({.length = this->length(), .angle = new_angle});
    }

    [[nodiscard]] constexpr auto with_length(f64 new_length) const noexcept
    {
        const auto factor = new_length / this->length();
        return Vector2_t<T>{x * factor, y * factor};
    }

    [[nodiscard]] constexpr auto with_angle(f64 new_angle) const noexcept
    {
        return from_polar({.length = this->length(), .angle = new_angle});
    }

    [[nodiscard]] constexpr auto operator-() const noexcept { return Vector2_t<T>{-x, -y}; }

    constexpr auto operator+=(const Vector2_t& rhs) noexcept
    {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    constexpr auto operator-=(const Vector2_t& rhs) noexcept
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    constexpr auto operator*=(const Vector2_t& rhs) noexcept
    {
        this->x *= rhs.x;
        this->y *= rhs.y;
        return *this;
    }

    constexpr auto operator*=(T rhs) noexcept
    {
        this->x *= rhs;
        this->y *= rhs;
        return *this;
    }

    constexpr auto operator/=(const Vector2_t& rhs) noexcept
    {
        this->x /= rhs.x;
        this->y /= rhs.y;
        return *this;
    }

    constexpr auto operator/=(T rhs) noexcept
    {
        this->x /= rhs;
        this->y /= rhs;
        return *this;
    }

    template <typename U> constexpr auto as() const noexcept
    {
        return Vector2_t<U>{static_cast<U>(this->x), static_cast<U>(this->y)};
    }

    [[nodiscard]] static constexpr auto dot(Vector2_t<T> p1, Vector2_t<T> p2) noexcept
    {
        return p1.x * p2.x + p1.y * p2.y;
    }

    [[nodiscard]] static constexpr auto cross(Vector2_t<T> p1, Vector2_t<T> p2) noexcept
    {
        return p1.x * p2.y - p1.y * p2.x;
    }

    [[nodiscard]] constexpr auto abs() const noexcept
    {
        return Vector2_t<T>{std::abs(x), std::abs(y)};
    }

    [[nodiscard]] static constexpr auto angle_between(Vector2_t<T> from, Vector2_t<T> to) noexcept
    {
        return to.angle() - from.angle();
    }

    constexpr void rotate(f64 amount) noexcept { *this = this->with_angle(this->angle() + amount); }

    [[nodiscard]] constexpr auto rotated_by(f64 amount) const noexcept
    {
        auto temp = *this;
        temp.rotate(amount);
        return temp;
    }

    constexpr void reflect(Vector2_t<T> vec) noexcept
    {
        this->rotate(2 * angle_between(*this, vec));
    }

    constexpr auto clamp_length(Extents<T> extents) noexcept
    {
        this->set_length(extents.clamp(this->length()));
    }

    template <typename U> [[nodiscard]] constexpr auto clamped_to(Vector2_t<U> box) const
    {
        return Vector2_t<T>{
            math::min(math::max(this->x, static_cast<T>(0)), static_cast<T>(box.x)),
            math::min(math::max(this->y, static_cast<T>(0)), static_cast<T>(box.y)),
        };
    }

    [[nodiscard]] constexpr auto is_zero() const noexcept
    {
        return math::almost_equal(this->length_sq(), 0.0);
    }

    [[nodiscard]] constexpr auto negated() const noexcept { return Vector2_t<T>{-x, -y}; }

    [[nodiscard]] constexpr auto operator<=>(const Vector2_t<T>&) const = default;
};

template <typename T>
[[nodiscard]] constexpr auto operator+(Vector2_t<T> lhs, const Vector2_t<T>& rhs) noexcept
{
    lhs += rhs;
    return lhs;
}

template <typename T>
[[nodiscard]] constexpr auto operator-(Vector2_t<T> lhs, const Vector2_t<T>& rhs) noexcept
{
    lhs -= rhs;
    return lhs;
}

template <typename T>
[[nodiscard]] constexpr auto operator*(Vector2_t<T> lhs, const Vector2_t<T>& rhs) noexcept
{
    lhs *= rhs;
    return lhs;
}

template <typename T> [[nodiscard]] constexpr auto operator*(Vector2_t<T> lhs, T rhs) noexcept
{
    lhs *= rhs;
    return lhs;
}

template <typename T> [[nodiscard]] constexpr auto operator*(T lhs, Vector2_t<T> rhs) noexcept
{
    rhs *= lhs;
    return rhs;
}

template <typename T>
[[nodiscard]] constexpr auto operator/(Vector2_t<T> lhs, const Vector2_t<T>& rhs) noexcept
{
    lhs /= rhs;
    return lhs;
}

template <typename T> [[nodiscard]] constexpr auto operator/(Vector2_t<T> lhs, T rhs) noexcept
{
    lhs /= rhs;
    return lhs;
}

template <std::size_t Index, typename T> auto&& Vector2_t_get(T&& p)
{
    static_assert(Index < 2, "Index out of bounds for sm::Vector2_t");
    if constexpr (Index == 0) { return std::forward<T>(p).x; }
    if constexpr (Index == 1) { return std::forward<T>(p).y; }
}

template <std::size_t Index, typename T> auto&& get(Vector2_t<T>& p)
{
    return Vector2_t_get<Index>(p);
}

template <std::size_t Index, typename T> auto&& get(Vector2_t<T> const& p)
{
    return Vector2_t_get<Index>(p);
}

template <std::size_t Index, typename T> auto&& get(Vector2_t<T>&& p)
{
    return Vector2_t_get<Index>(std::move(p));
}

template <std::size_t Index, typename T> auto&& get(Vector2_t<T> const&& p)
{
    return Vector2_t_get<Index>(std::move(p));
}

using Vector2    = Vector2_t<f64>;
using Vector2f   = Vector2_t<f32>;
using Indices    = Vector2_t<u64>;
using Dimensions = Vector2_t<u64>;

namespace literals
{
consteval auto operator"" _x(f80 x) noexcept { return Vector2{static_cast<f64>(x), 0.0}; }
consteval auto operator"" _y(f80 y) noexcept { return Vector2{0.0, static_cast<f64>(y)}; }
} // namespace literals
} // namespace sm

namespace std
{
template <typename T> struct tuple_size<sm::Vector2_t<T>>
{
    static constexpr size_t value = 2;
};

template <typename T> struct tuple_element<0, sm::Vector2_t<T>>
{
    using type = T;
};

template <typename T> struct tuple_element<1, sm::Vector2_t<T>>
{
    using type = T;
};
} // namespace std
