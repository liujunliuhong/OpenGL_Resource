//
//  main.cpp
//  OpenGLDemoForSquar
//
//  Created by apple on 2020/7/6.
//  Copyright © 2020 yinhe. All rights reserved.
//

#include <stdio.h>
#include "GLTools.h"
#include <GLUT/GLUT.h>
#include "GLShaderManager.h"

//#define kUserMatrix


// 着色管理器
GLShaderManager shaderManager;
// 批次容器
GLBatch squareBatch;

// 正方形边长
GLfloat squareWidth = 0.1f;

// 初始正方形的4个点的坐标
GLfloat vVerts[] = {
        -squareWidth, -squareWidth, 0.0f,
        squareWidth,  -squareWidth, 0.0f,
        squareWidth,  squareWidth,  0.0f,
        -squareWidth, squareWidth,  0.0f
};


// 记录移动图形时，在x轴上平移的距离
GLfloat xPos = 0.0f;
// //记录移动图形时，在y轴上平移的距离
GLfloat yPos = 0.0f;


void changeSize(int w, int h) {
    /*
     glViewport (GLint x, GLint y, GLsizei width, GLsizei height);
     x、y代表窗口中视图左下角的坐标，通常都为0
     width、height代表像素
     */
    glViewport(0, 0, w, h);
}


void renderScene() {
    // 清除一个或者一组特定的缓冲区
    /*
     缓冲区是一块存在图像信息的储存空间，红色、绿色、蓝色和alpha分量通常一起分量通常一起作为颜色缓存区或像素缓存区引用。
     OpenGL 中不止一种缓冲区（颜色缓存区、深度缓存区和模板缓存区）
      清除缓存区对数值进行预置
     参数：指定将要清除的缓存的
     GL_COLOR_BUFFER_BIT : 指示当前激活的用来进行颜色写入缓冲区
     GL_DEPTH_BUFFER_BIT : 指示深度缓存区
     GL_STENCIL_BUFFER_BIT : 指示模板缓冲区
     */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // 设置正方形的颜色
    GLfloat vRedColor[] = {1.0, 0.0, 0.0, 1.0};
    
    
#ifdef kUserMatrix
    M3DMatrix44f mTransfromMatrix;
    
    // 平移
    m3dTranslationMatrix44(mTransfromMatrix, xPos, yPos, 0.0);
    shaderManager.UseStockShader(GLT_SHADER_FLAT, mTransfromMatrix, vRedColor);
#else
    // UseStockShader : 使用固定着色器
    // 将颜色传递到存储着色器
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRedColor);
#endif
    
    // 提交着色器
    squareBatch.Draw();
    
    //在开始的设置openGL 窗口的时候，我们指定要一个双缓冲区的渲染环境。这就意味着将在后台缓冲区进行渲染，渲染结束后交换给前台。这种方式可以防止观察者看到可能伴随着动画帧与动画帧之间的闪烁的渲染过程。缓冲区交换平台将以平台特定的方式进行。
    //将后台缓冲区进行渲染，然后结束后交换给前台
    glutSwapBuffers();
}


void setupRC() {
    // 设置窗口颜色
    glClearColor(0.98, 0.4, 0.7, 1.0);
    
    // 初始化固定着色器
    // 没有着色器，在OpenGL 核心框架中是无法进行任何渲染的
    // 在熟悉阶段我们采用固管线渲染，后面会用OpenGL着色语言来写着色器
    shaderManager.InitializeStockShaders();
    
    // 用GL_TRIANGLE_FAN来表示正方形，正方形有4个顶点
    // 当单元着色器不够用时，使用平面着色器
    squareBatch.Begin(GL_TRIANGLE_FAN, 4);
    
    squareBatch.CopyVertexData3f(vVerts);
    
    squareBatch.End();
}


