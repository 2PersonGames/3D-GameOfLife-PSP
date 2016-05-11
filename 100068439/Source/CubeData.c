/**
  * Author: Sam Albon
  * Created: 21/02/2011
  * Defines functions related to modifying the data related to the grid.
  */

#include "CubeData.h"

#pragma inline
unsigned char UpdateCubes(void)
{	
	/* Declare the counters */
	CartesianCoordinate location;
	
	if (_updateSpeed > MAXUPDATESPEED || _frame % _updateSpeed != 0)
	{
		return 0;
	}

	/* Load the grid into cache */
	for (location.z = 0; location.z < DEPTH; ++location.z)
	{
		for (location.y = 0; location.y < WIDTH; ++location.y)
		{
			//__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_FILL_I, &_grid[(location.z * DEPTH) + location.y].previous);
			__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_FILL_WITH_LOCK_I, &_grid[(location.z * DEPTH) + location.y].previous);
		}
	}
	
	/* Iterate through the grid */
	for (location.z = 0; location.z < DEPTH; ++location.z)
	{
		for (location.y = 0; location.y < WIDTH; ++location.y)
		{
			for (location.x = 0; location.x < HEIGHT; ++location.x)
			{
				CheckNeighbours(location);
			}
		}
	}
	
	/* Archive all working states */
	for (location.z = 0; location.z < DEPTH; ++location.z)
	{
		for (location.y = 0; location.y < WIDTH; ++location.y)
		{
			_grid[(location.z * DEPTH) + location.y].previous = _grid[(location.z * DEPTH) + location.y].current;
		}
	}
	
	/* Force a write back */
	for (location.z = 0; location.z < DEPTH; ++location.z)
	{
		for (location.y = 0; location.y < WIDTH; ++location.y)
		{
			//__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_INDEX_WRITEBACK_INVALIDATE_D, &_grid[(location.z * DEPTH) + location.y].previous);
			__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_INDEX_UNLOCK_I, &_grid[(location.z * DEPTH) + location.y].previous);
		}
	}

	/* Return successful */
	return 0;
}

/**
  * Created: 24/02/2011
  * Returns a boolean of alive
  * Needs changing for efficiency
  * Inline for efficiency
  */
#pragma inline
unsigned int GetCubeState(unsigned char x, unsigned char y, unsigned char z)
{
	unsigned int mask = 0x1;	/*< Creates a mask of the value ...0001 */
	mask <<= x;		/*< Shifts the mask along to meet the value required */
	
	/* Masks the line and returns the result */
	return _grid[(z * DEPTH) + y].previous & mask;
}

/**
  * Created: 24/02/2011
  * Inverts the state of the cube at that location in the grid
  * Inline for efficiency
  */
