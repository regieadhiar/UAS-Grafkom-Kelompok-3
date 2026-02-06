#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

// ---------------------------------- global variabel
const float PI = 3.14159265f;
const float MOVE_SPEED = 0.002f;
const float MOUSE_SENSITIVITY = 0.002f;
float cameraPosX = 0.0f, cameraPosY = 3.0f, cameraPosZ = 8.0f;
float cameraYaw = 0.0f, cameraPitch = -20.0f;
bool mouseActive = false;
bool keys[256] = {false};
int windowWidth = 1200, windowHeight = 800;

// Pengaturan cahaya tetap
bool lightingEnabled = true;
int lightMode = 1;
bool isDirectional = false;
bool isSpotlight = false;
float spotCutoff = 45.0f;
float spotExponent = 2.0f;
float linearAttenuation = 0.0f;
bool isShiny = true;

// ---------------------------------- fungsi pencahayaan
void initLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void updateLightingLogic() {
    if (!lightingEnabled) {
        glDisable(GL_LIGHTING);
        return;
    }
    glEnable(GL_LIGHTING);

    GLfloat val_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat val_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat val_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat val_black[] = {0.0f, 0.0f, 0.0f, 1.0f};

    if (lightMode == 1) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_specular);
    } else if (lightMode == 2) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_black);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_black);
    } else if (lightMode == 3) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_black);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_black);
    } else if (lightMode == 4) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, val_black);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, val_black);
        glLightfv(GL_LIGHT0, GL_SPECULAR, val_specular);
    }

    GLfloat w_component = isDirectional ? 0.0f : 1.0f;
    GLfloat light_pos[] = {0.0f, 5.0f, 0.0f, w_component};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    if (isSpotlight) {
        GLfloat spotDir[] = {0.0f, -1.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutoff);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
    } else {
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
    }

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttenuation);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

    if (isShiny) {
        GLfloat mat_spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    } else {
        GLfloat mat_spec[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    }
}

// ---------------------------------- fungsi objek dan tampilan
void drawFloor() {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.6f, 0.2f);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();

    glDisable(GL_LIGHTING);
    glColor3f(0.3f, 0.8f, 0.3f);
    glBegin(GL_LINES);
    for (float i = -10.0f; i <= 10.0f; i += 1.0f) {
        glVertex3f(i, 0.01f, -10.0f); glVertex3f(i, 0.01f, 10.0f);
        glVertex3f(-10.0f, 0.01f, i); glVertex3f(10.0f, 0.01f, i);
    }
    glEnd();
    if (lightingEnabled) glEnable(GL_LIGHTING);
}