void specialKeys(int key, int x, int y) {
    // 步长
    GLfloat stepSize = 0.025;
#ifdef kUserMatrix
    // 以正方形中心点为基准，中心点移动
    if (key == GLUT_KEY_UP) {
        // 上
        yPos += stepSize;
    }
    if (key == GLUT_KEY_LEFT) {
        // 左
        xPos -= stepSize;
    }
    if (key == GLUT_KEY_DOWN) {
        // 下
        yPos -= stepSize;
    }
    if (key == GLUT_KEY_RIGHT) {
        // 右
        xPos += stepSize;
    }
    // 碰撞检测
    if (xPos < -1.0 + squareWidth) {
        xPos = -1.0 + squareWidth;
    }
    if (xPos > 1.0 - squareWidth) {
        xPos = 1.0 - squareWidth;
    }
    if (yPos < -1.0 + squareWidth) {
        yPos = -1.0 + squareWidth;
    }
    if (yPos > 1.0 - squareWidth) {
        yPos = 1.0 - squareWidth;
    }
#else
    // 取出其中一个点(左上角的那个点)
    GLfloat blockX = vVerts[0];
    GLfloat blockY = vVerts[10];
    
    if (key == GLUT_KEY_UP) {
        // 上
        blockY += stepSize;
    }
    if (key == GLUT_KEY_LEFT) {
        // 左
        blockX -= stepSize;
    }
    if (key == GLUT_KEY_DOWN) {
        // 下
        blockY -= stepSize;
    }
    if (key == GLUT_KEY_RIGHT) {
        // 右
        blockX += stepSize;
    }
    
    // 碰撞检测
    // 上边
    if (blockY > 1.0) {
        blockY = 1.0;
    }
    // 左边
    if (blockX < -1.0) {
        blockX = -1.0;
    }
    // 下边
    if (blockY < -(1.0 - squareWidth * 2)) {
        blockY = -(1.0 - squareWidth * 2);
    }
    // 右边
    if (blockX > 1.0 - squareWidth * 2) {
        blockX = 1.0 - squareWidth * 2;
    }
    
    // 第三象限
    vVerts[0] = blockX;
    vVerts[1] = blockY - squareWidth * 2;
    vVerts[2] = 0;
    
    // 第四象限
    vVerts[3] = blockX + squareWidth * 2;
    vVerts[4] = blockY - squareWidth * 2;
    vVerts[5] = 0;
    
    // 第一象限
    vVerts[6] = blockX + squareWidth * 2;
    vVerts[7] = blockY;
    vVerts[8] = 0;
    
    // 第二象限
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    vVerts[11] = 0;
    
    //更新顶点数据
    squareBatch.CopyVertexData3f(vVerts);
#endif
    // 重新渲染提交
    glutPostRedisplay();
    
}


int main(int argc,char* argv[]) {
    
    // /设置当前工作目录，针对MAC OS X
    gltSetWorkingDirectory(argv[0]);
    
    // 初始化GLUT库
    glutInit(&argc, argv);
    
    // 初始化双缓冲窗口
    // GLUT_DOUBLE : 双缓冲窗口
    // GLUT_RGBA : RGBA颜色模式
    // GLUT_DEPTH : 深度测试
    // GLUT_STENCIL : 模板缓冲区
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    
    // 初始化窗口大小
    glutInitWindowSize(500, 500);
    
    // 创建窗口
    glutCreateWindow("正方形移动");
    
    
    /*
     GLUT内部运行了一个本地消息循环，拦截适当的消息。然后调用我们不同时间注册的回调函数。
     我们一共注册2个回调函数：
    1、为窗口改变大小而设置的一个回调函数
    2、包含OpenGL 渲染的回调函数
    */
    
    // 注册重塑函数
    glutReshapeFunc(changeSize);
    
    // 注册显示函数
    glutDisplayFunc(renderScene);
    
    // 注册特殊函数
    glutSpecialFunc(specialKeys);
    
    // 初始化一个GLEW库,确保OpenGL API对程序完全可用。
    GLenum status = glewInit();
    
    const GLubyte *description = glewGetErrorString(status);
    printf("初始化状态: %s\n", description);
    if (status != GLEW_OK) {
        return 1;
    }
    
    // 设置渲染环境
    setupRC();
    
    glutMainLoop();
    
    return 0;
}





