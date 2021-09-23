#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
#define WIDTH 800
#define HEIGHT 800

struct Painter {
    TGAImage canvas;
    TGAColor color = white;
    void save();
    void drawLine(int x0, int y0, int x1, int y1, const TGAColor &colore);
};

void Painter::save() {
    //canvas.flip_vertically();
    canvas.write_tga_file("output.tga");
}

void Painter::drawLine(int x0, int y0, int x1, int y1, const TGAColor &colore){
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
    float slope = (y1-y0)/(float)(x1-x0);
    float y_increment = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if(transposed) {
            canvas.set(y,x, colore);
        }
        else {
            canvas.set(x,y, colore);
        }
        
        y_increment += slope;
        if(y_increment > .5) {
            y += (y0>y1?-1:1);
            y_increment -= 1;
        }
        
    }
}

int main(int argc, char** argv) {
    Painter giotto;
    Model *model = new Model("obj\\african_head\\african_head.obj");
    giotto.canvas = TGAImage(WIDTH, HEIGHT, TGAImage::RGB);
    
    
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
    
    return 0;
}