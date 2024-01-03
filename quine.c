#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
int font[94] = {
    0b0010000100001000000000100,
    0b1001010010000000000000000,
    0b0101011111010101111101010,
    0b0111110100011100010111110,
    0b1000100010001000100010001,
    0b0110010100011011001001101,
    0b0010000100000000000000000,
    0b0010001000010000100000100,
    0b0010000010000100001000100,
    0b0010011111001000101000000,
    0b0010000100111110010000100,
    0b0000000000000000010001000,
    0b0000000000111110000000000,
    0b0000000000000000000000100,
    0b0000100010001000100010000,
    0b1111110011101011100111111,
    0b0010001100001000010001110,
    0b1111000001011101000011111,
    0b1111100001011100000111111,
    0b0010001100111110010000100,
    0b1111110000111110000111111,
    0b1111110000111111000111111,
    0b1110000100111110010000100,
    0b1111110001111111000111111,
    0b1111110001111110000111111,
    0b0010000000000000000000100,
    0b0010000000000000010001100,
    0b0010001000100000100000100,
    0b0000011111000001111100000,
    0b0010000010000010001000100,
    0b0111010001001100000000100,
    0b1111110101101111000011111,
    0b0010001010011101000110001,
    0b1110010010111001001011100,
    0b1111110000100001000011111,
    0b1110010010100101001011100,
    0b1111110000111111000011111,
    0b1111110000111111000010000,
    0b1111110000101111000111111,
    0b1000110001111111000110001,
    0b1111100100001000010011111,
    0b1111100100001000010011100,
    0b1000110010111001001010001,
    0b1000010000100001000011111,
    0b1000111011101011000110001,
    0b1000111001101011001110001,
    0b1111110001100011000111111,
    0b1111010001111101000010000,
    0b1111010010101101111000001,
    0b1110010010111001001010001,
    0b0111110000011100000111110,
    0b1111100100001000010000100,
    0b1000110001100011000111111,
    0b1000110001100010101000100,
    0b1000110001101011101110001,
    0b1000101010001000101010001,
    0b1000101010001000010000100,
    0b1111100010001000100011111,
    0b1100010000100001000011000,
    0b1000001000001000001000001,
    0b0001100001000010000100011,
    0b0010001010000000000000000,
    0b0000000000000000000011111,
    0b0100000100000000000000000,
    0b0000000000111001010011110,
    0b1000010000111001010011100,
    0b0000000000111001000011100,
    0b0010000100111001010011100,
    0b0111010001111101000001111,
    0b0111101000111100100001000,
    0b0111110001011110000111111,
    0b1000010000111001010010100,
    0b0010000000001000010000100,
    0b0010000000001000010011100,
    0b1000110010101001101010001,
    0b0000000100001000010000100,
    0b0000001010101011010110101,
    0b0000000000111001001010010,
    0b0000001110100011000101110,
    0b0000011100101001110010000,
    0b0000011100101001110000100,
    0b1011101000100001000010000,
    0b0011001000001000001011100,
    0b0010000100011100010000111,
    0b0000000000100101001001111,
    0b0000000000010100101000100,
    0b0000000000100011010101010,
    0b0000000000100010111010001,
    0b1000110001011110000111111,
    0b0000011110001000100011110,
    0b0111001000110000100001110,
    0b0010000100001000010000100,
    0b0111000010000110001001110,
    0b0000001010101000000000000,
};
char char2pixel(char c, int x, int y) {
    return !isspace(c) && (font[c - 0x21] >> (y * 5 + x)) & 1;
}
char *code2image(char *code, int height, int width) {
    char *out = malloc(height * width * 36);
    memset(out, 0, height * width * 36);
    int i = 0, index = 0;
    for (int j = 0; j < height * width && code[j] && index <= (height - 1) * width * 36; j += i + (code[i + j] == '\n'), index += width * 6)
        for (int y = 4; y >= 0; y--, index += (width - i) * 6)
            for (i = 0; i < width && code[i + j] != '\n' && code[i + j] && i < height * width - j; i++, index++)
                for (int x = 4; x >= 0; x--, index++)
                    out[index] = char2pixel(code[j + i], x, y);
    return out;
}
int frames_needed(char *code, int height, int width) {
    int count = 2 - height, x = 0;
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
    return 38 + frame_count * (22 + 36 * width * height);
}
char *frames2gif(char **frames, int frame_count, int height, int width, float fps, int size) {
    char *out = malloc(size);
    memset(out, 0, size);
    int index = sprintf(out, "GIF89a%c%c%c%c\x82\x02%c\xff\xff\xff", (width * 6) & 0xFF, (width * 6) >> 8, (height * 6) & 0xFF, (height * 6) >> 8, 0) + 21;
    int delay = 100 / fps;
    for (int frame_number = 0; frame_number < frame_count; frame_number++) {
        char *frame = frames[frame_number];
        index += sprintf(out + index, "!\xf9\x04\x04%c%c\x02%c,%c%c%c%c%c%c%c%c%c\x03", delay & 0xFF, delay >> 8, 0, 0, 0, 0, 0, (width * 6) & 0xFF, (width * 6) >> 8, (height * 6) & 0xFF, (height * 6) >> 8, 0);
        for (int i = 0; i < width * height * 36; i += 4)
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
    const char *static_head = "#include <stdlib.h>\n#include <string.h>\n#include <stdio.h>\n#include <ctype.h>\nint font[94] = {";
    int offset = strlen(static_head);
    char *head = strcpy(malloc(offset + 3106), static_head);
    for (int i = 0; i < 94; i++) {
        offset += sprintf(head + offset, "\n    0b");
        for (int j = 24; j >= 0; j--)
            offset += sprintf(head + offset, "%d", (font[i] >> j) & 1);
        head[offset++] = ',';
    }
    char *source = "%c};%cchar char2pixel(char c, int x, int y) {%c    return !isspace(c) && (font[c - 0x21] >> (y * 5 + x)) & 1;%c}%cchar *code2image(char *code, int height, int width) {%c    char *out = malloc(height * width * 36);%c    memset(out, 0, height * width * 36);%c    int i = 0, index = 0;%c    for (int j = 0; j < height * width && code[j] && index <= (height - 1) * width * 36; j += i + (code[i + j] == '%cn'), index += width * 6)%c        for (int y = 4; y >= 0; y--, index += (width - i) * 6)%c            for (i = 0; i < width && code[i + j] != '%cn' && code[i + j] && i < height * width - j; i++, index++)%c                for (int x = 4; x >= 0; x--, index++)%c                    out[index] = char2pixel(code[j + i], x, y);%c    return out;%c}%cint frames_needed(char *code, int height, int width) {%c    int count = 2 - height, x = 0;%c    for (int index = 0; index < strlen(code); index++)%c        if ((code[index] == '%cn') || (x++ == width)) {%c            count++;%c            x = code[index] != '%cn';%c        }%c    return count <= 1 ? 1 : count;%c}%cchar **code2frames(char *code, int height, int width) {%c    int frames = frames_needed(code, height, width), y = 0, x = 0;%c    char **out = malloc(frames * sizeof(char*));%c    for (int index = 0; index < strlen(code); index++)%c        if (!index || (code[index] == '%cn') || (x++ == width)) {%c            x = code[index] != '%cn';%c            if (y == frames) return out;%c            out[y++] = code2image(code + index, height, width);%c        }%c    return out;%c}%cint gif_size(int frame_count, int height, int width) {%c    return 38 + frame_count * (22 + 36 * width * height);%c}%cchar *frames2gif(char **frames, int frame_count, int height, int width, float fps, int size) {%c    char *out = malloc(size);%c    memset(out, 0, size);%c    int index = sprintf(out, %cGIF89a%cc%cc%cc%cc%cx82%cx02%cc%cxff%cxff%cxff%c, (width * 6) & 0xFF, (width * 6) >> 8, (height * 6) & 0xFF, (height * 6) >> 8, 0) + 21;%c    int delay = 100 / fps;%c    for (int frame_number = 0; frame_number < frame_count; frame_number++) {%c        char *frame = frames[frame_number];%c        index += sprintf(out + index, %c!%cxf9%cx04%cx04%cc%cc%cx02%cc,%cc%cc%cc%cc%cc%cc%cc%cc%cc%cx03%c, delay & 0xFF, delay >> 8, 0, 0, 0, 0, 0, (width * 6) & 0xFF, (width * 6) >> 8, (height * 6) & 0xFF, (height * 6) >> 8, 0);%c        for (int i = 0; i < width * height * 36; i += 4)%c            index += sprintf(out + index, %c%cx03%cx88%cc%cc%c, frame[i] + (frame[i + 1] << 4), frame[i + 2] + (frame[i + 3] << 4));%c        index += sprintf(out + index, %c%cx01%cx09%c) + 1;%c    }%c    out[index] = ';';%c    return out;%c}%cint write_gif(char *text, int height, int width, float fps, FILE *file) {%c    int frame_count = frames_needed(text, height, width);%c    char **frames = code2frames(text, height, width);%c    int size = gif_size(frame_count, height, width);%c    fwrite(frames2gif(frames, frame_count, height, width, fps, size), 1, size, file);%c    return fclose(file);%c}%cint format_operations(const char *text) {%c    int result = 0;%c    for (int i = 0; i < strlen(text); i++)%c        if (text[i] == '%c')%c            result++;%c    return result;%c}%cchar *get_source() {%c    const char *static_head = %c#include <stdlib.h>%cn#include <string.h>%cn#include <stdio.h>%cn#include <ctype.h>%cnint font[94] = {%c;%c    int offset = strlen(static_head);%c    char *head = strcpy(malloc(offset + 3106), static_head);%c    for (int i = 0; i < 94; i++) {%c        offset += sprintf(head + offset, %c%cn    0b%c);%c        for (int j = 24; j >= 0; j--)%c            offset += sprintf(head + offset, %c%cd%c, (font[i] >> j) & 1);%c        head[offset++] = ',';%c    }%c    char *source = %c%s%c;%c    char *out = malloc(strlen(source) * 2 + strlen(head) - format_operations(source));%c    offset = sprintf(out, head);%c    sprintf(out + offset, source, 10, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34, 37, 37, 37, 37, 92, 92, 37, 92, 92, 92, 34, 10, 10, 10, 10, 34, 92, 92, 92, 37, 37, 92, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 92, 34, 10, 10, 34, 92, 92, 37, 37, 34, 10, 34, 92, 92, 34, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 37, 10, 10, 10, 10, 10, 34, 92, 92, 92, 92, 34, 10, 10, 10, 10, 34, 92, 34, 10, 10, 34, 37, 34, 10, 10, 10, 34, source, 34, 10, 10, 10, 10, 10, 10, 10, 10);%c    return out;%c}%cint main(void) {%c    return write_gif(get_source(), 30, 60, 10, stdout);%c}";
    char *out = malloc(strlen(source) * 2 + strlen(head) - format_operations(source));
    offset = sprintf(out, head);
    sprintf(out + offset, source, 10, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 92, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 34, 37, 37, 37, 37, 92, 92, 37, 92, 92, 92, 34, 10, 10, 10, 10, 34, 92, 92, 92, 37, 37, 92, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 92, 34, 10, 10, 34, 92, 92, 37, 37, 34, 10, 34, 92, 92, 34, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 37, 10, 10, 10, 10, 10, 34, 92, 92, 92, 92, 34, 10, 10, 10, 10, 34, 92, 34, 10, 10, 34, 37, 34, 10, 10, 10, 34, source, 34, 10, 10, 10, 10, 10, 10, 10, 10);
    return out;
}
int main(void) {
    return write_gif(get_source(), 30, 60, 10, stdout);
}
