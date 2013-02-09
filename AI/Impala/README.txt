Impala - The Path Smoother - How it works

The raw output from the A* algorithm can often appear to have a zig-zag effect when moving in a non-axis-aligned way. This is ultimately due to the destination being at a non-cardinal angle, relative to the robot's current position, so it can't travel there just going in one direction. Unfortunately, this is likely to cause small, jerky movements for the robot as it tries to emulate the path.

Imapala tries to smooth the path by eliminating these zig-zags and turning hard corners into arcs, using a triangle centroid method. For each point in the path, a triangle is created with the three corners being the point itself, the one immediately preceding it in the path and the one immediately following it. We then find the centroid of the triangle (the point at which the three lines bisecting its angles and going through the middle of each side meet), and the new path is constructed out of these.

Impala also will try to collapse the path into fewer points (if several sequential points are on the same gradient, they are collapsed), making it simpler for Navigation.
