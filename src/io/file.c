#include "slicer/io/file.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
char* file_read_all(const char* filename,uint32_t* out_size){

FILE* file;
file = fopen(filename,"r");

if(file == NULL){
 
  printf("error:file did not open");
  return NULL;
}
fseek(file,0,SEEK_END);
*out_size = ftell(file);

fseek(file,0,SEEK_SET);
char* buffer = malloc(*out_size + 1);

if(buffer == NULL){
fclose(file);
return NULL;
}

size_t reading = fread(buffer,1,(*out_size),file);
if(reading != *out_size){

  printf("Error: read only %zu of %u bytes\n",reading, *out_size);
  free(buffer);
  fclose(file);
  return NULL;

}
buffer[*out_size] = '\0';

fclose(file);

return buffer ;
}

void file_free(char* buffer){
if(buffer != NULL){
free(buffer);
}
}
