#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <sstream>
#include <thread>
#include <SOIL2/SOIL2.h>
#include "Camera.h"
#include "Object3D.h"
#include "Shader.h"
#include "Scene.h"
#include "TTT.h"
using namespace std;

float speed = 15;
map <char, bool> pressedKey = map<char, bool>();

float fpsCounter = 0;
float fpsInterval = 1.0f; // интервал для подсчета FPS
float currentTime = 0;
int frames = 0;
float TIME = -1;

Object3D* interior;
Object3D* outside;
Object3D* panel;

TTT* TicTacToe;

const int FIELD_SIZE = 9;

glm::vec3 Center = { 1.0f, -29.0f, 2.0f };
float radius = 17.0f;

float alpha = 0.0f;
float angSpeed = 0.01f;

Shader* lightShader;
Scene* mainScene;

glm::vec3 vert1 = { Center.x - 1.5f, Center.y - 2.2f, Center.z + 2.5f };
glm::vec3 vert2 = { Center.x + 2.5f, Center.y - 2.2f, Center.z + 2.5f };
glm::vec3 vert3 = { Center.x - 1.5f, Center.y - 2.2f, Center.z - 1.5f };
glm::vec3 vert4 = { Center.x + 2.5f, Center.y - 2.2f, Center.z - 1.5f };
glm::vec3 norm = { 0.0f, 1.0f, 0.0f };

auto poly1 = vector<glm::vec3>{ vert1, vert2, vert3 };
auto poly2 = vector<glm::vec3>{ vert2, vert4, vert3 };

void RenderScene() {
    mainScene->Render();
}

void changeSize(int width, int height) {
    mainScene->Camera().HandleResize(width, height);
}

void LoadModels() {
    interior = new Object3D("objects/interior.obj", ".\\objects/");
    interior->SetPosition({ 0.0f, 0.0f, -1.0f });
    interior->SetRotation({ 0.0f, 0.0f, 0.0f });
    interior->SetScale({ 0.15f, 0.15f, 0.15f });
    interior->LoadTexture("objects/interior.png");

    outside = new Object3D("objects/outside.obj", ".\\objects/");
    outside->SetPosition({ 0.0f, -80.0f, -15.0f });
    outside->SetRotation({ 90.0f, 180.0f, 0.0f });
    outside->SetScale({ 6.15f, 6.15f, 6.15f });
    outside->LoadTexture("objects/outside.jpeg");

    panel = new Object3D("objects/panel.obj", ".\\objects/");
    panel->SetPosition({ Center.x+0.50f, Center.y, Center.z - 0.75f });
    panel->SetRotation({ 90.0f, 90.0f, 0.0f });
    panel->SetScale({ 2.3f, 25.15f, 5.15f });
    panel->LoadTexture("objects/panel.jpg");

    TicTacToe = new TTT( { Center.x - 1.0f, Center.y - 2.2f , Center.z + 1.0f  } );
}

