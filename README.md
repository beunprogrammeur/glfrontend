# GLFrontend
GLFrontend is an Arcade frontend that makes use of OpenGL
It's in the very early stages and is heavily inspired by Hyperspin.
I just made this as an hobby project as I like making things for arcade cabinets.

## needed packages to run:
build-essential cmake libgl1-mesa-dev libglu1-mesa-dev xorg-dev

## build / install before building this project:
 * GLFW. (Can be found here: https://www.glfw.org/)
 * I will add a script to combine the packages and GLFW later.

## Directory structure
The directory structure starts at the current working directory of glfrontend.
It expects its data directory to be there.
In the data directory are at least:
 * settings.ini
 * systems/
 * shaders/

shaders is used for drawing with OpenGL, do not remove or change unless your know what you're doing.
systems contains the to be shown / emulated systems.
each system has its own folder in systems. example:
data/systems/nes.

a system folder should contain:
 * config.ini (generated if not present, but need variables to be corrected)
 * roms/
 * wheel/

optional:
 * theme.json
 * theme/ (for theme resources)

 
# theme support
There is now theme support in GLFrontend.
How to use:
a theme.json file has a single object.
The object must contain:
 * resources
 * scenes

resources is an array of objects to load into memory.
Each resource needs:
 * type (for now, only 'texture' is the only supported type)
 * name (for referencing in 'scenes', more about this later)
 * path (some resource types might not need a path. this will be changed later)
 
 Scenes are basically storyboards. 
 Each scene manipulates exactly one resource, multiple scenes can manipulate the same resource,
 but one scene can not manipulate more than one resource. 
 Scenes should always have:
 * name (for debugging and clarity)
 * resource (resource to load)
 * actions (array of actions to execute)
 
 
 Scene has one or more 'Action's that can be run in succession.
 an action must have:
  * id
  
optional:
 * time (duration in ms)
 * size (array ["x", "y"])
 * translate (array ["x", "y"])
 * next (id of the next action to execute)
 * rotation (in degrees)
 * opacity (0.0-1.0)
 
#### time 
is assumed to be 0 milliseconds if not given. this means that it will be executed once and the next action will be selected.
if time is bigger dan 0, it will be executed repeatedly (each update/frame) until the next is selected.

Note:
when time is > 0 every frame is just a small portion of the total execution time.
This means that a drawing formula will be applied. For now there is no option to select one.
the formulas are: 
 * teleport
 * linear
 
if any morphing is done (size, rotate, opacity and translate) it will happen over time.
if you want to teleport, select time = 0 and wait in the next action.

example:
        
        { "name": "linear translation", "resource": "ball", "actions": [
            {"id": "foo", "time":"500", "translate": ["100px", "100px"], "size": ["100px", "100px"], "next": "bar"},
            {"id": "bar", "time":"500", "translate": ["200px", "200px"], "next": "foo"}
        ]}

        { "name": "teleport translation", "resource": "ball", "actions": [
            {"id": "foo", "translate": ["100px", "100px"], "size": ["100px", "100px"], "next": "wait1"},
            {"id": "wait1", "time": "500", "next": "bar"},
            {"id": "bar", "translate": ["200px", "200px"], "next": "wait2"},
            {"id": "wait2", "time":"500", "next": "foo"}
        ]}

        { "name": "static draw", "resource": "ball", "actions": [
            {"id": "foo", "size": ["100px", "100px"], "translate": ["100px", "100px"], "next": "foo"}
        ]}



#### next
next is the "id" of the next action to run after it finishes.
if next is not given, its own id is assumed. this means that the sequence of
actions ends. the last one will be repeated untill the theme stops running.
id's can be circular (or partly circular)
in this case one could execute:
a, b, c, a, b, c etc. but also: a, b, c, b, c.
or when something needs to be drawn statically:
a, a, a, a etc. (this happens too when next is omitted)

#### size
size accepts a 2 dimensional array with either pixels or percentage.
if size is not given size transformation is omitted. this is useful if you dont want to change its value
in the current action. however, size is mandatory for drawable resources.
with no size given translate, angle, opacity have no use since nothing is drawn.

#### translate
translate is required for drawable resources. See 'size' for more information.

#### rotation
rotation is initialized with an angle of 0 degrees. if omitted it will not be taken into account.

#### opacity
opacity is initialized as 1.0f. the same rules as for rotation apply.


   
  
  
