#ifndef SURFACE_H
#define SURFACE_H

#include "../object/object.h"
#include <memory>  // Required for std::enable_shared_from_this

class Surface : public Component, public std::enable_shared_from_this<Surface> {
public:
    Surface(Object* parent);

    void loadFromJson(const nlohmann::json& json) override;
    void update() override;

private:
    int type;
};

#endif // SURFACE_H
