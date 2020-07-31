#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QTime>
#include <QVector>
#include <Camera.h>
#include <QOpenGLFramebufferObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include<vector>
#include"osgNode.h"
#include <QElapsedTimer>
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core
{
    Q_OBJECT
public:
    GLWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~GLWidget();
	vec3f getCameraPos();

protected:
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void initializeGL() override;
	void shaderCode();
	void loadShader(const char * shaderSource, unsigned int & shader, unsigned int type);

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	QPointF _mouseOld;
	bool _mouse_press = false;

private:
	xCamera _cam;
	mat4f w2s;
	QMatrix4x4 _w2c;
	QMatrix4x4 _c2s;
	QMatrix4x4 _mvp;
	int  _mvpHandler;
	osgScene scene;
	unsigned int _shaderProgram;
	int frameCount = 0;
	QElapsedTimer _fpsTimer;
	std::vector<QString> files = { "C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+000_+000/Tile_+000_+000.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+000_+001/Tile_+000_+001.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+000_+002/Tile_+000_+002.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+000_+003/Tile_+000_+003.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+001_+000/Tile_+001_+000.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+001_+001/Tile_+001_+001.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+001_+002/Tile_+001_+002.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+001_+003/Tile_+001_+003.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+002_+000/Tile_+002_+000.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+002_+001/Tile_+002_+001.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+002_+002/Tile_+002_+002.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+002_+003/Tile_+002_+003.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+003_+000/Tile_+003_+000.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+003_+001/Tile_+003_+001.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+003_+002/Tile_+003_+002.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+003_+003/Tile_+003_+003.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+004_+000/Tile_+004_+000.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+004_+001/Tile_+004_+001.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+004_+002/Tile_+004_+002.json",
		"C:/Users/zheng_group/Desktop/Production_OSBG/Data/Tile_+004_+003/Tile_+004_+003.json"

	};
public:
	static int screenHeight;
	static float fov;
};

#endif
