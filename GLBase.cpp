#include "stdafx.h"
#include "GLBase.h"
#include "Reversi.h"

HDC			GLBase::hdc;
HGLRC		GLBase::hrc;
std::mutex	GLBase::drawLock;
GLuint		GLBase::tex[20];

GLuint		GLBase::frameCount;
GLuint		GLBase::t1;
GLuint		GLBase::t2;
GLdouble	GLBase::fps;
GLdouble	GLBase::fpsMax = 60.0;
GLdouble	GLBase::viewEye[3];
GLdouble	GLBase::viewCenter[3];
GLdouble	GLBase::viewUp[3];
GLdouble	GLBase::radio;
GLdouble	GLBase::theta;
GLdouble	GLBase::phi;
GLdouble	GLBase::dTheta;
GLdouble	GLBase::dPhi;
GLdouble	GLBase::d2Theta;
GLdouble	GLBase::d2Phi;

// ACC_V != DEC_V
#define ACC_V		(PI / 4000.0)
#define DEC_V		(PI / 3000.0)
#define MAX_V		(PI / 64.0)
#define MAX_THETA	(PI / 3.0)

void drawAxes() {
	static GLdouble v[] = {
		-50.0, +00.0, +00.0, +50.0, +00.0, +00.0,
		+47.0, +00.0, +02.0, +50.0, +00.0, +00.0,
		+47.0, +00.0, -02.0, +50.0, +00.0, +00.0,
		+47.0, +02.0, +00.0, +50.0, +00.0, +00.0,
		+47.0, -02.0, +00.0, +50.0, +00.0, +00.0,
		-45.0, -40.0, +00.0, +45.0, -40.0, +00.0,
		-45.0, -30.0, +00.0, +45.0, -30.0, +00.0,
		-45.0, -20.0, +00.0, +45.0, -20.0, +00.0,
		-45.0, -10.0, +00.0, +45.0, -10.0, +00.0,
		-45.0, +10.0, +00.0, +45.0, +10.0, +00.0,
		-45.0, +20.0, +00.0, +45.0, +20.0, +00.0,
		-45.0, +30.0, +00.0, +45.0, +30.0, +00.0,
		-45.0, +40.0, +00.0, +45.0, +40.0, +00.0,
		+00.0, -50.0, +00.0, +00.0, +50.0, +00.0,
		+00.0, +47.0, +02.0, +00.0, +50.0, +00.0,
		+00.0, +47.0, -02.0, +00.0, +50.0, +00.0,
		+02.0, +47.0, +00.0, +00.0, +50.0, +00.0,
		-02.0, +47.0, +00.0, +00.0, +50.0, +00.0,
		-40.0, -45.0, +00.0, -40.0, +45.0, +00.0,
		-30.0, -45.0, +00.0, -30.0, +45.0, +00.0,
		-20.0, -45.0, +00.0, -20.0, +45.0, +00.0,
		-10.0, -45.0, +00.0, -10.0, +45.0, +00.0,
		+10.0, -45.0, +00.0, +10.0, +45.0, +00.0,
		+20.0, -45.0, +00.0, +20.0, +45.0, +00.0,
		+30.0, -45.0, +00.0, +30.0, +45.0, +00.0,
		+40.0, -45.0, +00.0, +40.0, +45.0, +00.0,
		+00.0, +00.0, -50.0, +00.0, +00.0, +50.0,
		+00.0, +02.0, +47.0, +00.0, +00.0, +50.0,
		+00.0, -02.0, +47.0, +00.0, +00.0, +50.0,
		+02.0, +00.0, +47.0, +00.0, +00.0, +50.0,
		-02.0, +00.0, +47.0, +00.0, +00.0, +50.0
	};
	static GLdouble c[] = {
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0
	};
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glLineWidth(1.5f);

	glVertexPointer(3, GL_DOUBLE, 0, v);
	glColorPointer(3, GL_DOUBLE, 0, c);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_LINES, 0, 62);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glColor3d(1.0, 1.0, 1.0);
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	glVertex3d(0.0, 0.0, 0.0);
	glEnd();
}

