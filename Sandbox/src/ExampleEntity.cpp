#include "Lake/Entity.h"
#include "Lake/Log.h"

class ExampleComponent final : public lake::EntityComponent {
public:
    ExampleComponent()
        : EntityComponent()
    {
        lake::info("ExampleComponent::ExampleComponent");
    }

    ~ExampleComponent() override {
        lake::info("ExampleComponent::~ExampleComponent");
    }

    void method() {
        lake::info("ExampleComponent::method");
    }
};

class TestEntity final : public lake::Entity {
public:
    TestEntity(const lake::Entity::Properties& properties)
        : Entity(properties)
    {
        this->addComponent<ExampleComponent>();
    }

    ~TestEntity() override {
        lake::info("TestEntity::~TestEntity");
    }

    void onUpdate(const f32 ts) override {
        lake::info("TestEntity::onUpdate ts: ", ts, ", position: x.", mPosition.x, " y.", mPosition.y, " z.", mPosition.z);

        auto exampleComponents = this->getComponents<ExampleComponent>();

        for (auto& component : exampleComponents) {
            component->method();
        }
    }
};

lake::Entity::RegisterEntity<TestEntity> testEntity("TestEntity");
