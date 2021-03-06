FOX toolkit integration
=======================

The Elementary Plot library can be used inside a `FOX Toolkit <http://fox-toolkit.org/>`_ application using a special class :cpp:class:`FX::FXElemWindow` declared in the header file ``FXElemWindow.h``.

The :cpp:class:`FX::FXElemWindow` inherit from ``FX::FXElemBaseWindow`` which in turns inherits from ``FX::FXWindow`` and it can be used as an ordinary FOX Window in every respect.
In addition the class implements the method :cpp:func:`FX::FXElemWindow::Attach` to bind a plot, like it can be done with a :cpp:class:`elem::Window`.

The plots themselves can be created and modified using the standard C++ API of the Elementary Plot Library.

Once a plot is bound to a window the application will be automatically updated for any plot's modification, without the need of any explicit action.

.. cpp:class:: FX::FXElemWindow : public FX::FXElemBaseWindow

  FOX toolkit class used to display one or more Elementary Plot plots.

  .. cpp:function:: FX::FXElemWindow(FX::FXComposite* p, const char *plot_layout = nullptr, FXuint opts=FRAME_NORMAL, FXint x=0, FXint y=0, FXint w=0, FXint h=0)

    Ordinary FOX constructor with the addition of ``plot_layout`` used to subdivide the windows in multiple plotting slots.

  .. cpp:function:: void activateElem(FXElemStartMessage *message, FXElemCreatePolicy create_policy)

    Prepare the window to be used with the Elementary Plot library.
    If the create_policy is ELEM_CREATE_NOW the window will be created and shown using the standard FOX methods.

  .. cpp:function:: int Attach(elem::Plot& p, const char* slot_str)

    Act like the method :cpp:class:`elem::Window` of the :cpp:class:`elem::Window` class to bind a plot to a given slot.

  .. cpp:function:: void SlotRefresh(unsigned index)

    Act like the :cpp:class:`elem::Window`'s method of the same name.

  .. cpp:function:: void SetLayout(const char *fmt)

    Change the plot's layout of the window.
    It works only if the window is not yet started.

  .. cpp:function:: void Wait()

    Act like the :cpp:class:`elem::Window`'s method of the same name.
    Normally not needed in classic FOX applications.

.. cpp:class:: FX::FXElemBaseWindow : public FX::FXWindow

  FOX toolkit class that provide support for and :cpp:class:`elem::Window`.
  It is used for the implementation and should not be used directly.

.. cpp:struct:: FXElemStartMessage

  Contains the following field:
  
  - `window_fox *window`
  - `unsigned width`
  - `unsigned height`
  - `unsigned flags`

.. cpp:function:: elem_window *elem_window_fox(FXGUISignal *start_signal)
  
  Create a new object that can be passed to :cpp:func:`FX::FXElemWindow::Attach` to create a :cpp:class:`elem::Window`.
  It should be used on a worker thread to create a :cpp:class:`elem::Window` that is started in the FOX GUI thread.
  The start_signal should be a signal, previously created in the GUI thread, that trigger a request to create a new Elementary Plot window.
  The method that handle the start_signal should create an object of type :cpp:class:`FXElemBaseWindow`, set its width and height and the call the method :cpp:func:`FXElemBaseWidow.activateElem`.
  
  Here an example of how the handler cound be done::

    long SomeClass::onElemWindowStart(FXObject *, FXSelector, void *ptr) {
      FXElemStartMessage *message = (FXElemStartMessage *) ptr;
      assert(message != nullptr);
      FXuint main_window_options = (DECOR_TITLE|DECOR_MINIMIZE|DECOR_MAXIMIZE|DECOR_CLOSE|DECOR_BORDER|DECOR_SHRINKABLE|DECOR_MENU);
      if (message->flags & WindowResize) {
          main_window_options |= DECOR_STRETCHABLE;
      }
      FXMainWindow *main_win = new FXMainWindow(getApp(), "Plot Window", nullptr, nullptr, main_window_options, 0, 0, message->width, message->height);
      auto elem_window = new FXElemBaseWindow(main_win, LAYOUT_FILL_X | LAYOUT_FILL_Y);
      elem_window->setWidth(message->width);
      elem_window->setHeight(message->height);
      elem_window->activateElem(message, ELEM_CREATE_DEFER);
      main_win->create();
      main_win->show(PLACEMENT_SCREEN);
      return 1;
    }

  The notable things are:

  - a :cpp:class:`FXElemStartMessage` message is provided to the handler
  - the method activateElem is called to prepare the window to work with the elementary plot library
  - the window is created and shown

  Otherwise, if the Elementary Plot window should be created in an already existing FXComposite parent the handler could be::

    long PlotWindow::onElemWindowStart(FXObject *, FXSelector, void *ptr) {
        FXElemStartMessage *message = (FXElemStartMessage *) ptr;
        assert(message != nullptr);
        auto elem_window = new FXElemBaseWindow(frame, LAYOUT_FILL_X | LAYOUT_FILL_Y);
        elem_window->setWidth(message->width);
        elem_window->setHeight(message->height);
        elem_window->activateElem(message, ELEM_CREATE_NOW);
        return 1;
    }
