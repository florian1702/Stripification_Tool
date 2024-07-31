#pragma once
#include "core/GlutWindow.h"
#include "app/globaldefs.h"


class ViewerWindow :
	public CGlutWindow
{

public:
	ViewerWindow();
	virtual ~ViewerWindow(void);

	//virtual void keyEvent(unsigned char key,int x,int y);
	void renderGeometry();

	void keyEvent(unsigned char key,int x,int y);

	// <Florian>
	// Methode zum Rendern der Benutzeroberfläche.
	void renderGui();
	// </Florian>
};
