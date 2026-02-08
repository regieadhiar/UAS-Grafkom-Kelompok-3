#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <cctype>

// ---------------------------------- konstanta
const float PI = 3.14159265f;
const float MOVE_SPEED = 5.0f;
const float MOUSE_SENSITIVITY = 0.1f;

// ---------------------------------- variabel kamera (TETAP SAMA)
float cameraPosX = 0.0f, cameraPosY = 3.0f, cameraPosZ = 10.0f;
float cameraYaw = 0.0f;
float cameraPitch = -15.0f;

int windowWidth = 1200, windowHeight = 800;
bool mouseCaptured = false;
bool keys[256] = { false };
float deltaTime = 0.0f, lastFrameTime = 0.0f;

// ---------------------------------- FUNGSI OBJEK TAMAN (TIKAR & KERANJANG)

void drawPiknikScene() {
    // 1. TIKAR (Motif Kotak-kotak Merah Putih)
    float matSize = 4.0f;
    float step = 0.5f;
    glPushMatrix();
    glTranslatef(0.0f, 0.01f, 0.0f); // Sedikit di atas tanah agar tidak berkedip
    for (float i = -matSize / 2; i < matSize / 2; i += step) {
        for (float j = -matSize / 2; j < matSize / 2; j += step) {
            // Selang-seling warna merah dan putih
            if ((int)((i + j) * 2) % 2 == 0) glColor3ub(200, 30, 30); // Merah
            else glColor3ub(255, 255, 255); // Putih
            
            glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glVertex3f(i, 0, j);
                glVertex3f(i + step, 0, j);
                glVertex3f(i + step, 0, j + step);
                glVertex3f(i, 0, j + step);
            glEnd();
        }
    }
    glPopMatrix();

    // 2. KERANJANG PIKNIK (Cokelat Anyaman)
    glPushMatrix();
    glTranslatef(0.5f, 0.3f, 0.0f); // Posisi di atas tikar
    glColor3ub(139, 69, 19); // Cokelat kayu
    
    // Badan Keranjang
    glPushMatrix();
    glScalef(1.2f, 0.6f, 0.8f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Tutup Keranjang (Agak terbuka sedikit)
    glPushMatrix();
    glTranslatef(0.0f, 0.35f, 0.0f);
    glRotatef(-10, 0, 0, 1);
    glScalef(1.3f, 0.1f, 0.9f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Gagang Keranjang (Torus yang digepengkan)
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.0f);
    glRotatef(90, 0, 1, 0);
    glScalef(1.0f, 1.5f, 0.1f);
    glutSolidTorus(0.05, 0.4, 10, 20);
    glPopMatrix();
    
    glPopMatrix();
}

void drawGround() {
    glPushMatrix();
    glColor3ub(34, 139, 34); // Hijau rumput lebih terang
    glTranslatef(0, -0.05f, 0);
    glScalef(50.0f, 0.1f, 50.0f);
    glutSolidCube(1.0);
    glPopMatrix();
}

// ---------------------------------- Sistem Pencahayaan
void updateLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPos[] = { 5.0f, 10.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

// ---------------------------------- Logic Kamera & Display
void updateCamera() {
    float speed = MOVE_SPEED * deltaTime;
    float yawRad = cameraYaw * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;
    float frontX = sin(yawRad) * cos(pitchRad);
    float frontY = sin(pitchRad);
    float frontZ = -cos(yawRad) * cos(pitchRad);

    if (keys['w']) { cameraPosX += frontX * speed; cameraPosY += frontY * speed; cameraPosZ += frontZ * speed; }
    if (keys['s']) { cameraPosX -= frontX * speed; cameraPosY -= frontY * speed; cameraPosZ -= frontZ * speed; }
    if (keys['a']) { cameraPosX -= cos(yawRad) * speed; cameraPosZ -= sin(yawRad) * speed; }
    if (keys['d']) { cameraPosX += cos(yawRad) * speed; cameraPosZ += sin(yawRad) * speed; }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)windowWidth / windowHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float yawRad = cameraYaw * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ,
              cameraPosX + sin(yawRad)*cos(pitchRad), cameraPosY + sin(pitchRad), cameraPosZ - cos(yawRad)*cos(pitchRad),
              0, 1, 0);

    updateLighting();
    drawGround();
    drawPiknikScene(); // Memanggil tikar dan keranjang

    glutSwapBuffers();
}

// ---------------------------------- Input & Main
void timer(int v) {
    float cur = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = cur - lastFrameTime; lastFrameTime = cur;
    updateCamera();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char k, int x, int y) { keys[tolower(k)] = true; if (k == 27) exit(0); }
void keyboardUp(unsigned char k, int x, int y) { keys[tolower(k)] = false; }
void mouseMotion(int x, int y) {
    if (!mouseCaptured) return;
    int cx = windowWidth / 2, cy = windowHeight / 2;
    cameraYaw += (x - cx) * MOUSE_SENSITIVITY;
    cameraPitch -= (y - cy) * MOUSE_SENSITIVITY;
    glutWarpPointer(cx, cy);
}
void mouse(int b, int s, int x, int y) {
    if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
        mouseCaptured = true;
        glutSetCursor(GLUT_CURSOR_NONE);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Taman Piknik Isal");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMotion);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
