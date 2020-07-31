#include "Drawable.h"
#include "osgNode.h"
#include <QFile>
/*
	Drawable 是对一组（顶点+纹理）的绘制
*/
Drawable::Drawable(QString geoPath,QString texPath,drawDataThread* loader)
{
	
	_geoPath = geoPath;
	_texPath = texPath;
	_state = DAB_NEW;
	_loader = loader;
}

Drawable::~Drawable()
{

}

void Drawable::draw()
{
	initializeOpenGLFunctions();
	switch (_state)
	{
	case Drawable::DAB_NEW:
		onNew();
		break;
	case Drawable::DAB_LOADING:
		onLoading();
		break;
    case Drawable::DAB_LOADED:
		onLoaded();
		break;
	default:
		qWarning() << "error drawable state " << _state;
		break;
	}
	
}

void Drawable::releaseGpu()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
	glDeleteBuffers(1, &_vboT);
	delete _tex;
	_tex = nullptr;
}
void Drawable::notifyAbort()
{
	switch (_state)
	{
		case Drawable::DAB_LOADED:
			releaseGpu();
			break;
		default:
			qWarning() << "error drawable state " << _state;
			break;
	}
	_state = DAB_NEW;
}
void Drawable::onNew()
{
	if (_loader->append(DrawFilePath(this, std::pair<QString, QString>(_geoPath, _texPath))))
	{
		_state = DAB_LOADING;
	}
}

void Drawable::onLoading()
{
	if (osgScene::DrawableLoadOnFrame > 0)
	{
		DrawData tempdata;
		if (_loader->query(this, tempdata))
		{
			if (tempdata._vertex.size() > 0 && tempdata._indice.size() > 0 &&
				!tempdata._tex.isNull())
			{
				
				glGenBuffers(1, &_vbo);
				glBindBuffer(GL_ARRAY_BUFFER, _vbo);
				glBufferData(GL_ARRAY_BUFFER, tempdata._vertex.size() * sizeof(float), tempdata._vertex.data(), GL_STATIC_DRAW);

				glGenBuffers(1, &_vboT);
				glBindBuffer(GL_ARRAY_BUFFER, _vboT);
				glBufferData(GL_ARRAY_BUFFER, tempdata._textureCord.size() * sizeof(float), tempdata._textureCord.data(), GL_STATIC_DRAW);

				glGenBuffers(1, &_ebo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempdata._indice.size() * sizeof(int), tempdata._indice.data(), GL_STATIC_DRAW);

				_tex = new QOpenGLTexture(tempdata._tex);
				_tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
				_tex->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
				_tex->setWrapMode(QOpenGLTexture::Repeat);

				_drawCount = tempdata._indice.size();
				_state = DAB_LOADED;
				osgScene::DrawableLoadOnFrame = osgScene::DrawableLoadOnFrame - 1;
			}
			
		}
	}
}

void Drawable::onLoaded()
{
	if (_drawCount > 0)
	{
		QString temp = _texPath;
		int t = temp.lastIndexOf("/");
		QString tempSubStr = temp.right(temp.length() - t - 1);
		//qWarning() << tempSubStr;

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, _vboT);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 2 * sizeof(float), NULL);
		glEnableVertexAttribArray(1);

		_tex->bind(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glDrawElements(GL_TRIANGLES, _drawCount, GL_UNSIGNED_INT, 0);

		_tex->release(0);
		_loader->update(this);
	}
}

bool Drawable::isReady()
{
	if (_state == DAB_LOADED)
		return true;
	return false;
}
QString Drawable::getFilePath()
{
	return _geoPath;
}
drawDataThread::drawDataThread()
{

}

drawDataThread::~drawDataThread()
{

}

void drawDataThread::run()
{
	while (!isInterruptionRequested()) {
		DrawFilePath curPath(nullptr, std::pair<QString, QString>("", ""));
		{
			QMutexLocker locker(&_pathLocker);
			if (_paths.size() > 0)
			{
				curPath = _paths.front();
				_paths.pop_front();
			}
		}
		
		if (curPath.first != nullptr)
		{
			DrawData tempdata;
			QFile f(curPath.second.first);
			if (f.open(QIODevice::ReadOnly))
			{

				QByteArray data = f.readAll();
				int * data_ptr = (int*)data.data();
				int vertexCount = data_ptr[0];
				tempdata._vertex.resize(vertexCount);
				memcpy(tempdata._vertex.data(), &data_ptr[1], tempdata._vertex.size() * sizeof(float));

				int indexCount = data_ptr[tempdata._vertex.size() + 1];
				tempdata._indice.resize(indexCount);
				memcpy(tempdata._indice.data(), &data_ptr[tempdata._vertex.size() + 2], tempdata._indice.size() * sizeof(int));

				int textCordCount = data_ptr[tempdata._vertex.size() + tempdata._indice.size() + 2];
				tempdata._textureCord.resize(textCordCount);
				memcpy(tempdata._textureCord.data(), &data_ptr[tempdata._vertex.size() + tempdata._indice.size() + 3], textCordCount*sizeof(float));

				tempdata._tex.load(curPath.second.second);

				{
					QMutexLocker locker1(&_dataLocker);
					QMutexLocker locker2(&_lockerMap);
					if (_datas.size() > _maxDataSize)
					{
						Drawable * d = *_dabList.rend();
						_dabList.pop_back();
						_dabMatch.erase(d);
						_datas.erase(d);
						//delete random_it;
					}
					_datas[curPath.first] = tempdata;

				}
			}
		}
		else
		{
			msleep(10);
		}
	}
}
void drawDataThread::update(Drawable * drawable)
{
	QMutexLocker locker(&_lockerMap);
	if (_dabMatch.find(drawable) != _dabMatch.end())
	{
		_dabList.erase(_dabMatch[drawable]);
		_dabList.insert(_dabList.begin(),drawable);
		_dabMatch[drawable] = _dabList.begin();
	}
	else
	{
		_dabList.insert(_dabList.begin(),drawable);
		_dabMatch[drawable] = _dabList.begin();
	}
}
bool drawDataThread::append(const DrawFilePath& path)
{
	if (_locker.try_lock())
	{
		_paths.push_back(path);
		_locker.unlock();
		test();
		return true;
	}
	return false;
}


bool drawDataThread::query(Drawable* drawable, DrawData& dt)
{
	bool ret = false;
	if (_locker.try_lock())
	{
		auto it = _datas.find(drawable);
		if (it != _datas.end())
		{
			dt = it->second;
			_datas.erase(it);
			ret = true;
		}
		_locker.unlock();
	}
	return ret;
}
void drawDataThread::test()
{
	
}