void GLBase::drawCNString(const char *str, int size, int charset, const char *face, GLdouble posX, GLdouble posY) {
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glTranslated(-1.0, 0.85, 0.0);
	glRasterPos2d(posX, -posY);
	glColor3d(1.0, 1.0, 1.0);

	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
							  charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							  DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);

	int len, i;
	wchar_t *wstring;
	GLuint list = glGenLists(1);

	len = 0;
	for (i = 0; str[i] != '\0'; ++i) {
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	for (i = 0; i<len; ++i) {
		wglUseFontBitmapsW(hdc, wstring[i], 1, list);
		glCallList(list);
	}

	free(wstring);
	glDeleteLists(list, 1);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

GLvoid GLBase::LoadTextures(GLsizei n, const int rcid[], GLenum fmt[]) {
	glGenTextures(n, tex);
	for (GLsizei i = 0; i < n; i++) {
		auto img = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(rcid[i]), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		BITMAP bm;
		GetObject(img, sizeof(bm), &bm);
		glBindTexture(GL_TEXTURE_2D, tex[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, bm.bmWidth, bm.bmHeight, 0, fmt[i], GL_UNSIGNED_BYTE, bm.bmBits);
		DeleteObject(img);
	}
}

GLvoid GLBase::updateView() {
	if (d2Theta == DEC_V && dTheta > 0.0 || d2Theta == -DEC_V && dTheta < 0.0) {
		d2Theta = dTheta = 0;
	} else {
		dTheta += d2Theta;
	}
	dTheta = dTheta > -MAX_V ? dTheta < MAX_V ? dTheta : MAX_V : -MAX_V;
	theta += dTheta;
	theta = theta > 0.0 ? theta < MAX_THETA ? theta : MAX_THETA : 0.0;

	if (d2Phi == DEC_V && dPhi > 0.0 || d2Phi == -DEC_V && dPhi < 0.0) {
		d2Phi = dPhi = 0;
	} else {
		dPhi += d2Phi;
	}
	dPhi = dPhi > -MAX_V ? dPhi < MAX_V ? dPhi : MAX_V : -MAX_V;
	phi += dPhi;
	phi = phi > 0.0 ? phi < PI * 2 ? phi : phi - PI * 2 : phi + PI * 2;

	viewCenter[0] = viewCenter[1] = viewCenter[2] = 0.0;
	viewEye[0] = radio * sin(theta) * cos(phi);
	viewEye[1] = radio * sin(theta) * sin(phi);
	viewEye[2] = radio * cos(theta);
	viewUp[0] = -cos(theta) * cos(phi);
	viewUp[1] = -cos(theta) * sin(phi);
	viewUp[2] = sin(theta);
}

bool GLBase::initialize(HWND hWnd) {
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// 上述格式描述符的大小
		1,								// 版本号
		PFD_DRAW_TO_WINDOW |			// 格式支持窗口
		PFD_SUPPORT_OPENGL |			// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,				// 必须支持双缓冲
		PFD_TYPE_RGBA,					// 申请 RGBA 格式
		32,								// 选定色彩深度
		0, 0, 0, 0, 0, 0,				// 忽略的色彩位
		0,								// 无Alpha缓存
		0,								// 忽略Shift Bit
		0,								// 无累加缓存
		0, 0, 0, 0,						// 忽略聚集位
		16,								// 16位 Z-缓存 (深度缓存)
		0,								// 无蒙板缓存
		0,								// 无辅助缓存
		PFD_MAIN_PLANE,					// 主绘图层
		0,								// Reserved
		0, 0, 0							// 忽略层遮罩
	};
	hdc = GetDC(hWnd);
	if (!hdc) {
		return false;
	}
	GLuint pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (!pixelFormat || !SetPixelFormat(hdc, pixelFormat, &pfd)) {
		return false;
	}
	hrc = wglCreateContext(hdc);
	if (!hrc || !wglMakeCurrent(hdc, hrc)) {
		return false;
	}
	return true;
}

void GLBase::fpsRestrict() {
	double fps0;
	frameCount++;
	do {
		t2 = GetTickCount();
		fps0 = (double)frameCount * 1000.0 / (double)(t2 - t1);
	} while (fps0 > fpsMax);
	if (t2 - t1 > 500) {
		frameCount = 0;
		t1 = t2;
		fps = fps0;
	}
}

GLvoid GLBase::drawGLScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 150.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	GLfloat lpos1 [] = { 14.0f + (float)theta * 12.0f, 14.0f - (float)theta * 12.0f, -5.0f, 0.0f };
	GLfloat lamb1 [] = { 0.10f, 0.11f, 0.12f, 1.0f };
	GLfloat ldiff1[] = { 0.57f, 0.58f, 0.59f, 1.0f };
	GLfloat lspec1[] = { 1.00f, 1.00f, 1.00f, 1.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, lpos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lamb1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, ldiff1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lspec1);
	glEnable(GL_LIGHT1);
	glLoadIdentity();
	updateView();
	gluLookAt(
		viewEye[0], viewEye[1], viewEye[2],
		viewCenter[0], viewCenter[1], viewCenter[2],
		viewUp[0], viewUp[1], viewUp[2]
	);

	GLfloat lpos [] = { -15.0f, 22.0f, 9.00f, 0.0f };
	GLfloat lamb [] = {  0.22f, 0.21f, 0.20f, 1.0f };
	GLfloat ldiff[] = {  0.32f, 0.31f, 0.30f, 1.0f };
	GLfloat lspec[] = {  0.10f, 0.10f, 0.10f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lamb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lspec);
	glEnable(GL_LIGHT0);

	drawLock.lock();
	if (Reversi::game) {
		Reversi::game->situ.draw();
		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		Reversi::game->cb->draw();
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				Reversi::game->cp[i][j]->draw();
			}
		}

		glEnable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Reversi::game->cb->draw();
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				Reversi::game->cp[i][j]->draw();
			}
		}
		Reversi::game->situ.draw();
	}
	drawLock.unlock();
	//drawAxes();
	fpsRestrict();
	SwapBuffers(hdc);
}

