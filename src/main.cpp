// ---------------------------------- library
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>

// ---------------------------------- konstanta
const float PI = 3.14159265f;
const float MOVE_SPEED = 5.0f;          // unit per detik
const float MOUSE_SENSITIVITY = 0.1f;   // sensitivitas mouse

// ---------------------------------- variabel kamera
float cameraPosX = 0.0f, cameraPosY = 3.0f, cameraPosZ = 8.0f;
float cameraYaw = 0.0f;      // rotasi horizontal (derajat)
float cameraPitch = -20.0f; // rotasi vertikal (derajat)

// Tambahkan di bagian global (dekat variabel kamera lainnya)
enum CameraMode { FREE_CAMERA, ORBIT_CAMERA };
CameraMode currentMode = FREE_CAMERA;  // default: bebas gerak

// Untuk mode Orbit
float orbitRadius = 10.0f;          // jarak kamera dari pusat
float orbitCenterX = 0.0f;          // pusat yang dilihat (biasanya 0,0,0)
float orbitCenterY = 1.0f;
float orbitCenterZ = 0.0f;
float orbitYaw   = 0.0f;            // sudut horizontal (mirip yaw)
float orbitPitch = 20.0f;           // sudut vertikal (dari horizontal)
float targetLookX = 0.0f;
float targetLookY = 1.0f;   // biasanya agak di atas lantai supaya lebih natural
float targetLookZ = 0.0f;

float autoRotateSpeed = 20.0f;     // derajat per detik (positif = searah jarum jam, negatif = berlawanan)
bool autoRotateEnabled = false;     // bisa di-toggle kalau mau

int windowWidth = 1200, windowHeight = 800;
bool mouseCaptured = false;

bool keys[256] = {false};

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

void stair(int colorType = 0) {
    glPushMatrix();
        switch (colorType)
        {
        case 1:
            glColor3ub(62, 62, 62);
            break;
        default:
            glColor3ub(65, 42, 20);
            break;
        }
        glTranslatef(0.0f, 0.5f, 0.0f);
        glScalef(1.0f, 0.5f, 1.0f);
        glutSolidCube(1.0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.25f, 0.0f, 0.0f);
        glScalef(0.5f, 0.5f, 1.0f);
        glutSolidCube(1.0);
    glPopMatrix();
}

void trapdoor() {
    glPushMatrix();
        glColor3ub(58, 39, 17);
        glScalef(1.0f, 0.2f, 1.0f);
        glutSolidCube(1.0);
    glPopMatrix();
}

void slab(int colorType = 0) {
    glPushMatrix();
        switch (colorType)
        {
        case 1:
            glColor3ub(62, 62, 62);
            break;
        default:
            glColor3ub(65, 42, 20);
            break;
        }
        glScalef(1.0f, 0.5f, 1.0f);
        glutSolidCube(1.0);
    glPopMatrix();
}

void cube(int colorType = 0) {
    switch (colorType)
    {
    case 1:
        glColor3ub(179, 147, 88); // Wood Plank Color
        break;
    case 2:
        glColor3ub(128, 127, 128); // Stone Color
        break;
    case 3:
        glColor3ub(121, 89, 51); // Wood Slab Color
        break;
    case 4:
        glColor3ub(234, 226, 218); // Quartz White Color
        break;
    case 5:
        glColor3ub(65, 42, 20); // Roof Color
        break;
    case 6:
        glColor3ub(134, 146, 140);
        break;
    default:
        glColor3ub(62, 48, 31); // Log Beam Color
        break;
    }
    glutSolidCube(1.0);
}

// Variabel global (taruh di atas, bersama variabel lain)
float doorAngle = 0.0f;           // sudut saat ini
float doorTargetAngle = 0.0f;     // 0 = tertutup, 90 = terbuka
float doorOpenSpeed = 180.0f;     // cepat buka/tutup (~0.5 detik)

// Fungsi update animasi (panggil setiap frame)
void updateDoor(float deltaTime) {
    if (fabs(doorAngle - doorTargetAngle) > 0.01f) {
        float direction = (doorTargetAngle > doorAngle) ? 1.0f : -1.0f;
        doorAngle += direction * doorOpenSpeed * deltaTime;

        // Agar tidak overshoot
        if (direction > 0 && doorAngle > doorTargetAngle) doorAngle = doorTargetAngle;
        if (direction < 0 && doorAngle < doorTargetAngle) doorAngle = doorTargetAngle;
    }
}

