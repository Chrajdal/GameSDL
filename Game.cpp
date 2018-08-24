#include "Game.h"

class Player
{
public:
	Player(const Image & bmp)
		:
		//player_sprite(&bmp, 0, 0, bmp.width, bmp.height),
		m_curr_state(PlayerState::stand)
	{
		pos.x = 0.0;
		pos.y = 0.0;

		vel.x = 0.0;
		vel.y = 0.0;
	}
	void Update()
	{
		switch (m_curr_state)
		{
		case PlayerState::stand:
			break;
		case PlayerState::walk:
			break;
		case PlayerState::jump:
			break;
		case PlayerState::attack:
			break;
		}
	}

	enum PlayerState
	{
		stand,
		walk,
		jump,
		attack
	};

	v2d pos;
	v2d vel;
	Image player_sprite;
	PlayerState m_curr_state;
};


QuadTree terrain;
Image tile_sheet_dirt;
Image tile_sheet_stone;
std::vector<Trect<int>> tile_map_rects;
Image player_img;
Random rnd;
double dx = 1e-1;
double dy = 1e-1;
int framecount = 0;
double time_concatenated = 0.0;
constexpr double dmax = std::numeric_limits<double>::max();
constexpr double dmin = std::numeric_limits<double>::lowest();
constexpr int tile_size = 16;
tile_type selected_tile = tile_type::dirt;
Player player(player_img);

