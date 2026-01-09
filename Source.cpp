#include "Game.hpp"

using namespace ng;

int main(void)
{
  
	Game* g = new Game();
	g->Load();		// Load things required for game
	g->Start();		// Start game ( once loading completed )
	g->Run();		// Enter run loop (leaving run loop will call g.Exit()

}