GLvoid GLBase::initGLScene() {
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(1.5f);
	glEnable(GL_LIGHTING);

	const int texID[] = { IDB_BITMAP1, IDB_BITMAP2 };
	GLenum fmt[] = { GL_BGR_EXT, GL_BGR_EXT };
	LoadTextures(2, texID, fmt);

	radio = 120.0;
	theta = 0.0;
	phi = PI * 3 / 2;
	dTheta = 0.0;
	dPhi = 0.0;
}

GLvoid GLBase::resizeGLScene(GLsizei width, GLsizei height) {
	height = height ? height : 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 1.0, 1000.0);
	SwapBuffers(hdc);
}

GLvoid GLBase::keyEvent(bool keyDown, WPARAM keyId) {
	switch (keyId) {
	case VK_LEFT:
		d2Phi = keyDown ? ACC_V : -DEC_V; break;
	case VK_RIGHT:
		d2Phi = keyDown ? -ACC_V : DEC_V; break;
	case VK_UP:
		d2Theta = keyDown ? ACC_V : -DEC_V; break;
	case VK_DOWN:
		d2Theta = keyDown ? -ACC_V : DEC_V; break;
	}
}

GLvoid GLBase::mouseEvent(int button, int posX, int posY) {
	drawLock.lock();
	if (Reversi::game) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				Reversi::game->cp[i][j]->handleMouse(button, posX, posY);
			}
		}
	}
	drawLock.unlock();
	return;
}
