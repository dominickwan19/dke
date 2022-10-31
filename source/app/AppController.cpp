#include <app/AppController.h>
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <GL/glew.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------

void AppController::initialize()
{
}

//-------------------------------------------------------------------------------------------------

void AppController::finalize()
{
}

//-------------------------------------------------------------------------------------------------

void AppController::update()
{
}

//-------------------------------------------------------------------------------------------------

void AppController::render()
{

}

//-------------------------------------------------------------------------------------------------

void AppController::onMouseLeftDown(const MouseEventData&& eventData)
{
	printf("mouse left down: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void AppController::onMouseLeftUp(const MouseEventData&& eventData)
{
	printf("mouse left up: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void AppController::onMouseMiddleDown(const MouseEventData&& eventData)
{
	printf("mouse middle down: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void AppController::onMouseMiddleUp(const MouseEventData&& eventData)
{
	printf("mouse middle up: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void AppController::onMouseRightDown(const MouseEventData&& eventData)
{
	printf("mouse right down: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void AppController::onMouseRightUp(const MouseEventData&& eventData)
{
	printf("mouse right up: %d %d %d\n", eventData.x, eventData.y, eventData.mods);
}

//-------------------------------------------------------------------------------------------------

void AppController::onMouseMove(const MouseEventData&& eventData)
{
}

//-------------------------------------------------------------------------------------------------

void AppController::onMouseWheel(const MouseEventData&& eventData)
{
}

//-------------------------------------------------------------------------------------------------
} // namespace dke
//-------------------------------------------------------------------------------------------------
