#include <QApplication>
#include <QSurfaceFormat>
#include <QStackedLayout>
#include <QPushButton>
#include <QTimer>
#include "glwidget.h"
/*
	创建OpenGL的窗口
*/
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	//format.setSamples(4);
	QSurfaceFormat::setDefaultFormat(format);

	GLWidget w;
	w.show();

    return app.exec();
}
