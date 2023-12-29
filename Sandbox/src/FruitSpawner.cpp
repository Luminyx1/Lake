#include "Lake/Entity.h"

#include "Lake/Scene.h"

class FruitSpawner final : public lake::Entity {
    LK_REGISTER_ENTITY(FruitSpawner);

public:
    FruitSpawner(lake::Entity::Properties properties)
        : Entity()
        , mTimeSinceLastSpawn(0.0f)
    { }

    ~FruitSpawner() override = default;

    void onUpdate(f32 timeStep) override {
        mTimeSinceLastSpawn += timeStep;

        constexpr f32 cTimeBetweenSpawns = 0.5f;
        if (mTimeSinceLastSpawn > cTimeBetweenSpawns) {
            mTimeSinceLastSpawn = 0.0f;

            glm::vec3 position = glm::vec3(
                ((std::rand() % 100) / 100.0f * 2.0f - 1.0f) * 0.9f,
                1.0f,
                0.0f
            );

            const std::string positionString = std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z);

            const std::string propertiesJson = "{\"position\": [" + positionString + "]}";
            mScene->spawnEntity("FruitEntity", propertiesJson);
        }
    }

    f32 mTimeSinceLastSpawn;
};
