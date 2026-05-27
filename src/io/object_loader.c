#include "slicer/io/object_loader.h"
#include "slicer/io/file.h"
#include "slicer/io/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Mesh_t obj_load(const char *filename){

	uint32_t size = 0;
	char* buffer = file_read_all(filename, &size);
	
	if (buffer == NULL) {
		return mesh_create();
	}
	
	Parser_t parser = parser_create(buffer, size);
	Mesh_t mesh = mesh_create();

while(1){

ParserLine_t p = parser_next_line(&parser);
if(p.ptr== NULL){
break;
}

uint32_t max_token = 100;
char* token[max_token]; 
uint32_t count = parser_tokenize(p.ptr,token,max_token);

if(count == 0){continue;}

if(strcmp(token[0],"v")==0 && count>=4){

  Vec3_t vertex;
  vertex.x = atof(token[1]);
  vertex.y = atof(token[2]);
  vertex.z = atof(token[3]);
  mesh_add_vertex(&mesh,vertex);
}

else if(strcmp(token[0],"f")==0 && count>=4){

  Triangle_t face;
  face.indices[0] = atoi(token[1])-1;
  face.indices[1] = atoi(token[2])-1;
  face.indices[2] = atoi(token[3])-1;
  mesh_add_triangle(&mesh,face.indices[0],face.indices[1],face.indices[2]);
}
}
parser_free(&parser);

if (mesh_vertex_count(&mesh) > 0) {
	mesh_compute_bounds(&mesh);
}

return mesh;

}
