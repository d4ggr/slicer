#ifndef SLICER_IO_PARSER_H
#define SLICER_IO_PARSER_H

#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	char* buffer;
	uint32_t size;
	uint32_t position;
} Parser_t;

typedef struct {
	char* ptr;
	uint32_t length;
} ParserLine_t;

Parser_t parser_create(char* buffer, uint32_t size);

ParserLine_t parser_next_line(Parser_t* parser);

uint32_t parser_tokenize(char* line,char** tokens, uint32_t max_token);

void parser_free(Parser_t* parser);

#endif /* SLICER_IO_PARSER_H  */
