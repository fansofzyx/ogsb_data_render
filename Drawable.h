#pragma once
#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLFunctions>
#include <QThread>
#include <QMutex>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QByteArray>
#include <QImage>
#include <QQueue>
#include <unordered_map>
#include<list>
struct DrawData
{
	std::vector<float> _vertex;
	std::vector<int> _indice;
	std::vector<float> _textureCord;
	QImage _tex;
};

class Drawable;
class drawDataThread;

using DrawFilePath = std::pair<Drawable*, std::pair<QString, QString>>;

class Drawable : public QOpenGLFunctions_3_2_Core
{
public:
	enum DrawableState {
		DAB_NEW,
		DAB_LOADING,
		DAB_LOADED
	};
	Drawable(QString _geoPath, QString _texPath, drawDataThread* loader);
	~Drawable();

	void draw();
	void releaseGpu();
	void notifyAbort();
	bool isReady();
	QString getFilePath();

private:
	void onNew();
	void onLoading();
	void onLoaded();

private:
	QString _geoPath;
	QString _texPath;
	GLuint _vbo = 0;
	GLuint _vboT = 0;
	GLuint _ebo = 0;
	QOpenGLTexture* _tex = nullptr;
	DrawableState _state;
	drawDataThread* _loader = nullptr;
	int _drawCount = 10000;
};

class drawDataThread : public QThread
{
	Q_OBJECT
public:
	drawDataThread();
	~drawDataThread();

	virtual void run();
	bool append(const DrawFilePath& path);
	bool query(Drawable* drawable, DrawData& dt);
	void update(Drawable * drawable);
	void test();

private:
	std::map<Drawable*, std::list<Drawable*>::iterator> _dabMatch;
	std::list<Drawable*> _dabList;
	QMutex _pathLocker;
	QQueue<DrawFilePath> _paths;
	QMutex _dataLocker;
	std::unordered_map<Drawable*, DrawData> _datas;
	int _maxDataSize = 100;
	QMutex _locker;
	QMutex _lockerMap;
};