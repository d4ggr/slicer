#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "slicer/io/file.h"
#include "slicer/io/parser.h"

int main() {
uint32_t size = 0;
char* buffer = file_read_all("tests/test_model.obj", &size);

Parser_t parser = parser_create(buffer, size);
ParserLine_t line = parser_next_line(&parser);

char* tokens[100];
uint32_t count = parser_tokenize(line.ptr, tokens, 100);

printf("Line: [%s]\n", line.ptr);
printf("Token 0: [%s]\n", tokens[0]);
printf("Token 0 length: %lu\n", strlen(tokens[0]));
printf("Token 0 bytes: ");
for (int i = 0; i < 5; i++) {
printf("%02x ", (unsigned char)tokens[0][i]);
}
printf("\n");
printf("'v' byte: %02x\n", (unsigned char)'v');
printf("strcmp(tokens[0], \"v\") = %d\n", strcmp(tokens[0], "v"));

// Try comparing just first character
printf("tokens[0][0] = '%c' (%02x)\n", tokens[0][0], (unsigned char)tokens[0][0]);
printf("tokens[0][1] = '%c' (%02x)\n", tokens[0][1], (unsigned char)tokens[0][1]);

free(buffer);
return 0;
}
