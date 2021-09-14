
#include <tice.h>
#include <graphx.h>
#include <debug.h>

// MATH FUNCTIONS
float invSqrt(float number){ //fast inverse sqrt implementation, or more like direct copy/paste from john carmacks work in quake

	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration

    //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

// Remember to optimise for memory by removing readability stuff. 
struct vector{
    float x;
    float y;
    float z;
};

struct vector vecAdd(struct vector a, struct vector b){
    struct vector ret = a;
    ret.x += b.x;
    ret.y += b.y;
    ret.z += b.z;
    return ret;
}

struct vector vecSub(struct vector a, struct vector b){
    struct vector ret = a;
    ret.x -= b.x;
    ret.y -= b.y;
    ret.z -= b.z;
    return ret;
}

struct vector vecScalarMul(struct vector a, float b){
    struct vector ret = a;
    ret.x *= b;
    ret.y *= b;
    ret.z *= b;
    return ret;
}

struct vector vecScalarDiv(struct vector a, float b){
    struct vector ret = a;
    ret.x /= b;
    ret.y /= b;
    ret.z /= b;
    return ret;
}

struct vector vecCrossProdut(struct vector a, struct vector b){
    struct vector ret = a;
    ret.x = a.y*b.z-a.z*b.y;
    ret.y = a.z*b.x-a.x*b.z;
    ret.z = a.x*b.y-a.y*b.x;
    return ret;
}

float dotProduct(struct vector a, struct vector b){
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

struct vector vecNormalise(struct vector a){
    struct vector ret = a;
    float normy = a.x*a.x+a.y*a.y+a.z*a.z;
    normy = invSqrt(normy);
    ret.x = normy*a.x;
    ret.y = normy*a.y;
    return ret;
}

// LIGHT RAY STUFF (just note that ti84ce memory/resources are limited, put end stop on recursion depth)

struct ray{
    struct vector origin;
    struct vector direction;
};

struct vector at(struct ray r, float t){
    struct vector oD = vecScalarMul(r.direction, t);
    struct vector ret = vecAdd(oD,r.origin);
    return ret;
}

struct vector rayColor(struct ray r){
    struct ray ray_ = r;
    struct vector dir = vecNormalise(ray_.direction);
    float t = (dir.y+1);
    t = 0.5*t;
    struct vector col = { 255 , 255 , 255 };
    struct vector col_ = { 128 , 179 , 255};
    struct vector ret = vecAdd(vecScalarMul(col, 1.0-t), vecScalarMul(col_, t));
    return ret;

}


// Main
int main(void){
    //Image
    const float aspect = 1;
    const int width = 100;
    const int height = 100;

    //camera
    float vp_h = 2;
    float vp_w = aspect * vp_h;
    float focal_L = 1;
    struct vector origin = { 0 , 0 , 0 };
    struct vector horizontal = { vp_w, 0 , 0 };
    struct vector vertical = { 0 , vp_h , 0 };
    struct vector lower_left = vecAdd(vecScalarDiv(horizontal,2), vecScalarDiv(vertical,2));
    struct vector l = { 0 , 0 , focal_L};
    lower_left = vecAdd(lower_left, l);
    lower_left = vecSub(origin, lower_left);

    //gfx_Begin();
    //gfx_ZeroScreen();
    //gfx_SetColor(18);
    for(int y = 100; y>0; y--){
        for(int x = 0; x<100; x++){
            float u = ((float) x)/99;
            float v = ((float) y)/99;
            struct ray r = { origin, vecSub(vecAdd(vecAdd(lower_left, vecScalarMul(horizontal, u)), vecScalarMul(vertical,v)), origin) };
            struct vector pix_color = rayColor(r);
            int R = (int)pix_color.x;
            int G = (int)pix_color.y;
            int B = (int)pix_color.z;
            dbg_sprintf(dbgout, "%f", pix_color.x);
            dbg_sprintf(dbgout, " ");
            dbg_sprintf(dbgout, "%f", pix_color.y);
            dbg_sprintf(dbgout, " ");
            dbg_sprintf(dbgout, "%f\n", pix_color.z);
            //gfx_SetColor(gfx_RGBTo1555(238,255,154));
            //gfx_RGBTo1555(pix_color.x,pix_color.y,pix_color.z)
            /*if (pix_color.x>10){
                gfx_SetColor(18);
            } else {
                gfx_SetColor(255);
            }*/
            //gfx_SetColor(gfx_RGBTo1555(pix_color.x*10,0,0));
            //gfx_SetPixel(x,y);
        }
    }
    while (!os_GetCSC());
    //gfx_End();
    return 0;
}

