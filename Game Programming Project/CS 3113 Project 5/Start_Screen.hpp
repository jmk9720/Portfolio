#include "Scene.hpp"

class Start_Screen : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 0;
    // ————— CONSTRUCTOR ————— //
    ~Start_Screen();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
