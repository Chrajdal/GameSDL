#include "Game.h"

QuadTree terrain;
Image tile_sheet_dirt;
Image tile_sheet_stone;
std::vector<Trect<int>> tile_map_rects;
Image m_player_img;

constexpr int tile_size = 16;
tile_type selected_tile = tile_type::dirt;
Player player(&m_player_img);
bool player_has_been_placed = false;

v2d screen_to_world(double x, double y, const Camera & cam)
{
	v2d res;
	res.x = (x + cam.m_pos.x) / tile_size;
	res.y = (y + cam.m_pos.y) / tile_size;
	return res;
}

v2d world_to_screen(double x, double y, const Camera & cam)
{
	return v2d(x * tile_size - cam.m_pos.x, y * tile_size - cam.m_pos.y);
}

void load_from_file(void)
{
	terrain.LoadFromFile(".\\Save\\save_file.txt");
}

Game::Game()
{
	auto load_big_file_handle = std::async(std::launch::async, load_from_file);

	m_IsRunning = true;
	gfx.BeginFrame();
	gfx.EndFrame();
	
	tile_sheet_dirt.LoadData(gfx, "./Assets/grass tile sheet.png");
	tile_sheet_stone.LoadData(gfx, "./Assets/stone tile sheet.png");
	m_player_img.LoadData(gfx, "./Assets/Main Character.png");
	player.m_bbox = { { 0.0,0.0 }, {(double)m_player_img.m_width / tile_size,(double)m_player_img.m_height / tile_size} };

	if (tile_sheet_dirt.GetData() == nullptr || tile_sheet_stone.GetData() == nullptr)
	{
		m_IsRunning = false;
		return;
	}
	
	for (int j = 0; j < (tile_sheet_dirt.m_height / tile_size); ++j)
	{
		for (int i = 0; i < (tile_sheet_dirt.m_width / tile_size); ++i)
		{
			Trect<int> res{ {i * tile_size, j * tile_size}, { (i + 1) * tile_size, (j + 1) * tile_size} };
			tile_map_rects.push_back(res);
		}
	}

	cam.m_pos.x = player.m_pos.x - gfx.ScreenWidth / 2;
	cam.m_pos.y = player.m_pos.y - gfx.ScreenHeight / 2;

	load_big_file_handle.get();
}

Game::~Game(void)
{
	terrain.SaveToFile(".\\Save\\save_file.txt");
}

void Game::Go()
{
	gfx.BeginFrame();

	if (m_IsRunning)
	{
		HandleInput();
	}
	if (m_IsRunning)
	{
		UpdateModel();
	}
	if (m_IsRunning)
	{
		ComposeFrame();
	}

	gfx.EndFrame();
}

void Game::HandleInput()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	mouse.SetMouseState(e);
	
	// keyboard code
	if (kbd.KeyIsPressed(SDL_SCANCODE_ESCAPE))
	{
		m_IsRunning = false;
		return;
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_DELETE))
	{
		terrain.clear();
	}

	double step = 2e-1; // will be changed to player.m_vel;
	if (kbd.KeyIsPressed(SDL_SCANCODE_SPACE) and player.m_curr_state != Player::PlayerState::jump)
	{
		if(player.stands_on_ground(terrain))
		{
			player.do_jump(terrain);
			player.m_vel = player.m_vel + (10 * step * 0.1);

			// Clamp to maximum velocity
			player.m_vel.x = std::min(10 * step, std::max(player.m_vel.x, -10 * step));
			player.m_vel.y = std::min(10 * step, std::max(player.m_vel.y, -10 * step));

			//player.ApplyForce(v2d(0, -10 *step));
			//player.m_curr_state = Player::PlayerState::jump;
		}
		
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_LEFT) || kbd.KeyIsPressed(SDL_SCANCODE_A))
	{
		player.m_vel.x = -step;
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_RIGHT) || kbd.KeyIsPressed(SDL_SCANCODE_D))
	{
		player.m_vel.x = +step;
	}

	if (kbd.KeyIsPressed(SDL_SCANCODE_1))
	{
		selected_tile = tile_type::dirt;
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_2))
	{
		selected_tile = tile_type::stone;
	}
	
	//mouse.SetMouseState(e);
	// mouse code
	{
		int x = mouse.GetX();
		int y = mouse.GetY();

		v2d world_pos = screen_to_world(x, y, cam);
		Node * tmp = terrain.access((int)std::floor(world_pos.x), (int)std::floor(world_pos.y)); // instead of floor it is possible to use overtyping to int
		if (tmp == nullptr)
		{
			std::cout << "ERROR" << std::endl;
			return;
		}
		if (mouse.LeftIsPressed())
		{
			tmp->m_tile = tile_type::air;
		}
		if (mouse.RightIsPressed())
		{
			if (tmp->m_tile == tile_type::air)
				tmp->m_tile = selected_tile;
		}
	}

	cam.m_pos.x = (player.m_pos.x + player.m_bbox.Width() / 2) * tile_size - gfx.ScreenWidth / 2;
	cam.m_pos.y = (player.m_pos.y + player.m_bbox.Height() / 2)* tile_size - gfx.ScreenHeight / 2;

	SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
	//while (SDL_PollEvent(&e));
}

