#ifndef _EASYEDITOR_CAMERA_MGR_H_
#define _EASYEDITOR_CAMERA_MGR_H_

namespace ee
{

class Camera;

class CameraMgr
{
public:
	enum Type
	{
		ORTHO = 0,
		PSEUDO3D,
	};

public:
	static CameraMgr* Instance();

	void SetCamera(Type t);
	Camera* GetCamera() const;

	bool IsType(Type t) const;

private:
	CameraMgr();
	~CameraMgr();

private:
	static CameraMgr* m_instance;

private:
	Type m_curr_type;
	Camera* m_curr;

}; // CameraMgr

}

#endif // _EASYEDITOR_CAMERA_MGR_H_