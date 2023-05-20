#include "FLevent.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

using namespace std;
int main() {
    //显示窗口
    IndexWindows indexWindows;
    indexWindows.show();
    return Fl::run();
}