void Game::UpdateModel()
{
	// terrain generation:
	double dx = 1e-1;
	double dy = 1e-1;
	int tmp = -200;
	for (int i = (int)((tmp + 0 + cam.m_pos.x) / tile_size); i < (int)((gfx.width() + cam.m_pos.x - tmp) / tile_size); ++i)
	{
		for (int j = (int)((tmp + 0 + cam.m_pos.y) / tile_size); j < (int)((gfx.height() + cam.m_pos.y - tmp) / tile_size); ++j)
		{
			double height_treshold = perlin::noise(perlin::noise(i * dx * 0.05)) * 100;
			double stone_threshold = perlin::noise(j * dx * 0.1) * 200;
			int x = i;
			int y = j;
			if (height_treshold > j)
			{
				terrain.insert(Node(x, y, tile_type::air));
			}
			else if (height_treshold <= j && height_treshold > j - stone_threshold)
			{
				terrain.insert(Node(x, y, tile_type::dirt));
			}
			else if (height_treshold <= j - stone_threshold)
			{
				double noise = perlin::noise(i * dx, j * dy);
				if (noise > 0.0)
					terrain.insert(Node(x, y, tile_type::dirt));
				else
					terrain.insert(Node(x, y, tile_type::stone));
			}
		}
	}

	//position the player accordingly:
	if (player_has_been_placed == false)
	{
		//move the player up - so that he stands on the ground
		player.m_pos.x = 0;

		while (player.GetBColl(terrain))
		{
			player.m_pos.y -= 1.0;
		}

		//const Node * standing_on = terrain.at(int(std::round(player.m_pos.x)), int(std::round(player.m_pos.y + player.m_bbox.m_downright.m_y)));
		//while (standing_on != nullptr && standing_on->m_tile != tile_type::air)
		//{
		//	player.m_pos.y -= 0.1;
		//	//std::cout << "Moving player up" << std::endl;
		//	standing_on = terrain.at(int(std::round(player.m_pos.x)), int(std::round(player.m_pos.y + player.m_bbox.m_downright.m_y)));
		//}
		player_has_been_placed = true;
	}

	player.Update(terrain);
}

Trect<int> const & pick_correct_tile_rect(int x, int y, const QuadTree & terrain)
{
	int index = 0;
	const Node * up = terrain.at(x, y - 1);
	const Node * dw = terrain.at(x, y + 1);
	const Node * lf = terrain.at(x - 1, y);
	const Node * rt = terrain.at(x + 1, y);
	
	if (up != nullptr && dw != nullptr && lf != nullptr && rt != nullptr)
	{
		std::bitset<4> relative_position;
		relative_position[0]= up->m_tile != tile_type::air;
		relative_position[1]= dw->m_tile != tile_type::air;
		relative_position[2]= lf->m_tile != tile_type::air;
		relative_position[3]= rt->m_tile != tile_type::air;

		switch (relative_position.to_ulong())
		{
		case 0:  index =15; break;
		case 1:  index =11; break;
		case 2:  index = 3; break;
		case 3:  index = 7; break;
		case 4:  index =14; break;
		case 5:  index =10; break;
		case 6:  index = 2; break;
		case 7:  index = 6; break;
		case 8:  index =12; break;
		case 9:  index = 8; break;
		case 10: index = 0; break;
		case 11: index = 4; break;
		case 12: index =13; break;
		case 13: index = 9; break;
		case 14: index = 1; break;
		case 15: index = 5; break;
		}
	}
	return tile_map_rects[index];
}

