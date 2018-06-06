/**************************************************************************//**
 * @file
 * 
 * Color mapping for SDL.
 *****************************************************************************/

#include "colormap.h"

#define COLOR_DEPTH 255 /**< Depth of a color channel in bits */

// Colormap names
static const std::string bw =      "bw";
static const std::string rainbow = "rainbow";
static const std::string holiday = "holiday";
static const std::string neon =    "neon";
static const std::string lava =    "lava";
static const std::string ice =     "ice";
static const std::string dawn =    "dawn";
static const std::string toxic =   "toxic";

static const uint32_t colors_bw[] =
{
    0xFF000000, // Black
    0xFFFFFFFF  // White
};

static const uint32_t colors_rainbow[] =
{
    0xFFFF0000, // Red
    0xFFFF8000, // Orange
    0xFFFFFF00, // Yellow
    0xFF00FF00, // Green
    0xFF0000FF, // Blue
    0xFF4B0082, // Indigo
    0xFF8000FF  // Violet
};

static const uint32_t colors_holiday[] = 
{
    0xFF6386FF, // Pastel blue
    0xFFFFE574, // Pastel yellow
    0xFFFFBC8F, // Pastel orange
    0xFFFFBFD4, // Pastel pink
    0xFF53DFBD  // Pastel green
};

static const uint32_t colors_neon[] =
{
    0xFF0000FF, // Blue
    0xFFFFFF00, // Yellow
    0xFFFF00FF, // Magenta
    0xFF00FFFF, // Cyan
    0xFF00FF00  // Green
};

static const uint32_t colors_lava[] =
{
    0xFF000000, // Black
    0xFFFF0000, // Red
    0xFFFF8000, // Orange
    0xFFFFFF00, // Yellow
    0xFFFFFFFF  // White
};

static const uint32_t colors_ice[] =
{
    0xFF000000, // Black
    0xFF000080, // Dark blue
    0xFF0000FF, // Blue
    0xFF00FFFF, // Cyan
    0xFFFFFFFF  // White
};

static const uint32_t colors_dawn[] =
{
    0xFF000080, // Dark blue
    0xFF800080, // Purple
    0xFFFF8080, // Reddish
    0xFFFFFF80, // Dark yellowish
    0xFFFFFFC0  // Light yellowish
};

static const uint32_t colors_toxic[] =
{
    0xFF000000, // Black
    0xFF008000, // Dark green
    0xFF80FF00, // Light green
    0xFFFFFF80, // Yellowish
    0xFFFFFFFF  // White
};

/** Generated colormap */
static uint32_t colormap_lookup[COLOR_DEPTH];

/**************************************************************************//**
 * Builds a colormap (lookup table between a pixel value and a color code)
 * 
 * @param[in] n_colors Number of anchor colors in the map
 * @param[in] colors Anchor colors
 * @param[out] colormap Generated colormap
 *****************************************************************************/
static void build_colormap(const size_t n_colors, const uint32_t* colors,
    uint32_t* colormap);

/**************************************************************************//**
 * Builds a gradient array of ARGB codes
 * 
 * @param[in] color_begin ARGB code of the beginning color
 * @param[in] color_end ARGB code of the end color
 * @param[in] gradient_depth Number of colors in the array
 * @param[out] gradient Array where the gradient will be stored
 *****************************************************************************/
static void build_argb_gradient(const uint32_t color_begin,
    const uint32_t color_end, const size_t gradient_depth, uint32_t* gradient);

/**************************************************************************//**
 * Computes a gradient between two values
 * 
 * @param[in] begin Begin value
 * @param[in] end End value
 * @param[in] depth Gradient depth
 * 
 * @return Gradient
 *****************************************************************************/
float_t compute_gradient(const uint8_t begin, const uint8_t end,
    const size_t depth);

