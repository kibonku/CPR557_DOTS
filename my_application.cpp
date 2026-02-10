#include "my_application.h"
#include <stdexcept>
#include <array>
#include <iostream>

MyApplication::MyApplication()
{
    _createModel();
    _createPipelineLayout();
    _createPipeline();
    _createCommandBuffers();
}

MyApplication::~MyApplication()
{
    vkDestroyPipelineLayout(m_myDevice.device(), m_vkPipelineLayout, nullptr);
}

void MyApplication::run() 
{
    m_myWindow.bindMyApplication(this);

    while (!m_myWindow.shouldClose()) 
    {
        m_myWindow.pollEvents();

        // TODO: Implement _updateModel() first
        _updateModel();
        _drawFrame();
    }

    // GPU will block until all CPU is complete
    vkDeviceWaitIdle(m_myDevice.device());
}

void MyApplication::_createModel()
{
    m_pMyModel = std::make_unique<MyModel>(m_myDevice);

    //* Add: Phase 1: some static dots in NDC coordinates
    m_points = {
        { { -0.6f, -0.2f } },
        { { -0.2f,  0.3f } },
        { {  0.1f, -0.4f } },
        { {  0.4f,  0.2f } },
        { {  0.7f, -0.1f } },
    };
    m_pointsDirty = true;
}

void MyApplication::_createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(m_myDevice.device(), &pipelineLayoutInfo, nullptr, &m_vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline layout!");
    }
}

void MyApplication::_createPipeline()
{
    // Note: the swap chain buffer size may not be the same as the Window size
    // Pipeline config can be considered as a blue print to create graphics pipeline
    PipelineConfigInfo pipelineConfig{};

    MyPipeline::defaultPipelineConfigInfo(pipelineConfig, m_mySwapChain.width(), m_mySwapChain.height());

    // For now render pass contains the structure and the format
    // of the frame buffer object and its attachments
    // for example in the defect FBO, attachment 0 is the color buffer
    // and attachment 1 is the depth buffer
    pipelineConfig.renderPass = m_mySwapChain.renderPass();
    pipelineConfig.pipelineLayout = m_vkPipelineLayout;

    m_pMyPipeline = std::make_unique< MyPipeline >(
        m_myDevice,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig
        );
}

void MyApplication::_createCommandBuffers()
{
    // Command buffers contain one or multiple commends that are to be executed on the GPU
    // 
    // 1:1 mapping between the command buffer and the frame buffer for now
    // Could be either 2 or 3 depending on the graphics hardware
    m_vVkCommandBuffers.resize(m_mySwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_myDevice.commandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_vVkCommandBuffers.size());

    if (vkAllocateCommandBuffers(m_myDevice.device(), &allocInfo, m_vVkCommandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void MyApplication::_recordCommandBuffer(int imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_vVkCommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffers!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_mySwapChain.renderPass();
    renderPassInfo.framebuffer = m_mySwapChain.frameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = m_mySwapChain.swapChainExtent();

    // becuse index 0 is color buffer and 1 is depth buffer
    // so we only need to clear the proper index only
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 0.1f };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    // VK_SUBPASS_CONTENTS_INLINE means only use Primary command buffer. No secondary command buffer
    vkCmdBeginRenderPass(m_vVkCommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    m_pMyPipeline->bind(m_vVkCommandBuffers[imageIndex]);

    m_pMyModel->bind(m_vVkCommandBuffers[imageIndex]);
    m_pMyModel->draw(m_vVkCommandBuffers[imageIndex]);

    vkCmdEndRenderPass(m_vVkCommandBuffers[imageIndex]);
    if (vkEndCommandBuffer(m_vVkCommandBuffers[imageIndex]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to recording command buffers!");
    }
}

void MyApplication::_drawFrame()
{
    uint32_t imageIndex = 0;
    auto result = m_mySwapChain.acquireNextImage(&imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    _recordCommandBuffer(imageIndex);

    // Submit the command buffer to the graphics queue
    // the GPU and CPU synchronization will be handled properly
    result = m_mySwapChain.submitCommandBuffers(&m_vVkCommandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit comamnd buffers!");
    }
}

void MyApplication::_updateModel()
{
    // TODO: Update the vertex buffer here
    if (!m_pointsDirty) return;
    m_pMyModel->updateVertices(m_points);
    m_pointsDirty = false;
}

void MyApplication::addPoint(float x, float y)
{
    // TODO: add points to the scene
    auto ext = m_myWindow.extent();
    float w = static_cast<float>(ext.width);
    float h = static_cast<float>(ext.height);

    float x_ndc = 2.0f * (x / w) - 1.0f;
    float y_ndc = 1.0f - 2.0f * (y / h);

    if (m_points.size() < MyModel::MAX_POINTS)
    {
        m_points.push_back({ { x_ndc, y_ndc } });
        m_pointsDirty = true;
    }

    std::cout << "My applicaiton: Left mouse button pressed at x = " << x
              << " y = " << y << "  (NDC: " << x_ndc << ", " << y_ndc << ")" << std::endl;
}

void MyApplication::clearPoints()
{
    //* Add: Phase 3: Reset
    m_points.clear();
    m_pointsDirty = true;
}

void MyApplication::removeLastPoint()
{
    //* Add: Phase 4: Right Click = Remove Last Dot-
    if (!m_points.empty())
    {
        m_points.pop_back();
        m_pointsDirty = true;
    }
}

