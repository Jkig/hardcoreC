// Start with stripping the file, then parsing to different things, start with just an if statement, then a for loop,,, frame pointer, etc?

// {}(); are important, cut out all " ", "\n", and so on, keywords are special, functions are special, variables are things
// Have a stack to know what frame its relative too...


#include <stdio.h>
#include <stdbool.h>


// Refine this ofc.
void condense_file(const char *input_filename, const char *output_filename) {
    FILE *input = fopen(input_filename, "r");
    if (!input) {
        perror("Error opening input file");
        return;
    }

    FILE *output = fopen(output_filename, "w");
    if (!output) {
        perror("Error opening output file");
        fclose(input);
        return;
    }

    int c;
    bool in_single_line_comment = false;
    bool in_multi_line_comment = false;

    while ((c = fgetc(input)) != EOF) {
        // Handle single-line comments
        if (!in_multi_line_comment && !in_single_line_comment && c == '/') {
            int next = fgetc(input);
            if (next == '/') {
                in_single_line_comment = true;
                continue;
            } else if (next == '*') {
                in_multi_line_comment = true;
                continue;
            } else {
                // Not a comment, write the '/' and push back the next character
                if (next != EOF)
                    ungetc(next, input);
                fputc('/', output);
                continue;
            }
        }

        if (in_single_line_comment) {
            if (c == '\n' || c == '\r') {
                in_single_line_comment = false;
            }
            continue;
        }

        if (in_multi_line_comment) {
            if (c == '*') {
                int next = fgetc(input);
                if (next == '/') {
                    in_multi_line_comment = false;
                } else if (next != EOF) {
                    ungetc(next, input);
                }
            }
            continue;
        }

        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            continue;

        fputc(c, output);
    }

    fclose(input);
    fclose(output);
}

int main(int argc, char *argv[]) {
    // takes file name, creates a parsed file that is everything chopped up
    condense_file("emulateProcessor.c", "emulateProcessor.cdensed");


    return 0;
}