void colormap_init(const std::string str)
{
    size_t n_colors;
    const uint32_t* colors;
    
    if (str == bw)
    {
        n_colors = sizeof(colors_bw)/sizeof(colors_bw[0]);
        colors = colors_bw;
    }
    else if (str == rainbow)
    {
        n_colors = sizeof(colors_rainbow)/sizeof(colors_rainbow[0]);
        colors = colors_rainbow;
    }
    else if (str == holiday)
    {
        n_colors = sizeof(colors_holiday)/sizeof(colors_holiday[0]);
        colors = colors_holiday;
    }
    else if (str == neon)
    {
        n_colors = sizeof(colors_neon)/sizeof(colors_neon[0]);
        colors = colors_neon;
    }
    else if (str == lava)
    {
        n_colors = sizeof(colors_lava)/sizeof(colors_lava[0]);
        colors = colors_lava;
    }
    else if (str == ice)
    {
        n_colors = sizeof(colors_ice)/sizeof(colors_ice[0]);
        colors = colors_ice;
    }
    else if (str == dawn)
    {
        n_colors = sizeof(colors_dawn)/sizeof(colors_dawn[0]);
        colors = colors_dawn;
    }
    else if (str == toxic)
    {
        n_colors = sizeof(colors_toxic)/sizeof(colors_toxic[0]);
        colors = colors_toxic;
    }
    else
    {
        n_colors = sizeof(colors_bw)/sizeof(colors_bw[0]);
        colors = colors_bw;
    }
    
    build_colormap(n_colors, colors, colormap_lookup);
}

static void build_colormap(const size_t n_colors, const uint32_t* colors,
    uint32_t* colormap)
{    
    // A gradient of n colors is (n - 1) concatenated gradients
    size_t gradient_depth = COLOR_DEPTH/(n_colors - 1);
    for (size_t i = 0; i < (n_colors - 1); i++)
    {
        build_argb_gradient(colors[i], colors[i + 1], gradient_depth,
            &(colormap[i*gradient_depth]));
    }

    // Because of the rounding error when calculating gradient_depth, the last
    // couple values of the color map could remain empty. Correct that by
    // assigning them the last color. It's a kludge but on the display you
    // can't see the difference.
    for (size_t i = gradient_depth * (n_colors - 1); i < COLOR_DEPTH; i++)
    {
        colormap[i] = colors[n_colors - 1];
    }
}

static void build_argb_gradient(const uint32_t color_begin,
    const uint32_t color_end, const size_t gradient_depth, uint32_t* gradient)
{   
    // Compute the begin and end values for each channel
    uint8_t alpha_begin =   (color_begin &  0xFF000000) >> 24;
    uint8_t alpha_end =     (color_end &    0xFF000000) >> 24;
    uint8_t red_begin =     (color_begin &  0x00FF0000) >> 16;
    uint8_t red_end =       (color_end &    0x00FF0000) >> 16;
    uint8_t green_begin =   (color_begin &  0x0000FF00) >> 8;
    uint8_t green_end =     (color_end &    0x0000FF00) >> 8;
    uint8_t blue_begin =    (color_begin &  0x000000FF);
    uint8_t blue_end =      (color_end &    0x000000FF);
    
    float_t gradient_alpha = compute_gradient(alpha_begin, alpha_end,
        gradient_depth);
    float_t gradient_red = compute_gradient(red_begin, red_end,
        gradient_depth);
    float_t gradient_green = compute_gradient(green_begin, green_end,
        gradient_depth);
    float_t gradient_blue = compute_gradient(blue_begin, blue_end,
        gradient_depth);

    // Compute the colors one by one
    for (size_t i = 0; i < gradient_depth; i++)
    {
        uint8_t alpha = (uint8_t)(i*gradient_alpha) + alpha_begin;
        uint8_t red = (uint8_t)(i*gradient_red) + red_begin;
        uint8_t green = (uint8_t)(i*gradient_green) + green_begin;
        uint8_t blue = (uint8_t)(i*gradient_blue) + blue_begin;
        gradient[i] = (alpha << 24) + (red << 16) + (green << 8) + blue;
    }
}

float_t compute_gradient(const uint8_t begin, const uint8_t end,
    const size_t depth)
{
    return (float_t)(end - begin)/(float)(depth);
}

void colormap_ARGB8888(const size_t w, const size_t h, const float_t* s,
    uint32_t* d)
{    
    for (size_t i = 0; i < w*h; i++)
    {
        uint8_t pixel = (s[i]*COLOR_DEPTH);
        d[i] = colormap_lookup[pixel];
    }
}
