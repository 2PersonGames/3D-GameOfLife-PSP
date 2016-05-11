/**
  * Author: Sam Albon
  * Created: 21/02/2011
  * Defines functions related to modifying the data related to the grid.
  */

#include "CubeData.h"

unsigned char UpdateCubes(void)
{	
	/* Declare the counters */
	CartesianCoordinate location;
	
	/**
	  * TODO:
	  * Force the PSP to cache my array
	  * Not sure if this is possible, but the entire array should fit in the cache
	  */
	  
	/* Arhice all working states */
	for (location.z = 0; location.z < DEPTH; location.z++)
	{
		for (location.y = 0; location.y < WIDTH; location.y++)
		{
			_grid[(location.z * DEPTH) + location.y].previous = _grid[(location.z * DEPTH) + location.y].current;
		}
	}
	
	/* Iterate through the grid */
	for (location.z = 0; location.z < DEPTH; location.z++)
	{
		for (location.y = 0; location.y < WIDTH; location.y++)
		{
			for (location.x = 0; location.x < HEIGHT; location.x++)
			{
				CheckNeighbours(location);
			}
		}
	}
	
	/**
	  * TODO:
	  * Possible force the PSP to write the cache back
	  */

	/* Return successful */
	return 0;
}

/**
  * Created: 24/02/2011
  * Returns a boolean of alive
  * Needs changing for efficiency
  * Inline for efficiency
  */
unsigned int GetCubeState(unsigned char x, unsigned char y, unsigned char z)
{
	unsigned short mask = 0x1;	/*< Creates a mask of the value ...0001 */
	mask <<= x;		/*< Shifts the mask along to meet the value required */
	
	/* Masks the line and returns the result */
	return _grid[(z * DEPTH) + y].previous & mask;

}

/**
  * Created: 24/02/2011
  * Inverts the state of the cube at that location in the grid
  * Inline for efficiency
  */
void SetCubeState(CartesianCoordinate location)
{
	unsigned short mask = 0x1;	/* Creates a mask of the value ...0001 */
	mask <<= location.x;		/* Shifts the mask along to meet the value being set */
	
	/* Masks the line to invert the coordinate's state */
	_grid[(location.z * DEPTH) + location.y].current ^= mask;		
}

/**
  * Created: 24/02/2011
  * Checks the neighbours of a cube at that location and applies the GOL rules
  * Needs changing for efficiency
  * Inline for efficiency
  */
void CheckNeighbours(CartesianCoordinate location)
{
	/* Declare counters */
	char z;
	char i;
	unsigned char living;
	
	/* Iterate through the neighbours */
	living = 0;
	for (i = -1; i < 2; i++)
	{
		/* Test z axis bounds */
		z = location.z + i;
		if (z >= 0 && z < DEPTH)
		{
			/* Test x axis bounds */
			if (location.x > 0)			/*< Left */
			{
				/* Top Left */
				if (location.y > 0 && GetCubeState(location.x - 1, location.y - 1, z) > 0)
				{
					living++;
				}
				/* Middle Left */
				if (GetCubeState(location.x - 1, location.y, z) > 0)
				{
					living++;
				}
				/* Bottom Left */
				if (location.y < WIDTH - 1 && GetCubeState(location.x - 1, location.y + 1, z) > 0)
				{
					living++;
				}
			}
			/* Test x axis bounds */
			if (location.x < HEIGHT - 1)	/*< Right */
			{
				if (location.y > 0 && GetCubeState(location.x + 1, location.y - 1, z) > 0)
				{
					living++;
				}
				if (GetCubeState(location.x + 1, location.y, z) > 0)
				{
					living++;
				}
				if (location.y < WIDTH - 1 && GetCubeState(location.x + 1, location.y + 1, z) > 0)
				{
					living++;
				}
			}
			/* Test y axis bounds */
			if (location.y > 0 && GetCubeState(location.x, location.y - 1, z) > 0)
			{
				living++;
			}
			if (location.y < WIDTH - 1 && GetCubeState(location.x, location.y + 1, z) > 0)
			{
				living++;
			}
			if (z != location.z && GetCubeState(location.x, location.y, z))
			{
				living++;
			}
			
			/* Test if living is greater than 3 */
			if (living > 3)
			{
				break;
			}
		}
	}
	
	/* Apply rules */
	if (GetCubeState(location.x, location.y, location.z) == 0)
	{
		/* Cube is currently dead */
		if (living == 3)
		{
			SetCubeState(location);	/*< Set state to alive */
		}
		return;	/* Possible make the function more efficient */
	}
	else if (living == 2 || living == 3)
	{
		/* Cube is currently alive */
		return;	/* Possible make the function more efficient */
	}
	else
	{
		SetCubeState(location);	/*< Set state to dread */
		return;
	}
}

/**
  * Created: 21/02/2011
  * Prints out to the console all cubes, dead and alive
  */
void DebugRender(void)
{
	/* Declare counters */
	CartesianCoordinate location;
	
	printf("DEBUG RENDER\n");
	printf("------------------------------------------\n");
	for (location.z = 0; location.z < DEPTH; location.z++)
	{
		printf("[DEPTH]%i:\n", location.z);
		for (location.y = 0; location.y < WIDTH; location.y++)
		{
			printf("[WIDTH]%i:\t", location.y);
			for (location.x = 0; location.x < HEIGHT; location.x++)
			{
				if(GetCubeState(location.x, location.y, location.z) == 0)
				{
					printf("0");
				}
				else
				{
					printf("x");
				}
			}
			/* Push out a new line */
			printf("\n");
		}
		printf("------------------------------------------\n");
	}
}

/**
  * Created: 24/02/2011
  * Renders the grid in 3D
  */
void ReleaseRender()
{
	/* Declare a counter */
	CartesianCoordinate location;
	
	StartRendering();	/*< Begin rendering */
	
	/* Iterate through and draw cubes */
	for (location.z = 0; location.z < DEPTH; location.z++)
	{
		for (location.y = 0; location.y < HEIGHT; location.y++)
		{
			/* Check if any of the line is alive */
			//if (_grid[(location.z * DEPTH) + location.y].previous > 0x0)
			{
				for (location.x = 0; location.x < WIDTH; location.x++)
				{
					//if (GetCubeState(location.x, location.y, location.z) > 0x0)
					{
						RenderCube(location.x, location.y, location.z);
					}
				}
			}
		}
	}
	
	StopRendering();	/*< Finish rendering */
}

/**
  * Created: 21/02/2011
  * Initialises the array with all zeroes
  */
void CreateCubes(void)
{
	/* Declare counters */
	unsigned int i;
	
	/* Create grid on the stack */
	_grid = calloc(DEPTH * WIDTH, sizeof(LineData));
	
	for (i = 0; i < DEPTH * WIDTH; i++)
	{
		_grid[i].current = 0x0;	/* Set to all alive */
	}
	
	_grid[13].current = 0x8;	
	_grid[14].current = 0x8;	
	_grid[15].current = 0x8;	
}

/**
  * Created: 01/03/2011
  * Deletes all unmanaged memory in the CubeData.h file
  */
void DeleteCubes(void)
{
	free(_grid);
}