void MouseHandle(int x, int y) {
    static int mouseX = 0, mouseY = 0;
    static float sensitivity = 0.1f;
    float dx = mouseX - x;
    float dy = mouseY - y;

    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2,
        glutGet(GLUT_WINDOW_HEIGHT) / 2);
    mouseX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    mouseY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    float ang = mainScene->Camera().GetAngle(),
        rot = mainScene->Camera().GetRotation();

    mainScene->Camera().SetAngle(ang + dx * sensitivity);
    mainScene->Camera().SetRotation(rot + dy * sensitivity);
    TicTacToe->HandleMouseMovement(mainScene->Camera().GetPosition(), mainScene->Camera().GetDir());
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D scene");
    // Initialize GLEW
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        cout << "Error initializing GLEW: " << glewGetErrorString(glewStatus);
        return 1;
    }
    mainScene = new Scene();
    mainScene->Camera() = Camera({ 0.0f, -42, 2 }, 0, 0);
    LoadModels();
    mainScene->AddObject(TicTacToe);
    mainScene->AddObject(interior);
    mainScene->AddObject(panel);
    mainScene->AddObject(outside);

    mainScene->DirectionLight() = DirectionLight({0.0f, 0.0f, 1.0f},
                                                 {1.0f, 1.0f, 1.0f},
                                                 {1.0f, 1.0f, 1.0f},
                                                 {1.0f, 1.0f, 1.0f});
    // Register event callbacks
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(changeSize);

    glutSetOption(GLUT_MULTISAMPLE, 4);

    glutKeyboardFunc([](unsigned char key, int x, int y) -> void {
        pressedKey[key] = true;

        });
    glutSpecialFunc([](int key, int x, int y) -> void {
        });
    glutKeyboardUpFunc([](unsigned char key, int x, int y) -> void {
        if (key == 27) {
            glutLeaveMainLoop();
            return;
        }
        pressedKey[key] = false;
        });

    glutFullScreen();
    glutSetCursor(GLUT_CURSOR_NONE);

    glutPassiveMotionFunc(MouseHandle);
    glutMotionFunc(MouseHandle);

    glutMouseFunc([](int button, int state, int x, int y) -> void {
        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_UP) {
                TicTacToe->HandleMouseClick();
            }
        }
        if (button == GLUT_RIGHT_BUTTON) {
            if (state == GLUT_UP) {
                TicTacToe->RestartGame();
            }
        }
    });

    glutIdleFunc([]()->void {
        
        if (TIME == -1) {
            TIME = glutGet(GLUT_ELAPSED_TIME);
            return;
        }

        float currTime = glutGet(GLUT_ELAPSED_TIME);
        float dt = (currTime - TIME) / 1000;
        float dis = speed * dt;

        alpha += angSpeed * dt;
        mainScene->DirectionLight().SetDirection({ cosf(alpha * 180.0f / M_PI), 
                                                   sinf(alpha * 180.0f / M_PI), 
                                                   1.0f });

        glm::vec3 pos = mainScene->Camera().GetPosition();
        float ang = mainScene->Camera().GetAngle();


        if (pressedKey['w']) {
            pos = {
                pos.x - cosf(M_PI / 180.0 * ang) * dis,
                pos.y - sinf(M_PI / 180.0 * ang) * dis,
                pos.z };
        }
        else if (pressedKey['s']) {
            pos = {
                pos.x + cosf(M_PI / 180.0 * ang) * dis,
                pos.y + sinf(M_PI / 180.0 * ang) * dis,
                pos.z };
        }
        if (pressedKey['a']) {
            pos = { 
                pos.x - cosf(M_PI / 180.0 * (ang + 90)) * dis,
                pos.y - sinf(M_PI / 180.0 * (ang + 90)) * dis,
                pos.z };
        }
        else if (pressedKey['d']) {
            pos = { 
                pos.x + cosf(M_PI / 180.0 * (ang + 90)) * dis,
                pos.y + sinf(M_PI / 180.0 * (ang + 90)) * dis,
                pos.z };
        }

        float dist = sqrt(powf((pos.x - Center.x), 2) + powf((pos.y - Center.y), 2) + powf((pos.z - Center.z), 2));

        if (dist < radius && dist > 5) {
            mainScene->Camera().SetPosition(pos);
        }

        TIME = currTime;

        // Подсчет FPS
        currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // текущее время в секундах
        frames++;

       /* if (currentTime - fpsCounter > fpsInterval) {
            float fps = frames / (currentTime - fpsCounter);
            std::cout << "FPS: " << fps << std::endl;
            std::cout << "Pos: " << mainScene->Camera().GetPosition().x << " "
                      << mainScene->Camera().GetPosition().y << " "
                      << mainScene->Camera().GetPosition().z << endl;
            fpsCounter = currentTime;
            frames = 0;
        }*/
        glutPostRedisplay();
    });

    //lightShader = new Shader("./Shaders/light.shader", false);
    glutMainLoop();

    return 0;
}