void Game::ComposeFrame()
{
	// offset view by cam
	Trect<double> view(
		{ 0 + cam.m_pos.x - tile_size,0 + cam.m_pos.y - tile_size },
		{ gfx.width() + cam.m_pos.x + tile_size, gfx.height() + cam.m_pos.y + tile_size });

	// scale view to pick up only tile_size'th of the pixels
	view.m_upleft.m_x /= tile_size;
	view.m_upleft.m_y /= tile_size;
	view.m_downright.m_x /= tile_size;
	view.m_downright.m_y /= tile_size;

	std::vector<const Node *> vec = terrain.range(view);

	for (const auto & i : vec)
	{
		int x = (int)(i->m_x * tile_size - std::round(cam.m_pos.x));
		int y = (int)(i->m_y * tile_size - std::round(cam.m_pos.y));
	
		if (x >= 0 - tile_size && x + tile_size < (int)gfx.width()  + tile_size &&
			y >= 0 - tile_size && y + tile_size < (int)gfx.height() + tile_size)
		{
			if (i->m_tile == tile_type::dirt)
			{
				Trect<int> rect = pick_correct_tile_rect(i->m_x, i->m_y, terrain);
				gfx.DrawPartImage(x, y, rect.m_upleft.m_x, rect.m_upleft.m_y, rect.Width(), rect.Height(), tile_sheet_dirt);
			}
			if (i->m_tile == tile_type::stone)
			{
				Trect<int> rect = pick_correct_tile_rect(i->m_x, i->m_y, terrain);
				gfx.DrawPartImage(x, y, rect.m_upleft.m_x, rect.m_upleft.m_y, rect.Width(), rect.Height(), tile_sheet_stone);
			}
		}
	}

	v2d player_pos = world_to_screen(player.m_pos.x, player.m_pos.y, cam);
	
	gfx.DrawImage((int)player_pos.x, (int)player_pos.y, *player.m_player_img);

	// bounding box
	gfx.DrawRect(
		(int)player_pos.x + player.m_bbox.m_upleft.m_x*tile_size,
		(int)player_pos.y + player.m_bbox.m_upleft.m_y*tile_size,
		player.m_bbox.Width() *tile_size,
		player.m_bbox.Height()*tile_size, Colors::Blue);

	//player.CheckCeilingCollision(terrain, gfx, cam);
	//player.CheckFloorCollision(terrain, gfx, cam);
	//player.CheckLeftWallCollision(terrain, gfx, cam);
	//player.CheckRightWallCollision(terrain, gfx, cam);

	Trect<double> rect = player.GetTRect();
	rect.m_upleft.m_x = rect.m_upleft.m_x * tile_size - std::round(cam.m_pos.x);
	rect.m_upleft.m_y = rect.m_upleft.m_y * tile_size - std::round(cam.m_pos.y);
	rect.m_downright.m_x = rect.m_downright.m_x * tile_size - std::round(cam.m_pos.x);
	rect.m_downright.m_y = rect.m_downright.m_y * tile_size - std::round(cam.m_pos.y);
	gfx.DrawRect(rect.m_upleft.m_x, rect.m_upleft.m_y, rect.Width(), rect.Height(), Colors::Blue);

	rect = player.GetBRect();
	rect.m_upleft.m_x = rect.m_upleft.m_x * tile_size - std::round(cam.m_pos.x);
	rect.m_upleft.m_y = rect.m_upleft.m_y * tile_size - std::round(cam.m_pos.y);
	rect.m_downright.m_x = rect.m_downright.m_x * tile_size - std::round(cam.m_pos.x);
	rect.m_downright.m_y = rect.m_downright.m_y * tile_size - std::round(cam.m_pos.y);
	gfx.DrawRect(rect.m_upleft.m_x, rect.m_upleft.m_y, rect.Width(), rect.Height(), Colors::Green);

	rect = player.GetLRect();
	rect.m_upleft.m_x = rect.m_upleft.m_x * tile_size - std::round(cam.m_pos.x);
	rect.m_upleft.m_y = rect.m_upleft.m_y * tile_size - std::round(cam.m_pos.y);
	rect.m_downright.m_x = rect.m_downright.m_x * tile_size - std::round(cam.m_pos.x);
	rect.m_downright.m_y = rect.m_downright.m_y * tile_size - std::round(cam.m_pos.y);
	gfx.DrawRect(rect.m_upleft.m_x, rect.m_upleft.m_y, rect.Width(), rect.Height(), Colors::Red);

	rect = player.GetRRect();
	rect.m_upleft.m_x = rect.m_upleft.m_x * tile_size - std::round(cam.m_pos.x);
	rect.m_upleft.m_y = rect.m_upleft.m_y * tile_size - std::round(cam.m_pos.y);
	rect.m_downright.m_x = rect.m_downright.m_x * tile_size - std::round(cam.m_pos.x);
	rect.m_downright.m_y = rect.m_downright.m_y * tile_size - std::round(cam.m_pos.y);
	gfx.DrawRect((int)rect.m_upleft.m_x, (int)rect.m_upleft.m_y, rect.Width(), rect.Height() , Colors::Yellow);

	/**/
	//gfx.DrawLine(0, 0, gfx.ScreenWidth - 1, gfx.ScreenHeight - 1, Colors::Red);
	//gfx.DrawLine(gfx.ScreenWidth - 1, 0, 0, gfx.ScreenHeight - 1, Colors::Red);
	/**/
}
