This is the AI for our bot. It is currently broken down into four separate units:

Foresee - Attempts to work against the delays in data coming from the vision unit and ultimately being sent to the robot. Does this using linear extrapolation.

Eagle - Decides the high-level action to take. Do we want to get behind the ball? Or do we want to get back to our goal and defend? etc. What position do we need to be in?

AStar - Find a path from the robot's current position to the desired destination identified by Eagle.

Impala - Optimises the path found by the AStar, based on knowledge of our robots's capabilities.

=========================================================================================

The AI as a whole should be given (or retrieve) data [mostly] produced by the vision module and stored in the shared memory. This data should be consist of:

The position and orientation of our robot.
The position and orientation of the opposition robot
The position of the ball
The game state (e.g. penalty, open gameplay, stop)

In addition, when the AI is initialised, it should receive the co-ordinates of the corners of the pitch. From this, it can discern where on the pitch the various entities are.

=========================================================================================

DOCUMENTATION

All non-trivial methods in the AI are documented. This documentation can either be read inline with the code or by running doxygen, using the following commands:

doxygen
cd docs
xdg-open index.html

=========================================================================================

COMPILING

The AI can be compiled as a standalone binary, however this is only for debug and development purposes. In practice, it should be included in and compiled as part of the GUI binary.

The libs can and should be configured and compiled by executing:

./compile-libs.sh

However, if you do wish to compile the AI directly, a premake4.lua file is included to help you with this. It can be used to generate make files or projects for various IDEs, e.g.

./get_premake4.sh
./premake4 gmake
./premake4 codelite

=========================================================================================

UNIT TESTING

The AI comes with certain unit tests, using the included library cpptest. You can run these tests as follows:

./get_premake4.sh
./compile-libs.sh
./premake4 gmake
make config=test
cd test
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/group/teaching/sdp/sdp8/SDPCode/AI/libs"
./SDP-AI

Note that the 'export' call will need to be made each time you want to run the program in a new terminal, and is a temporary fix until I can get back to DICE and do something proper.