// Fungsi pintu yang kamu punya, ditambah rotasi saja
void pintu() {
    glPushMatrix();
        glColor3ub(101, 67, 33);

        glTranslatef(0.0f, 1.5f, 4.0f);           // posisi awal pintu
        
        // Engsel di sisi kiri (seperti pintu minecraft)
        glTranslatef(-0.5f, 0.0f, 0.0f);          // geser pivot ke kiri (setengah lebar)
        glRotatef(doorAngle, 0.0f, 1.0f, 0.0f);   // rotasi di sumbu Y
        glTranslatef(0.5f, 0.0f, 0.0f);           // balik ke tengah

        glScalef(1.0f, 2.0f, 0.2f);
        cube();                      // atau cube() kalau kamu pakai fungsi custom
    glPopMatrix();
}

void Lantai() {
}

void drawFloor() {
    glPushMatrix();
        glTranslatef(10.0f, -0.25f, 2.5);
        glScalef(8.0f, 0.5f, 31.0f);
        cube(6);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-10.0f, -0.25f, 2.5);
        glScalef(8.0f, 0.5f, 31.0f);
        cube(6);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0f, -0.25f, 9.0);
        glScalef(12.0f, 0.5f, 18.0f);
        cube(6);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0f, -0.25f, -12.5);
        glScalef(12.0f, 0.5f, 1.5f);
        cube(6);
    glPopMatrix();
}

void batu(int rockType = 0) {
    switch (rockType)
    {
    case 1:
        glColor3ub(112.0, 128.0, 144.0);
        glScalef(0.5f, 0.5f, 0.5f); //Scale
        glutSolidIcosahedron();
        break;
    default:
        glColor3ub(112.0, 128.0, 144.0);
        glScalef(0.25f, 0.25f, 0.25f); //Scale
        glutSolidDodecahedron();
        break;
    }
}

// Fungsi untuk menggambar Lampu Taman Modern
void drawGardenLamp(float x, float y, float z, float scale) {
    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(scale, scale, scale);

        // --- 1. TIANG BESI (Batangan Bawah) ---
        glColor3ub(30, 30, 35); // Abu-abu gelap (besi tua)

        glPushMatrix();
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Tegakkan silinder
            glutSolidCylinder(0.10, 3.0, 16, 8);   // Tiang utama
            // Dudukan bawah (base)
            glTranslatef(0.0f, 0.0f, -0.05f);
            glutSolidCylinder(0.30, 0.10, 16, 4);
        glPopMatrix();

        // --- 2. BOLA LAMPU (Bulb) - hanya visual, tanpa menyala ---
        glPushMatrix();
            glTranslatef(0.0f, 3.0f, 0.0f); // Posisi di ujung tiang
            // Bola lampu warna kuning-putih hangat (mati/tidak menyala)
            glColor3ub(240, 235, 200); // Warna kuning lembut agak pudar
            glutSolidSphere(0.50, 20, 20);
        glPopMatrix();

        // --- 3. TOPI/PENUTUP ATAS (opsional) ---
        glPushMatrix();
            glColor3ub(30, 30, 35); // Warna besi sama seperti tiang
            glTranslatef(0.0f, 2.5f, 0.0f);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            glutSolidCone(0.18, 0.25, 12, 3); // Topi agak lebih lebar
        glPopMatrix();

    glPopMatrix();
}

void lampu() {
    drawGardenLamp(12.0f, 0.0f, 15.0f, 1.0f); 
    drawGardenLamp(-12.0f, 0.0f, 15.0f, 1.0f); 
    drawGardenLamp(12.0f, 0.0f, 8.0f, 1.0f); 
    drawGardenLamp(-12.0f, 0.0f, 8.0f, 1.0f); 
    drawGardenLamp(12.0f, 0.0f, -10.0f, 1.0f); 
    drawGardenLamp(-12.0f, 0.0f, -10.0f, 1.0f); 
}


// 1. Tambahkan di bagian global (dekat konstanta lain)
#define MAX_LIGHTS 6

float lightX[MAX_LIGHTS] = { 12.0f,  -12.0f, 12.0f,  -12.0f,  12.0f,  -12.0f };
float lightY[MAX_LIGHTS] = { 3.0f,  3.0f,  3.0f,  3.0f,  3.0f,  3.0f };
float lightZ[MAX_LIGHTS] = { 15.0f,  15.0f,  8.0f,  8.0f, -10.0f, -10.0f };

bool lightIsSpot[MAX_LIGHTS] = { false, false, false, false, false, false }; // true = spotlight

float spotCutoff = 35.0f;    // bisa diubah-ubah nanti
float spotExponent = 5.0f;

int totalLights = 6;


