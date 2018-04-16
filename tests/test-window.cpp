#include "window.h"
#include "path.h"

int main()
{
    graphics::initialize_fonts();

    graphics::window win("h.");

    graphics::plot p(graphics::plot::show_units);
    agg::rect_d lim(-1.0, 0.0, 1.0, 10.0);
    p.set_limits(lim);
    p.set_axis_labels_angle(graphics::x_axis, 3.141592 / 4);
    p.enable_label_format(graphics::x_axis, "%.6f");

    draw::path ln;
    agg::path_storage& l = ln.self();
    l.move_to(-0.5, 0.0);
    l.line_to(-0.5, 8.0);
    l.line_to(0.5, 4.0);
    l.close_polygon();

    agg::rgba8 none(0,0,0,0);
    agg::rgba8 red(180, 0, 0, 255);
    agg::rgba8 blue(0, 0, 180, 255);
    agg::rgba8 yellow(245, 254, 0, 255);

    trans::scaling_a* trln = new trans::scaling_a(&ln);
    sg_element lne(trln, yellow, red, 2.5, ELEMENT_FILL|ELEMENT_STROKE);
    p.add(lne);

    p.commit_pending_draw();

    int index = win.attach(&p, "1");

    win.start(640, 480, graphics::window_resize);

    sleep(4);

    draw::path ln2;
    agg::path_storage& l2 = ln2.self();
    l2.move_to(0.8, 1.0);
    l2.line_to(0.8, 7.0);
    l2.line_to(0.3, 4.0);
    l2.close_polygon();

    trans::scaling_a* trln2 = new trans::scaling_a(&ln2);
    sg_element ln2e(trln2, none, blue, 2.5);
    p.add(ln2e);

    win.slot_refresh(index);
    p.commit_pending_draw();

    for (;;) {
        fprintf(stderr, "sleeping...\n");
        fflush(stderr);
        sleep(2);
    }

    return 0;
}