void drawAyunan() {
    GLUquadric* quad = gluNewQuadric();

    // Tiang : silinder cokelat
	glColor3f(0.99f, 1.00f, 0.00f);
	glPushMatrix();
	glTranslatef(0.8f, 0.0f, 2.0f);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(quad, 0.15, 0.15, 4.0, 20, 20);
	glPopMatrix();
	
	//tiang kiri
	glColor3f(0.61f, 1.00f, 0.18f);
	glPushMatrix();
	glTranslatef(-2.8f, 0.0f, 2.0f);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(quad, 0.15, 0.15, 4.0, 20, 20);
	glPopMatrix();
	
	//tiang kanan
	glColor3f(0.61f, 1.00f, 0.18f);
	glPushMatrix();
	glTranslatef(4.8f, 0.0f, 2.0f);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(quad, 0.15, 0.15, 4.0, 20, 20);
	glPopMatrix();		

	glColor3f(0.22f, 0.90f, 0.30f);
	glPushMatrix();
	glTranslatef(-3.0f, 4.0f, 2.0f);
	glRotatef(90, 0, 1, 0);
	gluCylinder(quad, 0.15, 0.15, 4.0, 30, 30);
	glPopMatrix();
	
	glColor3f(0.22f, 0.90f, 0.30f);
	glPushMatrix();
	glTranslatef(1.0f, 4.0f, 2.0f);
	glRotatef(90, 0, 1, 0);
	gluCylinder(quad, 0.15, 0.15, 4.0, 30, 30);
	glPopMatrix();
	
	   
	 //rantai yang kiri 
	glColor3f(0.3f, 0.3f, 0.35f);
    glPushMatrix();
    glTranslatef(2.1f, 1.0f, 2.0f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quad, 0.05, 0.05, 3.0, 30, 30);
    glPopMatrix();

    // =========================
    // RANTAI KANAN
    // =========================
    glPushMatrix();
    glTranslatef(3.3f, 1.0f, 2.0f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quad, 0.05, 0.05, 3.0, 10, 10);
    glPopMatrix();

    // =========================
    // DUDUKAN AYUNAN
    // =========================
    glColor3f(0.1f, 0.7f, 0.2f);
    glPushMatrix();
    glTranslatef(2.7f, 1.0f, 2.0f);
    glScalef(1.5f, 0.2f, 0.6f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    
    
    //rantai yang kiri 
	glColor3f(0.3f, 0.3f, 0.35f);
    glPushMatrix();
    glTranslatef(-1.6f, 1.0f, 2.0f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quad, 0.05, 0.05, 3.0, 30, 30);
    glPopMatrix();

    // =========================
    // RANTAI KANAN
    // =========================
    glPushMatrix();
    glTranslatef(-0.4f, 1.0f, 2.0f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quad, 0.05, 0.05, 3.0, 10, 10);
    glPopMatrix();

    // =========================
    // DUDUKAN AYUNAN
    // =========================
    glColor3f(0.1f, 0.7f, 0.2f);
    glPushMatrix();
    glTranslatef(-1.0f, 1.0f, 2.0f);
    glScalef(1.5f, 0.2f, 0.6f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawLightSource() {
    if (!lightingEnabled) return;

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(0.0f, 5.0f, 0.0f);

    if (isDirectional) glColor3f(1.0f, 1.0f, 0.0f);
    else glColor3f(1.0f, 1.0f, 1.0f);

    if (isSpotlight) glutWireSphere(0.2, 10, 10);
    else glutSolidSphere(0.15, 20, 20);

    glPopMatrix();
    if (lightingEnabled) glEnable(GL_LIGHTING);
}

// ---------------------------------- fungsi kontrol kamera
void updateCamera() {
    float yawRad = cameraYaw * PI / 180.0f;
    float pitchRad = cameraPitch * PI / 180.0f;
    float forwardX = sin(yawRad) * cos(pitchRad);
    float forwardY = -sin(pitchRad);
    float forwardZ = -cos(yawRad) * cos(pitchRad);
    float rightX = sin(yawRad + PI/2);
    float rightZ = -cos(yawRad + PI/2);

    if (keys['w'] || keys['W']) {
        cameraPosX += forwardX * MOVE_SPEED;
        cameraPosY += forwardY * MOVE_SPEED;
        cameraPosZ += forwardZ * MOVE_SPEED;
    }
    if (keys['s'] || keys['S']) {
        cameraPosX -= forwardX * MOVE_SPEED;
        cameraPosY -= forwardY * MOVE_SPEED;
        cameraPosZ -= forwardZ * MOVE_SPEED;
    }
    if (keys['a'] || keys['A']) {
        cameraPosX -= rightX * MOVE_SPEED;
        cameraPosZ -= rightZ * MOVE_SPEED;
    }
    if (keys['d'] || keys['D']) {
        cameraPosX += rightX * MOVE_SPEED;
        cameraPosZ += rightZ * MOVE_SPEED;
    }
    if (keys[' ']) cameraPosY += MOVE_SPEED;
    if (keys[17] || keys[18]) cameraPosY -= MOVE_SPEED;
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
    float lookX = cameraPosX + sin(yawRad) * cos(pitchRad);
    float lookY = cameraPosY - sin(pitchRad);
    float lookZ = cameraPosZ - cos(yawRad) * cos(pitchRad);

    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);

    updateLightingLogic();

    drawFloor();

    // 5 Pohon di posisi berbeda
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);   // pohon tengah
    drawAyunan();
    glPopMatrix();

    drawLightSource();

    glutSwapBuffers();
}

// ---------------------------------- callback GLUT
void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y) {
    keys[key] = true;
    if (key == 27) exit(0);  // ESC untuk keluar
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void specialKey(int key, int x, int y) {
    keys[key] = true;
}

void specialKeyUp(int key, int x, int y) {
    keys[key] = false;
}

void mouseMotion(int x, int y) {
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    if (x == centerX && y == centerY) return;

    float deltaX = (x - centerX) * MOUSE_SENSITIVITY * 50.0f;
    float deltaY = (y - centerY) * MOUSE_SENSITIVITY * 50.0f;

    cameraYaw += deltaX;
    cameraPitch += deltaY;
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;

    glutWarpPointer(centerX, centerY);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseActive = true;
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(windowWidth/2, windowHeight/2);
    }
}

void idle() {
    updateCamera();
    glutPostRedisplay();
}

// ---------------------------------- main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Project GRAFKOM");
    glutFullScreen();

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKey);
    glutSpecialUpFunc(specialKeyUp);
    glutPassiveMotionFunc(mouseMotion);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
