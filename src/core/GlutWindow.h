#pragma once

#include "mathutil\CVector.h"
#include "mathutil\CPosition.h"
#include <windows.h>

class CArcBall;

class CGlutWindow
{
public:
	CGlutWindow(void);
	~CGlutWindow(void);

	enum {
		MOVE_CAMERA,
		MOVE_LIGHT,
	};

	enum {
		MBUTTON_LEFT = 0x01,
		MBUTTON_MIDDLE = 0x02,
		MBUTTON_RIGHT = 0x04,
		KBUTTON_CTRL = 0x08,
		KBUTTON_SHIFT = 0x10
	};

	virtual void renderFrame();
	virtual void idle();
	virtual void resize(int width, int height);
	virtual void keyEvent(unsigned char key, int x, int y);
	virtual void mouseButtonEvent(int button, int state, int x, int y);
	virtual void mouseMoveEvent(int x, int y);
	virtual void renderGeometry();
	virtual bool handleButtonEvent(int button, int state, int x, int y);
	virtual bool handleMoveEvent(int x, int y);

protected:
	float m_nWidth, m_nHeight;
	double m_dFieldOfView;
	double m_dCenter, m_dSceneRadius, m_dZoom;

	CArcBall* m_pCameraArcball;
	CArcBall* m_pLightArcball;

	CPosition m_vecMouseDown;
	bool m_bDragging;
	bool m_bZooming;
	bool m_bRotating;
	bool m_bExternalMouseHandling;

	CVector m_vecCameraPosition;
	double m_dTranslateX, m_dTranslateY;

	int m_nMouseState;
	int m_nInteractionMode;

	void initializeGL();

};