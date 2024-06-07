/* GENERAL DEFINES */
// use degrees instead of radians, as rotation is in degree
#define UNIT 180.0
#define DEBUG_ST false

/* SwipeAttack Defines (SW) */
#define SW_LATENCY 1.0f // accounts for animation delay
#define FULL_RANGE (2 * UNIT / 3) // full attack range
#define ACTIVE_RANGE (UNIT / 3)   // attack range in effect
#define SW_ANGSPEED (UNIT / 3)
#define SW_RADIUS 30
#define SW_HEIGHT 5
#define SW_LIFE ((FULL_RANGE - ACTIVE_RANGE) / SW_ANGSPEED) + SW_LATENCY
#define SW_DAMAGE 10

/* StompAttack Defines (ST) */
// TODO: test width and height so it's dodgable by jumping
// TODO: unitless player hitbox radius is 30
#define ST_LATENCY 2.0f // accounts for animation delay
#define WAVE_WIDTH 10   // TODO: not meters - changed for collision testing
#define START_RADIUS 1   // in meters
#define END_RADIUS 200 // TODO: not meters - changed for collision testing
#define ST_HEIGHT 0.5    // in meters
#define ST_LIFE (2.5 + ST_LATENCY) // in seconds 
#define ST_SPEED ((END_RADIUS - START_RADIUS) / (ST_LIFE - ST_LATENCY)) // delta radius (m/s)
#define ST_DAMAGE 15

/* MarkedAttack Defines (M) */
#define M_LATENCY 3.1
#define M_RADIUS 25
#define M_LIFETIME 3.666
#define M_DAMAGE 35

/* LaserAttack Defines (L) */
#define ANGRANGE 1
#define L_RADIUS 500
#define L_HEIGHT 3
#define L_ANGSPEED (UNIT / 8)
#define L_LIFE 4
#define L_DAMAGE 40