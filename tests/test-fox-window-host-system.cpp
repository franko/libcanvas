#include <cmath>
#include <thread>

#include "libelplot.h"
#include "libelplot_utils.h"
#include "FXElemPlotWindow.h"
#include "window_platform_fox.h"

using namespace elp;

class PlotWindow : public FXMainWindow {
    FXDECLARE(PlotWindow)
protected:
    PlotWindow() { }
private:
    PlotWindow(const PlotWindow&);
    PlotWindow &operator=(const PlotWindow&);
public:
    PlotWindow(FXApp* a, const FXString& name, FXIcon *ic=NULL, FXIcon *mi=NULL, FXuint opts=DECOR_ALL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=0,FXint pr=0,FXint pt=0,FXint pb=0,FXint hs=0,FXint vs=0):
        FXMainWindow(a, name, ic, mi, opts, x, y, w, h, pl, pr, pt, pb, hs, vs) {
    }

    long onElemWindowStart(FXObject *, FXSelector, void *);

    enum {
        ID_PLOT_WINDOW_START = FXApp::ID_LAST,
        ID_LAST,
    };
};

FXDEFMAP(PlotWindow) PlotWindowMap[] = {
    FXMAPFUNC(SEL_IO_READ, PlotWindow::ID_PLOT_WINDOW_START, PlotWindow::onElemWindowStart),
};

FXIMPLEMENT(PlotWindow, FXMainWindow, PlotWindowMap, ARRAYNUMBER(PlotWindowMap))

long PlotWindow::onElemWindowStart(FXObject *, FXSelector, void *ptr) {
    window_fox_start_data *message_data = (window_fox_start_data *) ptr;
    if (!message_data) {
        fprintf(stderr, "internal error: no message data with window's start signale\n");
        return 1;
    }
    FXApp *app = getApp();
    // PROBLEM: the logic of resize is demanded to the client application.
    FXuint main_window_options = (DECOR_TITLE|DECOR_MINIMIZE|DECOR_MAXIMIZE|DECOR_CLOSE|DECOR_BORDER|DECOR_SHRINKABLE|DECOR_MENU);
    if (message_data->flags & WindowResize) {
        main_window_options |= DECOR_STRETCHABLE;
    }
    auto main_win = new FXMainWindow(app, "Plot Window", nullptr, nullptr, main_window_options, 0, 0, message_data->width, message_data->height);
    auto plot_win = new FXElemPlotWindow(main_win, message_data->window, LAYOUT_FILL_X | LAYOUT_FILL_Y);
    message_data->window->bind_drawable(plot_win, FXElemPlotWindow::ID_UPDATE_REGION);
    main_win->create();
    main_win->show(PLACEMENT_SCREEN);
    return 1;
}

template <typename Function>
void AddFunction(Plot& plot, double x0, double x1, Function f, Color color, int n = 512) {
    Path line;
    line.MoveTo(x0, f(x0));
    for (int i = 1; i <= n; i++) {
        const double x = x0 + i * (x1 - x0) / n;
        line.LineTo(x, f(x));
    }
    plot.Add(std::move(line), color, 1.5, color::None, property::Stroke);
}

Plot CreateNewPlot() {
    Plot plot(Plot::ShowUnits | Plot::AutoLimits);
    plot.SetClipMode(false);
    const double x0 = 0.0001, x1 = 8 * math::Tau();
    AddFunction(plot, x0, x1, [](double x) { return std::sin(x) / x; }, color::Blue);
    plot.SetTitle("Function plot example");
    plot.SetXAxisTitle("x variable");
    return plot;
}

void WorkerThreadStart(FXApp *app, FXObject *host_object, FXSelector start_sel) {
    InitializeFonts();
    utils::Sleep(3);
    Plot plot = CreateNewPlot();
    Window win(new elp_window_fox(app, host_object, start_sel));
    win.Attach(plot, "");
    win.Start(640, 480, WindowResize);
    win.Wait();
}

int main(int argc, char *argv[]) {
    FXApp app("Plot Windows", "libelplot");
    app.init(argc, argv);
    auto main_window = new PlotWindow(&app, "FOX Window host example", nullptr, nullptr, DECOR_ALL, 0, 0, 320, 320);
    new FXLabel(main_window, "Window Plot demonstrator");

    std::thread worker_thread(WorkerThreadStart, &app, main_window, PlotWindow::ID_PLOT_WINDOW_START);
    worker_thread.detach();

    app.create();
    main_window->show(PLACEMENT_SCREEN);
    return app.run();
}
