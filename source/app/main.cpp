#include <app/DKEViewerApp.h>

//double Halton_Seq(int index, int base) {
//	double f = 1, r = 0;
//	while (index > 0) {
//		f = f / base;
//		r = r + f * (index % base);
//		index = index / base;
//	}
//	return r;
//}

int main()
{
	//auto val = Halton_Seq(10, 2);

	dke::DKEViewerApp *app = dke::DKEViewerApp::instance();
	app->initialize();
	app->run();
	app->terminate();

	return 0;
}