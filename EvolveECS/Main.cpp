#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
// 2022-03-25
// God I have 100's of memory leaks. Lets fix it up using smart pointers
#include <memory>
#include <string>
#include "SceneManager.h"
#include "Debug.h"

// 2022-04-05
// Hooked up with GitHub thanks to Gail Harris!

// 2022-02-04 assignment
// Scott will upload meshes, and components for shaders and materials onto the web
// see if you can get Mario up on the screen!
// 2022-02-11 Assignment 1
// write up the MaterialComponent to get pants on Mario
// load png from harddrive, send it down to the GPU
// steal code from Texture.cpp on Scott's website, follow pattern/outline from this semester
// understand it. Was called Textures in OpenGL class but Unity calls it a Material
// Stick it into the Material Component
// make tectureID a GLUint member variable
// use the textureVert and textureFrag shaders instead
// finish the lightActor. Build material
// bind the material, before the draw call
// pass the light position to the GPU
  
int main(int argc, char* args[]) {
	// Garen Johnston figured out a way to clear any memory leaks
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //using this to check on program exit the memory leak involving ofstream is gone, i think calling _crtDumpMemoryLeaks before exit its finding a memory leak that is actually cleaned up on exit

	/// Comment out the line below if you really want to make a 64-bit build
	static_assert(sizeof(void*) == 4, "Are you ready for 64-bit build?");

	Debug::DebugInit("GameEngineLog.txt");
	
	// Let's fix up this raw pointer:
	// SceneManager* gsm = new SceneManager();

	// Create a local scope so game smart pointer dies before we dump the memory leaks
	{
		// This is the cool way to do it with one allocation
		// The only unique pointer we'll have in the whole engine. We'll use shared for the rest
		std::unique_ptr<SceneManager> game = std::make_unique<SceneManager>();
		if (game->Initialize("Game Engine", 1280, 720) == true) {
			game->Run();
		}
	}
	/// This writes out memory leaks to the output window not the console window
	_CrtDumpMemoryLeaks();
	// Garen says to comment out the line above so that we don't dump leaks before exiting
	// Seems to fix Scott's phantom leaks
	exit(0);
}