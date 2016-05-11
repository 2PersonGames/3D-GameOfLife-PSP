using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;

namespace Prototype
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager mGraphics;
        SpriteBatch mSpriteBatch;

        struct LineData
        {
            public ushort current;
            public ushort previous;
        };

        const int HEIGHT = 16;
        const int WIDTH = 16;
        const int DEPTH = 1;
        LineData[] mGrid;
        Texture2D mBlank;

        public Game1()
        {
            mGraphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            TargetElapsedTime = new TimeSpan(0, 0, 0, 0, 1000);
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

            mGrid = new LineData[16];
            for (int i = 0; i < mGrid.Length; i++)
            {
                mGrid[i].current = 0x0;
                mGrid[i].previous = 0x0;
            }

            mGrid[13].current = 0x8;
            mGrid[14].current = 0x8;
            mGrid[15].current = 0x8;

            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            mSpriteBatch = new SpriteBatch(GraphicsDevice);
            mBlank = Content.Load<Texture2D>("Blank");

            // TODO: use this.Content to load your game content here
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();
            UpdateCubes();

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.White);

            // TODO: Add your drawing code here
            Rectangle rect = new Rectangle(0, 0, 20, 20);

            mSpriteBatch.Begin();

            for (int i = 0; i < DEPTH; i++)
            {
                for (int j = 0; j < WIDTH; j++)
                {
                    for (int k = 0; k < HEIGHT; k++)
                    {
                        Color colour;
                        rect.X = k * (rect.Width + 1);
                        rect.Y = j * (rect.Height + 1);
                        if (GetCubeState(new Vector3(k, j, i)) != 0)
                        {
                            colour = Color.DarkBlue;
                        }
                        else
                        {
                            colour = Color.DarkRed;
                        }
                        mSpriteBatch.Draw(mBlank, rect, colour);
                    }
                }
            }

            mSpriteBatch.End();

            base.Draw(gameTime);
        }

        /* Tools ported straight from C */
        UInt16 UpdateCubes()
        {	
	        /* Declare the counters */
	        Vector3 location = new Vector3();
	
	        /**
	          * TODO:
	          * Force the PSP to cache my array
	          * Not sure if this is possible, but the entire array should fit in the cache
	          */

            /* Archive all working states */
            for (location.Z = 0; location.Z < DEPTH; location.Z++)
            {
                for (location.Y = 0; location.Y < WIDTH; location.Y++)
                {
                    mGrid[(int)((location.Z * DEPTH) + location.Y)].previous = mGrid[(int)((location.Z * DEPTH) + location.Y)].current;
                }
            }
	
	        /* Iterate through the grid */
	        for (location.Z = 0; location.Z < DEPTH; location.Z++)
	        {
		        for (location.Y = 0; location.Y < WIDTH; location.Y++)
		        {
			        for (location.X = 0; location.X < HEIGHT; location.X++)
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
        ushort GetCubeState(Vector3 location)
        {
	        /* EFFICIENCY OUT LATER: Checking too slow */
	        //if (location.X < WIDTH && location.Y < HEIGHT && location.Z < DEPTH &&
            //    location.X >= 0 && location.Y >= 0 && location.Z >= 0)
	        {
		        /* KEEP THIS BLOCK: Below */
                ushort mask = 0x1;						/* Creates a mask of the value ...0001 */
		        mask <<= (ushort)(location.X);					/* Shifts the mask along to meet the value required */
		        return (ushort)(mGrid[(int)((location.Z * DEPTH) + location.Y)].previous & mask);	/* Masks the line and returns the result */
		        /* KEEP THIS BLOCK: Above */
	        }
	        //else
	        {
		        return 0x0;	/* False */
	        }
        }

        /**
          * Created: 24/02/2011
          * Inverts the state of the cube at that location in the grid
          * Inline for efficiency
          */
        void SetCubeState(Vector3 location)
        {
            ushort mask = 0x1;					/* Creates a mask of the value ...0001 */
	        mask <<= (ushort)(location.X);					/* Shifts the mask along to meet the value being set */
	        mGrid[(int)((location.Z * DEPTH) + location.Y)].current ^= mask;		/* Masks the line to invert the coordinate's state */
        }

        /**
          * Created: 24/02/2011
          * Checks the neighbours of a cube at that location and applies the GOL rules
          * Needs changing for efficiency
          * Inline for efficiency
          */
        void CheckNeighbours(Vector3 location)
        {
	        /* Declare counters */
	        sbyte i;
	        byte living;
            sbyte z;
	
	        /* Iterate through the neighbours */
	        living = 0;
            for (i = -1; i < 2; i++)
            {
                /* Test Z axis bounds */
                z = (sbyte)(location.Z + i);
                if (z >= 0 && z < DEPTH)
                {
                    /* Test X axis bounds */
                    if (location.X > 0)         /*< Left */
                    {
                        if (location.Y > 0 && GetCubeState(new Vector3(location.X - 1, location.Y - 1, z)) > 0) /*< Top left */
                        {
                            living++;
                        }
                        if (GetCubeState(new Vector3(location.X - 1, location.Y, z)) > 0) /*< Middle left */
                        {
                            living++;
                        }
                        if (location.Y < WIDTH - 1 && GetCubeState(new Vector3(location.X - 1, location.Y + 1, z)) > 0) /*< Bottom left */
                        {
                            living++;
                        }
                    }
                    if (location.X < HEIGHT - 1)    /*< Right */
                    {
                        if (location.Y > 0 && GetCubeState(new Vector3(location.X + 1, location.Y - 1, z)) > 0) /*< Top right */
                        {
                            living++;
                        }
                        if (GetCubeState(new Vector3(location.X + 1, location.Y, z)) > 0) /*< Middle left */
                        {
                            living++;
                        }
                        if (location.Y < WIDTH - 1 && GetCubeState(new Vector3(location.X + 1, location.Y + 1, z)) > 0) /*< Bottom right */
                        {
                            living++;
                        }
                    }
                    /* Test Y axis */
                    if (location.Y > 0 && GetCubeState(new Vector3(location.X, location.Y - 1, z)) > 0)
                    {
                        living++;
                    }
                    if (location.Y < WIDTH - 1 && GetCubeState(new Vector3(location.X, location.Y + 1, z)) > 0)
                    {
                        living++;
                    }
                    if (z != location.Z && GetCubeState(new Vector3(location.X, location.Y, z)) > 0)
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
            if (GetCubeState(location) == 0)    /*< Cube is currently dead */
	        {
                if (living == 3)
                {
                    SetCubeState(location);	/* Set state to alive */
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
                SetCubeState(location); /*< Set state to dead */
                return;
            }
        }
    }
}
