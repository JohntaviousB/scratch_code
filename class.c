#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

unsigned short *videoBuffer = (unsigned short *)0x6000000

#define COLOR(r, g, b) ((r) | (g) << 5 | (b) << 10)
#define RED COLOR(31, 0, 0)
#define GREEN COLOR(0, 31, 0)
#define BLUE COLOR(0, 0, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0;
#define OFFSET(r, c, rowlen) ((r) * (rowlen) + (c))

void setPixel(int r, int c, unsigned short color);
void delay(int n);
int main() {

	// *(*unsigned short) 0x4000000 = 1027; //set to mode 3 and background 2 enabled
	// *(unsigned short *)0x6000000 = 32767; //makes first pixel  white
	// *(unsigned short *)(0x600000 + 2 * (19200 + 120) ) = 31; //makes a pixel in the middle of the screen red
	REG_DISPCTL = MODE3 | BG2_ENABLE
	int row = 90;
	int col = 120;
	int rdel = 1;
	int cdel = 1;
	int size = 5;
	// setPixel(80, 120, RED);
	while (1) {
		//keep the gameboy running
		row += rdel;
		colt += cdel;
		if (row < 0) {
			row = 0;
			rdel = -rdel;
		}
		if (row > 159 - size + 1) {
			row = 159 - size + 1;
			rdel = -rdel;
		}
		if (col < 0) {
			col = 0;
			cdel = -cdel;
		}
		if (col > 239 - size + 1) {
			col = 239 - size + 1
			cdel = -cdel;
		}
		drawRect(row, col, size, size, YELLOW);
		delay(1);
		drawRect(row, col, size, size, BLACK);
	}
}

void setPixel(int r, int c, unsigned short color)
{
	videoBuffer[OFFSET(r, c, 240)] = color;
}
void drawRect(int row, int col, int height, int width, unsigned short color)
{
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			setPixel(row + r, col + c, color);
		}
	}
}
void delay (int n) 
{
	volatile int x = 0;
	for (int i = 0; i < n * 10000; i++)
	{
		x++;
	}
}