#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include "../utils/sysmem.h"
#include "../utils/sysio.h"
#include "../utils/sysstr.h"
#include "../utils/font8x8.h"
#include "../drivers/keyboard.h"




__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;










struct limine_framebuffer *framebuffer;

void drawPixel(int x, int y){
    volatile uint32_t *fb_ptr = framebuffer->address;
    fb_ptr[x + (framebuffer->width) * y] = 0xffffff;
}
void drawBlock(int x, int y, int sizex, int sizey){
    volatile uint32_t *fb_ptr = framebuffer->address;
    for(int i = 0; i < sizex; i++){
        for(int j = 0; j < sizey; j++){
            fb_ptr[(x+i) + (framebuffer->width) * (y+j)] = 0xffffff;
        }
    }
}


void drawLetter(char *bitmap ,int x, int y, int sizex, int sizey) {
    int i,j;
    int set;

    for (i=0; i < 8; i++) {
        for (j=0; j < 8; j++) {
            set = bitmap[i] & 1 << j;
            set ? drawBlock((j+x)*sizex, (i+y)*sizey, sizex, sizey) : NULL;
        }
    }
}

void print(char str[]){
    int len = strlen(str);

    int height = 0;
    int width = 0;
    for(int i = 0; i < len; i++){
        if(str[i] == '\n'){
            height+=8;
            width = 0;
            continue;
        }

        drawLetter(font8x8[str[i]], 1 + width, 1 + height, 2, 2);
        width += 8;
    }
}

void kmain(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
    framebuffer = framebuffer_request.response->framebuffers[0];
    initKeyboard();


    
    //drawPixel(2,2);
    //drawLetter(font8x8['A'], 1 , 1, 2, 2);
    print("unsatisfactoryOS\ndasdsa");
    hcf();
}
