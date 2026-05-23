#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main() {
// Simulate what parser_next_line returns
char buffer[] = "v 0.0 0.0 0.0\n";  // This is what we'd get
buffer[14] = '\0';  // Null-terminate like parser_next_line does

printf("Buffer: [%s]\n", buffer);
printf("Buffer (with hex): ");
for (int i = 0; i < 15; i++) {
printf("%02x ", (unsigned char)buffer[i]);
}
printf("\n\n");

// Test tokenize
char* tokens[10];
uint32_t token_count = 0;
uint32_t i = 0;

while (buffer[i] != '\0' && token_count < 10) {
// Skip whitespace
while (buffer[i] != '\0' && (buffer[i] == ' ' || buffer[i] == '\t')) {
i++;
}

if (buffer[i] == '\0') break;

tokens[token_count] = &buffer[i];
printf("Token %u starts at &buffer[%d], char='%c'\n", token_count, i, buffer[i]);
token_count++;

// Skip to whitespace
while (buffer[i] != '\0' && buffer[i] != ' ' && buffer[i] != '\t') {
i++;
}
}

printf("\nTokens:\n");
for (uint32_t j = 0; j < token_count; j++) {
printf("  Token %u: [%s]\n", j, tokens[j]);
}

return 0;
}
