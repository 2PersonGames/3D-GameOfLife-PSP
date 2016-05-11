/**
  * Author: Sam Albon
  * Created: 21/02/2011
  * Defines the struct for the grid, as well as functions related to
  * 		modifying the data related to the grid.
  */
  
#include <stdio.h>
#include "FrontEnd.h"

typedef struct
{
	/* Locations in the grid */
	unsigned char x;
	unsigned char y;
	unsigned char z;
} CartesianCoordinate;

typedef struct
{
	unsigned short current;
	unsigned short previous;
} LineData;

/**
  * Size of the grid 
  * Max values 255
  */
#define HEIGHT 16
#define WIDTH 16
#define DEPTH 16

/* Variables */
LineData* _grid;	/* 16*16*16*2 takes up 8Kb of memory on the heap */

/* Functions */
void CreateCubes(void);
void DeleteCubes(void);

/* Update functions */
unsigned char UpdateCubes(void);
void CheckNeighbours(CartesianCoordinate);
void SetCubeCurrentState(CartesianCoordinate);
void SetCubePreviousState(CartesianCoordinate);
unsigned int GetCubeState(unsigned char, unsigned char, unsigned char);

/* Render functions */
void DebugRender(void);
void ReleaseRender(void);

