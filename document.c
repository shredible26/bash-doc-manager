/*
  Shrey Varma
  UID: 118019727
  DirectoryID: shreyv

  This program imitates a document and contains various methods so that the
  user can edit the document. Some functionality includes adding paragraphs,
  lines, removing paragraphs, lines, replacing text, resetting the document,
  and some other editing functions.
*/

#include <stdio.h>
#include <string.h>
#include "document.h"

/* initializes a new document */
int init_document(Document *doc, const char *name) {
	if (doc != NULL && name != NULL && strlen(name) <= MAX_STR_SIZE) {
		strcpy(doc->name, name);
		return SUCCESS;
	}
	return FAILURE;
}

/* clears number of paragraphs */
int reset_document(Document *doc) {
	if (doc != NULL) {
		doc->number_of_paragraphs = 0;
		return SUCCESS;
	}
	return FAILURE;
}

/* prints important document data */
int print_document(Document *doc) {
	int x, y;

	if (doc != NULL) {
		printf("Document name: \"%s\"\n", doc->name);
		printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
		for (x = 0; x < doc->number_of_paragraphs; x++) {
			if (doc->paragraphs[x].number_of_lines > 0) {
				for (y = 0; y < doc->paragraphs[x].number_of_lines; y++) {
					printf("%s\n", doc->paragraphs[x].lines[y]);
				}
				if (x < (doc->number_of_paragraphs - 1)) {
					printf("\n");
				}
			}
		}
		return SUCCESS;
	}
	return FAILURE;
}

/* adds a paragraph after a specified paragraph number provided by user */
int add_paragraph_after(Document *doc, int paragraph_number) {
	int x, first = 0, second = 0, num_paragraphs = 0;

	if (doc != NULL) {
		num_paragraphs = doc->number_of_paragraphs;
		if (num_paragraphs < MAX_PARAGRAPHS && paragraph_number <= num_paragraphs) {
			if (paragraph_number == 0 && !num_paragraphs) {
				doc->paragraphs[0].number_of_lines = 0;
			} else if (paragraph_number == doc->number_of_paragraphs) {
				second = doc->number_of_paragraphs;
				doc->paragraphs[second].number_of_lines = 0;
			} else {
				first = doc->number_of_paragraphs;
				for (x = first; x >= paragraph_number; x--) {
					doc->paragraphs[x + 1] = doc->paragraphs[x];
				}
				doc->paragraphs[paragraph_number].number_of_lines = 0;
			}
			(doc->number_of_paragraphs)++;
			return SUCCESS;
		}
	}
	return FAILURE;
}

/* adds a new line after the line at the specified line number by the user */
int add_line_after(Document *doc, int paragraph_number, int line_number,
				   const char *new_line) {
	int a, num_lines = 0;
	char *pos, *first_line, *second_line;

	if (doc != NULL && new_line != NULL) {
		if (paragraph_number <= doc->number_of_paragraphs) {
			num_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
			if (num_lines < MAX_PARAGRAPH_LINES && line_number <= num_lines) {
				if (line_number == 0 && !num_lines) {
					pos = doc->paragraphs[paragraph_number - 1].lines[0];
					strcpy(pos, new_line);
				} else if (line_number == num_lines) {
					pos = doc->paragraphs[paragraph_number - 1].lines[line_number];
					strcpy(pos, new_line);
				} else {
					for (a = num_lines; a >= line_number; a--) {
						first_line = doc->paragraphs[paragraph_number - 1].lines[a];
						second_line = doc->paragraphs[paragraph_number - 1].lines[a + 1];
						strcpy(second_line, first_line);
					}
					pos = doc->paragraphs[paragraph_number - 1].lines[line_number];
					strcpy(pos, new_line);
				}
				(doc->paragraphs[paragraph_number - 1].number_of_lines)++;
				return SUCCESS;
			}
		}
	}
	return FAILURE;
}

/* returns the number of lines in a paragraph */
int get_number_lines_paragraph(Document *doc, int paragraph_number,
							   int *number_of_lines) {
	int line_count = 0;

	if (doc != NULL) {
		if (paragraph_number <= doc->number_of_paragraphs) {
			line_count = doc->paragraphs[paragraph_number].number_of_lines;
			*number_of_lines = line_count;
			return SUCCESS;
		}
	}
	return FAILURE;
}

/* appends a line to a specified paragraph */
int append_line(Document *doc, int paragraph_number, const char *new_line) {
	int num_lines = 0, para = paragraph_number;

	if (doc != NULL && new_line != NULL) {
		num_lines = doc->paragraphs[para - 1].number_of_lines;
		if (para <= MAX_PARAGRAPHS && num_lines < MAX_PARAGRAPH_LINES) {
			if (!add_line_after(doc, para, num_lines, new_line)) {
				return SUCCESS;
			}
		}
	}
	return FAILURE;
}

