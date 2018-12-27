/*
2018 David DiPaola
licensed under CC0 (public domain, see https://creativecommons.org/publicdomain/zero/1.0/)
*/

#include <unistd.h>

#include <stdio.h>

static void
_print_codepoint(unsigned int codepoint) {
	if (codepoint <= 0xFFFF) {
		printf("U+%04X" "\n", codepoint);
	}
	else if (codepoint <= 0xFFFFF) {
		printf("U+%05X" "\n", codepoint);
	}
	else if (codepoint <= 0x10FFFF) {
		printf("U+%06X" "\n", codepoint);
	}
}

int
main() {
	unsigned char byte;
	unsigned int  remaining = 0;
	unsigned int  codepoint = 0;
	size_t        offset    = 0;
	while (read(STDIN_FILENO, &byte, sizeof(byte)) == sizeof(byte)) {
		if (remaining == 0) {
			if (byte <= 0x7F) {
				_print_codepoint(byte);
				codepoint = 0;
				remaining = 0;
			}
			else if (((byte >> 5) & 0b111) == 0b110) {
				codepoint = byte & 0b11111;
				remaining = 1;
			}
			else if (((byte >> 4) & 0b1111) == 0b1110) {
				codepoint = byte & 0b1111;
				remaining = 2;
			}
			else if (((byte >> 3) & 0b11111) == 0b11110) {
				codepoint = byte & 0b111;
				remaining = 3;
			}
			else {
				fprintf(stderr, "ERROR at offset 0x%zX: invalid start byte: 0x%02X" "\n", offset, byte);
				codepoint = 0;
				remaining = 0;
			}
		}
		else {
			if (((byte >> 6) & 0b11) == 0b10) {
				codepoint = (codepoint << 6) | (byte & 0b111111);
				remaining--;

				if (remaining == 0) {
					_print_codepoint(codepoint);
				}
			}
			else {
				fprintf(stderr, "ERROR at offset 0x%zX: invalid continuation byte: 0x%02X" "\n", offset, byte);
				codepoint = 0;
				remaining = 0;
			}
		}

		offset++;
	}

	return 0;
}

