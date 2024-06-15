#pragma once
#include "../object/point.h"
#include "../object/object.h"
#include "../renderer.h"
#include <glm/glm.hpp>

class Camera : public Component {
public:
    static Camera* getMainCamera();
    static void setMainCamera(Camera* camera);  // Static method to set the main camera

    Camera(Object* parent, glm::vec3 startFront, glm::vec3 startUp, float fov, float aspectRatio, float nearPlane, float farPlane);
    Camera(Object* parent);
    void updateAspectRatio(float aspectRatio);

    void loadFromJson(const nlohmann::json& json) override;
    void update() override;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    float aspectRatio;
    float fov;
    float nearPlane;
    float farPlane;

private:
    glm::vec3 front;
    glm::vec3 up;
    static Camera* mainCamera;  // Static pointer to the main camera
};
