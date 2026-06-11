#include <raylib.h>
#include <math.h>
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
const int s = 50; // lenght on 1 unit in pixels

Vector2 plot(Vector2 p) // plots cartesain co-ordiantes to screen
{						// O+P = P
	return (Vector2){
		origin.x + (s * p.x),
		origin.y - (s * p.y)};
}

Vector2 rotatePoint(Vector2 p, float angle)
{
	return (Vector2){
		p.x * cosf(angle) - p.y * sinf(angle),
		p.x * sinf(angle) + p.y * cosf(angle)};
}

void drawPlane(float);

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
		p.y + t * dy};
	// from p to negative direction
	Vector2 nline = {
		p.x - t * dx,
		p.y - t * dy};
	DrawLineEx(plot(nline), plot(pline), 5.0f, RED);
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
	camera.zoom=1.0;

	// float angle = 0.0f;
	line L1 = {{1.0f, -1.0f}, {-2.0f, 0.0f}};
	line L2 = {{3.0f, 1.0f}, {-2.0f, 4.0f}};
	Vector2 x = pointOfIntersection(L1, L2);
	float zoom_factor = 3;
	while (!WindowShouldClose())
	{
		if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

		
		if(IsKeyPressed(KEY_B))
			camera.zoom+=0.05;
		
		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(camera);
		
		drawPlane(zoom_factor);
		// draw stuff on the plane
		if(IsKeyPressed(KEY_A)){
			camera.zoom-=0.05;
			zoom_factor +=0.5;
		};
		drawLine(L1, 10);
		drawLine(L2, 10);
		DrawCircleV(plot(x), 6.0f, BLUE);
		//DrawLine(-4*width/2,0,2*width,height/2,RED);
		EndMode2D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

void drawPlane(float zoom_factor)
{
	int stepH = origin.x;
	int stepV = origin.y;
	int winwidth = (width / 2) / s;
	int winheight = (height / 2) / s;
	//Font my_font = LoadFont("NewCMMath-Regular.otf");
	// draw the cartesian plane
	for (int x = -zoom_factor*width; x <= zoom_factor*width; x += s)
	{
		DrawLine(x, 0, x, zoom_factor*height, BLACK);
		DrawLine(x, 0, x, -zoom_factor*height, BLACK);
	}
	for (int y = -zoom_factor*height; y <= zoom_factor*height; y += s)
	{
		DrawLine(0, y, zoom_factor*width, y, BLACK);
		DrawLine(0, y, -zoom_factor*width, y, BLACK);
	}

	// number the horizontal axis.
	for (int i = 1; i <= winwidth*zoom_factor; i++)
	{
		DrawText(TextFormat("%d", i), stepH + i * s - 5, stepV + s / 2 - 15, 20, BLACK);
		DrawText(TextFormat("%d", -i), stepH - i * s - 5, stepV + s / 2 - 15, 20, BLACK);
		
	}
	// number the vertical axis
	for (int i = 1; i <= winheight*zoom_factor; i++)
	{
		DrawText(TextFormat("%d", i), stepH - s / 2 - 10, stepV - i * s - 12, 20, BLACK);
		DrawText(TextFormat("%d", -i), stepH - s / 2 - 20, stepV + i * s - 12, 20, BLACK);
	}
	// draw the x and y axis
	DrawRectangle(-height*zoom_factor,height/2-2,width*zoom_factor*s,4,BLACK);
	DrawRectangle(width/2-2,-height,4,zoom_factor*width,BLACK);
	
}
