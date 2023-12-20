#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm/vec2.hpp>
#include <string>
#include <map>

class Window {
	static Window* w_mainWindow;
	glm::uvec2 w_size = { 800, 600 };
	std::string w_title = "";
	bool w_isFullScreen = false;
	bool w_isActivated = false;
	bool w_showCursor = true;
	unsigned int w_multiSample = 2;
	bool w_zbuf = true;
	std::map<int, bool> w_keyPressed = std::map<int, bool>();
	Window() {}
public:
	static void InitWindow(int width = 800, int height = 600, std::string title = "") {
		w_mainWindow = new Window();
		w_mainWindow->w_size = { width, height };
		w_mainWindow->w_title = title;
	}
	void SetMultisample(unsigned int ms) { 
		w_multiSample = ms; 

		glutSetOption(GLUT_MULTISAMPLE, w_multiSample);
	}
	unsigned int GetMultisample() const { return w_multiSample; }
	void SetZBuf(bool zbuf) { 
		w_zbuf = zbuf; 

		if (w_zbuf) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
	}
	bool GetZBuf() const { return w_zbuf; }
	void SetFullscreen(bool fs) { 
		w_isFullScreen = fs; 

		if (w_isFullScreen) {
			glutFullScreen();
		}
		else {
			glutReshapeWindow(w_size.x, w_size.y);
		}
	}
	bool GetFullscreen() const { return w_isFullScreen; }
	void SetCursor(bool crs) { 
		w_showCursor = crs; 

		if (w_showCursor) {
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		}
		else {
			glutSetCursor(GLUT_CURSOR_NONE);
		}
	}
	bool GetCursor() const { return w_showCursor; }

	void Start(int argc = 0, char** argv = nullptr) {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
		glutInitWindowSize(w_size.x, w_size.y);
		glutCreateWindow(w_title.c_str());
		glEnable(GL_LIGHTING);
	}
};
