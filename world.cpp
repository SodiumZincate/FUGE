#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "particle.h"
#include "pfgen.h"
#include "pcontacts.h"
#include "pworld.h"

float timeSinceStart = 0.0f;

// Draw a cube
void drawCube(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.5f); glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);   glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);   glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);   glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f); glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);   glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);   glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);   glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    glPopMatrix();
}

// lookAt and perspective functions
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
        "Wold Demo", 
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

    ParticleWorld world(100);
    ParticleForceRegistry registry = world.getForceRegistry();

    std::cout << "hello\n"; 

    Particle projectile1;
    projectile1.setPosition(Vector3(-10,10,0));
    projectile1.setMass(2.0f);
    projectile1.setVelocity(Vector3(0,0,0));
    projectile1.setDamping(0.99f);
    projectile1.clearAccumulator();

    Particle projectile2;
    projectile2.setPosition(Vector3(-20,0,0));
    projectile2.setMass(2.0f);
    projectile2.setVelocity(Vector3(100,0,0));
    projectile2.setDamping(0.99f);
    projectile2.clearAccumulator();

    Particle projectile3;
    projectile3.setPosition(Vector3(0,0,0));
    projectile3.setMass(500.0f);
    projectile3.setVelocity(Vector3(-2.0f,0,0));
    projectile3.setDamping(0.99f);
    projectile3.clearAccumulator();
    
    Particle projectile4;
    projectile4.setPosition(Vector3(-30,0,0));
    projectile4.setMass(500.0f);
    projectile4.setVelocity(Vector3(2.0f,0,0));
    projectile4.setDamping(0.99f);
    projectile4.clearAccumulator();

    Particles particleList;
    particleList.push_back(&projectile1);
    particleList.push_back(&projectile2);
    particleList.push_back(&projectile3);
    particleList.push_back(&projectile4);
    
    ParticleGravity gravity(Vector3(0, -9.81f, 0));
    
    for(Particles::iterator p=particleList.begin(); p!=particleList.end(); p++){
        world.addParticle(*p);

        registry.add(*p, &gravity);
    }

    GroundContact groundContact;
    groundContact.init(&particleList, 0.6f);
    ParticleCollisionContact collision;
    collision.init(&particleList, 0.8f, 0.5f);
    
    world.addContactGenerator(&groundContact);
    world.addContactGenerator(&collision);

    double lastTime = glfwGetTime();
    const double physicsDt = 0.001; // 1 ms physics step
    double accumulator = 0.0;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            
        }

        
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        accumulator += deltaTime;
        
        while (accumulator >= physicsDt) {
            timeSinceStart += physicsDt;
            registry.updateForces(physicsDt);
            world.runPhysics(physicsDt);
            accumulator -= physicsDt;
        }

        // Render current positions directly
        for (Particle* p : particleList) {
            drawCube(p->getPosition().x, p->getPosition().y, p->getPosition().z);
        }
        
        Vector3 pos = projectile1.getPosition();
        Vector3 vel = projectile1.getVelocity();

        glColor3f(1.0f, 0.2f, 0.2f);

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 300.0f);
        glVertex3f(0.0f, 0.0f, -300.0f);

        glVertex3f(0.0f, 300.0f, 0.0f);
        glVertex3f(0.0f, -300.0f, 0.0);

        glVertex3f(300.0f, 0.0f, 0.0f);
        glVertex3f(-300.0f, 0.0f, 0.0f);

        glVertex3f(300.0f, -10.0f, 0.0f);
        glVertex3f(-300.0f, -10.0f, 0.0f);
        glEnd();

        glColor3f(0.5f, 0.6f, 0.2f);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) break;
    }
    glfwTerminate();

    return 0;
}
