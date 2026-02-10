#ifndef __MY_MODEL_H__
#define __MY_MODEL_H__

#include "my_device.h"

// use radian rather degree for angle
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// Std
#include <vector>

// Read vertex data from CPU and copy the data into GPU
class MyModel
{
public:

	struct Vertex
	{
		glm::vec2 position;
	};

	static std::vector<VkVertexInputBindingDescription>   getBindingDescriptions();
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

	MyModel(MyDevice &device);
	~MyModel();

	MyModel(const MyModel&) = delete;
	MyModel& operator=(const MyModel&) = delete;
	MyModel(MyModel&&) = delete;
	MyModel& operator=(const MyModel&&) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

	// TODO: add a function to update vertices through mouse click
	// Fixed capacity for interactive point drawing
	static constexpr uint32_t MAX_POINTS = 10000;

	// Update vertices through mouse clicks / reset (CPU -> GPU)
	void updateVertices(const std::vector<Vertex>& vertices);

	private:
	void _writeToBuffer(const Vertex* data, VkDeviceSize bytes);


private:

	void _createVertexBuffer();

	MyDevice&      m_myDevice;
	VkBuffer       m_vkVertexBuffer;       // handle of the buffer on GPU side
	VkDeviceMemory m_vkVertexBufferMemory; // memory on GPU side of the buffer
	uint32_t       m_iVertexCount;
};

#endif

