#include <stddef.h>
#include <stdlib.h>
#include "slicer/math/aabb.h"
#include "slicer/mesh/mesh.h"
#include "slicer/core/constants.h"

Mesh_t mesh_create(){

  Mesh_t mesh;
  mesh.vertices = NULL;
  mesh.num_vertices = 0;
  mesh.vertex_capacity = 0;

  mesh.triangles = NULL;
  mesh.num_triangles = 0;
  mesh.triangle_capacity = 0;
  
  aabb_init_invalid(&mesh.bounds) ;

  return mesh;

}

slicer_status_t mesh_reserve_vertices(Mesh_t* mesh, uint32_t capacity){

  if(mesh->vertices == NULL){
  mesh->vertices = malloc(sizeof(Vec3_t)*capacity) ;
  
  if(mesh->vertices == NULL){return SLICER_STATUS_OUT_OF_MEMORY;
  }
  
  mesh->vertex_capacity = capacity;
  return SLICER_STATUS_OK ;
  }
  if(capacity > mesh->vertex_capacity){
  
    Vec3_t* new_ptr = realloc(mesh->vertices,sizeof(Vec3_t)* capacity);
    if(new_ptr == NULL){return SLICER_STATUS_OUT_OF_MEMORY ;}
    mesh->vertices = new_ptr;
    mesh->vertex_capacity = capacity;

  }

  return SLICER_STATUS_OK;
  
}

slicer_status_t mesh_reserve_triangles(Mesh_t* mesh, uint32_t capacity){
      
   if(mesh->triangles == NULL){
  mesh->triangles = malloc(sizeof(Triangle_t)*capacity) ;
  
  if(mesh->triangles == NULL){return SLICER_STATUS_OUT_OF_MEMORY;
  }
  
  mesh->triangle_capacity = capacity;
  return SLICER_STATUS_OK ;
  }
  if(capacity > mesh->triangle_capacity){
  
    Triangle_t* new_ptr = realloc(mesh->triangles,sizeof(Triangle_t)* capacity);
    if(new_ptr == NULL){return SLICER_STATUS_OUT_OF_MEMORY ;}
    mesh->triangles = new_ptr;
    mesh->triangle_capacity = capacity;

  }

  return SLICER_STATUS_OK;
  
}

slicer_status_t mesh_add_vertex(Mesh_t *mesh, Vec3_t vertex){

  if(mesh->num_vertices == mesh->vertex_capacity){
      uint32_t new_capacity = (mesh->vertex_capacity == 0) ? 16 : mesh->vertex_capacity * 2 ; 
  

slicer_status_t status = mesh_reserve_vertices(mesh, new_capacity);
if(status != SLICER_STATUS_OK){return status;}}

mesh->vertices[mesh->num_vertices] = vertex ; //mesh->vertices[i] is a value at ith index of the series of pointer
mesh->num_vertices++;
return SLICER_STATUS_OK;

}


slicer_status_t mesh_add_triangle(Mesh_t* mesh, uint32_t v0, uint32_t v1, uint32_t v2){

if(v0>=mesh->num_vertices || v1>=mesh->num_vertices || v2>=mesh->num_vertices){return SLICER_STATUS_OUT_OF_RANGE;}

if(mesh->num_triangles == mesh->triangle_capacity){

      uint32_t new_capacity = (mesh->triangle_capacity == 0) ? 16 : mesh->triangle_capacity * 2 ; 
  

slicer_status_t status = mesh_reserve_triangles(mesh, new_capacity);
if(status != SLICER_STATUS_OK){return status;}}

mesh->triangles[mesh->num_triangles].indices[0] = v0;
mesh->triangles[mesh->num_triangles].indices[1] = v1;
mesh->triangles[mesh->num_triangles].indices[2] = v2;

mesh->num_triangles++;
return SLICER_STATUS_OK;

}

void mesh_compute_bounds(Mesh_t* mesh){

  mesh->bounds = aabb_create_invalid() ; 
AABB_t* ptr = &mesh->bounds ;
for(uint32_t i = 0; i < mesh->num_vertices; i++){

aabb_expand_point(ptr,mesh->vertices[i]);

}

}
  
AABB_t mesh_get_bounds(const Mesh_t* mesh){

return mesh->bounds;

}

uint32_t mesh_vertex_count(const Mesh_t *mesh){

return mesh->num_vertices;

}

uint32_t mesh_triangle_count(const Mesh_t* mesh){

return mesh->num_triangles;

}

slicer_status_t mesh_validate(const Mesh_t* mesh){

for(uint32_t i=0; i< mesh->num_triangles; i++){
uint32_t v0,v1,v2;
v0 = mesh->triangles[i].indices[0]  ;
v1 = mesh->triangles[i].indices[1]  ;
v2 = mesh->triangles[i].indices[2]  ;

if(v0 >= mesh->num_vertices || v1 >= mesh->num_vertices || v2 >= mesh->num_vertices){

return SLICER_STATUS_OUT_OF_RANGE;
}

if(v0==v1 || v1==v2 || v0 == v2){

return SLICER_STATUS_DEGENERATE_GEOMETRY;

}

}
return SLICER_STATUS_OK;

}

void mesh_free(Mesh_t* mesh){

  free(mesh->vertices);
  mesh->vertices = NULL;
  mesh->num_vertices = 0;
  mesh->vertex_capacity = 0;

  free(mesh->triangles);
  mesh->triangles = NULL;
  mesh->num_triangles = 0;
  mesh->triangle_capacity = 0;


}
