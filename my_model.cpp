#include "my_model.h"
#include <cassert>
#include <cstring>


MyModel::MyModel(MyDevice& device) :
	m_myDevice{ device },
	m_iVertexCount{ 0 }
{
	_createVertexBuffer();
}

MyModel::~MyModel()
{
    vkDestroyBuffer(m_myDevice.device(), m_vkVertexBuffer, nullptr);
    vkFreeMemory(m_myDevice.device(), m_vkVertexBufferMemory, nullptr);
}

void MyModel::_createVertexBuffer()
{
    // Allocate a fixed-size HOST_VISIBLE vertex buffer so we can update points from CPU.
    // We'll only draw m_iVertexCount vertices each frame.
    VkDeviceSize bufferSize = sizeof(Vertex) * MAX_POINTS;

    m_myDevice.createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        m_vkVertexBuffer,
        m_vkVertexBufferMemory);

    m_iVertexCount = 0;
}

void MyModel::_writeToBuffer(const Vertex* data, VkDeviceSize bytes)
{
    //* Add for phase 2 & 3
    if (bytes == 0) return;

    void* mapped = nullptr;
    vkMapMemory(m_myDevice.device(), m_vkVertexBufferMemory, 0, bytes, 0, &mapped);
    std::memcpy(mapped, data, static_cast<size_t>(bytes));
    vkUnmapMemory(m_myDevice.device(), m_vkVertexBufferMemory);
}

void MyModel::updateVertices(const std::vector<Vertex>& vertices)
{
    //* Add for phase 2 & 3
    assert(vertices.size() <= MAX_POINTS && "Too many points for MAX_POINTS");

    m_iVertexCount = static_cast<uint32_t>(vertices.size());
    VkDeviceSize bytes = sizeof(Vertex) * m_iVertexCount;
    _writeToBuffer(vertices.data(), bytes);
}



void MyModel::bind(VkCommandBuffer commandBuffer)
{
    VkBuffer buffers[] = { m_vkVertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void MyModel::draw(VkCommandBuffer commandBuffer)
{
	vkCmdDraw(commandBuffer, m_iVertexCount, 1, 0, 0);
}

std::vector<VkVertexInputBindingDescription> MyModel::getBindingDescriptions()
{
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> MyModel::getAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;    // vec2
	attributeDescriptions[0].offset = 0;
	return attributeDescriptions;
}

