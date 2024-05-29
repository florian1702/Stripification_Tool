#ifndef GLOBALDEFS_INCLUDED
#define GLOBALDEFS_INCLUDED

#ifndef M_PI
#define M_PI (3.1415)
#endif

#define EPSILON (0.000001)

#define CLAMP(a,b,c) { b = ((b) < (a))? (a) : (((b) > (c))? (c): (b));}

#define XDIM (16)
#define YDIM (16)

#define GRAVITY_ACC (0.981)

#define SHEER_SPRING_CONSTANT      (8.0)
#define STRUCTURAL_SPRING_CONSTANT (8.0)
#define BENDING_SPRING_CONSTANT    (8.0)
#define FORCE_DAMPING			   (0.1)
#define SPRING_DAMPING			   (0.2)

#define COLLISION_SPHERE_RADIUS    (0.5)
#define COLLIDE_WITH_SPHERE 

#define NUM_ITERATIONS (10)
#endif