#include <raylib.h>
#include <math.h>
#include"raymath.h"
#include"rlgl.h"

// -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
// type: "make graph" to compile
#define width 1400
#define height 1000

typedef struct line
{
	Vector2 P;
	Vector2 Q;
} line;

const Vector2 origin = {width / 2.0f, height / 2.0f};
const int SCALE= 50; // lenght on 1 unit in pixels

Vector2 plot(Vector2 p) // plots cartesain co-ordiantes to screen
{					
		// O+P = P

	return (Vector2){
		origin.x + ( SCALE * p.x),
		origin.y - ( SCALE * p.y)};
}

Vector2 rotatePoint(Vector2 p, float angle)
{
	return (Vector2){
		p.x * cosf(angle) - p.y * sinf(angle),
		p.x * sinf(angle) + p.y * cosf(angle)};
}

void drawPlane(float);
void drawCircle(Vector2 centre, float r);

Vector2 lineDirection(line l1)
{

	Vector2 p = {l1.P.x, l1.P.y};
	Vector2 q = {l1.Q.x, l1.Q.y};
	// direction of the line
	float dx = (q.x - p.x);
	float dy = (q.y - p.y);
	return (Vector2){dx, dy};
}


void drawLine(line l1, float t)
{
	Vector2 p = {l1.P.x, l1.P.y};
	Vector2 q = {l1.Q.x, l1.Q.y};
	// direction of the line
	float dx = (q.x - p.x);
	float dy = (q.y - p.y);

	// parametric representation P +  t(Q -  P).
	Vector2 pline = {
		p.x + t * dx,
		p.y + t * dy
	};
	// from p to negative direction
	Vector2 nline = {
		p.x - t * dx,
		p.y - t * dy
	};

	DrawLineEx(plot(nline), plot(pline), 3.0f, RED);
	// find the x intercept using basic algebra
	if (dy != 0)
	{ // y(t) = p.y + t(dx,dy)
		Vector2 x_intercept = {p.x + (-p.y * dx) / dy, 0.0f};
		DrawCircleV(plot(x_intercept), 6.0f, BLUE);
	}

	// find the y intercept using basic algebra
	if (dx != 0)
	{ // x(t) = p.x + t(dx,dy)
		Vector2 y_intercept = {0.0f, p.y + (-p.x * dy) / dx};
		DrawCircleV(plot(y_intercept), 6.0f, BLUE);
	}
}
void drawParabola(float a, float b, float c)
{
	for(float x = -10.0f; x< 10.0f;x+=0.01f)
	{
		float y = a*x*x + b*x +c;
		DrawCircleV(plot((Vector2){x,y}),2,PURPLE);
	}
}

Vector2 pointOfIntersection(line l1, line l2)
{
	Vector2 A = lineDirection(l1);
	Vector2 Ap = lineDirection(l2);
	float t, d = (A.y * Ap.x - A.x * Ap.y);

	if (d != 0)
	{
		// c =(A.x * (l2.P.y-l1.P.y) - A.y * (l2.P.x-l1.P.x))/d;
		t = (Ap.x * (l2.P.y - l1.P.y) - Ap.y * (l2.P.x - l1.P.x)) / d;
	}
	else
	{
		return (Vector2){0.0f, 0.0f};
	}
	// the point of intersection is the same weather we use t*line1 or c*line 2.
	return (Vector2){l1.P.x + t * A.x, l1.P.y + t * A.y};
}
int main(void)
{
	InitWindow(width, height, "plane");
	SetTargetFPS(60);
	Camera2D camera;
	camera.target=origin;
	camera.offset= origin;
	camera.zoom = 1.0;

	//float angle = 0.0f;
	
	line L1 = {{0.5f, -0.5f}, {-1.5f, -4.5f}};
	line L2 = {{-1.5f, 3.0f}, {-4.0f,1.0f}};

	Vector2 x = pointOfIntersection(L1, L2);
	Vector2 pos = plot(x);
	float zoom_factor = 3;
	int font_size = 20;
	while (!WindowShouldClose())
	{
		if (camera.zoom > 3.0f) 
			camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) 
			camera.zoom = 0.1f;
		
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }
		//zoom in
		if(IsKeyPressed(KEY_EQUAL)|| IsKeyPressedRepeat(KEY_EQUAL)){
			camera.zoom+=0.05f;
		}
		// zoom out
		if(IsKeyPressed(KEY_MINUS) || IsKeyPressedRepeat(KEY_MINUS)){
			camera.zoom-=0.05;
			zoom_factor +=1.5;
		};

		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(camera);
		
		drawPlane(zoom_factor);
		// draw stuff on the plane
		drawParabola(-2,3,-2);
		drawLine(L1, 25);
		drawLine(L2, 25);
		DrawCircleV(pos, 6.0f, GREEN);
		//drawCircle((Vector2){3.0f,2.0f},3.0f);
		DrawText(TextFormat("(%.2f,%.2f)",x.x,x.y),pos.x+SCALE/2,pos.y-SCALE/2,font_size,BLACK);	
		EndMode2D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
// draws a circel, not filled. 
void drawCircle(Vector2 centre, float r)
{	
	float angle = PI/24.0f;
	Vector2 prev={
		centre.x + r*cosf(0.0f),
		centre.y + r*sinf(0.0f)
	};
	
	for(float theta = angle; theta <= 2*PI+angle; theta+= angle)
	{
		Vector2 p={
			centre.x + r*cosf(theta),
			centre.y+r*sinf(theta)
		};
		DrawLineEx(plot(prev),plot(p),4,PURPLE);
		prev = p;
	}
}

void drawPlane(float zoom_factor)
{
	int stepH = origin.x;
	int stepV = origin.y;
	int winwidth = (width / 2) / SCALE;
	int winheight = (height / 2) / SCALE;
	//Font my_font = LoadFont("NewCMMath-Regular.otf");
	// draw the cartesian plane
	for (int x = -zoom_factor*width; x <= zoom_factor*width; x += SCALE)
	{
		DrawLine(x, 0, x, zoom_factor*height, BLACK);
		DrawLine(x, 0, x, -zoom_factor*height, BLACK);
	}
	for (int y= -zoom_factor*height; y <= zoom_factor*height; y += SCALE)
	{
		DrawLine(0, y, zoom_factor*width, y, BLACK);
		DrawLine(0, y, -zoom_factor*width, y, BLACK);
	}

	// number the horizontal axis.
	for (int i = 1; i <= winwidth*zoom_factor; i++)
	{
		DrawText(TextFormat("%d", i), stepH + i *SCALE- 5, stepV +SCALE/ 2 - 15, 20, BLACK);
		DrawText(TextFormat("%d", -i), stepH - i *SCALE- 5, stepV +SCALE/ 2 - 15, 20, BLACK);
		
	}
	// number the vertical axis
	for (int i = 1; i <= winheight*zoom_factor; i++)
	{
		DrawText(TextFormat("%d", i), stepH -SCALE/ 2 - 10, stepV - i *SCALE- 12, 20, BLACK);
		DrawText(TextFormat("%d", -i), stepH -SCALE/ 2 - 20, stepV + i *SCALE- 12, 20, BLACK);
	}
	// draw the x and y axis
	DrawRectangle(-height*zoom_factor,height/2-2,width*zoom_factor*SCALE,4,BLACK);
	DrawRectangle(width/2-2,-height,4,zoom_factor*width,BLACK);
	
}