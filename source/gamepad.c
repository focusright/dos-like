/* 
tcc\tcc source\gamepad.c source\dos.c
wasm\node wasm\wajicup.js source/gamepad.c source/dos.c gamepad.html

clang source/gamepad.c source\dos.c -o gamepad.exe 
-L C:\libs\SDL2-2.28.5\lib\x64\ 
-I C:\libs\SDL2-2.28.5\include\ 
-lShell32 -lSDL2main -lSDL2 -lSDL2_ttf -Wno-narrowing -Xlinker /subsystem:console
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "dos.h"

SDL_GameController *findController() {
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            return SDL_GameControllerOpen(i);
        }
    }
    return NULL;
}

inline SDL_JoystickID getControllerInstanceID(SDL_GameController *controller) {
    return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
}

int main(int argc, char* argv[]) {
    setvideomode(videomode_320x200);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
    }

    SDL_GameController *controller = findController();
    SDL_Event event;
    
    while(!shuttingdown()) {
        waitvbl();
        if( keystate( KEY_ESCAPE ) )  { break; }

        if(SDL_PollEvent( &event ) != 0) {
            switch (event.type) {
                case SDL_CONTROLLERDEVICEADDED:
                    if (!controller) { controller = SDL_GameControllerOpen(event.cdevice.which); }
                    break;
                case SDL_CONTROLLERDEVICEREMOVED:
                    if (controller && event.cdevice.which == getControllerInstanceID(controller)) {
                        SDL_GameControllerClose(controller);
                        controller = findController();
                    }
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    if (controller && event.cdevice.which == getControllerInstanceID(controller)) {
                        printf("%d\n", (int)event.cbutton.button);
                        setcolor(rand() % 256);
                        line(rand() % 320, rand() % 200, rand() % 320, rand() % 200);
                        setcolor(rand() % 256);
                        fillcircle(rand() % 320, rand() % 200, rand() % 30);
                        setcolor(rand() % 256);
                        circle(rand() % 320, rand() % 200, rand() % 30);
                    }
                    break;
            }
        }

    }
    return 0;
}