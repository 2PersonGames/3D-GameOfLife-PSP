/**
  * Author: Sam Albon
  * Created: 21/02/2011
  * Defines the entry point of the program
  */

#include "CubeData.h"

/* Redefine some functions */
#define Update UpdateCubes
#define Render ReleaseRender
#define Clean DeleteCubes

SCE_MODULE_INFO( GameOfLife_100068439, 0, 1, 1 );

/* Entry point */
int main(void)
{
	CreateCubes(0);
	StartUpDisplay();

	while (Update() == 0)
	{
		Render();
	}
	
	Clean();

	return 0;
}
