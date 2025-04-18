#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#define WIDTH 5
#define HEIGHT 8
#define SIZES (width * (WIDTH + 1)) & 0xFF, (width * (WIDTH + 1)) >> 8, (height * (HEIGHT + 1)) & 0xFF, (height * (HEIGHT + 1)) >> 8
long font[94] = {
    0b0010000100001000010000100000000010000000,
    0b0101001010010100000000000000000000000000,
    0b0000001010111110101001010111110101000000,
    0b0010001110101000110000110001101110000100,
    0b0001010010101000010001000010101001010000,
    0b0010001010010100110010101100100110100000,
    0b0010000100001000000000000000000000000000,
    0b0001000100010000100001000010000010000010,
    0b0100000100000100001000010000100010001000,
    0b0000000000100100110011110011001001000000,
    0b0000000000001000010011111001000010000000,
    0b0000000000000000000000000001000010001000,
    0b0000000000000000000011110000000000000000,
    0b0000000000000000000000000000000010000000,
    0b0001000010001000010001000010001000010000,
    0b0000001100100101011011010100100110000000,
    0b0000000100011000010000100001000111000000,
    0b0000001100100100001001100100001111000000,
    0b0000001100100100010000010100100110000000,
    0b0000010000101001010011110001000010000000,
    0b0000011110100001110000010000101110000000,
    0b0000001100100001110010010100100110000000,
    0b0000011110100100001000100010000100000000,
    0b0000001100100100110010010100100110000000,
    0b0000001100100101001001110000100110000000,
    0b0000000000000000010000000000000010000000,
    0b0000000000000000010000000001000010001000,
    0b0000000010001000100001000001000001000000,
    0b0000000000000001111000000111100000000000,
    0b0000001000001000001000010001000100000000,
    0b0110010010000100010001000000000100000000,
    0b0000001100100101011010110100000111000000,
    0b0000001100100101001011110100101001000000,
    0b0000011100100101110010010100101110000000,
    0b0000001110100001000010000100000111000000,
    0b0000011100100101001010010100101110000000,
    0b0000001110100001110010000100000111000000,
    0b0000001110100001000011100100001000000000,
    0b0000001110100001011010010100100111000000,
    0b0000010010100101111010010100101001000000,
    0b0000001110001000010000100001000111000000,
    0b0000001110001000010000100001001100000000,
    0b0000010010100101110010010100101001000000,
    0b0000010000100001000010000100000111000000,
    0b0000010010111101111010010100101001000000,
    0b0000010010110101101010110101101001000000,
    0b0000001100100101001010010100100110000000,
    0b0000011100100101001011100100001000000000,
    0b0000001100100101001010010100100110000110,
    0b0000011100100101001011100100101001000000,
    0b0000001110100000110000010000101110000000,
    0b0000011111001000010000100001000010000000,
    0b0000010010100101001010010100100111000000,
    0b0000010010100101001010010011000110000000,
    0b0000010010100101001011110111101001000000,
    0b0000010010100100110001100100101001000000,
    0b0000010010100101001001110000101110000000,
    0b0000011110000100010001000100001111000000,
    0b0111001000010000100001000010000100001110,
    0b1000010000010000100000100001000001000010,
    0b0111000010000100001000010000100001001110,
    0b0000000100010101000100000000000000000000,
    0b0000000000000000000000000000000000011110,
    0b0100000100000000000000000000000000000000,
    0b0000000000011000001001110100100111000000,
    0b1000010000111001001010010100101110000000,
    0b0000000000011101000010000100000111000000,
    0b0001000010011101001010010100100111000000,
    0b0000000000011101001011110100000111000000,
    0b0011001000010001110001000010000100000000,
    0b0000000000011101001010010011000001011100,
    0b1000010000111001001010010100101001000000,
    0b0000000100000000110000100001000011000000,
    0b0000000100000000010000100001000010011000,
    0b1000010000100101010011000101001001000000,
    0b0100001000010000100001000010000011000000,
    0b0000000000100101111011110100101001000000,
    0b0000000000111001001010010100101001000000,
    0b0000000000011001001010010100100110000000,
    0b0000000000111001001010010111001000010000,
    0b0000000000011101001010010011100001000010,
    0b0000000000011101001010000100001000000000,
    0b0000000000011101000001100000101110000000,
    0b0100001000111000100001000010000011000000,
    0b0000000000100101001010010100100111000000,
    0b0000000000100101001010010011000110000000,
    0b0000000000100101001011110111101001000000,
    0b0000000000100100110001100100101001000000,
    0b0000000000100101001010010011100001011100,
    0b0000000000111100001000100010001111000000,
    0b0011001000010001100011000010000100000110,
    0b0010000100001000010000100001000010000100,
    0b1100000100001000011000110001000010011000,
    0b0000000000000000100110110000000000000000,
};
char char2pixel(char c, int x, int y) {
    return !isspace(c) && (font[c - 0x21] >> (y * WIDTH + x)) & 1;
}
char *code2image(char *code, int height, int width) {
    char *out = malloc(height * width * (WIDTH + 1) * (HEIGHT + 1));
    memset(out, 0, height * width * (WIDTH + 1) * (HEIGHT + 1));
    int i = 0, index = 0;
    for (int j = 0; j < height * width && code[j] && index <= (height - 1) * width * (WIDTH + 1) * (HEIGHT + 1); j += i + (code[i + j] == '\n'), index += width * (WIDTH + 1))
        for (int y = HEIGHT - 1; y >= 0; y--, index += (width - i) * (WIDTH + 1))
            for (i = 0; i < width && code[i + j] != '\n' && code[i + j] && i < height * width - j; i++, index++)
                for (int x = WIDTH - 1; x >= 0; x--, index++)
                    out[index] = char2pixel(code[j + i], x, y);
    return out;
}
int frames_needed(char *code, int height, int width) {
    int count = 3 - height, x = 0;
    for (int index = 0; index < strlen(code); index++)
        if ((code[index] == '\n') || (x++ == width)) {
            count++;
            x = code[index] != '\n';
        }
    return count <= 1 ? 1 : count;
}
char **code2frames(char *code, int height, int width) {
    int frames = frames_needed(code, height, width), y = 0, x = 0;
    char **out = malloc(frames * sizeof(char*));
    for (int index = 0; index < strlen(code); index++)
        if (!index || (code[index] == '\n') || (x++ == width)) {
            x = code[index] != '\n';
            if (y == frames) return out;
            out[y++] = code2image(code + index, height, width);
        }
    return out;
}
int gif_size(int frame_count, int height, int width) {
    return 38 + frame_count * (22 + width * height * (WIDTH + 1) * (HEIGHT + 1));
}
char *frames2gif(char **frames, int frame_count, int height, int width, float fps, int size) {
    char *out = malloc(size);
    memset(out, 0, size);
    int index = sprintf(out, "GIF89a%c%c%c%c\x82\x02%c\xff\xff\xff", SIZES, 0) + 21;
    int delay = 100 / fps;
    for (int frame_number = 0; frame_number < frame_count; frame_number++) {
        char *frame = frames[frame_number];
        index += sprintf(out + index, "!\xf9\x04\x04%c%c\x02%c,%c%c%c%c%c%c%c%c%c\x03", delay & 0xFF, delay >> 8, 0, 0, 0, 0, 0, SIZES, 0);
        for (int i = 0; i < width * height * (WIDTH + 1) * (HEIGHT + 1); i += 4)
            index += sprintf(out + index, "\x03\x88%c%c", frame[i] + (frame[i + 1] << 4), frame[i + 2] + (frame[i + 3] << 4));
        index += sprintf(out + index, "\x01\x09") + 1;
    }
    out[index] = ';';
    return out;
}
int write_gif(char *text, int height, int width, float fps, FILE *file) {
    int frame_count = frames_needed(text, height, width);
    char **frames = code2frames(text, height, width);
    int size = gif_size(frame_count, height, width);
    fwrite(frames2gif(frames, frame_count, height, width, fps, size), 1, size, file);
    return fclose(file);
}
int format_operations(const char *text) {
    int result = 0;
    for (int i = 0; i < strlen(text); i++)
        if (text[i] == '%')
            result++;
    return result;
}
char *get_source() {
    const char *static_head = "#include <stdlib.h>\n#include <string.h>\n#include <stdio.h>\n#include <ctype.h>\n#define WIDTH %d\n#define HEIGHT %d\n#define SIZES (width * (WIDTH + 1)) & 0xFF, (width * (WIDTH + 1)) >> 8, (height * (HEIGHT + 1)) & 0xFF, (height * (HEIGHT + 1)) >> 8\nlong font[94] = {";
    char *head = malloc(strlen(static_head) + 94 * (8 + WIDTH * HEIGHT) + 2);
    int offset = sprintf(head, static_head, WIDTH, HEIGHT);
    for (int i = 0; i < 94; i++) {
        offset += sprintf(head + offset, "\n    0b");
        for (int j = WIDTH * HEIGHT - 1; j >= 0; j--)
            offset += sprintf(head + offset, "%d", (font[i] >> j) & 1);
        head[offset++] = ',';
    }
    char *source = "%c};%cchar char2pixel(char c, int x, int y) {%c    return !isspace(c) && (font[c - 0x21] >> (y * WIDTH + x)) & 1;%c}%cchar *code2image(char *code, int height, int width) {%c    char *out = malloc(height * width * (WIDTH + 1) * (HEIGHT + 1));%c    memset(out, 0, height * width * (WIDTH + 1) * (HEIGHT + 1));%c    int i = 0, index = 0;%c    for (int j = 0; j < height * width && code[j] && index <= (height - 1) * width * (WIDTH + 1) * (HEIGHT + 1); j += i + (code[i + j] == '%cn'), index += width * (WIDTH + 1))%c        for (int y = HEIGHT - 1; y >= 0; y--, index += (width - i) * (WIDTH + 1))%c            for (i = 0; i < width && code[i + j] != '%cn' && code[i + j] && i < height * width - j; i++, index++)%c                for (int x = WIDTH - 1; x >= 0; x--, index++)%c                    out[index] = char2pixel(code[j + i], x, y);%c    return out;%c}%cint frames_needed(char *code, int height, int width) {%c    int count = 3 - height, x = 0;%c    for (int index = 0; index < strlen(code); index++)%c        if ((code[index] == '%cn') || (x++ == width)) {%c            count++;%c            x = code[index] != '%cn';%c        }%c    return count <= 1 ? 1 : count;%c}%cchar **code2frames(char *code, int height, int width) {%c    int frames = frames_needed(code, height, width), y = 0, x = 0;%c    char **out = malloc(frames * sizeof(char*));%c    for (int index = 0; index < strlen(code); index++)%c        if (!index || (code[index] == '%cn') || (x++ == width)) {%c            x = code[index] != '%cn';%c            if (y == frames) return out;%c            out[y++] = code2image(code + index, height, width);%c        }%c    return out;%c}%cint gif_size(int frame_count, int height, int width) {%c    return 38 + frame_count * (22 + width * height * (WIDTH + 1) * (HEIGHT + 1));%c}%cchar *frames2gif(char **frames, int frame_count, int height, int width, float fps, int size) {%c    char *out = malloc(size);%c    memset(out, 0, size);%c    int index = sprintf(out, %cGIF89a%cc%cc%cc%cc%cx82%cx02%cc%cxff%cxff%cxff%c, SIZES, 0) + 21;%c    int delay = 100 / fps;%c    for (int frame_number = 0; frame_number < frame_count; frame_number++) {%c        char *frame = frames[frame_number];%c        index += sprintf(out + index, %c!%cxf9%cx04%cx04%cc%cc%cx02%cc,%cc%cc%cc%cc%cc%cc%cc%cc%cc%cx03%c, delay & 0xFF, delay >> 8, 0, 0, 0, 0, 0, SIZES, 0);%c        for (int i = 0; i < width * height * (WIDTH + 1) * (HEIGHT + 1); i += 4)%c            index += sprintf(out + index, %c%cx03%cx88%cc%cc%c, frame[i] + (frame[i + 1] << 4), frame[i + 2] + (frame[i + 3] << 4));%c        index += sprintf(out + index, %c%cx01%cx09%c) + 1;%c    }%c    out[index] = ';';%c    return out;%c}%cint write_gif(char *text, int height, int width, float fps, FILE *file) {%c    int frame_count = frames_needed(text, height, width);%c    char **frames = code2frames(text, height, width);%c    int size = gif_size(frame_count, height, width);%c    fwrite(frames2gif(frames, frame_count, height, width, fps, size), 1, size, file);%c    return fclose(file);%c}%cint format_operations(const char *text) {%c    int result = 0;%c    for (int i = 0; i < strlen(text); i++)%c        if (text[i] == '%c')%c            result++;%c    return result;%c}%cchar *get_source() {%c    const char *static_head = %c#include <stdlib.h>%cn#include <string.h>%cn#include <stdio.h>%cn#include <ctype.h>%cn#define WIDTH %cd%cn#define HEIGHT %cd%cn#define SIZES (width * (WIDTH + 1)) & 0xFF, (width * (WIDTH + 1)) >> 8, (height * (HEIGHT + 1)) & 0xFF, (height * (HEIGHT + 1)) >> 8%cnlong font[94] = {%c;%c    char *head = malloc(strlen(static_head) + 94 * (8 + WIDTH * HEIGHT) + 2);%c    int offset = sprintf(head, static_head, WIDTH, HEIGHT);%c    for (int i = 0; i < 94; i++) {%c        offset += sprintf(head + offset, %c%cn    0b%c);%c        for (int j = WIDTH * HEIGHT - 1; j >= 0; j--)%c            offset += sprintf(head + offset, %c%cd%c, (font[i] >> j) & 1);%c        head[offset++] = ',';%c    }%c    char *source = %c%s%c;%c    char *out = malloc(strlen(source) * 2 + strlen(head) - format_operations(source));%c    offset = sprintf(out, head);%c    sprintf(out + offset, source, 10, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34, 37, 37, 37, 37, 92, 92, 37, 92, 92, 92, 34, 10, 10, 10, 10, 34, 92, 92, 92, 37, 37, 92, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 92, 34, 10, 10, 34, 92, 92, 37, 37, 34, 10, 34, 92, 92, 34, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 37, 10, 10, 10, 10, 10, 34, 92, 92, 92, 92, 37, 92, 37, 92, 92, 34, 10, 10, 10, 10, 34, 92, 34, 10, 10, 34, 37, 34, 10, 10, 10, 34, source, 34, 10, 10, 10, 10, 10, 10, 10, 10);%c    return out;%c}%cint main(void) {%c    return write_gif(get_source(), 60, 120, 10, stdout);%c}";
    char *out = malloc(strlen(source) * 2 + strlen(head) - format_operations(source));
    offset = sprintf(out, head);
    sprintf(out + offset, source, 10, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34, 37, 37, 37, 37, 92, 92, 37, 92, 92, 92, 34, 10, 10, 10, 10, 34, 92, 92, 92, 37, 37, 92, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 92, 34, 10, 10, 34, 92, 92, 37, 37, 34, 10, 34, 92, 92, 34, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 37, 10, 10, 10, 10, 10, 34, 92, 92, 92, 92, 37, 92, 37, 92, 92, 34, 10, 10, 10, 10, 34, 92, 34, 10, 10, 34, 37, 34, 10, 10, 10, 34, source, 34, 10, 10, 10, 10, 10, 10, 10, 10);
    return out;
}
int main(void) {
    return write_gif(get_source(), 60, 120, 10, stdout);
}
