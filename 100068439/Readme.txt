Author:		Sam Albon, 100068439
Created:	18/03/2011
Title:		Conway's Game of Life in 3D

Load the .prx file, not .elf if there is one as the .elf does not run correctly.
You should be presented with a welcome screen, telling you to press either:
Cross, Triangle, Square and Circle

The demo should default to one iteration per second.  The start types which are shown as "stable"
are stable as far as I have tested, they may die after several thousand iterations, but I haven't
tested them for fully.

Controls:

* Triangle ~	Loads 2D exploder (stable)
* Square ~	Loads two vertical lines (stable)
* Circle ~	Loads Each other horizontal line (stable)
* Cross ~	Loads every cube (dies after one iteration)

* L Shoulder ~	Cycle rotation type backwards
* R Shoulder ~	Cycle rotation type forwards

* Up DPad ~	Increase the number of iterations a second
* Down DPad ~	Decrease the number of iterations a second
* Right DPad ~	Increase the rotation speed
* Left DPad ~	Decrease the rotation speed

* Select ~	Pause rotations
* Start ~	Pause iterations