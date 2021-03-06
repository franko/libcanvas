#pragma once

#include "render_config.h"

#include "agg_pixfmt_rgb.h"

typedef graphics::gamma_type gamma_type;

struct pixel_gamma : public agg::pixfmt_rgb24_gamma<gamma_type>
{
    pixel_gamma(agg::rendering_buffer& ren_buf):
        agg::pixfmt_rgb24_gamma<gamma_type>(ren_buf, graphics::gamma)
    { }
};

struct pixel_gamma_lcd : public agg::pixfmt_rgb24_lcd_gamma<gamma_type>
{
    pixel_gamma_lcd(agg::rendering_buffer& ren_buf):
        agg::pixfmt_rgb24_lcd_gamma<gamma_type>(ren_buf, graphics::subpixel_lut, graphics::gamma)
    { }
};

struct pixel_simple : public agg::pixfmt_rgb24
{
    pixel_simple(agg::rendering_buffer& ren_buf):
        agg::pixfmt_rgb24(ren_buf)
    { }
};

struct pixel_lcd : public agg::pixfmt_rgb24_lcd
{
    pixel_lcd(agg::rendering_buffer& ren_buf):
        agg::pixfmt_rgb24_lcd(ren_buf, graphics::subpixel_lut)
    { }
};

#ifdef DISABLE_GAMMA_CORR
typedef pixel_simple pixel_type;
typedef pixel_lcd pixel_lcd_type;
#else
typedef pixel_gamma pixel_type;
typedef pixel_gamma_lcd pixel_lcd_type;
#endif
