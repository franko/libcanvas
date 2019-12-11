#pragma once

#include "canvas_object.h"

#include "agg_path_storage.h"
#include "agg_ellipse.h"

typedef elp_object_gen<agg::path_storage, no_approx_scale> sg_path;
typedef elp_object_gen<agg::ellipse, approx_scale> sg_ellipse;
