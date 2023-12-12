//
//  Start_Screen.cpp
//  CS 3113 Project 5
//
//  Created by Jonathan Kim on 2023/12/06.
//

#include "Start_Screen.hpp"
#include "Utility.hpp"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

unsigned int START_SCREEN_DATA[] =
{
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0, 103, 103, 103, 103, 103, 103, 103, 103, 103,   0,   0,   0,   0,
    0, 103,   0,   0,   0,   0,   0,   0,   0, 103,   0,   0,   0,   0,
    0, 103,   0,   0,   0,   0,   0,   0,   0, 103,   0,   0,   0,   0,
    0, 103,   0,   0,   0,   0,   0,   0,   0, 103,   0,   0,   0,   0,
    0, 103,   0,   0,   0,   0,   0,   0,   0, 103,   0,   0,   0,   0,
    0, 103, 103, 103, 103, 103, 103, 103, 103, 103,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};
GLuint text_texture_id;

Start_Screen::~Start_Screen()
{
    delete    m_state.map;
    Mix_FreeMusic(m_state.bgm);
}

void Start_Screen::initialise()
{
    m_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/images/tile_spritesheet.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, START_SCREEN_DATA, map_texture_id, 1.0f, 12, 13);
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_state.bgm = Mix_LoadMUS("assets/audio/bgm.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(50.0f);
    
    text_texture_id = Utility::load_texture("assets/fonts/font1.png");
}

void Start_Screen::update(float delta_time)
{
    
}


void Start_Screen::render(ShaderProgram *program)
{
    m_state.map->render(program);
    Utility::draw_text(program, text_texture_id, "Press ENTER to Play", 0.5f, 0.0f, glm::vec3(0.5f, -3.0f, 0.0f));
}
