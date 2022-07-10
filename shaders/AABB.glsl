#ifndef AABB_GLSL
#define AABB_GLSL

struct AABB {
    vec3 pMin;
    vec3 pMax;
};

AABB aabbInit(vec3 p1, vec3 p2) {
    AABB self;
    self.pMin = min(p1, p2);
    self.pMax = max(p1, p2);
    return self;
}

bool aabbIntersect(AABB self, Ray ray, out SurfaceInteraction interaction, float tmin, float tmax) {
    for (int i = 0; i < 3; ++i) {
        float t0 = min((self.pMin[i] - ray.o[i]) / ray.d[i], (self.pMax[i] - ray.o[i]) / ray.d[i]);
        float t1 = max((self.pMin[i] - ray.o[i]) / ray.d[i], (self.pMax[i] - ray.o[i]) / ray.d[i]);
    
        tmin = max(t0, tmin);
        tmax = min(t1, tmax);
        if (tmax <= tmin)
            return false;
    }
    interaction.t = tmin;
    interaction.p = rayAt(ray, tmin);
    vec3 dir = interaction.p - (self.pMin + self.pMax) * 0.5;
    if (abs(dir.x) > abs(dir.y) && abs(dir.x) > abs(dir.z)) {
        if (dir.x > 0)
            interaction.n = vec3( 1.0, 0.0, 0.0);
        else
            interaction.n = vec3(-1.0, 0.0, 0.0);
    }
    if (abs(dir.y) > abs(dir.x) && abs(dir.y) > abs(dir.z)) {
        if (dir.y > 0)
            interaction.n = vec3(0.0,  1.0, 0.0);
        else
            interaction.n = vec3(0.0, -1.0, 0.0);
    }
    if (abs(dir.z) > abs(dir.x) && abs(dir.z) > abs(dir.y)) {
        if (dir.z > 0)
            interaction.n = vec3(0.0, 0.0,  1.0);
        else
            interaction.n = vec3(0.0, 0.0, -1.0);
    }
    return true;
}

#endif // AABB_GLSL