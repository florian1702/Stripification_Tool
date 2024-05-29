#pragma once
#include "core/GlutWindow.h"
#include "app/globaldefs.h"

class Mesh3D;

class ViewerWindow :
	public CGlutWindow
{
	

public:
	ViewerWindow(const char*);
	virtual ~ViewerWindow(void);

	//virtual void keyEvent(unsigned char key,int x,int y);
	void renderGeometry();

	void keyEvent(unsigned char key,int x,int y);

protected:
	Mesh3D *mesh;

};
