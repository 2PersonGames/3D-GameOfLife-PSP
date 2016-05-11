/**
  * Author: Sam Albon
  * Created: 24/02/2011
  * Contains all static data and prototyes related to
  * rendering on the PSP display
  */

#include <libgu.h>
#include <libgum.h>
//#include <displaysvc.h>
//#include <ctrlsvc.h>
//#include <stdlib.h>
//#include <psptypes.h>
#include <kernel.h>

/* Redefine later with handrolled function if time */
#define BlockUntilFrameReady sceDisplayWaitVblankStart

unsigned char	_frame;
unsigned int 	_bgColour;
ScePspFVector3	_rotation;
float			_rotationSpeed;
#define MAXROTATIONSPEED 180

/**
  * 0 == Rotate y axis
  * 1 == Rotate x axis
  * 2 == Rotate z axis
  */
char _viewMode;
#define MAXMODE 2

/**
  * Data copied over from the Cube demo
  */
static char disp_list[0x100000] __attribute__((aligned(64)));
static ScePspFMatrix4 matrix_stack[1+1+2];
typedef struct 
{
	float x;
	float y;
	float z;
} Vertex;
static const Vertex Cube[36] = 
{
	{
		0.330000f, 0.330000f, -0.330000f,	/* 0 */
	},
	{
		0.330000f, 0.330000f, 0.330000f,	/* 1 */
	},
	{
		-0.330000f, 0.330000f, -0.330000f,	/* 2 */
	},

	{
		-0.330000f, 0.330000f, -0.330000f,	/* 2 */
	},
	{
		0.330000f, 0.330000f, 0.330000f,	/* 1 */
	},
	{
		-0.330000f, 0.330000f, 0.330000f,	/* 3 */
	},

	{
		-0.330000f, 0.330000f, -0.330000f,	/* 2 */
	},
	{
		-0.330000f,0.330000f, 0.330000f,	/* 3 */
	},
	{
		-0.330000f, -0.330000f, -0.330000f,	/* 4 */
	},

	{
		-0.330000f, -0.330000f, -0.330000f,	/* 4 */
	},
	{
		-0.330000f, 0.330000f, 0.330000f,	/* 3 */
	},
	{
		-0.330000f, -0.330000f, 0.330000f,	/* 5 */
	},

	{
		-0.330000f, 0.330000f, 0.330000f,	/* 3 */
	},
	{
		0.330000f, 0.330000f, 0.330000f,	/* 1 */
	},
	{
		-0.330000f, -0.330000f, 0.330000f,	/* 5 */
	},

	{
		-0.330000f, -0.330000f, 0.330000f,	/* 5 */
	},
	{
		0.330000f, 0.330000f, 0.330000f,	/* 1 */
	},
	{
		0.330000f, -0.330000f, 0.330000f,	/* 6 */
	},

	{
		0.330000f, 0.330000f, 0.330000f,	/* 1 */
	},
	{
		0.330000f, 0.330000f, -0.330000f,	/* 0 */
	},
	{
		0.330000f, -0.330000f, 0.330000f,	/* 6 */
	},

	{
		0.330000f, -0.330000f, 0.330000f,	/* 6 */
	},
	{
		0.330000f, 0.330000f, -0.330000f,	/* 0 */
	},
	{
		0.330000f, -0.330000f, -0.330000f,	/* 7 */
	},

	{
		0.330000f, 0.330000f, -0.330000f,	/* 0 */
	},
	{
		-0.330000f, 0.330000f, -0.330000f,	/* 2 */
	},
	{
		0.330000f, -0.330000f, -0.330000f,	/* 7 */
	},

	{
		0.330000f, -0.330000f, -0.330000f,	/* 7 */
	},
	{
		-0.330000f, 0.330000f, -0.330000f,	/* 2 */
	},
	{
		-0.330000f, -0.330000f, -0.330000f,	/* 4 */
	},

	{
		-0.330000f, -0.330000f, -0.330000f,	/* 4 */
	},
	{
		-0.330000f, -0.330000f,  0.330000f,	/* 5 */
	},
	{
		0.330000f, -0.330000f, -0.330000f,	/* 7 */
	},

	{
		0.330000f, -0.330000f, -0.330000f,	/* 7 */
	},
	{
		-0.330000f, -0.330000f, 0.330000f,	/* 5 */
	},
	{
		0.330000f, -0.330000f, 0.330000f,	/* 6 */
	}
};

inline void StartUpDisplay();
inline void StartRendering();
inline void StopRendering();
inline void RenderCube(unsigned char, unsigned char, unsigned char);
