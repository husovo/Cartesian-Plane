#include <raylib.h>
#include <math.h>
// -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
// type: "make graph" to compile
#define width 1800
#define height 1400

typedef struct line{
	Vector2 P;
	Vector2 Q;
}line;

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

Vector2 rotatePoint(Vector2 p,float angle)
{
	return (Vector2)
	{
		p.x*cosf(angle) -p.y*sinf(angle),
		p.x*sinf(angle) +p.y*cosf(angle)
	};
}

void drawPlane(void);
Vector2 lineDirection(line l1){
	
	Vector2 p={l1.P.x,l1.P.y};
	Vector2 q={l1.Q.x,l1.Q.y};
    // direction of the line
	float dx = (q.x - p.x);
	float dy = (q.y - p.y);
	return (Vector2){dx,dy};	
}
void drawLine(line l1,float t)
{
	Vector2 p={l1.P.x,l1.P.y};
	Vector2 q={l1.Q.x,l1.Q.y};
    // direction of the line
	float dx = (q.x - p.x);
	float dy = (q.y - p.y);
	
	// parametric representation P +  t(Q -  P).
	Vector2 pline = {
		p.x + t*dx,
		p.y + t*dy
	};
	//from p to negative direction
	Vector2 nline = {
		p.x - t*dx,
		p.y - t*dy
	};
	DrawLineEx(plot(nline),plot(pline),5.0f,RED);
	// find the x intercept using basic algebra	
	if(dy != 0)
	{ // y(t) = p.y + t(dx,dy)
		Vector2 x_intercept ={ p.x + (-p.y*dx)/dy,0.0f};
		DrawCircleV(plot(x_intercept),6.0f,BLUE); 
	}
	
	// find the y intercept using basic algebra	
	if(dx != 0)
	{ // x(t) = p.x + t(dx,dy)
		Vector2 y_intercept = {0.0f,p.y +(-p.x*dy)/dx };
		DrawCircleV(plot(y_intercept),6.0f,BLUE);
	}
	
}

Vector2 pointOfIntersection(line l1, line l2){
	Vector2 A = lineDirection(l1);
	Vector2 Ap= lineDirection(l2);
	float s,t, d = (A.y*Ap.x - A.x*Ap.y);
	
	if(d != 0){
		s =(A.x * (l2.P.y-l1.P.y) - A.y * (l2.P.x-l1.P.x))/d;
		t = (Ap.x * (l2.P.y-l1.P.y) - Ap.y * (l2.P.x-l1.P.x))/d;
	}
	// the point of intersection is the same for both lines.
	// return the x from l1 and y from l2.
	return (Vector2){l1.P.x + t*A.x,l2.P.y+s*Ap.y};
}
int main(void)
{
	InitWindow(width,height,"plane");
	SetTargetFPS(60);
	float angle = 0.0f;
	Vector2 p = {1.0f,2.0f};
	Vector2 q= {-3.0f,4.0f};
	Vector2 m = {3.0f,-2.0f};
	Vector2 n= {-4.0f,-1.0f};
	line L1 = {p,q};
	line L2 = {m,n};
	while(!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		drawPlane();
		// draw stuff on the plane
		drawLine(L1,10);	
		drawLine(L2,10);
		DrawCircleV(plot(pointOfIntersection(L1,L2)),5,BLUE);
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
		// number the horizontal axis.
		for(int i = 1; i <= winwidth; i++)
		{
			DrawText(TextFormat("%d",i), stepH + i*s-5, stepV+s/2-15, 20, BLACK);	
			DrawText(TextFormat("%d",-i), stepH - i*s-5, stepV+s/2-15, 20, BLACK);
		}
		// number the vertical axis
		for(int i = 1; i <= winheight; i++)
		{
			DrawText(TextFormat("%d",i), stepH-s/2-10, stepV - i*s-12, 20, BLACK);	
			DrawText(TextFormat("%d",-i), stepH-s/2-20, stepV + i*s-12, 20, BLACK);	
		}
		// draw the x and y axis
		DrawRectangle(0, height/2-2,width, 4, BLACK);
		DrawRectangle(width/2-2,0,4,height,BLACK);	
}
