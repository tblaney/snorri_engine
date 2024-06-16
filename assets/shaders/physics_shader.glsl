layout(local_size_x = 1) in;

struct BodyData {
    vec4 position;
    vec4 rotation;
    vec4 scale;
    vec4 velocity;
    int shapeType;
    float bounciness;
    float mass;
    int isFrameChecked;
};
layout(std430, binding = 0) buffer Bodies {
    BodyData bodies[];
};

layout(std430, binding = 1) buffer BodiesOut {
    BodyData bodiesOut[];
};

struct ResultData {
    vec3 worldPosition;
    float pad;
};
layout(std430, binding = 2) buffer Results {
    ResultData results[];
};

struct Ray {
    vec3 origin;
    vec3 direction;
};
struct Surface {
    float distanceToSurface;
    vec4 diffuse;
    float outline;
};

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

}
