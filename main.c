#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>

typedef struct {
  double x;
  double y;
} Vec;


double dotproduct(Vec a, Vec b) {
	return a.x * b.x + a.y * b.y;
}

Vec offset(Vec a, Vec b) {
  Vec result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
  return result;
}

double lerp(double v0, double v1, double t) {
  return v0 + t * (v1 - v0);
}

#define grid_side_length 400

Vec grid[grid_side_length * grid_side_length];

void init() {
  for (int y = 0; y < grid_side_length; y++) {
    for (int x = 0; x < grid_side_length; x++) {
			double randomnumber = (double)rand();
      grid[grid_side_length*y + x].x = cos(randomnumber)/2+0.5;
      grid[grid_side_length*y + x].y = sin(randomnumber)/2+0.5;
    }
  }
}

Vec indexgrid(Vec pos) {
	return grid[(int)pos.y * grid_side_length + (int)pos.x];
}

double perlin(Vec input) {
  int cornerx0 = floor(input.x);
  int cornery0 = floor(input.y);
  //int cornerx1 = cornerx0 + 1;
  //int cornery1 = cornery0 + 1;
	//printf("%d %d %d %d\n", cornerx1, cornery1, cornerx0, cornery0);

	Vec upleft = (Vec) {.x = floor(input.x), .y = floor(input.y)};
	Vec upright = (Vec) {.x = floor(input.x)+1, .y = floor(input.y)};
	Vec downleft = (Vec) {.x = floor(input.x), .y = floor(input.y)+1};
	Vec downright = (Vec) {.x = floor(input.x)+1, .y = floor(input.y)+1};

	double wx = input.x - (double)cornerx0;
	double wy = input.y - (double)cornery0;

	Vec upleftgradient = indexgrid(upleft);
	Vec uprightgradient = indexgrid(upright);
	Vec downleftgradient = indexgrid(downleft);
	Vec downrightgradient = indexgrid(downright);

  Vec upleftoff = offset(input, upleft);
  Vec uprightoff = offset(input, upright);
  Vec downleftoff = offset(input, downleft);
  Vec downrightoff = offset(input, downright);

	double upleftproduct = dotproduct(upleftoff, upleftgradient);
	double uprightproduct = dotproduct(uprightoff, uprightgradient);
	double downleftproduct = dotproduct(downleftoff, downleftgradient);
	double downrightproduct = dotproduct(downrightoff, downrightgradient);
	
	double ab = lerp(upleftproduct, uprightproduct, wx);
	double cd = lerp(downleftproduct, downrightproduct, wx);
	double result = lerp(ab, cd, wy);
	return result;
}

int main() {
  srand(time(NULL));
	init();
	InitWindow(grid_side_length, grid_side_length, "idkman"); 
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
			//ClearBackground(RAYWHITE);
			for (double x = 0; x < grid_side_length; x += 1) {
				for (double y = 0; y < grid_side_length; y += 1) {
					double pee = (perlin((Vec) {.x = x/16, .y = y/16})/2+0.5)*255;
					DrawPixel(x, y, 
							(Color) {
								.r = (char)pee, 
								.g = (char)pee, 
								.b = (char)pee, 
								.a = 255
							});
				}
			}
		EndDrawing();
	}
	
  return 0;
}
