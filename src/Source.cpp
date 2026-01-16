#include <AVGNG/Game.hpp>

using namespace ng::Core;

int main(void)
{
  
	// Setup Debug Logging
	Debug::Init("game.log");

	Game* g = new Game();
	g->Init();		// Initialize game
	g->Load();		// Load things required for game
	g->Start();		// Start game ( once loading completed )
	g->Run();		// Enter run loop (leaving run loop will call g.Exit()

}