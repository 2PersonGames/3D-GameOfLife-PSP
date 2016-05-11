/**
  * Author: Sam Albon
  * Created: 24/02/2011
  * Defines all the functions related to rendering
  * on the PSP display
  */
  
#include "FrontEnd.h"

/**
  * Created: 24/02/2011
  * Initialises the PSP display for rendering
  */
inline void StartUpDisplay()
{
	//ScePspFVector3	vec;

	_rotationSpeed = 1.0f;
	_rotation.x = 0.0f;
	_rotation.y = 0.0f;
	_rotation.z = 0.0f;
	_bgColour = 0;
	_frame = 0;
	_viewMode = 0;
	sceGuInit();
	sceGuStart(SCEGU_IMMEDIATE, (void *)disp_list, sizeof(disp_list));

	sceGuDrawBuffer(SCEGU_PF5551, SCEGU_VRAM_BP_0, SCEGU_VRAM_WIDTH);
	sceGuDispBuffer(SCEGU_SCR_WIDTH, SCEGU_SCR_HEIGHT, SCEGU_VRAM_BP_1,
		SCEGU_VRAM_WIDTH);
	//sceGuDepthBuffer(SCEGU_VRAM_BP_2, SCEGU_VRAM_WIDTH);

	sceGuOffset(SCEGU_SCR_OFFSETX, SCEGU_SCR_OFFSETY);
	sceGuViewport(2048, 2048, SCEGU_SCR_WIDTH, SCEGU_SCR_HEIGHT);
	//sceGuDepthRange(50000, 10000);

	//sceGuDisable(SCEGU_BLEND);

	sceGuScissor(0, 0, SCEGU_SCR_WIDTH, SCEGU_SCR_HEIGHT);
	sceGuEnable(SCEGU_SCISSOR_TEST);

	//sceGuDepthFunc(SCEGU_GEQUAL);
	//sceGuEnable(SCEGU_DEPTH_TEST);

	//sceGuFrontFace(SCEGU_CW);

	//sceGuDisable(SCEGU_TEXTURE);
	//sceGuShadeModel(SCEGU_SMOOTH);

	//sceGuEnable(SCEGU_DITHER);

	//vec.x = 0.0f;
	//vec.y = 0.0f;
	//vec.z = 1.0f;
	//sceGuLightColor(0, SCEGU_DIFFUSE, 0xffffffff);
	//sceGuLight(0, SCEGU_LIGHT_DIRECTION, SCEGU_DIFFUSE, &vec);
	//vec.x = 1.0f;
	//vec.y = 0.0f;
	//vec.z = 0.0f;
	//sceGuLightColor(1, SCEGU_DIFFUSE, 0xff00ffff);
	//sceGuLight(1, SCEGU_LIGHT_DIRECTION, SCEGU_DIFFUSE, &vec);
	//vec.x = -1.0f;
	//vec.y = 0.0f;
	//vec.z = 0.0f;
	//sceGuLightColor(2, SCEGU_DIFFUSE, 0xffff00ff);
	//sceGuLight(2, SCEGU_LIGHT_DIRECTION, SCEGU_DIFFUSE, &vec);
	//sceGuEnable(SCEGU_LIGHT0);
	//sceGuEnable(SCEGU_LIGHT1);
	//sceGuEnable(SCEGU_LIGHT2);
	//sceGuEnable(SCEGU_LIGHTING);

	//sceGuFog(14.50, 25.0, 0x007f00ff);

	//sceGuAmbient(0xcc004cb2);
	sceGuAmbient(0xffffffff);
	sceGuColor(0x00ff00ff);

	//sceGuTexMode(SCEGU_PF5551, 1, SCEGU_SINGLE_CLUT, SCEGU_TEXBUF_NORMAL);
	//sceGuTexFilter(SCEGU_LINEAR_MIPMAP_NEAREST, SCEGU_LINEAR);
	//sceGuTexFunc(SCEGU_TEX_MODULATE, SCEGU_RGB);
	//sceGuTexWrap(SCEGU_REPEAT, SCEGU_REPEAT);

	sceGumSetMatrixStack(matrix_stack, 1, 1, 2, 0);
	sceGumMatrixMode(SCEGU_MATRIX_PROJECTION);
	sceGumLoadIdentity();
	sceGumPerspective(SCEGU_RAD(45.0f), SCEGU_SCR_ASPECT,
		1.000000f, 100.000000f);

	sceGumMatrixMode(SCEGU_MATRIX_WORLD);

	sceGuClearDepth(0);
	sceGuClearStencil(0);

	sceGuFinish();
	sceGuSync(SCEGU_SYNC_FINISH, SCEGU_SYNC_WAIT);

	sceGuDisplay(SCEGU_DISPLAY_ON);
}

/**
  * Created 24/02/2011
  * Prepares and clears the PSP display for rendering
  */
inline void StartRendering()
{
	//printf("FrontEnd::StartRendering\n");
	
	/* Increment the frame */
	if (_frame >= 0xff)
	{
		_frame = 0;
	}
	else
	{
		++_frame;
	}
	
	BlockUntilFrameReady();	/*< Blocks the program until the frame is ready */
	
	/* Preps the display for rendering */
	sceGuSwapBuffers();
	sceGuStart(SCEGU_IMMEDIATE, (void*)disp_list, sizeof(disp_list));
	sceGuClearColor(_bgColour);
	sceGuClear(SCEGU_CLEAR_ALL);
}

/**
  * Created 24/02/2011
  * Finishes rendering
  */
inline void StopRendering()
{
	sceGumLoadIdentity();
	sceGuFinish();
	sceGuSync(SCEGU_SYNC_FINISH, SCEGU_SYNC_WAIT);
	//printf("FrontEnd::StopRendering\n");
}

/**
  * Created: 01/03/2011
  * Renders a cube to the demo space according to it's game location
  */
inline void RenderCube(unsigned char x, unsigned char y, unsigned char z)
{
	ScePspFVector3 vec;
	//printf("FrontEnd::RenderCube() started\n");
	
	vec.x = -(float)x;
	vec.y = -(float)y;
	vec.z = -(float)z;
	sceGumTranslate(&vec);
	
	sceGumDrawArray(SCEGU_PRIM_TRIANGLES, SCEGU_VERTEX_FLOAT, 36, 0, Cube);
}
