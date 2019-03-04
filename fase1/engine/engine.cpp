#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <GL/glut.h>
#include "../Estrutura/estrutura.h"
#include "tinyxml2.h"

std::vector<vertices> estrutura;

float camX = 10, camY = 5, camZ = 10;
float translate_x, translate_y, translate_z;
float rotate_x, rotate_y, rotate_z;
float eixo_x, eixo_y, eixo_z;
int modo_desenho = GL_LINE;
float R=1;
float G=1;
float B=1;





void changeSize(int w, int h) {

    if (h == 0) h = 1;

    float ratio = w * 1.0f / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}


void tecladoSpecial(int key_code, int x , int y){
    switch(key_code){
        case GLUT_KEY_UP:
            camY+=1;
            break;
        case GLUT_KEY_DOWN:
            camY-=1;
            break;
        case GLUT_KEY_LEFT:
            camX-=1;
            break;
        case GLUT_KEY_RIGHT:
            camX+=1;
            break;
    }
    glutPostRedisplay();
}

void letrasTeclado(unsigned char key, int x, int y){
    switch (key) {

        case 'a':
        case 'A':
            translate_x+=3;
            break;

        case 'd':
        case 'D':
            translate_x-=3;
            break;

        case 'w':
        case 'W':
            translate_z+=3;
            break;

        case 's':
        case 'S':
            translate_z-=3;
            break;

        case 'p':
        case 'P':
            modo_desenho = GL_POINT;
            break;

        case 'l':
        case 'L':
            modo_desenho = GL_LINE;
            break;

        case 'o':
        case 'O':
            modo_desenho = GL_FILL;
            break;

        case '-':
            gluLookAt(camX += 0.5, camY += 0.5, camZ += 0.5, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);
            break;

        case '+':
            gluLookAt(camX -= 0.5, camY -= 0.5, camZ -= 0.5, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);
            break;

    }

    glutPostRedisplay();
}





void lerFicheiro(std::string caminho) {

    std::ifstream ficheiro(caminho);
    std::string linha;

    if(ficheiro.fail()) {
        std::cout << "Doh, não consegui encontrar o ficheiro 3D!"<< std::endl;
    }
    else {
        while(getline(ficheiro,linha)) {
            vertices coordenadas;
            size_t pos;
            coordenadas.x = std::stof(linha,&pos);

            linha.erase(0,pos+1);
            coordenadas.y = std::stof(linha,&pos);

            linha.erase(0,pos+1);
            coordenadas.z = std::stof(linha,&pos);

            estrutura.push_back(coordenadas);
        }
    }
}

void lerXML(std::string caminho) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement *elem;

    if(!(doc.LoadFile(caminho.c_str()))) {

        elem = doc.FirstChildElement();

        for (elem = elem->FirstChildElement();elem;elem = elem->NextSiblingElement()){
            std::string caminho3D= "../Files3D/";
            caminho3D.append(elem->Attribute("file"));
            lerFicheiro(caminho3D);
        }
    }
    else {
        std::cout << "Doh, não encontrei o ficheiro :(" << std::endl;
    }
}



void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
              0.0, 0.0, 0.0,
              0.0f, 1.0f, 0.0f);


    glTranslatef(translate_x, translate_y, translate_z);
    glBegin(GL_LINES);

    // Eixo X
    glColor3f(1.0,0,0);
    glVertex3f(0,0,0);
    glVertex3f(5 + eixo_x,0,0);

    // Eixo Y
    glColor3f(0,1.0,0);
    glVertex3f(0,0,0);
    glVertex3f(0,5 + eixo_y,0);

    // Eixo Z
    glColor3f(0,0,1.0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,5 + eixo_z);

    glEnd();

    glColor3f(1,1,1);
    glPolygonMode(GL_FRONT_AND_BACK,modo_desenho); // Changes how our shapes are represented (drawn). Lines, Points or Filled
    glBegin(GL_TRIANGLES);
    for (int i=0;i<estrutura.size();i++) {
        glVertex3f(estrutura[i].x,estrutura[i].y,estrutura[i].z);
    }
    glEnd();

    glutSwapBuffers();

}



int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("Homer Simpson's engine");

    if(argc < 2){
        std::cout << "Doh! Deu erro! :(" << std::endl;
        return 0;
    }

    else lerXML(argv[1]); // Read XML File

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutSpecialFunc(tecladoSpecial);
    glutKeyboardFunc(letrasTeclado);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    return 1;
}
