#include <app/DKEViewerAppController.h>
//-------------------------------------------------------------------------------------------------
#include <GL/glew.h>
#include <graphics/RenderManager.h>
#include <graphics/Camera.h>
#include <stdio.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------

DKEViewerAppController::DKEViewerAppController(graphics::RenderManager* renderManager)
	: m_renderManager(renderManager)
	, m_camera(new graphics::Camera)
{
}

//-------------------------------------------------------------------------------------------------

DKEViewerAppController::~DKEViewerAppController()
{
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::initialize()
{
	m_renderManager->initialize();
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::finalize()
{
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::update()
{
	m_renderManager->updateFrame();
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::render()
{
	m_renderManager->renderFrame(m_camera.get());
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::onMouseLeftDown(const MouseEventData&& eventData)
{
	printf("mouse left down: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::onMouseLeftUp(const MouseEventData&& eventData)
{
	printf("mouse left up: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::onMouseMiddleDown(const MouseEventData&& eventData)
{
	printf("mouse middle down: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::onMouseMiddleUp(const MouseEventData&& eventData)
{
	printf("mouse middle up: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::onMouseRightDown(const MouseEventData&& eventData)
{
	printf("mouse right down: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::onMouseRightUp(const MouseEventData&& eventData)
{
	printf("mouse right up: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::onMouseMove(const MouseEventData&& eventData)
{
}

//-------------------------------------------------------------------------------------------------

void DKEViewerAppController::onMouseWheel(const MouseEventData&& eventData)
{
}

//-------------------------------------------------------------------------------------------------
} // namespace dke
//-------------------------------------------------------------------------------------------------
