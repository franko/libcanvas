#include "canvas_svg.h"

template <>
void canvas_svg::draw<elem_object>(elem_object& vs, agg::rgba8 c)
{
    int id = m_current_id ++;
    str s = vs.write_svg(id, c, m_height);
    canvas_svg::writeln(m_output, s, "   ");
}

template <>
void canvas_svg::draw_outline<elem_object>(elem_object& vs, agg::rgba8 c)
{
    int id = m_current_id ++;
    str path;
    svg_property_list* ls = vs.svg_path(path, m_height);
    str s = svg_stroke_path(path, canvas_svg::default_stroke_width, id, c, ls);
    svg_property_list::free(ls);
    canvas_svg::writeln(m_output, s, "   ");
}
