#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
#define WIDTH 200
#define HEIGHT 200

struct Painter {
    TGAImage canvas;
    TGAColor paintColor = white;
    void save();
    void drawLine(int x0, int y0, int x1, int y1, const TGAColor &color);
    void drawTriangle(Vec2i p0, Vec2i p1, Vec2i p2, const TGAColor &color);
};

void Painter::save() {
    //canvas.flip_vertically();
    canvas.write_tga_file("output.tga");
}

void Painter::drawLine(int x0, int y0, int x1, int y1, const TGAColor &color){
    // make so that dx > dy always
    bool transposed = false;
    if (std::abs(x0-x1) < std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        transposed = true;
    }
    
    // draw drawLine from left to right
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0 ,y1);
    }
    
    for (int x=x0; x<=x1; x++) {
        int y = (x-x0)*(y1-y0)/(float)(x1-x0) + y0;
        if(transposed) {
            canvas.set(y,x, color);
        }
        else {
            canvas.set(x,y, color);
        }
    }
    /* BROKEN ALGORITHM
    int dx = x1-x0;
    int dy = std::abs(y1-y0);
    //float slope = std::abs((y1-y0)/float(x1-x0)); // how much y has changed for each x step of 1
    int y_increment = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if(transposed) {
            canvas.set(y,x, color);
        }
        else {
            canvas.set(x,y, color);
        }
        
        y_increment += dy;
        if(2*y_increment > dx) {
            y += (y0>y1?-1:1);
            y_increment -+ dx;
        }
        
    }*/
}

void Painter::drawTriangle(Vec2i p0, Vec2i p1, Vec2i p2, const TGAColor &color) {
    // sort vertices bottom-top
    if(p0.y>p1.y) std::swap(p0,p1);
    if(p0.y>p2.y) std::swap(p0,p2);
    if(p1.y>p2.y) std::swap(p1,p2);
    //drawLine(p0.x,p0.y, p1.x,p1.y, color);
    //drawLine(p1.x,p1.y, p2.x,p2.y, color);
    //drawLine(p2.x,p2.y, p0.x,p0.y, color);
    int dy_p2 = p2.y-p0.y;
    int dy_p1 = p1.y-p0.y;
    for (int y=p0.y; y<=p2.y; y++) {
        float x_left = p0.x + (y-p0.y)*(p2.x-p0.x)/float(p2.y-p0.y);
        float x_right = 0;
        if(y>=p1.y){
            x_right = p1.x + (y-p1.y)*(p2.x-p1.x)/float(p2.y-p1.y);
        } else {
            x_right = p0.x + (y-p0.y)*(p1.x-p0.x)/float(p1.y-p0.y);
        }
        drawLine(int(x_left),y, int(x_right),y, color);
        canvas.set(int(x_left),y, color);
        canvas.set(int(x_right),y, color);      
    }
}

void drawMesh(const char *path) {
    Painter giotto;
    giotto.canvas = TGAImage(WIDTH, HEIGHT, TGAImage::RGB);
    
    Model *model = new Model(path);     
    for (int i=0; i < model->nfaces(); i++){
        std::vector<int> faceVertices = model->face(i);
        for (int j=0; j<3; j++) {
            Vec3f extremity1 = model->vert(faceVertices[j]);
            Vec3f extremity2 = model->vert(faceVertices[ (j+1)%3 ]);
            int x0 = (extremity1.x + 1)*WIDTH/2;
            int y0 = (extremity1.y + 1)*HEIGHT/2;
            int x1 = (extremity2.x + 1)*WIDTH/2;
            int y1 = (extremity2.y + 1)*HEIGHT/2;
            giotto.drawLine(x0,y0, x1,y1, white);
        }
    }
    giotto.save();
}

int main(int argc, char** argv) {
    Painter giotto;
    giotto.canvas = TGAImage(WIDTH, HEIGHT, TGAImage::RGB);
    //drawMesh("obj\\african_head\\african_head.obj");
    
    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)}; 
    giotto.drawTriangle(t0[0],t0[1],t0[2], white);
    giotto.drawTriangle(t1[0],t1[1],t1[2], red);
    giotto.drawTriangle(t2[0],t2[1],t2[2], green);
    
    giotto.save();
}
