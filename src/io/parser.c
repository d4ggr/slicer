#include <stdio.h>
#include "slicer/io/parser.h"

Parser_t parser_create(char *buffer, uint32_t size){

Parser_t p;

p.buffer = buffer;
p.size = size;
p.position = 0;

return p;
}

ParserLine_t parser_next_line(Parser_t* parser){

	// Skip whitespace and comments
	while (parser->position < parser->size) {
		char c = parser->buffer[parser->position];
		
		// Skip comments
		if (c == '#') {
			while (parser->position < parser->size && 
			       parser->buffer[parser->position] != '\n') {
				parser->position++;
			}
			if (parser->position < parser->size) {
				parser->position++;  // Skip newline
			}
			continue;
		}
		
		// Skip empty lines
		if (c == '\n') {
			parser->position++;
			continue;
		}
		
		// Found non-empty, non-comment line
		break;
	}
	
	// End of file
	if (parser->position >= parser->size) {
		ParserLine_t line = {NULL, 0};
		return line;
	}
	
	// Point to start of line
	char* line_start = parser->buffer + parser->position;
	
	// Find end of line (newline or null terminator)
	while (parser->position < parser->size && 
	       parser->buffer[parser->position] != '\n') {
		parser->position++;
	}
	
	// Calculate line length
	uint32_t line_length = parser->position - (line_start - parser->buffer);
	
	// Replace newline with null terminator
	parser->buffer[parser->position] = '\0';
	
	// Move past the (now-null) character for next call
	if (parser->position < parser->size) {
		parser->position++;
	}
	
	ParserLine_t line = {line_start, line_length};
	return line;
}

uint32_t parser_tokenize(char* line, char** tokens, uint32_t max_token){
	
	if (line == NULL || tokens == NULL || max_token == 0) {
		return 0;
	}
	
	uint32_t token_count = 0;
	uint32_t i = 0;
	
	// Scan through line until null terminator
	while (line[i] != '\0' && token_count < max_token) {
		
		// Skip whitespace
		while (line[i] != '\0' && isspace(line[i])) {
			i++;
		}
		
		// End of line reached
		if (line[i] == '\0') {
			break;
		}
		
		// Found start of token
		tokens[token_count] = &line[i];
		token_count++;
		
		// Skip to end of token (until whitespace or null)
		while (line[i] != '\0' && !isspace(line[i])) {
			i++;
		}
		
		// Null-terminate this token
		if (line[i] != '\0') {
			line[i] = '\0';
			i++;  // Move past the now-null character
		}
	}
	
	return token_count;
}

void parser_free(Parser_t* parser){
	
	if (parser == NULL) {
		return;
	}
	
	if (parser->buffer != NULL) {
		free(parser->buffer);
		parser->buffer = NULL;
	}
	
	parser->size = 0;
	parser->position = 0;
}
