#include "GUI.h"


// Check project configuration. Linker->System->SubSystem should be Windows
// the choice of subsystem affects the entry point symbol(or entry point function) that the linker will choose
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    // global variable
    mainWindow = new MainWindow(hInstance, 100, 100, 800, 600);

    LoginButton* loginButton = new LoginButton(0, 0, 400, 50, mainWindow->mainWindowId, mainWindow->instanceId);

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    return runLoop();

}