// 2. Fungsi updateLighting (copy utuh)
void updateLighting() {
    glEnable(GL_LIGHTING);

    // Matikan semua light dulu (aman)
    for (int i = 0; i < 8; i++) {
        glDisable(GL_LIGHT0 + i);
    }

    for (int i = 0; i < totalLights; i++) {
        GLenum light = GL_LIGHT0 + i;
        glEnable(light);

        GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat ambient[]    = {0.05f, 0.05f, 0.08f, 1.0f};

        glLightfv(light, GL_DIFFUSE,  lightColor);
        glLightfv(light, GL_SPECULAR, lightColor);
        glLightfv(light, GL_AMBIENT,  ambient);

        GLfloat pos[] = {lightX[i], lightY[i], lightZ[i], 1.0f};
        glLightfv(light, GL_POSITION, pos);

        if (lightIsSpot[i]) {
            GLfloat dir[] = {0.0f, -1.0f, 0.0f};
            glLightfv(light, GL_SPOT_DIRECTION, dir);
            glLightf(light, GL_SPOT_CUTOFF, spotCutoff);
            glLightf(light, GL_SPOT_EXPONENT, spotExponent);
        } else {
            glLightf(light, GL_SPOT_CUTOFF, 45.0f);
        }

        // Attenuation supaya realistis
        glLightf(light, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(light, GL_LINEAR_ATTENUATION, 0.17f);
        glLightf(light, GL_QUADRATIC_ATTENUATION, 0.035f);
    }
}


// 3. (Opsional) Visualisasi sumber cahaya — bagus untuk debug
void drawLightSource(float x, float y, float z, bool isSpot) {
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(x, y, z);
    if (isSpot) {
        glColor3f(1.0f, 1.0f, 0.3f);
        glRotatef(180, 1, 0, 0);
        glRotatef(90, 1, 0, 0);
        glutWireCone(0.4, 1.2, 12, 8);
    } else {
        glColor3f(0.9f, 0.9f, 1.0f);
        glutSolidSphere(0.55, 20, 20);
    }
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// ---------------------------------- Bagian Akhir Muhammad Rafli Ramdhani ----------------------------------

// ---------------------------------- pergerakan kamera
void updateCameraMovement() {
    float speed = MOVE_SPEED * deltaTime;

    if (currentMode == FREE_CAMERA) {
        // Mode Free Camera (FPS style)
        float yawRad   = cameraYaw   * PI / 180.0f;
        float pitchRad = cameraPitch * PI / 180.0f;

        float frontX = sin(yawRad) * cos(pitchRad);
        float frontY = sin(pitchRad);
        float frontZ = -cos(yawRad) * cos(pitchRad);

        if (keys['w'] || keys['W']) {
            cameraPosX += frontX * speed;
            cameraPosY += frontY * speed;
            cameraPosZ += frontZ * speed;
        }
        if (keys['s'] || keys['S']) {
            cameraPosX -= frontX * speed;
            cameraPosY -= frontY * speed;
            cameraPosZ -= frontZ * speed;
        }
        if (keys['a'] || keys['A']) {
            float rightX = cos(yawRad);
            float rightZ = sin(yawRad);
            cameraPosX -= rightX * speed;
            cameraPosZ -= rightZ * speed;
        }
        if (keys['d'] || keys['D']) {
            float rightX = cos(yawRad);
            float rightZ = sin(yawRad);
            cameraPosX += rightX * speed;
            cameraPosZ += rightZ * speed;
        }
        if (keys['e'] || keys['E']) cameraPosY += speed;
        if (keys['q'] || keys['Q']) cameraPosY -= speed;
    }
    else {  // ORBIT_CAMERA
        // Kontrol manual tetap ada (opsional)
        if (keys['w'] || keys['W']) orbitPitch += 40.0f * deltaTime;
        if (keys['s'] || keys['S']) orbitPitch -= 40.0f * deltaTime;
        if (keys['a'] || keys['A']) orbitYaw   -= 80.0f * deltaTime;
        if (keys['d'] || keys['D']) orbitYaw   += 80.0f * deltaTime;

        // Zoom in/out
        if (keys['q'] || keys['Q']) orbitRadius += 6.0f * deltaTime;
        if (keys['e'] || keys['E']) orbitRadius -= 6.0f * deltaTime;

        // ── ROTASI OTOMATIS ───────────────────────────────
        if (autoRotateEnabled) {
            orbitYaw += autoRotateSpeed * deltaTime;
            // Opsional: batasi agar tidak terlalu besar (hindari overflow lama kelamaan)
            // orbitYaw = fmod(orbitYaw, 360.0f);
        }
        // ──────────────────────────────────────────────────

        // Clamp nilai supaya tetap nyaman
        if (orbitPitch > 85.0f)  orbitPitch = 85.0f;
        if (orbitPitch < -85.0f) orbitPitch = -85.0f;
        if (orbitRadius < 2.0f)  orbitRadius = 2.0f;
        if (orbitRadius > 30.0f) orbitRadius = 30.0f;

        // Hitung posisi kamera dari sudut orbit
        float yawRad   = orbitYaw   * PI / 180.0f;
        float pitchRad = orbitPitch * PI / 180.0f;

        cameraPosX = orbitCenterX + orbitRadius * sin(yawRad) * cos(pitchRad);
        cameraPosY = orbitCenterY + orbitRadius * sin(pitchRad);
        cameraPosZ = orbitCenterZ + orbitRadius * cos(yawRad) * cos(pitchRad);
    }
}

// ---------------------------------- GLUT callbacks
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)windowWidth / windowHeight, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (currentMode == FREE_CAMERA) {
        // perhitungan lookAt seperti sebelumnya (arah depan)
        float yawRad   = cameraYaw * PI / 180.0f;
        float pitchRad = cameraPitch * PI / 180.0f;
        gluLookAt(cameraPosX, cameraPosY, cameraPosZ,
                  cameraPosX + sin(yawRad)*cos(pitchRad),
                  cameraPosY + sin(pitchRad),
                  cameraPosZ - cos(yawRad)*cos(pitchRad),
                  0,1,0);
    } else {
        // Orbit: selalu menghadap tepat ke pusat
        gluLookAt(cameraPosX, cameraPosY, cameraPosZ,
                  orbitCenterX, orbitCenterY, orbitCenterZ,
                  0,1,0);
    }
    // Update pencahayaan DULUAN sebelum view matrix
    updateLighting();

    // Opsional: tampilkan visualisasi lampu (untuk debug)
    for (int i = 0; i < totalLights; i++) {
        drawLightSource(lightX[i], lightY[i], lightZ[i], lightIsSpot[i]);
    }
    lampu();

    glutSwapBuffers();
}

