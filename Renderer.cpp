
#include "Renderer.hpp"

using namespace ng::Graphics;

void Renderer::Render(std::vector<Mesh*> meshBatch)
{
	// Draw each mesh in batch
	for (int i = 0; i < meshBatch.size(); i++) {
		meshBatch[i]->Draw();
	}
}