Game::Game()
{
	m_IsRunning = true;
	gfx.BeginFrame();
	gfx.EndFrame();
	
	tile_sheet_dirt.LoadData(gfx, "./Assets/grass tile sheet.png");
	tile_sheet_stone.LoadData(gfx, "./Assets/stone tile sheet.png");
	if (tile_sheet_dirt.GetData() == NULL || tile_sheet_stone.GetData() == NULL)
	{
		m_IsRunning = false;
		std::cout << "asdfasdfadsfasdf" << std::endl;
		return;
	}
	
	for (int i = 0; i < tile_sheet_dirt.m_width / tile_size; ++i)
	{
		for (int j = 0; j < tile_sheet_dirt.m_height / tile_size; ++j)
		{
			Trect<int> res{ {i * tile_size, j * tile_size}, { i * tile_size + tile_size,j * tile_size + tile_size } };
			//res.x = i * tile_size;
			//res.y = j * tile_size;
			//res.w = tile_size;
			//res.h = tile_size;
			tile_map_rects.push_back(res);
		}
	}


	int tmp = -200;
	for (int i = (int)((tmp + 0 + cam.m_x) / tile_size); i < (int)((1024 + cam.m_x - tmp) / tile_size); ++i)
	{
		for (int j = (int)((tmp + 0 + cam.m_y) / tile_size); j < (int)((1024 + cam.m_y - tmp) / tile_size); ++j)
		{
			double height_treshold = perlin::noise(/*i * dx * 0.1, */ perlin::noise(i * dx * 0.05)) * 100;
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




}

Game::~Game(void)
{
	
}

void Game::Go()
{
	gfx.BeginFrame();

	HandleInput();
	UpdateModel();
	ComposeFrame();

	gfx.EndFrame();
}

void Game::HandleInput()
{
	SDL_Event e;
	if (SDL_PollEvent(&e))
	{
	}
	
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	//continuous-response keys
	if (keystate != NULL)
	{
		if (keystate[SDL_SCANCODE_ESCAPE])
		{
			m_IsRunning = false;
			return;
		}
		if (keystate[SDL_SCANCODE_LEFT])
		{
			cam.m_x -= 1e1;
		}
		if (keystate[SDL_SCANCODE_RIGHT])
		{
			cam.m_x += 1e1;
		}
		if (keystate[SDL_SCANCODE_UP])
		{
			cam.m_y -= 1e1;
		}
		if (keystate[SDL_SCANCODE_DOWN])
		{
			cam.m_y += 1e1;
		}
	}
	

	//SDL_Event e;
	//if (SDL_PollEvent(&e))
	//{
	//	
	//	switch(e.type)
	//	{
	//	case SDL_KEYDOWN:
	//		switch (e.key.keysym.sym)
	//		{
	//		case SDLK_ESCAPE: m_IsRunning = false; break;
	//
	//		//case SDLK_UP:    cam.m_y -= 1e1; break;
	//		//case SDLK_DOWN:  cam.m_y += 1e1; break;
	//		//case SDLK_LEFT:  cam.m_x -= 1e1; break;
	//		//case SDLK_RIGHT: cam.m_x += 1e1; break;
	//		}
	//	break;
	//	}
	//}
}

void Game::UpdateModel()
{
	int tmp = -200;
	for (int i = (int)((tmp + 0 + cam.m_x) / tile_size); i < (int)((gfx.width() + cam.m_x - tmp) / tile_size); ++i)
	{
		for (int j = (int)((tmp + 0 + cam.m_y) / tile_size); j < (int)((gfx.height() + cam.m_y - tmp) / tile_size); ++j)
		{
			double height_treshold = perlin::noise(/*i * dx * 0.1, */ perlin::noise(i * dx * 0.05)) * 100;
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
}

Trect<int> const & pick_correct_tile_rect(int x, int y, const QuadTree & terrain)
{
	std::vector<const Node *> up = terrain.range(Trect<double>(Tpoint<double>(x - 0.5, y - 1.0 - 0.5), Tpoint<double>(x + 0.5, y - 1.0 + 0.5)));
	std::vector<const Node *> dw = terrain.range(Trect<double>(Tpoint<double>(x - 0.5, y + 1.0 - 0.5), Tpoint<double>(x + 0.5, y + 1.0 + 0.5)));
	std::vector<const Node *> lf = terrain.range(Trect<double>(Tpoint<double>(x - 1.0 - 0.5, y - 0.5), Tpoint<double>(x - 1.0 + 0.5, y + 0.5)));
	std::vector<const Node *> rt = terrain.range(Trect<double>(Tpoint<double>(x + 1.0 - 0.5, y - 0.5), Tpoint<double>(x + 1.0 + 0.5, y + 0.5)));
	if (!up.empty() && !dw.empty() && !lf.empty() && !rt.empty())
	{
		std::bitset<4> draw_flag;
		draw_flag[0] = bool(up[0]->m_tile != tile_type::air);
		draw_flag[1] = bool(dw[0]->m_tile != tile_type::air);
		draw_flag[2] = bool(lf[0]->m_tile != tile_type::air);
		draw_flag[3] = bool(rt[0]->m_tile != tile_type::air);
		uint8_t flag = (uint8_t)draw_flag.to_ulong();

		switch (flag)
		{
		case 0:  return tile_map_rects[15];
		case 1:  return tile_map_rects[12];
		case 2:  return tile_map_rects[14];
		case 3:  return tile_map_rects[13];
		case 4:  return tile_map_rects[3];
		case 5:  return tile_map_rects[0];
		case 6:  return tile_map_rects[2];
		case 7:  return tile_map_rects[1];
		case 8:  return tile_map_rects[11];
		case 9:  return tile_map_rects[8];
		case 10: return tile_map_rects[10];
		case 11: return tile_map_rects[9];
		case 12: return tile_map_rects[7];
		case 13: return tile_map_rects[4];
		case 14: return tile_map_rects[6];
		case 15: return tile_map_rects[5];
		}
	}
	return tile_map_rects[0];
}

void Game::ComposeFrame()
{
	// offset screen by cam
	Trect<double> screen(
		{ 0 + cam.m_x - tile_size,0 + cam.m_y - tile_size },
		{ gfx.width() + cam.m_x + tile_size, gfx.height() + cam.m_y + tile_size });

	// scale screen to pick up only tile_size'th od the pixels
	screen.m_upleft.m_x /= tile_size;
	screen.m_upleft.m_y /= tile_size;
	screen.m_downright.m_x /= tile_size;
	screen.m_downright.m_y /= tile_size;

	std::vector<const Node *> vec = terrain.range(screen);

	for (const auto & i : vec)
	{
		int x = (int)(i->m_x * tile_size - cam.m_x);
		int y = (int)(i->m_y * tile_size - cam.m_y);
	
		if (x >= 0 - tile_size && x + tile_size < (int)gfx.width()  + tile_size &&
			y >= 0 - tile_size && y + tile_size < (int)gfx.height() + tile_size)
		{
			if (i->m_tile == tile_type::dirt)
			{
				Trect<int> rect = pick_correct_tile_rect(i->m_x, i->m_y, terrain);
				gfx.DrawPartImage(x, y, rect.m_upleft.m_x, rect.m_upleft.m_x, rect.Width(), rect.Height(), tile_sheet_dirt);
			}
			if (i->m_tile == tile_type::stone)
			{
				Trect<int> rect = pick_correct_tile_rect(i->m_x, i->m_y, terrain);
				gfx.DrawPartImage(x, y, rect.m_upleft.m_x, rect.m_upleft.m_x, rect.Width(), rect.Height(), tile_sheet_stone);
			}
		}
	}
	
	//v2d tmp;
	//tmp.x = player.m_x;
	//tmp.y = player.m_y;
	//v2d player_pos = world_to_screen(tmp.x, tmp.y, cam);
	//gfx.draw_surface_alpha((int)player_pos.x, (int)player_pos.y, player.player_sprite, Colors::White);

	// easy way out:
	// todo gfx.draw_part_of_surface
	//gfx.draw_rect(0, 0, (int)tile_size, Graphics::ScreenHeight, Colors::White);
	//gfx.draw_rect((int)Graphics::ScreenWidth - tile_size, 0, (int)tile_size, Graphics::ScreenHeight, Colors::White);
	//gfx.draw_rect(0, 0, Graphics::ScreenWidth, (int)tile_size, Colors::White);
	//gfx.draw_rect(0, (int)Graphics::ScreenHeight - tile_size, Graphics::ScreenWidth, (int)tile_size, Colors::White);
	
}
