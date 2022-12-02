#include "Engine.h"
#include "Setting.h"


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    Engine game{ {settings::screenWidth, settings::screenHeight, "Mechtoria"} };

    while (!game.ExitGameNow())
    {
        game.Tick();
    }

    return 0;
}