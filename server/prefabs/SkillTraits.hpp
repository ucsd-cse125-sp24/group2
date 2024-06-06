/* GENERAL DEFINES */
// use degrees instead of radians, as rotation is in degree
#define UNIT 180.0
#define DEBUG_ST false

/* SwipeAttack Defines (SW) */
#define FULL_RANGE (2 * UNIT / 3) // full attack range
#define ACTIVE_RANGE (UNIT / 3)   // attack range in effect
#define SW_ANGSPEED (UNIT / 3)
#define SW_RADIUS 500
#define SW_HEIGHT 5
#define SW_LIFE (FULL_RANGE - ACTIVE_RANGE) / SW_ANGSPEED
#define SW_DAMAGE 10

/* StompAttack Defines (ST) */
// TODO: test width and height so it's dodgable by jumping
// TODO: unitless player hitbox radius is 30
#define WAVE_WIDTH 100   // TODO: not meters - changed for collision testing
#define START_RADIUS 1   // in meters
#define END_RADIUS 10000 // TODO: not meters - changed for collision testing
#define ST_HEIGHT 0.5    // in meters
#define ST_LIFE 8        // in seconds
#define ST_SPEED (END_RADIUS - START_RADIUS) / ST_LIFE // delta radius (m/s)
#define ST_DAMAGE 15

/* MarkedAttack Defines (M) */
#define LATENCY 3
#define M_DAMAGE 25

/* LaserAttack Defines (L) */
#define ANGRANGE 1
#define L_RADIUS 500
#define L_HEIGHT 3
#define L_ANGSPEED (UNIT / 16)
#define L_LIFE 4
#define L_DAMAGE 40