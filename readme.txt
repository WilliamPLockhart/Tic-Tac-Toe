to start program, open integrated terminal in project1, then type: ./bin./run




  // sets icon
static SDL_Surface *icon;
    icon = SDL_LoadBMP("assets/logo.bmp");
    SDL_SetWindowIcon(win, icon);