// ---------------------------------- inisialisasi OpenGL
void init() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);           // tetap nyalakan lighting secara global
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    // Material default yang bagus (akan dipakai semua objek kecuali di-override)
    GLfloat default_specular[]  = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat default_shininess[] = {35.0f};   // agak lebih lembut dari 50
    glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, default_shininess);

}

void mouseMotion(int x, int y) {
    if (!mouseCaptured) return;

    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    float deltaX = (x - centerX) * MOUSE_SENSITIVITY;
    float deltaY = (y - centerY) * MOUSE_SENSITIVITY;

    if (currentMode == FREE_CAMERA) {
        cameraYaw   += deltaX;
        cameraPitch -= deltaY;
        if (cameraPitch > 89.0f)  cameraPitch = 89.0f;
        if (cameraPitch < -89.0f) cameraPitch = -89.0f;
    }
    // else {  // ORBIT_CAMERA
    //     orbitYaw   += deltaX * 1.5f;   // lebih responsif di orbit
    //     orbitPitch -= deltaY * 1.2f;
    //     if (orbitPitch > 85.0f)  orbitPitch = 85.0f;
    //     if (orbitPitch < -85.0f) orbitPitch = -85.0f;
    // }

    glutWarpPointer(centerX, centerY);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseCaptured = true;
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(windowWidth / 2, windowHeight / 2);
    }
}

void keyboard(unsigned char key, int x, int y) {
    key = tolower(key);
    keys[key] = true;

    if (key == 'm') {  // Tekan M untuk switch mode
        if (currentMode == FREE_CAMERA) {
            currentMode = ORBIT_CAMERA;
            printf("Switched to ORBIT CAMERA\n");
            // Opsional: reset orbit ke posisi awal
            orbitYaw = 0;
            orbitPitch = 20;
            orbitRadius = 10;
        } else {
            currentMode = FREE_CAMERA;
            printf("Switched to FREE CAMERA\n");
        }
    }
    if (key == 'f') {
        // Toggle buka / tutup
        doorTargetAngle = (doorTargetAngle == 0.0f) ? 90.0f : 0.0f;
    }

    if (key == 27) {  // ESC
        if (mouseCaptured) {
            mouseCaptured = false;
            glutSetCursor(GLUT_CURSOR_INHERIT);
        } else {
            exit(0);
        }
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    key = tolower(key);
    keys[key] = false;
}

void timer(int value) {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    updateCameraMovement();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("TB Kelompok 4 - Onsen Jepang dengan Asap");

    glutFullScreen(); // uncomment jika ingin fullscreen

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMotion);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouse);   
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}