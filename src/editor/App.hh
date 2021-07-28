#include "wx.hh"

/**
 * Contains wxwidgets based classses.
 */
namespace App {
    /**
     * Ids of event thingos that can get called by buttons.
     */
    enum Event {
        Hello = 1
    };

    /**
     * wxwidgets application class. Core of the editor program.
     */
    class Editor: public wxApp {
        virtual bool OnInit() override;
    };

    /**
     * I don't really get what this does yet.
     */
    class LevelFrame: public wxFrame {
        public:
            LevelFrame();

        private:
            void onHello(wxCommandEvent &event);
    };
};
