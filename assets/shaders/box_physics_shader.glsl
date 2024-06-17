#version 430

struct AABB {
    vec2 position;
    vec2 size;
    float mass;
    vec2 velocity; // Added velocity component
};

layout (std430, binding = 0) buffer AABBBuffer {
    AABB boxes[];
};

// Simple vector subtraction
vec2 subtract(vec2 a, vec2 b) {
    return a - b;
}

// Absolute value of a vector
vec2 vabs(vec2 v) {
    return abs(v);
}

void main() {
    uint idx = gl_GlobalInvocationID.x;  // Unique index for each thread
    
    if (idx >= uint(length(boxes))) return;  // Bounds checking

    AABB self = boxes[idx];

    for (uint i = 0; i < uint(length(boxes)); ++i) {
        if (i == idx) continue;  // Skip self

        AABB other = boxes[i];
        vec2 distance = subtract(other.position, self.position);
        vec2 halfSize = (self.size * 0.5) + (other.size * 0.5);
        vec2 overlap = subtract(halfSize, vabs(distance));

        if (overlap.x > 0.0 && overlap.y > 0.0) {
            // Determine the smaller overlap dimension
            if (overlap.x < overlap.y) {
                float correction = overlap.x * (distance.x > 0.0 ? 1.0 : -1.0);
                float impulse = correction / (self.mass + other.mass);
                self.velocity.x -= impulse * other.mass;
                other.velocity.x += impulse * self.mass;
            } else {
                float correction = overlap.y * (distance.y > 0.0 ? 1.0 : -1.0);
                float impulse = correction / (self.mass + other.mass);
                self.velocity.y -= impulse * other.mass;
                other.velocity.y += impulse * self.mass;
            }
            boxes[i] = other;
        }
    }

    boxes[idx] = self;
}
