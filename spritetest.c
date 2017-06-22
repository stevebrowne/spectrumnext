/*
 *	Sprite test
 *  Steve Browne, steveb@quickweb.co.uk
 *
 *  Compile with: 
 *      zcc +zx -vn -startup=5 -clib=new spritetest.c -o spritetest -create-app
 */
#include <stdlib.h>
#include <stdio.h>
#include <z80.h>

#define OUT                 z80_outp
#define cls()               printf("%c",12)
#define printAt(row,col)    printf("%c%c%c",22,col,row)

const int XCoord = 0, YCoord = 1, XDir = 2, YDir = 3;

const int XboundLow = 0, XboundHi = 319, YboundLow = 0, YboundHi = 255; // Over borders
//const int XboundLow = 31, XboundHi = 288, YboundLow = 31, YboundHi = 224; // Within borders

const int NumberOfSprites = 20; // Number of sprites
const int SpriteArrayData = 4; // Dimensions of data: (x,y) and x and y directions

int sprites[80]; // MUST BE NumberOfSprites * SpriteArrayData to represent  [NumberOfSprites][SpriteArrayData]

main()
{
    int i,l;
    cls();
    // Put a border on
    OUT(254,2);
    printf("Z88DK Sprite test\n\n");

    loadPatternData(0); // Load the pattern data into pattern 0

    for(l=0; l< NumberOfSprites; l++) {
        // Set each sprite to start at 100,100
        setSpriteStartPositions(l,100,100);
    }

    // Clear the screen
    //cls;

    //Show the sprites
    showSprites();

    // Main loop, loop forever
    for(i=0; i < 1000 ; i++) {
        for(l=0; l< NumberOfSprites; l++) {

            // Check we're not going off the left or right, and if we do, then bounce
            if(sprites[SpriteArrayData*l+XCoord] + 16 + sprites[SpriteArrayData*l+XDir] > XboundHi || sprites[SpriteArrayData*l+XCoord] + sprites[SpriteArrayData*l+XDir] < XboundLow) {
                sprites[SpriteArrayData*l+XDir] = sprites[SpriteArrayData*l+XDir] * - 1;
            }
            
            // Check we're not going off the top or bottom, and if we do, then bounce
            if(sprites[SpriteArrayData*l+YCoord] + 16 + sprites[SpriteArrayData*l+YDir] > YboundHi || sprites[SpriteArrayData*l+YCoord] + sprites[SpriteArrayData*l+YDir] < YboundLow) {
                sprites[SpriteArrayData*l+YDir] = sprites[SpriteArrayData*l+YDir] * - 1;
            }

            // Update the position in the direction of travel
            sprites[SpriteArrayData*l+XCoord] = sprites[SpriteArrayData*l+XCoord] + sprites[SpriteArrayData*l+XDir];
            sprites[SpriteArrayData*l+YCoord] = sprites[SpriteArrayData*l+YCoord] + sprites[SpriteArrayData*l+YDir];

            // Select sprite
            OUT(0x303B, l);

            // OUT the 4 bytes related to a sprite
            if(sprites[SpriteArrayData*l+XCoord] < 255) {
                OUT(0x57, sprites[SpriteArrayData*l+XCoord]);
                OUT(0x57, sprites[SpriteArrayData*l+YCoord]);
                OUT(0x57, 0);
                OUT(0x57, 128);
            } else {
                OUT(0x57, sprites[SpriteArrayData*l+XCoord]-255);
                OUT(0x57, sprites[SpriteArrayData*l+YCoord]);
                OUT(0x57, 1);  // If we have gone over 255 in x then we need to set the overflow flag
                OUT(0x57, 128);
            }
        }
    }

}

// Show sprites
int showSprites() {
    OUT(0x243B, 0x15);
    OUT(0x253B, 0x03); // Change to 0x01 to stay in borders
}

// Hide sprites
int hideSprites() {
    OUT(0x243B, 0x15);
    OUT(0x253B, 0x0);
}

// ------------ Setup stuff, patterns, start positions etc. --------------

int loadPatternData(int patternId) {
    int i;
    static int spriteData[] = {
        0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x04, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0x04, 0xFF, 0xFB, 0xFB, 0xFB, 0xFF, 0x04, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0x04, 0xFF, 0xFB, 0xF5, 0xF5, 0xFB, 0xFF, 0x04, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0x04, 0xFF, 0xFB, 0xF5, 0xA8, 0xA8, 0xFB, 0xFF, 0x04, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0x04, 0xFF, 0xFF, 0xFB, 0xA8, 0x44, 0xA8, 0xFB, 0xFF, 0x04, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0x04, 0x04, 0x04, 0xFF, 0xFB, 0xA8, 0x44, 0xA8, 0xFB, 0xFF, 0x04, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
        0xE3, 0xE3, 0xE3, 0x04, 0xFF, 0xFB, 0xA8, 0x44, 0x44, 0xFB, 0xFF, 0x04, 0xE3, 0x04, 0xE3, 0xE3,
        0xE3, 0xE3, 0xE3, 0xE3, 0x04, 0xFF, 0xFB, 0x44, 0x44, 0x44, 0xFB, 0xFF, 0x04, 0x4D, 0x04, 0xE3,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0x04, 0xFF, 0xFB, 0x44, 0x44, 0x44, 0x44, 0xFA, 0x4D, 0x04, 0xE3,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0x04, 0xFF, 0xFB, 0x44, 0xFF, 0xF5, 0x44, 0x04, 0xE3, 0xE3,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0x04, 0xFF, 0x44, 0xF5, 0xA8, 0x04, 0xE3, 0xE3, 0xE3,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0x04, 0xFA, 0x44, 0x04, 0xA8, 0x04, 0xE3, 0xE3,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0x04, 0x4D, 0x4D, 0x04, 0xE3, 0x04, 0xF5, 0x04, 0xE3,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0x04, 0x04, 0xE3, 0xE3, 0xE3, 0x04, 0xFA, 0x04,
        0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0x04, 0x04,
        0x0
      };
    
    // Set the pattern ID where this pattern will be loaded
    OUT(0x303B, patternId);
    
    //Loop through and load the pattern data
    for(i=0; i < 256; i++) {
        OUT(0x55, spriteData[i]);
    }
}

int setSpriteStartPositions(int spriteId, int x, int y) {
    sprites[SpriteArrayData*spriteId+XCoord] = x;
    sprites[SpriteArrayData*spriteId+YCoord] = y;
    sprites[SpriteArrayData*spriteId+XDir] = getRandomDir();
    sprites[SpriteArrayData*spriteId+YDir] = getRandomDir();

    OUT(0x303B, spriteId);
    OUT(0x57, x);
    OUT(0x57, y);
    OUT(0x57, 0);
    OUT(0x57, 128);
}

// Pick a random direction/speed.
// Change speedVariants to set how many different speeds there are
int getRandomDir() {
    int tmp, speedVariants;

    speedVariants = 10;

    tmp = rand() % speedVariants;
    if(tmp <= (speedVariants/2)-1) {
        return tmp-(speedVariants/2);  // Skip 0 speed
    }

    return tmp-(speedVariants/2)-1;
}
