#pragma once

#include "debug_priv.h"
#include "gc_context.h"
#include "plot.h"
#include "plot_agent.h"

class elem_plot : public graphics::plot {
public:
    elem_plot(unsigned flags):
        graphics::plot{flags} {
    }
    elem_plot(const elem_plot& other):
        graphics::plot{other},
        m_plot_agent{} {
    }
    elem_plot(elem_plot&& other):
        graphics::plot{std::move(other)},
        m_plot_agent{std::move(other.m_plot_agent)} {
    }

    ~elem_plot() {
        debug_log(2, "elem_plot::~elem_plot() [%p]", this);
    }

    void update_windows() {
        m_plot_agent.update_windows();
    }

    void clear_windows_pending_flags() {
        m_plot_agent.clear_pending_flags();
    }

    void add_window_link(elem_window *window, int slot_index) {
        m_plot_agent.add_window(window, slot_index);
    }

    void clear_windows_links() {
        m_plot_agent.clear();
    }

    void release();
    // Returns true if object has some remaining references.
    bool has_references(gc_context& gc);
private:
    graphics::plot_agent m_plot_agent;
    int m_ref_count = 1;
};
