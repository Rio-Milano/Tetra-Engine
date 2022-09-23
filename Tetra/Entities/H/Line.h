#ifndef LINE_H
#define LINE_H

#include"Entity.h"
#include"Model.h"

class Line : public Entity, public Model
{
public:
	Line(const std::string& lineName);
	~Line() = default;

	void AddVertex(const glm::vec3& position, const glm::vec3& color = glm::vec3(1.0f));

	void Init() override final;

	void Update(const float& dt) override final {};

	void Render(Renderer& renderer)override final;


};

#endif