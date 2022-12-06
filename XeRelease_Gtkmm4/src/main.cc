#include "MyWin.hh"

int main(int argc,char **argv){
    auto app=Gtk::Application::create("org.gtk.daleclack");
    MyWin window;
    return app->run(window);
}
