#include <stdio.h>
#include "slicer/io/file.h"
#include "slicer/io/parser.h"
#include "slicer/io/object_loader.h"

int main() {
printf("=== OBJ Loader Debug ===\n\n");

// Test file reading
uint32_t size = 0;
char* buffer = file_read_all("tests/test_model.obj", &size);

if (buffer == NULL) {
printf("ERROR: Could not read file!\n");
return 1;
}

printf("File read successfully: %u bytes\n\n", size);
printf("First 200 chars of buffer:\n");
printf("%.200s\n\n", buffer);

// Test parser
Parser_t parser = parser_create(buffer, size);

int line_count = 0;
while (1) {
ParserLine_t line = parser_next_line(&parser);
if (line.ptr == NULL) break;

line_count++;
printf("Line %d (len=%u): %.*s\n", line_count, line.length, line.length, line.ptr);

// Test tokenization
char* tokens[10];
uint32_t count = parser_tokenize(line.ptr, tokens, 10);

printf("  Tokens (%u): ", count);
for (uint32_t i = 0; i < count; i++) {
printf("[%s] ", tokens[i]);
}
printf("\n");
}

printf("\nTotal lines read: %d\n", line_count);

free(buffer);
return 0;
}
