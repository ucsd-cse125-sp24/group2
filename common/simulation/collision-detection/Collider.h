struct Collider {
    bool isSector;
    bool isPoint;
    int owner;
    float x, y, z;
    float radius;
    float height;
    float startAngle;
    float endAngle;

    Collider(bool s, bool p, int o, float ix, float iy, float iz, float r, float h, float sa, float ea)
    : isSector(s), isPoint(p), owner(o), x(ix), y(iy), z(iz), radius(r), height(h), startAngle(sa), endAngle(ea) {}

    // Method to update cylinder properties
    void updateCylinder(float nx, float ny, float nz, float nradius, float nheight) {
        x = nx; y = ny; z = nz; radius = nradius; height = nheight;
    }

    void updateSector(float nx, float ny, float nz, float nradius, float nheight, float nstartAngle, float nendAngle) {
        x = nx; y = ny; z = nz; radius = nradius; height = nheight; startAngle = nstartAngle; endAngle = nendAngle;
    }

    void updatePoint(float nx, float ny, float nz) {
        x = nx; y = ny; z = nz;
    }
};