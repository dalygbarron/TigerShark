#include "App.hh"

bool App::Editor::OnInit() {
    LevelFrame *frame = new LevelFrame();
    frame->Show();
    return true;
}
