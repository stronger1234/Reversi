#pragma once

class IDrawable {
public:
	virtual void draw() {}
};

class GLBase {
	static HDC hdc;
	static HGLRC hrc;
	static GLuint frameCount, t1, t2;
	static GLdouble fps, fpsMax;
	static GLdouble viewEye[3], viewCenter[3], viewUp[3];
	static GLdouble radio, theta, phi, dTheta, dPhi, d2Theta, d2Phi;
	static GLvoid updateView();
public:
	static std::mutex drawLock;
	static GLuint tex[20];
	static bool initialize(HWND hWnd);
	static void fpsRestrict();
	static GLvoid drawGLScene();
	static GLvoid initGLScene();
	static GLvoid resizeGLScene(GLsizei width, GLsizei height);
	static void drawCNString(const char *str, int size, int charset, const char *face, GLdouble posX, GLdouble posY);
	static GLvoid LoadTextures(GLsizei n, const int rcid[], GLenum fmt[]);
	static GLvoid keyEvent(bool keyDown, WPARAM keyId);
	static GLvoid mouseEvent(int button, int posX, int posY);
	GLBase();
	~GLBase();
};
