#include "MyWin.hh"

/*
Xe Release Gtkmm4 Reset Version
The project is originally build as a console app, in 2020, 
and after several months, I add a dialog for the application,
the next is Gtk3 builds, and at that time, the project is uploaded to github,
the first Gtk4 build is finished, but there is some font rendering issues, 
and the Gtkmm3 and Gtkmm4 build is finished in 2021, 
in a long time, the Gtkmm3 is the main framework for Xe Release, 
the font issue seems to be fixed in Gtk4 4.8.0, so a reset build is here.
2022/12/6, dale clack
The Xe&VM Project
*/

int main(int argc,char **argv){
    // Create a application
    auto app=Gtk::Application::create("org.gtk.daleclack");
    // MyWin window;
    return app->make_window_and_run<MyWin>(argc, argv);
}
