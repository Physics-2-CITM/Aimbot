#include "IntegratorFunctions.h"
#include"ModuleVerlet.h"

VerletIntegrator::VerletIntegrator()
{}

VerletIntegrator::VerletIntegrator(Application* app)
{
	App = app;
}

//Point definitin
void VerletIntegrator::InitPoint(Point* p, vector2 pos)
{
	//Position
	p->x = pos.x;
	p->y = pos.y;
	p->old_x = pos.x;
	p->old_y = pos.y;

	//Velocity
	p->vx =	0.f;
	p->vy = 0.f;

	//Hardcode or calculate?
	p->dt = (1.f/ 60.f);
	p->acc_x = 0.f;
	p->acc_y = -gravity;

	//Color
	p->color.r = 255;
	p->color.a = 80;
	p->canCollide = true;
	p->isSimulated = true;

	//Rect to allow particle selection
	p->selector_rect = { (int)pos.x, (int)pos.y, p->radius * 2, p->radius * 2};
}

//Adds a force to a selected particle via a vector
void VerletIntegrator::AddForce(Point* p, vector2 force) 
{
	p->vx += force.x;
	p->vy += force.y;
}

void VerletIntegrator::updateSinglePoint(Point* p)
{

	p->old_x = p->x;
	p->old_y = p->y;
	if (p->isSimulated) 
	{
		//Update particle position with 
		p->x = p->x + (p->vx * p->dt) + (0.5f * 0.f * (p->dt * p->dt));
		p->y = p->y + (p->vy * p->dt) + (0.5f * gravity * (p->dt * p->dt));

		//BOTTOM LIMIT
		if (p->y > (float)floor_Limit_Y)
		{
			p->y = (float)floor_Limit_Y;
			p->vy *= -1.f * bounce;
		}
		else if (p->y <= (float)p->radius)
		{
			p->y = (float)p->radius;
			p->vy *= -1 * bounce;
		}

		if (p->x >= (float)floor_Limit_X)
		{
			p->x = (float)floor_Limit_X;
			p->vx *= -1 * bounce;
		}
		else if (p->x <= (float)p->radius)
		{
			p->x = (float)p->radius;
			p->vx *= -1 * bounce;
		}

		//Collision detection between particles (not lines)
		for (unsigned int j = 0; j < world_points.count(); j++)
		{
			Point* check_Point = world_points[j];
			if (check_Point != p && check_Point->canCollide && p->canCollide)
			{
				if (CheckCollision(check_Point, p))
				{
					OnCollision(p, check_Point);
				}
			}
		}

		//Velocity update for the next frame (and next position calculation)
		if (p->vx > 0)
		{
			p->vx = p->vx + (-friction * p->dt);
		}
		else if (p->vx < 0)
		{
			p->vx = p->vx + (friction * p->dt);
		}
		if (p->vx == 0) {
			p->vx = p->vx + (0.f * p->dt);
		}
		p->vy = p->vy + (gravity * p->dt);
		
	}

}

//Check collision
bool VerletIntegrator::CheckCollision(Point* a, Point* b) 
{
	if (pow((a->x - b->x), 2) + pow((b->y - a->y), 2) <= pow((b->radius + a->radius), 2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Updates all particles on the world
void VerletIntegrator::updatePoints()
{

	p2List_item<Point*>* temp_list_item = world_points.start;

	//Itinerates all particles
	while (temp_list_item)
	{
		Point* p = temp_list_item->data;

		updateSinglePoint(p);

		App->renderer->DrawLine((int)p->old_x, (int)p->old_y, (int)p->x, (int)p->y, 0, 0, 255, 255);

		temp_list_item = temp_list_item->next;

	}
}

Point* VerletIntegrator::AddPoint(int x, int y) 
{
	
	Point* list = world_points.add(new Point())->data;

	InitPoint(list, {(float)x, (float)y});

	App->debug->debugPointNumber++;


	return list;
}

void VerletIntegrator::OnCollision(Point* p, Point* check_Point)
{
	p->vx *= -1 * bounce;
	p->vy *= -1 * bounce;

	//Ball collision correction
	float fDistance = sqrtf((check_Point->x - p->x)*(check_Point->x - p->x) + (check_Point->y - p->y)*(check_Point->y - p->y));
	float fOverlap = 0.5f * (fDistance - check_Point->radius - p->radius) - 1;

	//Move objects outside collision + dt to fix overlap?
	if (check_Point->isSimulated) 
	{
		check_Point->x -= fOverlap * (check_Point->x - p->x) / fDistance;
		check_Point->y -= fOverlap * (check_Point->y - p->y) / fDistance;
	}
	p->x += fOverlap * (check_Point->x - p->x) / fDistance;
	p->y += fOverlap * (check_Point->y - p->y) / fDistance;

	//Invert velocity to simulate elastic collision
	if (check_Point->isSimulated)
	{
		check_Point->vx = p->vx;
		check_Point->vx *= -1 * bounce;
		check_Point->vy = p->vy;
		check_Point->vy *= -1 * bounce;
	}
}

