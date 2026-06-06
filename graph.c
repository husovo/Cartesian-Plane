#include <raylib.h>
#include <math.h>
// -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
// type: "make graph" to compile
#define width 1800
#define height 1400

const Vector2 origin = {width/2.0f, height/2.0f};
const int s = 50; // lenght on 1 unit in pixels

Vector2 plot(Vector2 p) // plots cartesain co-ordiantes to screen
{	// O+P = P	
	return (Vector2)
	{
		origin.x + (s*p.x),
		origin.y - (s*p.y)
	};
}

Vector2 rotP(Vector2 p,float angle)
{
	return (Vector2)
	{
		p.x*cosf(angle) -p.y*sinf(angle),
		p.x*sinf(angle) +p.y*cosf(angle)
	};
}

void drawPlane(void);


int main()
{
	InitWindow(width,height,"plane");
	SetTargetFPS(60);
	float angle = 0.0f;
	while(!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		drawPlane();
		// draw stuff on the plane
		DrawCircleV(plot((Vector2){3.0f,2.0f}),18.0f,RED);
		DrawCircleV(plot((Vector2){-3.0f,4.0f}),15.0f,RED);
		DrawCircleV(plot((Vector2){-5.0f,-5.0f}),10.0f,GREEN);
	
		angle += 0.01f;
		
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

void drawPlane(void){
	int stepH = origin.x;
	int stepV = origin.y;
	int winwidth = (width/2)/s;
	int winheight = (height/2)/s;


		// draw the cartesian plane
		for(int x = 0; x <= width; x += s){
			DrawLine(x,0,x,height,BLACK);
		}
		for(int y = 0; y <= height; y += s)
		{
			DrawLine(0,y,width,y,BLACK);
		}
		// number the horizonatal axis.
		for(int i = 1; i <= winwidth; i++)
		{
			DrawText(TextFormat("%d",i), stepH + i*s-5, stepV+s/2-15, 30, BLACK);	
			DrawText(TextFormat("%d",-i), stepH - i*s-5, stepV+s/2-15, 30, BLACK);
		}
		// number the vertical axis
		for(int i = 1; i <= winheight; i++)
		{
			DrawText(TextFormat("%d",i), stepH-s/2-10, stepV - i*s-12, 30, BLACK);	
			DrawText(TextFormat("%d",-i), stepH-s/2-20, stepV + i*s-12, 30, BLACK);	
		}
		// draw the x and y axis
		DrawRectangle(0, height/2-2,width, 4, BLACK);
		DrawRectangle(width/2-2,0,4,height,BLACK);	
}
