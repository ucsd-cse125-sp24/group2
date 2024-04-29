#include <array>
#include <cmath>

struct Vector2 {
    double x, y;

    // Normalize the vector
    Vector2 normalize() const {
        double len = std::sqrt(x * x + y * y);
        return { x / len, y / len };
    }

    // Dot product of two vectors
    double dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    // Cross product of two vectors
    double cross(const Vector2& other) const {
        return x * other.y - y * other.x;
    }
};

bool isBetween(const Vector2& C, const Vector2& A, const Vector2& B) {
    // Normalize vectors
    Vector2 nA = A.normalize();
    Vector2 nB = B.normalize();
    Vector2 nC = C.normalize();

    // Cross products to check the plane and direction
    double crossAC = nA.cross(nC);
    double crossAB = nA.cross(nB);
    double crossBC = nB.cross(nC);
    double crossBA = nB.cross(nA);

    // Check if the directions of the cross products are consistent
    bool cond1 = crossAC * crossAB > 0;  // C is on the same side of A as B
    bool cond2 = crossBC * crossBA > 0;  // C is on the same side of B as A

    return cond1 && cond2;
}