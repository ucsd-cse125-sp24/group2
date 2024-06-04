/* GENERAL DEFINES */
// use degrees instead of radians, as rotation is in degree
# define UNIT           180.0

/* SwipeAttack Defines (SW) */
# define FULL_RANGE    (2*UNIT/3) // full attack range
# define ACTIVE_RANGE  (UNIT/3)   // attack range in effect
# define SW_ANGSPEED   (UNIT/3)
# define SW_RADIUS     500
# define SW_HEIGHT     5
# define SW_LIFE       (FULL_RANGE - ACTIVE_RANGE)/SW_ANGSPEED
# define SW_DAMAGE     5

/* StompAttack Defines (ST) */
# define WAVE_WIDTH    0.5 // in meters
# define START_RADIUS  1   // in meters
# define END_RADIUS    70  // in meters
# define ST_HEIGHT     2   // in meters
# define ST_LIFE       8   // in seconds
# define ST_SPEED      (END_RADIUS - START_RADIUS)/ST_LIFE // delta radius (m/s)
# define ST_DAMAGE     20

/* MarkedAttack Defines (M) */
# define LATENCY       3
# define M_DAMAGE      10

/* LaserAttack Defines (L) */
# define ANGRANGE      1
# define L_RADIUS      500
# define L_HEIGHT      3
# define L_ANGSPEED    (UNIT/16)
# define L_LIFE        4
# define L_DAMAGE      10