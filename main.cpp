#include "tgaimage.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

struct Painter {
    TGAImage canvas;
    TGAColor color = white;
    void save();
    void line(int x0, int y0, int x1, int y1, TGAColor colore);
};

void Painter::save() {
    //canvas.flip_vertically();
    canvas.write_tga_file("output.tga");
}

void Painter::line(int x0, int y0, int x1, int y1, TGAColor colore){
    bool transposed = false;
    if (std::abs(x0-x1) < std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        transposed = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0 ,y1);
    }
    for (int x=x0; x<=x1; x++) {
        int y = (x-x0)*(y1-y0)/(float)(x1-x0) + y0;
        if(transposed) {
            canvas.set(y,x, colore);
        }
        else {
            canvas.set(x,y, colore);
        }
    }
}

int main(int argc, char** argv) {
    Painter giotto;
    giotto.canvas = TGAImage(100, 100, TGAImage::RGB);
    giotto.line(13,20, 80,40, white);
    giotto.line(20,13, 40,80, red);
    giotto.line(80,40, 13,20, red);
    giotto.save();
    
    return 0;
}