#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "particle.h"
#include "pfgen.h"

float timeSinceStart = 0.0f;

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
        nullptr,
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
    perspective(90.0f, aspect, 0.1f, 300.0f, proj);
    lookAt(Vector3(-25, 10, -25), Vector3(0.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0), view);
    glLoadMatrixf(proj);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(view);


    Particle projectile1, projectile2;
    projectile1.setPosition(Vector3(0,0,-10.0f));
    projectile1.setMass(6.0f);
    projectile1.setDamping(0.99f);
    projectile1.clearAccumulator();
    
    projectile2.setPosition(Vector3(0,0,10.0f));
    projectile2.setMass(6.0f);
    projectile2.setDamping(0.99f);
    projectile2.clearAccumulator();

    Vector3 anchor1(0,0,-25.0f);
    Vector3 anchor2(0,0,25.0f);

    ParticleGravity gravity(Vector3(0,-9.81f,0));
    ParticleDrag drag(0.1f, 0.02f);
    ParticleAnchoredSpring anchoredSpring1(&anchor1, 1.0f, 25.0f);
    ParticleAnchoredSpring anchoredSpring2(&anchor2, 1.0f, 25.0f);
    ParticleBungee bungee(&projectile1, &projectile2, 1.0f, 7.5f);
    ParticleBuoyancy buoyancy(-10.0f, 1000.0f);

    ParticleForceRegistry registry;
    registry.add(&projectile1, &gravity);
    registry.add(&projectile2, &gravity);

    registry.add(&projectile1, &drag);
    registry.add(&projectile2, &drag);
    registry.add(&projectile1, &bungee);

    registry.add(&projectile1, &anchoredSpring1);
    registry.add(&projectile2, &anchoredSpring2);

    registry.add(&projectile1, &buoyancy);
    registry.add(&projectile2, &buoyancy);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            projectile1.setPosition(Vector3(0,0,-10));
            projectile2.setPosition(Vector3(0,0,10));
            projectile1.clearAccumulator();
            projectile2.clearAccumulator();
        }

        float dt = 0.016f; // ~60 FPS
        registry.updateForces(dt);

        projectile1.integrate(dt);
        projectile2.integrate(dt);

        // Draw the spring anchor
        glColor3f(1.0f, 0.2f, 0.2f);
        drawCube(anchor1.x, anchor1.y, anchor1.z);
        drawCube(anchor2.x, anchor2.y, anchor2.z);

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 300.0f);
        glVertex3f(0.0f, 0.0f, -300.0f);

        glVertex3f(0.0f, 300.0f, 0.0f);
        glVertex3f(0.0f, -300.0f, 0.0);

        glVertex3f(300.0f, 0.0f, 0.0f);
        glVertex3f(-300.0f, 0.0f, 0.0f);

        glVertex3f(0.0f, -10.0f, 300.0f);
        glVertex3f(0.0f, -10.0f, -300.0f);

        glVertex3f(300.0f, -10.0f, 0.0f);
        glVertex3f(-300.0f, -10.0f, 0.0f);

        glColor3f(0.8f, 0.8f, 0.2f);
        glVertex3f(anchor1.x,anchor1.y,anchor1.z);
        glVertex3f(projectile1.position.x, projectile1.position.y, projectile1.position.z);

        glVertex3f(projectile1.position.x, projectile1.position.y, projectile1.position.z);
        glVertex3f(projectile2.position.x, projectile2.position.y, projectile2.position.z);

        glVertex3f(projectile2.position.x, projectile2.position.y, projectile2.position.z);
        glVertex3f(anchor2.x,anchor2.y,anchor2.z);
        glEnd();

        glColor3f(0.5f, 0.6f, 0.2f);
        drawCube(projectile1.position.x, projectile1.position.y, projectile1.position.z);

        glColor3f(0.5f, 0.6f, 0.2f);
        drawCube(projectile2.position.x, projectile2.position.y, projectile2.position.z);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) break;
    }

    glfwTerminate();
    return 0;
}