#pragma inline
void SetCubeState(CartesianCoordinate location)
{
	unsigned int mask = 0x1;	/* Creates a mask of the value ...0001 */
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
#pragma inline
void CheckNeighbours(CartesianCoordinate location)
{
	/* Declare counters */
	char z;
	char i;
	unsigned char living;
	
	/* Iterate through the neighbours */
	living = 0;
	for (i = -1; i < 2; ++i)
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
					++living;
				}
				/* Middle Left */
				if (GetCubeState(location.x - 1, location.y, z) > 0)
				{
					++living;
				}
				/* Bottom Left */
				if (location.y < WIDTH - 1 && GetCubeState(location.x - 1, location.y + 1, z) > 0)
				{
					++living;
				}
			}
			/* Test x axis bounds */
			if (location.x < HEIGHT - 1)	/*< Right */
			{
				if (location.y > 0 && GetCubeState(location.x + 1, location.y - 1, z) > 0)
				{
					++living;
				}
				if (GetCubeState(location.x + 1, location.y, z) > 0)
				{
					++living;
				}
				if (location.y < WIDTH - 1 && GetCubeState(location.x + 1, location.y + 1, z) > 0)
				{
					++living;
				}
			}
			/* Test y axis bounds */
			if (location.y > 0 && GetCubeState(location.x, location.y - 1, z) > 0)
			{
				++living;
			}
			if (location.y < WIDTH - 1 && GetCubeState(location.x, location.y + 1, z) > 0)
			{
				++living;
			}
			if (z != location.z && GetCubeState(location.x, location.y, z))
			{
				++living;
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
  * Created: 08/03/2011
  *	Checks the PSP controls and applies them as defined
  */
#pragma inline
void CheckControls()
{
	/* Get the control buffer data */
	SceCtrlData input;
	sceCtrlReadBufferPositive(&input, 1);
	
	/* Test the shoulders */
	if (TESTCONTROLS(SCE_CTRL_L))
	{
		/* L button pressed */
		//printf("L Button Pressed Exclusively\n");
		if (_viewMode != 0)
		{
			--_viewMode;
		}
		else
		{
			_viewMode = MAXMODE;
		}
	}
	else if (TESTCONTROLS(SCE_CTRL_R))
	{
		/* R button pressed */
		//printf("R Button Pressed Exclusively\n");
		++_viewMode;
		if (_viewMode > MAXMODE)
		{
			_viewMode = 0;
		}
	}
	else if (TESTCONTROLS(SCE_CTRL_SELECT))
	{
		/* Pause rotation */
		if (_viewMode > MAXMODE)
		{
			_viewMode -= 1 + MAXMODE;
		}
		else
		{
			_viewMode += 1 + MAXMODE;
		}
	}
	else if (TESTCONTROLS(SCE_CTRL_START))
	{
		/* Pause updates */
		if (_updateSpeed > MAXUPDATESPEED)
		{
			_updateSpeed -= MAXUPDATESPEED;
		}
		else
		{
			_updateSpeed += MAXUPDATESPEED;
		}
	}
	else if (TESTCONTROLS(SCE_CTRL_LEFT))
	{
		/* Rotation speed down */
		if (_rotationSpeed > 1)
		{
			--_rotationSpeed;
		}
	}
	else if (TESTCONTROLS(SCE_CTRL_RIGHT))
	{
		/* Rotation speed up */
		if (_rotationSpeed <= MAXROTATIONSPEED)
		{
			++_rotationSpeed;
		}
	}
	else if (TESTCONTROLS(SCE_CTRL_UP))
	{
		/* Update speed up */
		if (_updateSpeed > 1)
		{
			--_updateSpeed;
		}
	}
	else if (TESTCONTROLS(SCE_CTRL_DOWN))
	{
		/* Update speed down */
		if (_updateSpeed <= MAXUPDATESPEED)
		{
			++_updateSpeed;
		}
	}
	else if (TESTCONTROLS(SCE_CTRL_TRIANGLE))
	{
		DeleteCubes();
		CreateCubes(1);
	}
	else if (TESTCONTROLS(SCE_CTRL_SQUARE))
	{
		DeleteCubes();
		CreateCubes(2);
	}
	else if (TESTCONTROLS(SCE_CTRL_CIRCLE))
	{
		DeleteCubes();
		CreateCubes(3);
	}
	else if (TESTCONTROLS(SCE_CTRL_CROSS))
	{
		DeleteCubes();
		CreateCubes(4);
	}
	
	_previousController = input;
}

/**
  * Created: 24/02/2011
  * Renders the grid in 3D
  */
#pragma inline
void ReleaseRender()
{
	/* Declare variables */
	unsigned char isAlive;			/*< Whether the game is still alive */
	CartesianCoordinate location;	/*< Declare a counter */
	unsigned int colour;			/*< Colour to render the cube as */
	ScePspFVector3 translation;		/*< Declare a vector for translation */
	ScePspFMatrix4 worldRotation;	/*< World rotation matrix */

	CheckControls();
	
	/* Instantiate variables */
	colour = 0xff000000;
	isAlive = 0x0;
	
	/* Update the rotation */
	switch (_viewMode)
	{
		case -1:
			snprintf(disp_list, sizeof(disp_list), 
			"Conway's Game of Life in 3D\nSam Albon\n100068439\n\nPress:\nTriangle\nSquare\nCircle\nCross");
			sceGuDebugPrint(43, 46, 0xffffffff, disp_list);
			//printf("CubeData::ReleaseRender print to screen\n");
			break;
		case 0:
			_rotation.x += SCEGU_RAD(_rotationSpeed);
			if (_rotation.x >= SCEGU_RAD(360.0f))
			{
				_rotation.x = 0;
			}		
			break;
		case 1:
			_rotation.y += SCEGU_RAD(_rotationSpeed);
			if (_rotation.y >= SCEGU_RAD(360.0f))
			{
				_rotation.y = 0;
			}
			break;
		case 2:
			_rotation.z += SCEGU_RAD(_rotationSpeed);
			if (_rotation.z >= SCEGU_RAD(360.0f))
			{
				_rotation.z = 0;
			}
			break;
	}
	
	/* Create the world rotation matrix and store it */
	sceGumPushMatrix();
	sceGumLoadIdentity();
	sceGumRotateXYZ(&_rotation);
	sceGumStoreMatrix(&worldRotation);
	sceGumPopMatrix();
	
	//StartRendering();	/*< Begin rendering */
	
	/* Translate the screen */
	translation.x = WIDTH * 0.5f;
	translation.y = HEIGHT * 0.5f;
	translation.z = -25.0f;
	sceGumTranslate(&translation);
	StartRendering();
	
	/* Calculate the cube translation */
	translation.z = DEPTH * 0.5f;
	
	/* Iterate through and draw cubes */
	for (location.z = 0; location.z < DEPTH; ++location.z)
	{	
		/* Clear blue */
		colour = 0xff000000;	/*< Wipe RGB clean */
		colour |= (__builtin_allegrex_round_w_s(((float)location.z / DEPTH) * 255.0f)) << 16;	/* Blue component */
		//colour |= ((unsigned int)(((float)location.z / DEPTH) * 255)) << 16;
				
		for (location.y = 0; location.y < HEIGHT; ++location.y)
		{
			/* Preload the line */
			//__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_FILL_I, &_grid[(location.z * DEPTH) + location.y].previous);
		
			/* Check if any of the line is alive */
			if (_grid[(location.z * DEPTH) + location.y].previous > 0x0)
			{
				/* Clear green */
				colour &= 0xffff0000;	/*< Wipe RG clean */
				colour |= (__builtin_allegrex_round_w_s(((float)location.y / HEIGHT) * 255.0f)) << 8;	/* Green component */
				//colour |= ((unsigned int)(((float)location.y / HEIGHT) * 255)) << 8;
				
				for (location.x = 0; location.x < WIDTH; ++location.x)
				{
					if (GetCubeState(location.x, location.y, location.z) > 0x0)
					{
						sceGumPushMatrix();
						
						translation.x = -translation.x;
						translation.y = -translation.y;
						translation.z = -translation.z;
						sceGumTranslate(&translation);
						
						/* Set up the camera matrices */
						//sceGumRotateXYZ(&_rotation);
						sceGumMultMatrix(&worldRotation);
						
						translation.x = -translation.x;
						translation.y = -translation.y;
						translation.z = -translation.z;
						sceGumTranslate(&translation);	
						
						/* Clear red */
						colour &= 0xffffff00;	/*< Wipe R clean */
						colour |= (__builtin_allegrex_round_w_s(((float)location.x / WIDTH) * 255.0f));
						//colour |= ((unsigned int)(((float)location.x / WIDTH) * 255));	/*< Red component */						
						sceGuColor(colour);
						
						RenderCube(location.x, location.y, location.z);
						
						sceGumPopMatrix();
					}
				}
				isAlive = 0x1;
			}
		}
	}
	
	StopRendering();	/*< Finish rendering */
	
	/* Check if the game is still alive */
	if (isAlive == 0x0)
	{
		_viewMode = -1;
		//printf("CubeData::Release render _viewMode set to -1");
	}
}

/**
  * Created: 21/02/2011
  * Initialises the array with all zeroes
  */
#pragma inline
void CreateCubes(unsigned char mode)
{
	/* Declare counters */
	unsigned int i;
	
	/* Create grid on the stack */
	i = DEPTH * WIDTH;
	_grid = calloc(i, sizeof(LineData));
	
	while (i > 0)
	{	/* Decrement the counter */
		--i;
		
		_grid[i].current = 0x0;	/* Set to all alive */		
		
		/**
		  * Force the PSP to cache my array
		  * Not sure if this is possible, but the entire array should fit in the cache
		  */
		//__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_FILL_WITH_LOCK_I, &_grid[i]);
	}	
	
	switch (mode)
	{
		/* Creates a pulsar from an exploder */
		case 1:
			_grid[6].current = 0x540;
			_grid[7].current = 0x440;
			_grid[8].current = 0x440;
			_grid[9].current = 0x440;
			_grid[10].current = 0x540;
			break;
			
		/* Creates a beacon */
		case 2:
			for (i = 0; i < DEPTH; ++i)
			{
				_grid[i].current = 0x1;
			}
			
			for (i = (DEPTH * WIDTH) - 1; i > DEPTH * (WIDTH - 1); --i)
			{
				_grid[i].current = 0x1;
			}
			break;
		
		case 3:
			for (i = 0; i < DEPTH * WIDTH; i += 2)
			{
				_grid[i].current = 0xaaaa;
			}
			break;
			
		case 4:
			for (i = 0; i < DEPTH * WIDTH; ++i)
			{
				_grid[i].current = 0xffff;
			}
			break;
		default:
			/* Set the default update speed to one per second */
			_updateSpeed = MAXUPDATESPEED;				
			break;
	}
	
	i = DEPTH * WIDTH;
	while (i > 0)
	{
		--i;
		_grid[i].previous = _grid[i].current;
	}
	
	/* Set the default rotations */
	_rotation.x = 0.0f;
	_rotation.y = 0.0f;
	_rotation.z = 0.0f;
	_viewMode = 1;
	//printf("CubeData::CreateCubes finished");
}

/**
  * Created: 01/03/2011
  * Deletes all unmanaged memory in the CubeData.h file
  */
#pragma inline
void DeleteCubes(void)
{
	unsigned int i;
	i = DEPTH * WIDTH;

	while (i > 0)
	{	/* Decrement the counter */
		--i;	
		
		/**
		  * Force the PSP to cache my array
		  * Not sure if this is possible, but the entire array should fit in the cache
		  */
		__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_INDEX_UNLOCK_I, &_grid[i]);
	}	

	free(_grid);
}
