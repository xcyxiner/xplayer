#ifndef HGLWIDGET_H
#define HGLWIDGET_H

#include "glew.h"
#include "QtOpenGLWidgets/qopenglwidget.h"
#include "hframe.h"
typedef struct GLTexture_s {
    unsigned int id;  // for glGenTextures
    HFrame frame;
} GLTexture;

#define CLR_B(c)    (c         & 0xff)
#define CLR_G(c)    ((c >> 8)  & 0xff)
#define CLR_R(c)    ((c >> 16) & 0xff)
#define CLR_A(c)    ((c >> 24) & 0xff)
#define ARGB(a, r, g, b) MAKE_FOURCC(a, r, g, b)

#ifndef ABS
#define ABS(n)  ((n) > 0 ? (n) : -(n))
#endif
#define FLOAT_PRECISION     1e-6
#define FLOAT_EQUAL_ZERO(f) (ABS(f) < FLOAT_PRECISION)

void bindTexture(GLTexture* tex, QImage* img);
class HGLWidget : public QOpenGLWidget
{
public:
    explicit HGLWidget(QWidget *parent = nullptr);


    // ratio = 0 means spread
    void setAspectRatio(double ratio);

    void drawFrame(HFrame *pFrame);
    void drawTexture(QRect rc, GLTexture *tex);
    void drawRect(QRect rc, QColor clr, int line_width = 1, bool bFill = false);
    void drawText(QPoint lb, const char* text, int fontsize, QColor clr);
signals:

    // QOpenGLWidget interface
protected:


    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;


    void setVertices(double ratio);
    void setVertices(QRect rc);

    static void loadYUVShader();
    void initVAO();
    void initYUV();

    void drawYUV(HFrame* pFrame);

protected:
    static std::atomic_flag s_glew_init;
    static GLuint prog_yuv;
    static GLuint texUniformY;
    static GLuint texUniformU;
    static GLuint texUniformV;
    GLuint  tex_yuv[3];

    double  aspect_ratio;
    GLfloat vertices[8];
    GLfloat textures[8];

    // NOTE: QPainter used 3 VertexAttribArray
    enum VER_ATTR {
        VER_ATTR_VER = 3,
        VER_ATTR_TEX,
    };

};

#endif // HGLWIDGET_H
