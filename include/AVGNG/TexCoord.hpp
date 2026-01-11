#pragma once


//ss >> v >> t >> u >> v_coord;
namespace ng::Graphics {

	struct TexCoord {
		const char* v;
		const char* t;
		float u, v_coord;
	};
}