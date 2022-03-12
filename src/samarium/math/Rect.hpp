/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2022 Jai Bellare
 * See <https://opensource.org/licenses/MIT/> or LICENSE.md
 * Project homepage: https://github.com/strangeQuark1041/samarium
 */

#pragma once

#include "Extents.hpp"
#include "Vector2.hpp"

namespace sm
{
template <concepts::Number T = f64> struct Rect
{
    Vector2_t<T> min;
    Vector2_t<T> max;

    template <concepts::Number U> [[nodiscard]] constexpr auto as() const
    {
        // very weird: https://stackoverflow.com/a/3505738/17100530
        return Rect<U>{min.template as<U>(), max.template as<U>()};
    }

    [[nodiscard]] static constexpr auto find_min_max(Vector2_t<T> p1, Vector2_t<T> p2)
    {
        return Rect<T>{{math::min(p1.x, p2.x), math::min(p1.y, p2.y)},
                       {math::max(p1.x, p2.x), math::max(p1.y, p2.y)}};
    }

    constexpr auto validate() { *this = find_min_max(this->min, this->max); }

    [[nodiscard]] constexpr auto validated() const
    {
        auto rect = *this;
        rect.validate();
        return rect;
    }

    [[nodiscard]] static constexpr auto
    from_centre_width_height(Vector2_t<T> centre, T width, T height) noexcept
    {
        const auto vec = Vector2_t{.x = width / static_cast<T>(2), .y = height / static_cast<T>(2)};
        return Rect{centre - vec, centre + vec};
    }

    [[nodiscard]] constexpr auto contains(Vector2_t<T> vec) const noexcept
    {
        return vec.x >= min.x and vec.x <= max.x and vec.y >= min.y && vec.y <= max.y;
    }

    [[nodiscard]] constexpr auto clamped_to(Rect<T> bounds) const
    {
        using ext        = Extents<f64>;
        const auto ext_x = ext{bounds.min.x, bounds.max.x};
        const auto ext_y = ext{bounds.min.y, bounds.max.y};
        return Rect<T>{{ext_x.clamp(min.x), ext_y.clamp(min.y)},
                       {ext_x.clamp(max.x), ext_y.clamp(max.y)}};
    }

    [[nodiscard]] constexpr auto width() const { return max.x - min.x; }
    [[nodiscard]] constexpr auto height() const { return max.y - min.y; }

    [[nodiscard]] constexpr friend bool operator==(const Rect<T>& lhs,
                                                   const Rect<T>& rhs) noexcept = default;
};
} // namespace sm


template <sm::concepts::Number T> class fmt::formatter<sm::Rect<T>>
{
  public:
    constexpr auto parse(const format_parse_context& ctx) { return ctx.begin(); }

    auto format(const sm::Rect<T>& p, auto& ctx)
    {
        return format_to(ctx.out(),
                         R"(
Rect(min = {},
     max = {}))",
                         p.min, p.max);
    }
};
