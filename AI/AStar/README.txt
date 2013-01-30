Our A* - how it works:

Current approx. speed (with ~30K waypoints - one per cm^2 of pitch space): ~45hz, when navigating from one extreme of the pitch to the other.

THINGS SPECIFIC TO THIS IMPLEMENTATION

* I've added a penalty to the heuristic measure. This is because due to the nature of the task, a lot of waypoints were being expanded unnecessarily, especially given that there are very few obstacles on the pitch. This penalty makes the algorithm less likely to backtrack. The speed increase from this is a factor of several hundred.

I have several other optimisations which I'd like to do and ultimately, I'd like it to be running ~200Hz for most cases. Note that this won't be the overall speed of the AI as other components will be involved. It'll also likely be impacted as the algorithm becomes more domain-specific, but I still think it's worth having something to aim for.
