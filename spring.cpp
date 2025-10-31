#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "particle.h"

Particle projectile;
bool fired = false;
float timeSinceStart = 0.0f;

void fire() {
    projectile.setMass(800.0f);
    projectile.setPosition(Vector3(0.0f, 1.5f, -100.0f));
    projectile.setVelocity(Vector3(0.0f, 0.0f, 0.0f));
    projectile.setAcceleration(Vector3(0.0f, -9.81f, 0.0f));
    projectile.setDamping(0.95f);
    timeSinceStart = 0.0f;

    std::cout << "initial time = " << timeSinceStart << std::endl;
    std::cout << "position = " << projectile.position.x << " " << projectile.position.y << " " << projectile.position.z << std::endl;
}

// Draw a cube (unchanged)
void drawCube(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);
    glVertex3f(-0.2f, -0.2f, 0.2f); glVertex3f(0.2f, -0.2f, 0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);   glVertex3f(-0.2f, 0.2f, 0.2f);
    glVertex3f(-0.2f, -0.2f, -0.2f); glVertex3f(-0.2f, 0.2f, -0.2f);
    glVertex3f(0.2f, 0.2f, -0.2f);   glVertex3f(0.2f, -0.2f, -0.2f);
    glVertex3f(-0.2f, -0.2f, -0.2f); glVertex3f(-0.2f, -0.2f, 0.2f);
    glVertex3f(-0.2f, 0.2f, 0.2f);   glVertex3f(-0.2f, 0.2f, -0.2f);
    glVertex3f(0.2f, -0.2f, -0.2f); glVertex3f(0.2f, 0.2f, -0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);   glVertex3f(0.2f, -0.2f, 0.2f);
    glVertex3f(-0.2f, 0.2f, -0.2f); glVertex3f(-0.2f, 0.2f, 0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);   glVertex3f(0.2f, 0.2f, -0.2f);
    glVertex3f(-0.2f, -0.2f, -0.2f); glVertex3f(0.2f, -0.2f, -0.2f);
    glVertex3f(0.2f, -0.2f, 0.2f);   glVertex3f(-0.2f, -0.2f, 0.2f);
    glEnd();
    glPopMatrix();
}

// lookAt and perspective functions (unchanged)
void lookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up, float m[16]) {
    Vector3 f = center - eye; f.normalize();
    Vector3 s = f.vectorProduct(up); s.normalize();
    Vector3 u = s.vectorProduct(f);

    m[0] = s.x; m[4] = s.y; m[8]  = s.z; m[12] = -s.x*eye.x - s.y*eye.y - s.z*eye.z;
    m[1] = u.x; m[5] = u.y; m[9]  = u.z; m[13] = -u.x*eye.x - u.y*eye.y - u.z*eye.z;
    m[2] = -f.x;m[6] = -f.y;m[10] = -f.z;m[14] = f.x*eye.x + f.y*eye.y + f.z*eye.z;
    m[3] = 0;   m[7] = 0;   m[11] = 0;   m[15] = 1;
}

void perspective(float fov, float aspect, float near, float far, float m[16]) {
    float f = 1.0f / tan(fov * 3.14159f / 360.0f);
    m[0] = f/aspect; m[4] = 0; m[8]  = 0; m[12] = 0;
    m[1] = 0;        m[5] = f; m[9]  = 0; m[13] = 0;
    m[2] = 0;        m[6] = 0; m[10] = (far+near)/(near-far); m[14] = (2*far*near)/(near-far);
    m[3] = 0;        m[7] = 0; m[11] = -1; m[15] = 0;
}

int main() {
    if (!glfwInit()) return -1;
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    GLFWwindow* window = glfwCreateWindow(
        mode->width, 
        mode->height, 
        "Ballistics Cube Demo", 
        primary,    // pass monitor for fullscreen
        nullptr
    );

    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cerr << "Failed to init GLAD\n"; return -1; }
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = 800.0f / 600.0f;
    float proj[16], view[16];
    perspective(45.0f, aspect, 0.1f, 100.0f, proj);
    lookAt(Vector3(-20, 10, 20), Vector3(0,0,0), Vector3(0,1,0), view);
    glLoadMatrixf(proj);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(view);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            fire();
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            projectile.addForce(Vector3(0.0f, 40.0f, 0.0f));
            std::cout << "position = " << projectile.position.y << " " << projectile.position.z << std::endl; 
            std::cout << "velocity = " << projectile.velocity.x << " " << projectile.velocity.y << " " << projectile.velocity.z << std::endl; 
            std::cout << "acceleration = " << projectile.acceleration.x << " " << projectile.acceleration.y << " " << projectile.acceleration.z << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            projectile.addForce(Vector3(20.0f, 0.0f, 20.0f));
            std::cout << "position = " << projectile.position.y << " " << projectile.position.z << std::endl; 
            std::cout << "velocity = " << projectile.velocity.x << " " << projectile.velocity.y << " " << projectile.velocity.z << std::endl; 
            std::cout << "acceleration = " << projectile.acceleration.x << " " << projectile.acceleration.y << " " << projectile.acceleration.z << std::endl;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            projectile.addForce(Vector3(-20.0f, 0.0f, -20.0f));
            std::cout << "position = " << projectile.position.y << " " << projectile.position.z << std::endl; 
            std::cout << "velocity = " << projectile.velocity.x << " " << projectile.velocity.y << " " << projectile.velocity.z << std::endl; 
            std::cout << "acceleration = " << projectile.acceleration.x << " " << projectile.acceleration.y << " " << projectile.acceleration.z << std::endl;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            projectile.addForce(Vector3(0.0f, -20.0f, 0.0f));
            std::cout << "position = " << projectile.position.y << " " << projectile.position.z << std::endl; 
            std::cout << "velocity = " << projectile.velocity.x << " " << projectile.velocity.y << " " << projectile.velocity.z << std::endl; 
            std::cout << "acceleration = " << projectile.acceleration.x << " " << projectile.acceleration.y << " " << projectile.acceleration.z << std::endl;
        }

        std::cout << "mass=" << projectile.mass
          << " invMass=" << projectile.inverseMass
          << " forceAccum=" << projectile.forceAccum.x << "," << projectile.forceAccum.y << "," << projectile.forceAccum.z
          << " acceleration(base)=" << projectile.acceleration.x << "," << projectile.acceleration.y << "," << projectile.acceleration.z
          << std::endl;


        float dt = 0.016f; // ~60 FPS
        projectile.integrate(dt);

        glColor3f(0.5f, 0.6f, 0.2f);
        drawCube(projectile.position.x, projectile.position.y, projectile.position.z);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) break;
    }

    glfwTerminate();
    return 0;
}