/* removes the specified line from the paragraph */
int remove_line(Document *doc, int paragraph_number, int line_number) {
	int x, y, num_lines = 0, para_num = paragraph_number;
	char *line, *second;

	if (doc != NULL) {
		num_lines = doc->paragraphs[para_num - 1].number_of_lines;
		if (para_num <= doc->number_of_paragraphs && line_number <= num_lines) {
			if (line_number < num_lines) {
				for (x = line_number - 1; x < num_lines - 1; x++) {
					line = doc->paragraphs[para_num - 1].lines[x];
					second = doc->paragraphs[para_num - 1].lines[x + 1];
					for (y = 0; y < MAX_STR_SIZE; y++) {
						line[y] = second[y];
					}
				}
			}
			(doc->paragraphs[para_num - 1].number_of_lines)--;
			return SUCCESS;
		}
	}
	return FAILURE;
}

/* adds lines from a data array to the document */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int count) {
	int x = 0;

	if (doc != NULL && data != NULL && count) {
		add_paragraph_after(doc, doc->number_of_paragraphs);
		for (x = 0; x < count; x++) {
			if (strcmp(data[x], "") == 0) {
				add_paragraph_after(doc, doc->number_of_paragraphs);
				(doc->number_of_paragraphs)++;
			} else {
				append_line(doc, doc->number_of_paragraphs, data[x]);
			}
		}
		return SUCCESS;
	}
	return FAILURE;
}

/* replaces the text "target" with "replacement" in the document */
int replace_text(Document *doc, const char *target, const char *replacement) {
	int x, y, line_count = 0;
	char *curr, buffer[MAX_STR_SIZE + 1] = {0};

	if (doc != NULL && target != NULL && replacement != NULL) {
		for (x = 0; x < doc->number_of_paragraphs; x++) {
			line_count = doc->paragraphs[x].number_of_lines;
			for (y = 0; y < line_count; y++) {
				curr = strstr(doc->paragraphs[x].lines[y], target);
				while (curr != '\0') {
					strcpy(buffer, curr);
					strcpy(curr, "");
					strcat(curr, replacement);
					strcat(curr + strlen(replacement), buffer + strlen(target));
					curr = strstr(curr + strlen(target), target);
				}
			}
		}
		return SUCCESS;
	}
	return FAILURE;
}

/* highlights specific text defined by target */
int highlight_text(Document *doc, const char *target) {
	char end[MAX_STR_SIZE + 1] = "";

	if (doc != NULL && target != NULL) {
		strcat(end, HIGHLIGHT_START_STR);
		strcat(end, target);
		strcat(end, HIGHLIGHT_END_STR);
		replace_text(doc, target, end);
		return SUCCESS;
	}
	return FAILURE;
}

/* removes target text in the document */
int remove_text(Document *doc, const char *target) {
	if (doc != NULL && target != NULL) {
		replace_text(doc, target, "");
		return SUCCESS;
	}
	return FAILURE;
}

/* loads data into the document from a file */
int load_file(Document *doc, const char *filename) {
	FILE *input;
	char line[MAX_STR_SIZE + 1], first, second[MAX_STR_SIZE + 1];
	int num_values, paragraph_number = 1;

	if (doc != NULL && filename != NULL) {
		if ((input = fopen(filename, "r")) != NULL) {
			add_paragraph_after(doc, 0);
			while (fgets(line, MAX_STR_SIZE + 1, input)) {
				num_values = sscanf(line, "%c%s", &first, second);
				strtok(line, "\n");
				if (first != '#') {
					if (num_values < 2) {
						add_paragraph_after(doc, paragraph_number);
						paragraph_number++;
						if (paragraph_number > MAX_PARAGRAPHS) {
							return FAILURE;
						}
					} else {
						append_line(doc, paragraph_number, line);
					}
				}
			}
			return SUCCESS;
		}
	}
	return FAILURE;
}

/* saves the document paragraphs to a file */
int save_document(Document *document, const char *filename) {
	FILE *outfile;
	int x, y, num_lines;

	if (document != NULL && filename != NULL) {
		if ((outfile = fopen(filename, "w")) != NULL) {
			for (x = 0; x <= document->number_of_paragraphs; x++) {
				num_lines = document->paragraphs[x].number_of_lines;
				for (y = 0; y <= num_lines; y++) {
					fputs(document->paragraphs[x].lines[y], outfile);
					if (y < num_lines) {
						fputs("\n", outfile);
					}
				}
				if ((x + 1) < document->number_of_paragraphs) {
					fputs("\n", outfile);
				}
			}
			fclose(outfile);
			return SUCCESS;
		}
	}
	return FAILURE;
}
