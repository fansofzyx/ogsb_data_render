#pragma once
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QThread>
#include <QMutex>
#include <vec4.h>
#include <QMatrix4x4>
#include <QQueue>
#include <unordered_map>
#include <tuple>
#include"Camera.h"
#include"Drawable.h"
class osgFile;
class Drawable;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
class osgb2JsonThread;

using OsgbFilePath = std::pair<osgFile*, QString>;

class osgNode {
private:
	float calculateDistance();
	double calculatePixel();
	void DABdraws();

public:
	 osgNode();
	~osgNode();
	bool draw();
	bool getDABstates();
	QString getNextFileName();
	void osgNode::set_bs(vec4f bs);
	void osgNode::set_dir(QString dir);
	void osgNode::set_nextLsit(QString list1, QString list2);
	void osgNode::set_nextValue(vec4f nextValue);
	void osgNode::add_drawable(Drawable * dab);
	void osgNode::set_nextFile(osgFile* nextFile);
	void osgNode::set_thisFile(osgFile* thisFile);
	static void osgNode::set_camPos(vec3f camPos);
private:
	static vec3f _nowCamPos;
	vec4f _bs;
	QString _dir;
	QString _nextList[2];
	vec4f _nextValue;
	osgFile* _nextFile = nullptr;
	osgFile* _thisFile = nullptr;
	std::vector<Drawable*> _drawables;
	
};

class osgFile
{
public:
	enum FileState
	{
		FILE_NEW,
		FILE_LOADING,
		FILE_LOADED
	};
	osgFile(const QString& filePath, osgb2JsonThread* fileLoadThread);
	~osgFile();

	bool render();
	bool getDABstates();
	void _fakeRender();
	bool getFileReady();
private:
	QString _path;
	std::vector<osgNode*> _nodes;
	FileState _state = FILE_NEW;
	osgb2JsonThread* _fileLoadThread; 
	void onNew();
	void onLoading();
	bool onLoaded();
	void onFakeLoaded();
};

class osgb2JsonThread : public QThread
{
	Q_OBJECT
public:
	osgb2JsonThread();
	~osgb2JsonThread();

	virtual void run();

	bool append(osgFile* fptr, const QString& filepath);
	bool query(osgFile* fptr, std::vector<osgNode*>& nodes);
	void test();
	void setThread(drawDataThread* loaderDAB);
	std::unordered_map<osgFile*, std::vector<osgNode*>> _files;
	QQueue<OsgbFilePath> _paths;
	QMutex _lockerQuery;
	QMutex _locker;
	drawDataThread* _loaderDAB = nullptr;
};


class osgScene {
public:
	osgScene();
	~osgScene();

	void load(const QString& dataDir);
	void osgScene::render();
	

public:
	static int DrawableLoadOnFrame;
private:
	std::vector<osgFile*> _files;
	osgb2JsonThread* _loader = nullptr;
	drawDataThread* _loaderDAB = nullptr;
	
};