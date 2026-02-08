#include <GL/glut.h>

// ===============================
// TRANSFORM PARAMETER
// ===============================
float tx=0, ty=0, tz=0;
float rx=0, ry=0, rz=0;
float sx=1, sy=1, sz=1;

// ===============================
// MATERIAL
// ===============================
void materialKayu(float shade){
    GLfloat mat[]  = {0.75f-shade, 0.55f-shade*0.7f, 0.25f-shade*0.5f, 1};
    GLfloat spec[] = {0.15f,0.15f,0.15f,1};
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,mat);
    glMaterialfv(GL_FRONT,GL_SPECULAR,spec);
    glMaterialf(GL_FRONT,GL_SHININESS,12);
}

void materialHitam(){
    GLfloat mat[] = {0.08f,0.08f,0.08f,1};
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,mat);
}

void materialMetal(){
    GLfloat mat[]  = {0.8f,0.8f,0.8f,1};
    GLfloat spec[] = {1,1,1,1};
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,mat);
    glMaterialfv(GL_FRONT,GL_SPECULAR,spec);
    glMaterialf(GL_FRONT,GL_SHININESS,90);
}

// ===============================
// OBJEK DASAR
// ===============================
void cube(){
    glutSolidCube(1);
}

// ===============================
// PANEL KAYU (DENGAN GRADASI)
// ===============================
void panelKayu(float x,float y,float z,float shade){
    glPushMatrix();
        glScalef(x,y,z);
        materialKayu(shade);
        cube();
    glPopMatrix();
}

// ===============================
// TONG SAMPAH KAYU MODERN
// ===============================
void tongSampah(
    float tx,float ty,float tz,
    float sx,float sy,float sz,
    float rx,float ry,float rz
){
    glPushMatrix();

    glTranslatef(tx,ty,tz);
    glRotatef(rx,1,0,0);
    glRotatef(ry,0,1,0);
    glRotatef(rz,0,0,1);
    glScalef(sx,sy,sz);

    // FRAME HITAM TIPIS
    glPushMatrix();
        glScalef(2.05f,4.0f,2.05f);
        materialHitam();
        cube();
    glPopMatrix();

    // PANEL KAYU DEPAN (GRADASI WARNA)
    float shade = 0.0f;
    for(float i=-0.85f;i<=0.85f;i+=0.22f){
        glPushMatrix();
            glTranslatef(i,0,1.06f);
            panelKayu(0.18f,3.2f,0.08f, shade);
        glPopMatrix();
        shade += 0.03f; // gradasi halus
    }

    // LUBANG DEPAN (CEKUNG)
    glPushMatrix();
        glTranslatef(0,0.4f,1.18f);
        glScalef(1.2f,0.6f,0.3f);
        materialHitam();
        cube();
    glPopMatrix();

    // PENUTUP ATAS
    glPushMatrix();
        glTranslatef(0,2.05f,0);
        glScalef(2.2f,0.3f,2.2f);
        materialHitam();
        cube();
    glPopMatrix();

    // RING STAINLESS
    glPushMatrix();
        glTranslatef(0,2.25f,0);
        materialMetal();
        glutSolidTorus(0.12,0.38,40,40);
    glPopMatrix();

    glPopMatrix();
}

// ===============================
// DISPLAY
// ===============================
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(8,8,12, 0,2,0, 0,1,0);

    tongSampah(
        tx,ty,tz,
        sx,sy,sz,
        rx,ry,rz
    );

    glutSwapBuffers();
}

// ===============================
// KEYBOARD
// ===============================
void keyboard(unsigned char key,int,int){
    switch(key){
        case 'w': ty+=0.2f; break;
        case 's': ty-=0.2f; break;
        case 'a': tx-=0.2f; break;
        case 'd': tx+=0.2f; break;
        case 'q': tz+=0.2f; break;
        case 'e': tz-=0.2f; break;

        case 'i': rx-=5; break;
        case 'k': rx+=5; break;
        case 'j': ry-=5; break;
        case 'l': ry+=5; break;
        case 'u': rz-=5; break;
        case 'o': rz+=5; break;

        case '+': sx+=0.1f; sy+=0.1f; sz+=0.1f; break;
        case '-': sx-=0.1f; sy-=0.1f; sz-=0.1f; break;

        case 27: exit(0);
    }
    glutPostRedisplay();
}

// ===============================
// LIGHTING
// ===============================
void lighting(){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat pos[] = {10,15,10,1};
    GLfloat amb[] = {0.3f,0.3f,0.3f,1};
    GLfloat dif[] = {0.8f,0.8f,0.8f,1};

    glLightfv(GL_LIGHT0,GL_POSITION,pos);
    glLightfv(GL_LIGHT0,GL_AMBIENT,amb);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,dif);
}

// ===============================
// INIT
// ===============================
void init(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor(0.88f,0.92f,1,1);

    lighting();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,1.33,1,100);
    glMatrixMode(GL_MODELVIEW);
}

// ===============================
// MAIN
// ===============================
int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("Tong Sampah Kayu Modern - Realistis");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

