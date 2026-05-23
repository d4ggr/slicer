#include <stdio.h>
#include "slicer/io/file.h"
#include "slicer/io/parser.h"
#include "slicer/io/object_loader.h"

int main() {
printf("=== Full OBJ Load Debug ===\n\n");

// Manually load to debug
uint32_t size = 0;
char* buffer = file_read_all("tests/test_model.obj", &size);

if (buffer == NULL) {
printf("ERROR: File not read!\n");
return 1;
}

Parser_t parser = parser_create(buffer, size);
Mesh_t mesh = mesh_create();

printf("Starting parse loop...\n");

int line_num = 0;
while (1) {
ParserLine_t line = parser_next_line(&parser);
if (line.ptr == NULL) {
printf("End of file\n");
break;
}

line_num++;

char* tokens[100];
uint32_t count = parser_tokenize(line.ptr, tokens, 100);

if (count == 0) {
printf("Line %d: EMPTY\n", line_num);
continue;
}

printf("Line %d: tokens=%u, [%s]", line_num, count, tokens[0]);

if (strcmp(tokens[0], "v") == 0 && count >= 4) {
printf(" -> VERTEX!\n");
Vec3_t v;
v.x = atof(tokens[1]);
v.y = atof(tokens[2]);
v.z = atof(tokens[3]);
slicer_status_t status = mesh_add_vertex(&mesh, v);
printf("    Added vertex: (%f, %f, %f), status=%d\n", v.x, v.y, v.z, status);
}
else if (strcmp(tokens[0], "f") == 0 && count >= 4) {
printf(" -> FACE!\n");
uint32_t v0 = atoi(tokens[1]) - 1;
uint32_t v1 = atoi(tokens[2]) - 1;
uint32_t v2 = atoi(tokens[3]) - 1;
slicer_status_t status = mesh_add_triangle(&mesh, v0, v1, v2);
printf("    Added face: (%u, %u, %u), status=%d\n", v0, v1, v2, status);
}
else {
printf(" -> OTHER\n");
}
}

printf("\nMesh final state:\n");
printf("Vertices: %u\n", mesh_vertex_count(&mesh));
printf("Triangles: %u\n", mesh_triangle_count(&mesh));

mesh_free(&mesh);

return 0;
}
