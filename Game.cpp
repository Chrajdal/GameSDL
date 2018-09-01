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
	Trect<double> m_bounding_box;
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

v2d screen_to_world(double x, double y, const Camera & cam)
{
	v2d res;
	res.x = (x + cam.m_x) / tile_size;
	res.y = (y + cam.m_y) / tile_size;
	return res;
}

v2d world_to_screen(double x, double y, const Camera & cam)
{
	v2d res;
	res.x = (x + cam.m_x) * tile_size;
	res.y = (y + cam.m_y) * tile_size;
	return res;
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
	if (tile_sheet_dirt.GetData() == NULL || tile_sheet_stone.GetData() == NULL)
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

	load_big_file_handle.get();
}

Game::~Game(void)
{
	terrain.SaveToFile(".\\Save\\save_file.txt");
}

void Game::Go()
{
	gfx.BeginFrame();

	if(m_IsRunning) HandleInput();
	if(m_IsRunning) UpdateModel();
	if(m_IsRunning) ComposeFrame();

	gfx.EndFrame();
}

bool mouseArray[3] = { 0,0,0 };

void Game::HandleInput()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	mouse.SetMouseState(e);

	if (kbd.KeyIsPressed(SDL_SCANCODE_ESCAPE))
	{
		m_IsRunning = false;
		return;
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_UP) || kbd.KeyIsPressed(SDL_SCANCODE_W))
	{
		cam.m_y -= 1e1;
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_DOWN) || kbd.KeyIsPressed(SDL_SCANCODE_S))
	{
		cam.m_y += 1e1;
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_LEFT) || kbd.KeyIsPressed(SDL_SCANCODE_A))
	{
		cam.m_x -= 1e1;
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_RIGHT) || kbd.KeyIsPressed(SDL_SCANCODE_D))
	{
		cam.m_x += 1e1;
	}

	if (kbd.KeyIsPressed(SDL_SCANCODE_1))
	{
		selected_tile = tile_type::dirt;
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_2))
	{
		selected_tile = tile_type::stone;
	}

	// mouse code
	{
		int x = mouse.GetX();
		int y = mouse.GetY();

		v2d world_pos = screen_to_world(x, y, cam);
		Node * tmp = terrain.access((int)std::floor(world_pos.x), (int)std::floor(world_pos.y)); // instead of floor it is possible to use overtyping to int
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
	int index = 0;
	const Node * up = terrain.at(x, y - 1);
	const Node * dw = terrain.at(x, y + 1);
	const Node * lf = terrain.at(x - 1, y);
	const Node * rt = terrain.at(x + 1, y);
	
	if (up != NULL && dw != NULL && lf != NULL && rt != NULL)
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
		{ 0 + cam.m_x - tile_size,0 + cam.m_y - tile_size },
		{ gfx.width() + cam.m_x + tile_size, gfx.height() + cam.m_y + tile_size });

	// scale view to pick up only tile_size'th od the pixels
	view.m_upleft.m_x /= tile_size;
	view.m_upleft.m_y /= tile_size;
	view.m_downright.m_x /= tile_size;
	view.m_downright.m_y /= tile_size;

	std::vector<const Node *> vec = terrain.range(view);

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
				gfx.DrawPartImage(x, y, rect.m_upleft.m_x, rect.m_upleft.m_y, rect.Width(), rect.Height(), tile_sheet_dirt);
			}
			if (i->m_tile == tile_type::stone)
			{
				Trect<int> rect = pick_correct_tile_rect(i->m_x, i->m_y, terrain);
				gfx.DrawPartImage(x, y, rect.m_upleft.m_x, rect.m_upleft.m_y, rect.Width(), rect.Height(), tile_sheet_stone);
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
