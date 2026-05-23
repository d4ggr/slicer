#include "slicer/math/aabb.h"
#include "slicer/core/constants.h"
#include "slicer/math/numeric.h"
#include "slicer/math/vec3.h"
#include <math.h>

AABB_t aabb_create_invalid(){

    AABB_t invalid_box;
    invalid_box.min.x = INFINITY;
    invalid_box.min.y = INFINITY;
    invalid_box.min.z = INFINITY;
    
    invalid_box.max.x = -INFINITY;
    invalid_box.max.y = -INFINITY;
    invalid_box.max.z = -INFINITY;

    return invalid_box;

}

void aabb_init_invalid(AABB_t* box){

    *box = aabb_create_invalid();

}

void aabb_expand_point(AABB_t* box, Vec3_t point){

    if(!aabb_is_valid(box)){
          
       box->min = box->max = point ;   

    }
    else{

      box->min.x = slicer_math_min(box->min.x,point.x);
      box->min.y = slicer_math_min(box->min.y,point.y);
      box->min.z = slicer_math_min(box->min.z,point.z);

      box->max.x = slicer_math_max(box->max.x,point.x);
      box->max.y = slicer_math_max(box->max.y,point.y);
      box->max.z = slicer_math_max(box->max.z,point.z);

    }

}

void aabb_merge(AABB_t* dest, const AABB_t* src){

if(!aabb_is_valid(src)){}
else{
  aabb_expand_point(dest,src->min);
  aabb_expand_point(dest,src->max);
}
}

bool aabb_contains_point(const AABB_t* box, Vec3_t point){

return (point.x >= box->min.x && point.x <= box->max.x) &&
       (point.y >= box->min.y && point.y <= box->max.y) &&
       (point.z >= box->min.z && point.z <= box->max.z) ;
}

bool aabb_is_valid(const AABB_t* box){

  return (box->min.x <= box->max.x) &&
         (box->min.y <= box->max.y) &&
         (box->min.z <= box->max.z) ;

}

Vec3_t aabb_center(const AABB_t* box){
  Vec3_t center;
  center.x = (box->min.x + box->max.x)/2 ;
  center.y = (box->min.y + box->max.y)/2 ;
  center.z = (box->min.z + box->max.z)/2 ;
return center;
}

Vec3_t aabb_size(const AABB_t* box){

  Vec3_t size;
  size.x = box->max.x - box->min.x ;
  size.y = box->max.y - box->min.y ;
  size.z = box->max.z - box->min.z ;

  return size;
}
