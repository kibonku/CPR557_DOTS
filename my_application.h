#ifndef __MY_APPLICATION_H__
#define __MY_APPLICATION_H__

#include "my_window.h"
#include "my_pipeline.h"
#include "my_device.h"
#include "my_swap_chain.h"
#include "my_model.h"

#include <string>
#include <memory>
#include <vector>

class MyApplication 
{
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	MyApplication();
	~MyApplication();

	// C++ rule of five
	MyApplication(const MyApplication&) = delete;
	MyApplication& operator=(const MyApplication&) = delete;
	MyApplication(MyApplication &&) = delete;
	MyApplication& operator=(const MyApplication&&) = delete;

	void addPoint(float x, float y);

	void run();
    
	//*Add *//
	void clearPoints();  // To reset  // void MyApplication::clearPoints()
    void removeLastPoint();  // Phase 4: Right Click = Remove Last Dot



private:
    void _createModel();
	void _updateModel();
	void _createPipelineLayout();
    void _createPipeline();
	void _createCommandBuffers();
	void _drawFrame();
    void _recordCommandBuffer(int imageIndex);

	MyWindow                     m_myWindow{ WIDTH, HEIGHT, "Assignment 1" };
	MyDevice                     m_myDevice{ m_myWindow };
	MySwapChain                  m_mySwapChain{ m_myDevice, m_myWindow.extent() };

	std::unique_ptr<MyPipeline>  m_pMyPipeline;

	VkPipelineLayout             m_vkPipelineLayout;
	std::vector<VkCommandBuffer> m_vVkCommandBuffers;
	std::unique_ptr<MyModel>     m_pMyModel;
    
	//* Add *//
	std::vector<MyModel::Vertex> m_points;
	bool                         m_pointsDirty{ true };

};

#endif

