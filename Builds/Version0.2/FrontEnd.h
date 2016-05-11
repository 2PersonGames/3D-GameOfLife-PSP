/**
  * Author: Sam Albon
  * Created: 24/02/2011
  * Contains all static data and prototyes related to
  * rendering on the PSP display
  */

#include <libgu.h>
#include <libgum.h>
#include <displaysvc.h>
#include <ctrlsvc.h>
#include <stdlib.h>
#include <psptypes.h>
#include <kernel.h>
#include <allegrex.h>

/* Redefine later with handrolled function if time */
#define BlockUntilFrameReady sceDisplayWaitVblankStart

unsigned char	_frame;
unsigned int 	_bgColour;

/**
  * 0 == Rotate y axis
  * 1 == Rotate x axis
  * 2 == Rotate z axis
  */
unsigned char	_viewMode;
#define MAXMODE 2

/**
  * Data copied over from the Cube demo
  */
static char disp_list[0x10000] __attribute__((aligned(64)));
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
		0.250000f, 0.250000f, -0.250000f,	/* 0 */
	},
	{
		0.250000f, 0.250000f, 0.250000f,	/* 1 */
	},
	{
		-0.250000f, 0.250000f, -0.250000f,	/* 2 */
	},

	{
		-0.250000f, 0.250000f, -0.250000f,	/* 2 */
	},
	{
		0.250000f, 0.250000f, 0.250000f,	/* 1 */
	},
	{
		-0.250000f, 0.250000f, 0.250000f,	/* 3 */
	},

	{
		-0.250000f, 0.250000f, -0.250000f,	/* 2 */
	},
	{
		-0.250000f,0.250000f, 0.250000f,	/* 3 */
	},
	{
		-0.250000f, -0.250000f, -0.250000f,	/* 4 */
	},

	{
		-0.250000f, -0.250000f, -0.250000f,	/* 4 */
	},
	{
		-0.250000f, 0.250000f, 0.250000f,	/* 3 */
	},
	{
		-0.250000f, -0.250000f, 0.250000f,	/* 5 */
	},

	{
		-0.250000f, 0.250000f, 0.250000f,	/* 3 */
	},
	{
		0.250000f, 0.250000f, 0.250000f,	/* 1 */
	},
	{
		-0.250000f, -0.250000f, 0.250000f,	/* 5 */
	},

	{
		-0.250000f, -0.250000f, 0.250000f,	/* 5 */
	},
	{
		0.250000f, 0.250000f, 0.250000f,	/* 1 */
	},
	{
		0.250000f, -0.250000f, 0.250000f,	/* 6 */
	},

	{
		0.250000f, 0.250000f, 0.250000f,	/* 1 */
	},
	{
		0.250000f, 0.250000f, -0.250000f,	/* 0 */
	},
	{
		0.250000f, -0.250000f, 0.250000f,	/* 6 */
	},

	{
		0.250000f, -0.250000f, 0.250000f,	/* 6 */
	},
	{
		0.250000f, 0.250000f, -0.250000f,	/* 0 */
	},
	{
		0.250000f, -0.250000f, -0.250000f,	/* 7 */
	},

	{
		0.250000f, 0.250000f, -0.250000f,	/* 0 */
	},
	{
		-0.250000f, 0.250000f, -0.250000f,	/* 2 */
	},
	{
		0.250000f, -0.250000f, -0.250000f,	/* 7 */
	},

	{
		0.250000f, -0.250000f, -0.250000f,	/* 7 */
	},
	{
		-0.250000f, 0.250000f, -0.250000f,	/* 2 */
	},
	{
		-0.250000f, -0.250000f, -0.250000f,	/* 4 */
	},

	{
		-0.250000f, -0.250000f, -0.250000f,	/* 4 */
	},
	{
		-0.250000f, -0.250000f,  0.250000f,	/* 5 */
	},
	{
		0.250000f, -0.250000f, -0.250000f,	/* 7 */
	},

	{
		0.250000f, -0.250000f, -0.250000f,	/* 7 */
	},
	{
		-0.250000f, -0.250000f, 0.250000f,	/* 5 */
	},
	{
		0.250000f, -0.250000f, 0.250000f,	/* 6 */
	}
};

inline void StartUpDisplay();
inline void StartRendering();
inline void StopRendering();
inline void RenderCube(unsigned char, unsigned char, unsigned char);
