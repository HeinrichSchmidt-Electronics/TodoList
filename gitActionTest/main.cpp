#include <iostream>
#include <windows.h>
#include "MenuState.h"

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    MenuContext context(std::make_unique<MainMenu>());
    while(context.isOpen()) {
        context.run();
    }
}