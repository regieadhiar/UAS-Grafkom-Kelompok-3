#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <fstream>

const float PI = 3.14159265f;
const float MOVE_SPEED = 6.0f;
const float MOUSE_SENSITIVITY = 0.08f;

float cameraPosX = 0.0f, cameraPosY = 4.5f, cameraPosZ = 14.0f;
float cameraYaw   = 0.0f;
float cameraPitch = -15.0f;

int windowWidth = 1280, windowHeight = 720;
bool mouseCaptured = false;
bool keys[256] = {false};

GLuint grassTexture;

GLuint loadBMP(const char* filename) {
    unsigned char header[54];
    unsigned int dataPos, imageSize, width, height;
    unsigned char* data = nullptr;
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cout << "Cannot open " << filename << "\n";
        return 0;
    }

    file.read((char*)header, 54);

    dataPos    = *(int*)&(header[10]);
    imageSize  = *(int*)&(header[34]);
    width      = *(int*)&(header[18]);
    height     = *(int*)&(header[22]);

    if (imageSize == 0) imageSize = width * height * 3;

    data = new unsigned char[imageSize];
    file.seekg(dataPos);
    file.read((char*)data, imageSize);
    file.close();

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

    delete[] data;
    return texID;
}

void init() {
    glClearColor(0.12f, 0.18f, 0.28f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    grassTexture = loadBMP("grass.bmp");   // pastikan file grass.bmp ada

    GLfloat light_pos[]   = {8.0f, 12.0f, 6.0f, 1.0f};
    GLfloat light_diff[]  = {0.9f, 0.9f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diff);
}

void drawFloor() {
    if (grassTexture) {
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glEnable(GL_TEXTURE_2D);
    }

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, 0.0f, -25.0f);
        glTexCoord2f(8.0f, 0.0f); glVertex3f(-25.0f, 0.0f,  25.0f);
        glTexCoord2f(8.0f, 8.0f); glVertex3f( 25.0f, 0.0f,  25.0f);
        glTexCoord2f(0.0f, 8.0f); glVertex3f( 25.0f, 0.0f, -25.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void drawWaterFlow(float x, float y, float z, float radius, float height) {
    glColor4f(0.4f, 0.7f, 1.0f, 0.35f);  // biru transparan = efek air
    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(-90, 1,0,0);
        GLUquadric* q = gluNewQuadric();
        gluCylinder(q, radius*0.9, radius*0.6, height, 24, 1);
        gluDeleteQuadric(q);
    glPopMatrix();
}

void drawFountain() {
    // ────────────────────────────────────────────────
    // Bagian OPAQUE (tidak transparan) → render dulu
    // ────────────────────────────────────────────────

    // 1. Kolam dasar (ring beton)
    glColor3ub(75, 75, 80);
    glPushMatrix();
        glTranslatef(0, 0.25f, 0);
        glRotatef(90, 1,0,0);
        glutSolidTorus(0.40, 6.2, 24, 64);
    glPopMatrix();

    // Tingkat 1 (mangkuk bawah)
    float level1_y = 0.6f;
    glColor3ub(140, 30, 100);
    glPushMatrix();
        glTranslatef(0, level1_y, 0);
        glRotatef(90, 1,0,0);
        glutSolidCone(4.2, 1.1, 32, 6);
    glPopMatrix();

    // Tingkat 2
    float level2_y = level1_y + 1.4f;
    glColor3ub(155, 45, 115);
    glPushMatrix();
        glTranslatef(0, level2_y, 0);
        glRotatef(90, 1,0,0);
        glutSolidCone(3.1, 0.9, 32, 6);
    glPopMatrix();

    // Tingkat 3
    float level3_y = level2_y + 1.3f;
    glColor3ub(170, 60, 130);
    glPushMatrix();
        glTranslatef(0, level3_y, 0);
        glRotatef(90, 1,0,0);
        glutSolidCone(2.0, 0.7, 32, 6);
    glPopMatrix();

    // ── Tiang tengah emas ──
    float tiang_radius = 0.50f;
    float tiang_tinggi = level3_y + 0.4f;   // ujung tiang berada di sini
    glColor3ub(220, 180, 60);
    glPushMatrix();
        glTranslatef(0, 0.5f, 0);
        glRotatef(-90, 1,0,0);
        glutSolidCylinder(tiang_radius, tiang_tinggi, 24, 1);
    glPopMatrix();

    // ── Mangkuk kecil (penutup celah) ──
    // Posisi: tepat di ujung tiang + sedikit overlap kecil
    float overlap = 0.12f;                    // sesuaikan ini jika masih ada celah (0.08f – 0.20f biasanya pas)
    float mangkuk_kecil_y = 0.5f + tiang_tinggi - overlap;

    glColor3ub(170, 60, 130);                 // sama dengan tingkat 3 agar nyambung
    glPushMatrix();
        glTranslatef(0, mangkuk_kecil_y, 0);
        glRotatef(90, 1,0,0);                 // cone terbalik → bentuk mangkuk
        glutSolidCone(tiang_radius + 0.9f, 0.55f, 28, 1);  // radius alas lebih lebar, tinggi sedang
    glPopMatrix();

    // ── Ornamen puncak (segitiga/kerucut emas) ──
    // Posisi: tepat di atas mangkuk kecil + sedikit overlap
    float puncak_y = mangkuk_kecil_y + 0.55f - 0.10f;   // 0.55f = tinggi mangkuk kecil, -0.10f = overlap kecil

    glColor3ub(255, 220, 80);
    glPushMatrix();
        glTranslatef(0, puncak_y, 0);
        glRotatef(-90, 1,0,0);
        glutSolidCone(0.75f, 1.2f, 16, 1);    // radius dasar agak lebih kecil dari alas mangkuk kecil
    glPopMatrix();

    // ────────────────────────────────────────────────
    // Bagian TRANSPARAN → render setelah opaque
    // ────────────────────────────────────────────────

    glDepthMask(GL_FALSE);

    // Permukaan air kolam
    glColor4f(0.15f, 0.38f, 0.65f, 0.75f);
    glPushMatrix();
        glTranslatef(0, 0.18f, 0);
        glRotatef(-90, 1,0,0);
        GLUquadric* q = gluNewQuadric();
        gluDisk(q, 0, 6.2, 64, 1);
        gluDeleteQuadric(q);
    glPopMatrix();

    // Aliran air
    drawWaterFlow(0, level1_y + 0.4f, 0, 4.2f, 1.8f);
    drawWaterFlow(0, level2_y + 0.3f, 0, 3.1f, 1.4f);
    drawWaterFlow(0, level3_y + 0.25f, 0, 2.0f, 1.2f);

    glDepthMask(GL_TRUE);
}

void updateCamera() {
    float yawRad   = cameraYaw   * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;

    float fx = sin(yawRad) * cos(pitchRad);
    float fy = sin(pitchRad);
    float fz = -cos(yawRad) * cos(pitchRad);

    float speed = MOVE_SPEED * 0.016f;

    if (keys['w']) { cameraPosX += fx*speed; cameraPosY += fy*speed; cameraPosZ += fz*speed; }
    if (keys['s']) { cameraPosX -= fx*speed; cameraPosY -= fy*speed; cameraPosZ -= fz*speed; }

    if (keys['a']) {
        float rx = cos(yawRad);
        float rz = sin(yawRad);
        cameraPosX -= rx*speed;
        cameraPosZ -= rz*speed;
    }
    if (keys['d']) {
        float rx = cos(yawRad);
        float rz = sin(yawRad);
        cameraPosX += rx*speed;
        cameraPosZ += rz*speed;
    }

    if (keys['e']) cameraPosY += speed;
    if (keys['q']) cameraPosY -= speed;
}

void display() {
    updateCamera();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (double)windowWidth / windowHeight, 0.1, 120.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float yawRad   = cameraYaw   * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;

    float lx = cameraPosX + sin(yawRad) * cos(pitchRad);
    float ly = cameraPosY + sin(pitchRad);
    float lz = cameraPosZ - cos(yawRad) * cos(pitchRad);

    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, lx, ly, lz, 0,1,0);

    drawFloor();
    drawFountain();

    glutSwapBuffers();
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (!mouseCaptured) return;

    cameraYaw   += (x - windowWidth/2)  * MOUSE_SENSITIVITY;
    cameraPitch -= (y - windowHeight/2) * MOUSE_SENSITIVITY;

    if (cameraPitch > 88)  cameraPitch = 88;
    if (cameraPitch < -88) cameraPitch = -88;

    glutWarpPointer(windowWidth/2, windowHeight/2);
}

void keyboard(unsigned char key, int, int) {
    keys[tolower(key)] = true;
    if (key == 27) exit(0);
}

void keyboardUp(unsigned char key, int, int) {
    keys[tolower(key)] = false;
}

void mouse(int button, int state, int, int) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseCaptured = true;
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(windowWidth/2, windowHeight/2);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Air Mancur Bertingkat 3D");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc([](int w, int h){
        glViewport(0,0,w,h);
        windowWidth = w; windowHeight = h;
    });

    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMotion);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}