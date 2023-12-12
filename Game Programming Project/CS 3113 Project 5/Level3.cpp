//
//  Level3.cpp
//  CS 3113 Project 5
//
//  Created by Jonathan Kim on 2023/12/06.
//

#include "Level3.hpp"
#include "Utility.hpp"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

unsigned int LEVEL3_DATA[] =
{
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0, 103, 103, 103,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    0, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152
};

GLuint level3_text_texture_id;

Level3::~Level3()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void Level3::initialise()
{
    m_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/images/tile_spritesheet.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL3_DATA, map_texture_id, 1.0f, 12, 13);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/player.png");
    m_state.player->stage_clear = false;
    m_state.player->death = false;
    set_dead(false);
    
    // Walking
    m_state.player->set_height(1.0f);
    m_state.player->set_width(1.0f);
    
    // Jumping
    m_state.player->m_jumping_power = 5.0f;
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture("assets/images/enemy.png");
    
    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(GUARD);
    m_state.enemies[0].set_ai_state(IDLE);
    m_state.enemies[0].m_texture_id = enemy_texture_id;
    m_state.enemies[0].set_position(glm::vec3(10.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].set_speed(1.0f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_state.bgm = Mix_LoadMUS("assets/audio/bgm.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(50.0f);
    
    m_state.jump_sfx = Mix_LoadWAV("assets/audio/boing.wav");
    
    level3_text_texture_id = Utility::load_texture("assets/fonts/font1.png");
}

void Level3::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map);
    }
    if (m_state.player->death) {
        set_dead(true);
        m_state.next_scene_id = 1;
    }
    if (m_state.player->get_position().y < -10.0f) {
        m_state.game_over = true;
//        m_state.player->set_movement(glm::vec3(0.0f, 0.0f, 0.0f));
//        m_state.player->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    }
}


void Level3::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
    
    if (m_state.game_over == true && m_state.mission == false) {
        Utility::draw_text(program, level3_text_texture_id, "YOU LOSE!", 0.5f, 0.0f, glm::vec3(m_state.player->get_position().x - 1.5f, m_state.player->get_position().y - 3.0f, 0.0f));
    }
    else if (m_state.game_over == true && m_state.mission == true) {
        Utility::draw_text(program, level3_text_texture_id, "YOU WIN!", 0.5f, 0.0f, glm::vec3(m_state.player->get_position().x - 1.5f, m_state.player->get_position().y + 7.0f, 0.0f));
    }
}
