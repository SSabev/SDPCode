Our A* - how it works:

Current approx. speed (with ~30K waypoints - one per cm^2 of pitch space): ~45hz, when navigating from one extreme of the pitch to the other.

THINGS SPECIFIC TO THIS IMPLEMENTATION

* Heuristic penalty. I've added a penalty to the heuristic measure. This is because due to the nature of the task, a lot of waypoints were being expanded unnecessarily, especially given that there are very few obstacles on the pitch. This penalty makes the algorithm less likely to backtrack. The speed increase from this is a factor of several hundred.

I have several other optimisations which I'd like to do and ultimately, I'd like it to be running ~200Hz for most cases. Note that this won't be the overall speed of the AI as other components will be involved. It'll also likely be impacted as the algorithm becomes more domain-specific, but I still think it's worth having something to aim for.

FUTURE OPTIMISATIONS

* Early-terminated paths. Since we're recalculating the path every frame at the moment, for all but the very shortest paths, the robot won't reach its ultimate destination before the path is recalculated. This makes a lot of the calculation redundant. As such, the path could be calculated up until a certain point (e.g. 20cm) and then terminated. Unless the robot is near to an obstacle, the straightest route will be optimal and this early-terminated path will be fine.

* Dynamic resolution changing. A high density of waypoints won't necessarily be needed if the robot's in open space. As such, the resolution of the waypoints could be reduced to speed up calculation, and then raised again when the robot's near the ball or an enemy robot and the added precision is a necessity.
