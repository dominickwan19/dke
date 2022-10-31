#include <app/DKEViewerApp.h>


int main()
{
	dke::DKEViewerApp *app = dke::DKEViewerApp::instance();
	app->initialize();
	app->run();
	app->terminate();

	return 0;
}