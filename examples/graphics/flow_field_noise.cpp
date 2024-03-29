/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2022 Jai Bellare
 * See <https://opensource.org/licenses/MIT/> or LICENSE.md
 * Project homepage: https://github.com/strangeQuark1041/samarium
 */

#include "samarium/graphics/colors.hpp"
#include "samarium/samarium.hpp"

#include "range/v3/view/enumerate.hpp"

using namespace sm;
using namespace sm::literals;

auto main() -> i32
{
    auto app   = App{{.dims{1800, 900}}};
    auto watch = Stopwatch{};

    const auto scale = .4;
    const auto dims  = Dimensions::combine(50);

    auto ps     = ParticleSystem{1000};
    auto forces = VectorField{dims};
    auto rand   = RandomGenerator{};

    for (auto& particle : ps)
    {
        particle.pos    = rand.vector({.min = Vector2{0.0, 0.0}, .max = dims.cast<f64>()});
        particle.radius = 0.18;
    }

    for (auto [pos, force] : forces.enumerate_2d())
    {
        const auto noisy_angle =
            noise::perlin_2d(pos.cast<f64>() +
                                 Vector2::combine(10) * static_cast<f64>(app.frame_counter),
                             {.scale = scale}) *
            math::two_pi;
        force = Vector2::from_polar({.length = 1.0, .angle = noisy_angle});
    }

    const auto update = [&](f64 dt)
    {
        for (auto [i, particle] : ranges::views::enumerate(ps))
        {
            const auto pos   = particle.pos.cast<u64>();
            const auto force = forces.at_or(pos, Vector2{0.0, 0.0});

            particle.apply_force(force);
        }

        ps.update(dt);
        ps.for_each(
            [=](Particle& particle)
            {
                particle.pos.x = math::wrap_max(particle.pos.x, static_cast<f64>(dims.x));
                particle.pos.y = math::wrap_max(particle.pos.y, static_cast<f64>(dims.y));
                particle.vel.clamp_length({0.0, 3.0});
            });
    };

    const auto draw = [&]
    {
        for (const auto& particle : ps)
        {
            const auto color = "#ff1745"_c.with_multiplied_alpha(0.05);

            const auto pos = app.transform.apply_inverse(particle.pos * app.dims().cast<f64>() /
                                                         dims.cast<f64>());
            app.draw(Circle{pos, particle.radius}, {.fill_color = color});
        }
    };

    app.run(update, draw);
}
