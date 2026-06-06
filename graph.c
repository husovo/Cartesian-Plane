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

int main()
{
	InitWindow(width,height,"plane");
	SetTargetFPS(60);
	float angle = 0.0f;
	while(!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		// draw the cartesian plane
		for(int x = 0; x <= width; x += s){
			DrawLine(x,0,x,height,BLACK);
		}
		for(int y = 0; y <= height; y += s)
		{
			DrawLine(0,y,width,y,BLACK);
		}
		// draw the x and y axis
		DrawRectangle(0, height/2-2,width, 4, BLACK);
		DrawRectangle(width/2-2,0,4,height,BLACK);	
		// draw stuff on the plane
		DrawCircleV(plot(rotP((Vector2){3.0f,2.0f},angle)),18,RED);
		angle += 0.01f;
		
		EndDrawing();
	}
CloseWindow();
	return 0;
}
