#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
ModuleUI::~ModuleUI()
{
}

// Called before render is available
bool ModuleUI::Init()
{
	bool ret = true;
	
	sprtsheet = App->textures->Load("Sprites/Menu_Spritesheet_2.png");

	SetButton(&menu_button, { SCREEN_WIDTH - 40, 20, 20, 20 }, true, ShapeType::NO_SHAPE, {0, 0, 0, 0}, {255, 255, 255, 255});
	SetButton(&selection_screen, { SCREEN_WIDTH - 200, 0, 200, SCREEN_HEIGHT }, false, ShapeType::NO_SHAPE, {0, 0, 200, 540}, { 0, 255, 255, 100 });
	SetButton(&selectors[0], { SCREEN_WIDTH - 167, 20, 140, 140 }, false, ShapeType::CIRCLE, {200, 0, 140, 140}, {0, 0, 255, 100});
	SetButton(&selectors[1], { SCREEN_WIDTH - 167, 175, 140, 140 }, false, ShapeType::PLANET, {200, 140, 140, 140}, { 0, 0, 255, 100 });
	SetButton(&selectors[2], { SCREEN_WIDTH - 167, SCREEN_HEIGHT - 60, 140, 49 }, false, ShapeType::NO_SHAPE, { 200, 280, 140, 49 }, { 0, 0, 255, 100 });

	return ret;
}

update_status ModuleUI::Update()
{

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) 
	{
		for (int i = 0; i < 4; i++)
		{
			if (selectors[i].enabled)
			{
				if (CanBeSelected({ App->input->GetMouseX(), App->input->GetMouseY(), 0, 0 }, selectors[i].rect))
				{
					if (i == 0)
					{
						//Calculte monte carlo
						App->aimbot->StartMonteCarlo();
					}
					else if (i == 1)
					{
						//Ejectue montecarlo
						App->aimbot->ExecuteTrajectory();
					}
					else if (i == 2)
					{
						//Clear all world
						ShapeType selType = App->verlet->creation_type;
						
						App->aimbot->CleanPaths();

						App->verlet->ClearWorld();

						App->aimbot->CreateTargetAndOrigin();
						App->verlet->creation_type = selType;
					}
				}
			}
		}


		if (CanBeSelected({ App->input->GetMouseX(), App->input->GetMouseY(), 0, 0 }, menu_button.rect)
			|| (menu_button.enabled == false && !CanBeSelected({ App->input->GetMouseX(), App->input->GetMouseY(), 0, 0 }, selection_screen.rect)))
		{
			for (unsigned int i = 0; i < ui_elements.count(); i++)
			{
				ui_elements[i]->enabled = !ui_elements[i]->enabled;
			}
		}
	}

	for (unsigned int i = 0; i < ui_elements.count(); i++)
	{
		if (ui_elements[i]->enabled) 
		{
			if (ui_elements[i]->sptr_rect.w != 0) 
			{
				App->renderer->Blit(sprtsheet, ui_elements[i]->rect.x, ui_elements[i]->rect.y, &ui_elements[i]->sptr_rect);
			}
			else
			{
				App->renderer->DrawQuad(ui_elements[i]->rect, ui_elements[i]->color.r, ui_elements[i]->color.g, ui_elements[i]->color.b, ui_elements[i]->color.a);
			}
		}
			/*App->renderer->DrawQuad(ui_elements[i]->rect, ui_elements[i]->color.r, ui_elements[i]->color.g, ui_elements[i]->color.b, ui_elements[i]->color.a);*/
	}
	for (int i = 0; i < 4; i++)
	{
		if (selectors[i].enabled && App->verlet->creation_type == selectors[i].type)
		{
			App->renderer->DrawQuad(selectors[i].rect, 0, 0, 0, 80);
		}
	}

	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleUI::CleanUp()
{
	ui_elements.clear();
	App->textures->Unload(sprtsheet);
	return true;
}


bool ModuleUI::CanBeSelected(const SDL_Rect& rect, const SDL_Rect& r)
{
	bool detectedX = true;
	bool detectedY = true;

	if ((rect.x + rect.w) < r.x || (r.x + r.w) < rect.x)
	{
		detectedX = false;
	}

	if (rect.y < r.y || (rect.y + rect.h) > r.y + r.h)
	{
		detectedY = false;
	}

	return detectedX && detectedY;
}

void ModuleUI::SetButton(UI_Button* button, SDL_Rect pos_size, bool enabled, ShapeType creation_type, SDL_Rect sprt_rect, SDL_Color color)
{

	button->rect = pos_size;
	button->enabled = enabled;
	button->type = creation_type;
	button->color = color;
	button->sptr_rect = sprt_rect;

	ui_elements.add(button);

}